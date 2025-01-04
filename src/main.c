#include <stdio.h>
#include "section.c"

int main() {
    EntryList mods[ISLE] = {0};
    section_main(&mods[0],&mods[1],&mods[2]);

    printf("Parsed Entries:\n");
    for (int n = 0; n < ISLE; n++) {
        printf("%s %d \n","section ", n );
        for (int i = 0; i < mods[n].count; i++) {
            printf("Key: %s, Data: %s, \n",
               mods[n].entries[i].key, mods[n].entries[i].data);
        }
    }
    for(int n = 0; n < ISLE; n++) {
        free_entry_list(&mods[n]);
    }
    return 0;
}
