#ifndef PLUGGIN_MANAGER_H
#define PLUGGIN_MANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

void get_output(char* nom_plugin, const char* options, char output[1024]);

char* help(char* nom_plugin);

#endif

