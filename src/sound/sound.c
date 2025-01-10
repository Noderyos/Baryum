#include "pulse_interface.h"

int main() {
    int channel_count = 0;
    int *volumes = get_volume(&channel_count);

    for (int i = 0; i < channel_count; i++) {
        printf("%d\n", volumes[i]);
    }

    free(volumes);
    return 0;
}
