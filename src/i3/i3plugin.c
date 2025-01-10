#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_RED "\x1b[31m"
#define ANSI_RESET "\x1b[0m"

int main() {
    FILE *fp;
    char buffer[1024];
    char *active_workspace = NULL;

    // Commande pour récupérer les workspaces et le workspace actif
    fp = popen("i3-msg -t get_workspaces | jq -r '.[] | .name + if .focused then \"*\" else \"\" end'", "r");
    if (fp == NULL) {
        perror("Erreur lors de l'exécution de i3-msg");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Enlever les espaces inutiles ou sauts de ligne
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strchr(buffer, '*')) {
            // Bureau actif (indiqué par *)
            active_workspace = buffer;
            *strchr(active_workspace, '*') = '\0';  // Supprimer le "*"
            printf(ANSI_RED "%s " ANSI_RESET, active_workspace);
        } else {
            printf("%s ", buffer);
        }
    }

    printf("\n");
    pclose(fp);
    return 0;
}

