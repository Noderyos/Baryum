#include "parsing.c"
#define MAX_SECTIONS 16
#define ISLE 3

void section_main(EntryList* left, EntryList* center, EntryList* right) {
    EntryList list = {0};

    char* section[ISLE][MAX_SECTIONS] = {0};

    load_config("config.toml", &list);
    for (int i = 0; i < list.count; i++) {
        char* token;
        if (strcmp(list.entries[i].key, "main.module-left") == 0) {
            token = strtok(list.entries[i].data, " ");
            int j = 0;
            while (token) {
                section[0][j] = token;
                token = strtok(NULL, " ");
                j++;
            }
        }
        if (strcmp(list.entries[i].key, "main.module-center") == 0) {
            token = strtok(list.entries[i].data, " ");
            int j = 0;
            while (token) {
                section[1][j] = token;
                token = strtok(NULL, " ");
                j++;
            }
        }
        if (strcmp(list.entries[i].key, "main.module-right") == 0) {
            token = strtok(list.entries[i].data, " ");
            int j = 0;
            while (token) {
                section[2][j] = token;
                token = strtok(NULL, " ");
                j++;
            }
        }
    }
    for (int i = 0; i < list.count; i++) {
        for (int n = 0; n < ISLE; n++) { //
            int j = 0;
            while (section[j][n]) {
                if (strncmp(list.entries[i].key, section[j][n], strlen(section[j][n])) == 0) {
                    // check if the section name correspond the current section
                    switch(n) {
                        case 0:
                            //left part of the bar
                            add_entry(left, list.entries[i].key,list.entries[i].data);
                            break;
                        case 1:
                            //center part
                            add_entry(center, list.entries[i].key, list.entries[i].data);
                            break;
                        case 2:
                            //right part
                            add_entry(right, list.entries[i].key, list.entries[i].data);
                            break;
                        default:
                            //normallly unreachable but iin the case of a sneeaky error message
                            add_entry(left, list.entries[i].key, list.entries[i].data);
                            break;
                    };
                }
                j++;
            }
        }
    }
}
