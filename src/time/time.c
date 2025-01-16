#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

char *weekday_color = "{0000FF}";
char *time_color = "{FFFFFF}";
char *date_color = "{FF0000}";
char *reset_color = "{-}";


// function to retrieve current time
void custom_time(char order[]) {
    char weekday[16], time_str[16], date_str[32];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char output[1024] = {0};

    for (int i = 0; i < 3; i++) {
        switch (order[i]) {
            case 'w': // WEEKDAY
		        strftime(weekday, sizeof(weekday), "%A", local);
		        strcat(output, weekday_color);
		        strcat(output, weekday);
                strcat(output, " ");
                break;
            case 't': // TIME
		        strftime(time_str, sizeof(time_str), "%H:%M:%S", local);
                strcat(output, time_color);
		        strcat(output, time_str);
                strcat(output, " ");
                break;
            case 'd':
		        strftime(date_str, sizeof(date_str), "%e %b %Y", local);
		        strcat(output, date_color);
		        strcat(output, weekday);
                strcat(output, " ");
                break;
            default:
                break;
        }
    }
    strcat(output, reset_color);
    printf("%s\n", output);
}

int main(int argc, char *argv[]) {
    char order[3] = {0};
    int opt;
    int cpt = 0;
    while ((opt = getopt(argc, argv, "wtd")) != -1) {
	    if(cpt > 3){
	        fprintf(stderr, 
		    "Usage: %s [-w] [-t] [-d]\n", argv[0]);
    	    exit(EXIT_FAILURE);

	    }
	    order[cpt] = (char)opt;	
	    cpt++;
    }
    if(cpt == 0){
	    order[0] = 'w';
	    order[1] = 't';
	    order[2] = 'd';
    }	
    custom_time(order);

    return 0;
}

