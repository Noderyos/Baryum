#include "draw_type.h"

void bar_style(int input, char bar[24]){
	int bar_percent = (input/10);
	bar[0] = '[';
    	for(int i = 0; i < 10; i++){
		if( i <= bar_percent ){
            		strcat(bar, "\u2501");
		}
		else {
			strcat(bar, "-");
		}	
	}
	strcat(bar, "]");
}
