#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "{ff0000}"
#define RESET "{-}"

int main(void) {
    FILE *fp;
    char buffer[1024];
    char *active_workspace = NULL;

    // Get workspaces
    fp = popen("i3-msg -t get_workspaces | jq -r '.[] | .name + if .focused then \"*\" else \"\" end'", "r");
    if (fp == NULL) {
        perror("Error executing i3-msg");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Replace new line
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strchr(buffer, '*')) {
            // Current workspace (marked *)
            active_workspace = buffer;
            *strchr(active_workspace, '*') = '\0';  // Remove "*"
            printf(RED "%s " RESET, active_workspace);
        } else {
            printf("%s ", buffer);
        }
    }

    pclose(fp);
    return 0;
}

