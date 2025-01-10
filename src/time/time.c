#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

// Couleurs par défaut
char *weekday_color = "\033[38;5;253m"; // Gris clair
char *time_color = "\033[38;5;227m";    // Jaune pâle
char *date_color = "\033[38;5;253m";    // Gris clair
char *reset_color = "\033[0m";          // Reset

// Fonction pour récupérer la date et l'heure actuelle
void custom_time(int show_weekday, int show_time, int show_date, int order[]) {
    char weekday[16], time_str[16], date_str[32];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    // Obtenir les valeurs formatées
    if (show_weekday) strftime(weekday, sizeof(weekday), "%A", local);
    if (show_time) strftime(time_str, sizeof(time_str), "%H:%M:%S", local);
    if (show_date) strftime(date_str, sizeof(date_str), "%e %b %Y", local);

    // Affichage basé sur l'ordre spécifié
    for (int i = 0; i < 3; i++) {
        switch (order[i]) {
            case 1: // WEEKDAY
                if (show_weekday) printf("%s%s%s ", weekday_color, weekday, reset_color);
                break;
            case 2: // TIME
                if (show_time) printf("%s%s%s ", time_color, time_str, reset_color);
                break;
            case 3: // DATE
                if (show_date) printf("%s%s%s ", date_color, date_str, reset_color);
                break;
            default:
                break;
        }
    }
    printf("\n");
}

// Fonction pour définir une couleur
void set_color(char **color_var, const char *color_code) {
    *color_var = strdup(color_code);
}

int main(int argc, char *argv[]) {
    int show_weekday = 1, show_time = 1, show_date = 1;
    int order[] = {1, 2, 3}; // Ordre par défaut : WEEKDAY, TIME, DATE

    // Options en ligne de commande
    int opt;
    while ((opt = getopt(argc, argv, "wtdo:c:")) != -1) {
        switch (opt) {
            case 'w': show_weekday = 0; break; // Désactiver WEEKDAY
            case 't': show_time = 0; break;    // Désactiver TIME
            case 'd': show_date = 0; break;    // Désactiver DATE
            case 'o': { // Définir un ordre personnalisé
                char *token = strtok(optarg, ",");
                int i = 0;
                while (token != NULL && i < 3) {
                    order[i++] = atoi(token);
                    token = strtok(NULL, ",");
                }
                break;
            }
            case 'c': { // Personnaliser les couleurs
                char *token = strtok(optarg, ",");
                int i = 0;
                while (token != NULL) {
                    if (i == 0) set_color(&weekday_color, token); // Couleur de WEEKDAY
                    if (i == 1) set_color(&time_color, token);    // Couleur de TIME
                    if (i == 2) set_color(&date_color, token);    // Couleur de DATE
                    token = strtok(NULL, ",");
                    i++;
                }
                break;
            }
            default:
                fprintf(stderr, "Usage: %s [-w] [-t] [-d] [-o order] [-c colors]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    custom_time(show_weekday, show_time, show_date, order);

    // Libération des couleurs personnalisées
    if (weekday_color != NULL) free(weekday_color);
    if (time_color != NULL) free(time_color);
    if (date_color != NULL) free(date_color);

    return 0;
}
