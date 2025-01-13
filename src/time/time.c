#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include "concat.h"

char *weekday_color = "{0000FF}";
char *time_color = "{FFFFFF}";
char *date_color = "{FF0000}";
char *reset_color = "{-}";


// function to retrieve current time
void custom_time(char order[]) {
    char weekday[16], time_str[16], date_str[32];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char *output = "";

    for (int i = 0; i < 3; i++) {
        switch (order[i]) {
            case 'w': // WEEKDAY
		strftime(weekday, sizeof(weekday), "%A", local);
		output = concat(output, concat(weekday_color, weekday, reset_color), " ");	
                break;
            case 't': // TIME
		strftime(time_str, sizeof(time_str), "%H:%M:%S", local);
                output = concat(output, concat(time_color, time_str, reset_color), " ");
                break;
            case 63: // DATE (number ascii for d)
		strftime(date_str, sizeof(date_str), "%e %b %Y", local);
		output = concat(output, concat(date_color, date_str, reset_color), " ");
                break;
            default:
                break;
        }
    }
    printf("%s\n", output);
}

int main(int argc, char *argv[]) {
    char order[3] = {0};
    int opt;
    int cpt = 0;
    while ((opt = getopt(argc, argv, "wtd:")) != -1) {
	if(cpt > 3){
	    fprintf(stderr, 
		"Usage: %s [-w] [-t] [-d]  \n", argv[0]);
    	    exit(EXIT_FAILURE);

	}
	order[cpt] = (char)opt;	
	cpt++;
    }
    if(cpt == 0){
	for(int i; i < 3; i++){
	    order[i] = i;
	}
     }	
    custom_time(order);

    free(weekday_color);
    free(time_color);
    free(date_color);
    free(reset_color);
    return 0;
}

