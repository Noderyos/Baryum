#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char* key;
    char* data;
} Entry;

typedef struct {
    Entry* entries;
    size_t count;
} EntryList;

/**
* enleve les espace de la chaine de caracter passée
*/

void trim(char* str) {

    //debut
    while (isspace((unsigned char)*str)) str++;

    char* end = str + strlen(str) - 1;
    //fin
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
}

/**
* ajoute un entrée a la liste
*/
void add_entry(EntryList* list, const char* key, const char* value) {
    list->entries = realloc(list->entries, (list->count + 1) * sizeof(Entry));
    if (!list->entries) {
        perror("Error reallocating memory");
        exit(EXIT_FAILURE);
    }

    Entry* entry = &list->entries[list->count];
    entry->key = strdup(key);
    entry->data = strdup(value);
    list->count++;
}

/**
* foncttion qui parse une ligne de toml au tour des =
*/
void parse_line(const char* line, const char* section, EntryList* list) {
    char* key = NULL;
    char* value = NULL;
    char* full_key = NULL;
    char* equal_sign = strchr(line, '=');

    if (equal_sign) {
        key = strndup(line, equal_sign - line);
        value = strdup(equal_sign + 1);

        trim(key);
        trim(value);

        // ajout section
        size_t full_key_size = strlen(section) + strlen(key) + 2;  // +2 pour le '.' et fin
        full_key = malloc(full_key_size);
        snprintf(full_key, full_key_size, "%s.%s", section, key);

        // Retirer les guillemets autour des chaînes si présents
        if (value[0] == '"' && value[strlen(value) - 1] == '"') {
            value[strlen(value) - 1] = '\0';
            memmove(value, value + 1, strlen(value));
        }

        // Ajouter l'entrée à la liste
        add_entry(list, full_key, value);

        free(key);
        free(value);
        free(full_key);
    }
}

/**
* load_config charge dans la liste passée en parametre les valeurs du fichier de config
*/
void load_config(const char* filename, EntryList* list) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening config file");
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t len = 0;
    char* current_section = NULL;

    while (getline(&line, &len, fp) != -1) {
        trim(line);

        if (line[0] == '\0' || line[0] == '#') {
            // Ignorer les lignes vides ou les commentaires
            continue;
        }

        if (line[0] == '[') {
            // Section (enlever les crochets)
            free(current_section);
            current_section = strndup(line + 1, strlen(line) - 2);
            trim(current_section);
        } else {
            // Ligne clé-valeur
            parse_line(line, current_section, list);
        }
    }

    free(line);
    free(current_section);
    fclose(fp);
}

void free_entry_list(EntryList* list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->entries[i].key);
        free(list->entries[i].data);
    }
    free(list->entries);
    list->count = 0;
}

