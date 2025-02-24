#include "pulse_interface.h"
#include "draw_type.h"

int main(int argc, char *argv[]) {
    int channel_count = 0;
    int *volumes = get_volume(&channel_count);
    
    if (argc > 1 && strcmp(argv[1], "-a") == 0){
        for (int i = 0; i < channel_count; i++) {
            printf("%d\n", volumes[i]);
        }
    }else{
        wprintf(L"%ls", bar(volumes[0] / 650));
    }
    free(volumes);
    return 0;
}
