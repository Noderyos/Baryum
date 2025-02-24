#include "draw_type.h"

#define NB_SEGMENT 20
#define BRIGHT L"{ffffff}"
#define DIM L"{666666}"
#define RESET L"{-}"

wchar_t* bar(int input) {
    setlocale(LC_ALL, "");
    if (input < 0)
        input = 0;
    else if (input > 100)
        input = 100;
    
    int fill = (input * NB_SEGMENT) / 100;
    static wchar_t buffer[64];
    int pos = 0;
    buffer[pos++] = L'[';
    buffer[pos] = L'\0';

    int i;
    //bright part(filled)
    wcscat(buffer, BRIGHT);
    pos += 8;
    for(i = 0; i < fill; i++){
        buffer[pos++] = L'─';
    }

    //dim part
    wcscat(buffer, DIM);
    pos += wcslen(DIM);
    for(i = 0; i < NB_SEGMENT - fill; i++){
        buffer[pos++] = L'─';
    }
    
    //end reset
    wcscat(buffer, RESET);
    pos += wcslen(RESET);

    buffer[pos++] = L']';
    buffer[pos] = L'\0';
    
    return buffer;
}
