#include <stdio.h>
#include "parsing.c"

int main() {
    EntryList list = {0};
    load_config("config.toml", &list);

    printf("Parsed Entries:\n");
    for (size_t i = 0; i < list.count; i++) {
        printf("Key: %s, Data: %s, \n",
               list.entries[i].key, list.entries[i].data);
    }
    free_entry_list(&list);
    return 0;
}
