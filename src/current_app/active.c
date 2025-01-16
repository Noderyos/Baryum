#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 8192

// Fonction find key/value in JSON
bool find_key_value(const char *json, const char *key, const char *value, char *result, size_t result_size) {
    const char *pos = json;

    while ((pos = strstr(pos, key)) != NULL) {
        // Check if "key" if followed by "value"
        const char *focus_pos = strstr(pos, value);
        if (focus_pos && focus_pos < strchr(pos, '}')) {
            // Find associated "name"
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

    // Run i3-msg to get window tree
    fp = popen("i3-msg -t get_tree", "r");
    if (fp == NULL) {
        perror("Error executing i3-msg");
        return 1;
    }

    // Read output JSON in a buffer
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, fp);
    if (len == 0) {
        perror("Error reading i3-msg output");
        pclose(fp);
        return 1;
    }
    buffer[len] = '\0';
    pclose(fp);

    // Find current window
    if (find_key_value(buffer, "\"focused\":true", "\"focused\":true", application_name, sizeof(application_name))) {
        printf("%s\n", application_name);
    } 
    return 0;
}

