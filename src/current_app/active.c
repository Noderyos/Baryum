#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 8192

// Fonction pour trouver une clé et sa valeur dans une chaîne JSON brute
bool find_key_value(const char *json, const char *key, const char *value, char *result, size_t result_size) {
    const char *pos = json;

    while ((pos = strstr(pos, key)) != NULL) {
        // Vérifie que le champ "key" est suivi par le "value"
        const char *focus_pos = strstr(pos, value);
        if (focus_pos && focus_pos < strchr(pos, '}')) {
            // Rechercher la valeur "name" associée
            const char *name_key = "\"name\":\"";
            const char *name_start = strstr(pos, name_key);
            if (name_start) {
                name_start += strlen(name_key);
                const char *name_end = strchr(name_start, '"');
                if (name_end) {
                    size_t len = name_end - name_start;
                    if (len < result_size - 1) {
                        strncpy(result, name_start, len);
                        result[len] = '\0';
                        return true;
                    }
                }
            }
        }
        pos++;
    }

    return false;
}

int main() {
    FILE *fp;
    char buffer[BUFFER_SIZE];
    char application_name[256];

    // Exécute la commande i3-msg pour récupérer l'arbre des fenêtres
    fp = popen("i3-msg -t get_tree", "r");
    if (fp == NULL) {
        perror("Erreur lors de l'exécution de i3-msg");
        return 1;
    }

    // Lire la sortie JSON dans un buffer
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, fp);
    if (len == 0) {
        perror("Erreur lors de la lecture de la sortie de i3-msg");
        pclose(fp);
        return 1;
    }
    buffer[len] = '\0';
    pclose(fp);

    // Trouver l'application active
    if (find_key_value(buffer, "\"focused\":true", "\"focused\":true", application_name, sizeof(application_name))) {
        printf("%s\n", application_name);
    } 
    return 0;
}

