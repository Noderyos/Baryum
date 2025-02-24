#include "pulse_interface.h"


static void context_state_callback(pa_context *c, void *userdata) {
    pa_context_state_t state = pa_context_get_state(c);
    switch (state) {
        case PA_CONTEXT_READY:
        case PA_CONTEXT_TERMINATED:
        case PA_CONTEXT_FAILED:
            *(int *)userdata = 1;
            break;
        default:
            break;
    }
}

static void server_info_callback(pa_context *c, const pa_server_info *i, void *userdata) {
    (void)c;
    sink_info_t *info = (sink_info_t *)userdata;
    if (i->default_sink_name) {
        strncpy(info->default_sink_name, i->default_sink_name, sizeof(info->default_sink_name) - 1);
        info->default_sink_name[sizeof(info->default_sink_name) - 1] = '\0'; // Ensure null-termination
    } else {
        info->default_sink_name[0] = '\0'; // Handle null case
    }
    info->done = 1; // Indicate operation completion
}

static void sink_info_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
    (void)c;
    sink_info_t *info = (sink_info_t *)userdata;

    if (eol > 0) {
        info->done = 1; // End of operation
        return;
    }

    if (i) {
        info->sink_volume = i->volume;
    }
}

int *get_volume(int *channel_count) {
    pa_mainloop *ml = pa_mainloop_new();
    if (!ml) {
        fprintf(stderr, "Failed to create mainloop.\n");
        return NULL;
    }

    pa_mainloop_api *ml_api = pa_mainloop_get_api(ml);
    pa_context *ctx = pa_context_new(ml_api, "Get Sink Info");
    if (!ctx) {
        fprintf(stderr, "Failed to create PulseAudio context.\n");
        pa_mainloop_free(ml);
        return NULL;
    }

    int done = 0;
    pa_context_set_state_callback(ctx, context_state_callback, &done);

    if (pa_context_connect(ctx, NULL, PA_CONTEXT_NOFLAGS, NULL) < 0) {
        fprintf(stderr, "Failed to connect PulseAudio context.\n");
        pa_context_unref(ctx);
        pa_mainloop_free(ml);
        return NULL;
    }

    // Wait for the context to be ready
    while (!done) {
        pa_mainloop_iterate(ml, 1, NULL);
    }

    // Initialize sink info struct
    sink_info_t sink_info = { .default_sink_name = "", .done = 0 };

    // Get server info to retrieve the default sink name
    pa_operation *op = pa_context_get_server_info(ctx, server_info_callback, &sink_info);
    if (!op) {
        fprintf(stderr, "Failed to get server info.\n");
        pa_context_disconnect(ctx);
        pa_context_unref(ctx);
        pa_mainloop_free(ml);
        return NULL;
    }

    while (!sink_info.done) {
        pa_mainloop_iterate(ml, 1, NULL);
    }
    pa_operation_unref(op);

    if (strlen(sink_info.default_sink_name) == 0) {
        fprintf(stderr, "Failed to retrieve the default sink name.\n");
        pa_context_disconnect(ctx);
        pa_context_unref(ctx);
        pa_mainloop_free(ml);
        return NULL;
    }

    // Reset done flag and get sink info by name
    sink_info.done = 0;
    op = pa_context_get_sink_info_by_name(ctx, sink_info.default_sink_name, sink_info_callback, &sink_info);
    if (!op) {
        fprintf(stderr, "Failed to get sink info.\n");
        pa_context_disconnect(ctx);
        pa_context_unref(ctx);
        pa_mainloop_free(ml);
        return NULL;
    }

    while (!sink_info.done) {
        pa_mainloop_iterate(ml, 1, NULL);
    }
    pa_operation_unref(op);

    *channel_count = sink_info.sink_volume.channels;
    int *output = malloc(*channel_count * sizeof(int));
    if (!output) {
        fprintf(stderr, "Failed to allocate memory for output.\n");
        pa_context_disconnect(ctx);
        pa_context_unref(ctx);
        pa_mainloop_free(ml);
        return NULL;
    }

    for (int i = 0; i < *channel_count; ++i) {
        output[i] = sink_info.sink_volume.values[i];
    }

    // Cleanup
    pa_context_disconnect(ctx);
    pa_context_unref(ctx);
    pa_mainloop_free(ml);
    return output;
}
