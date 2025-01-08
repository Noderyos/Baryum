#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pulse/pulseaudio.h>

#define FORMAT_VOLUME "%percentage%%"
#define FORMAT_MUTED "%percentage%% Muted"

typedef struct {
    char *sink_name;
    float volume;
    float decibels;
    int muted;
    int reverse_scroll;
    int unmute_on_scroll;
    int interval;
    pa_context *context;
    pa_mainloop *mainloop;
    pa_operation *operation;
} pulseaudio_module_t;

// Déclaration de la fonction callback
static void context_state_cb(pa_context *context, void *userdata);

// Callback pour surveiller l'état du contexte
static void context_state_cb(pa_context *context, void *userdata) {
    pa_context_state_t state = pa_context_get_state(context);
    if (state == PA_CONTEXT_READY || state == PA_CONTEXT_FAILED || state == PA_CONTEXT_TERMINATED) {
        *(int *)userdata = 1;
    }
}

// Callback pour afficher les informations du sink
static void sink_info_cb(pa_context *context, const pa_sink_info *info, int eol, void *userdata) {
    pulseaudio_module_t *module = (pulseaudio_module_t *)userdata;

    if (eol > 0 || !info || module->muted) {
        return;
    }

    // Calculer le volume en pourcentage
    module->volume = (float)(pa_cvolume_avg(&info->volume) * 100) / PA_VOLUME_NORM;
    module->muted = info->mute;

    printf("Volume actuel : %.2f%%\n", module->volume);
    if (module->muted) {
        printf("Muted\n");
    }
}

// Fonction pour initialiser et récupérer le volume PulseAudio
void get_pulseaudio_volume_once(pulseaudio_module_t *module) {
    int ready = 0;
    module->mainloop = pa_mainloop_new();
    pa_mainloop_api *mainloop_api = pa_mainloop_get_api(module->mainloop);
    module->context = pa_context_new(mainloop_api, "Volume Getter");

    pa_context_set_state_callback(module->context, context_state_cb, &ready);
    pa_context_connect(module->context, NULL, PA_CONTEXT_NOFLAGS, NULL);

    // Attendre que le contexte soit prêt
    while (ready == 0) {
        pa_mainloop_iterate(module->mainloop, 1, NULL);
    }

    if (pa_context_get_state(module->context) != PA_CONTEXT_READY) {
        fprintf(stderr, "Impossible de se connecter à PulseAudio\n");
        pa_context_disconnect(module->context);
        pa_context_unref(module->context);
        pa_mainloop_free(module->mainloop);
        return;
    }

    // Récupérer les informations du sink
    module->operation = pa_context_get_sink_info_list(module->context, sink_info_cb, module);
    while (pa_operation_get_state(module->operation) == PA_OPERATION_RUNNING) {
        pa_mainloop_iterate(module->mainloop, 1, NULL);
    }
    pa_operation_unref(module->operation);

    // Nettoyage
    pa_context_disconnect(module->context);
    pa_context_unref(module->context);
    pa_mainloop_free(module->mainloop);
}

// Afficher le volume actuel en pourcentage
void print_current_volume(pulseaudio_module_t *module) {
    printf("Volume actuel : %.2f%%\n", module->volume);
}

// Initialisation du module
void pulseaudio_module_init(pulseaudio_module_t *module, const char *sink_name, int interval, int unmute_on_scroll, int reverse_scroll) {
    module->sink_name = sink_name;
    module->volume = 50.0f;
    module->decibels = PA_DECIBEL_MININFTY;
    module->muted = 0;
    module->interval = interval;
    module->unmute_on_scroll = unmute_on_scroll;
    module->reverse_scroll = reverse_scroll;
}

// Libération de la mémoire
void pulseaudio_module_free(pulseaudio_module_t *module) {
    free(module->sink_name);
}

int main(int argc, char* argv[]) {
    pulseaudio_module_t module;
    char* sink = "default";
    int interval = 5;
    int unmute_on_scroll = 1;
    int reverse_scroll = 0;
    if(argc > 1){
        sink = argv[1];
    }
    if(argc > 2){
        interval = strtol(argv[2], NULL, 10);
    }
    if(argc > 3){
        if(strcmp(argv[3],"false")){unmute_on_scroll =  0;}
    }
    if(argc > 4){
        if(strcmp(argv[3],"true")){unmute_on_scroll =  1;}
    }
    pulseaudio_module_init(&module, sink, interval, unmute_on_scroll, reverse_scroll);

    get_pulseaudio_volume_once(&module);
    print_current_volume(&module);

    pulseaudio_module_free(&module);
    return 0;
}
