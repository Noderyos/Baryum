#ifndef PULSE_INTERFACE_H
#define PULSE_INTERFACE_H

#include <stdio.h>
#include <pulse/pulseaudio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char default_sink_name[256];
    pa_cvolume sink_volume;
    int done; 
} sink_info_t;

int *get_volume(int *channel_count);

#endif
