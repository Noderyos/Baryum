//
// Created by chlorate on 1/4/25.
//

#ifndef PLUGGIN_MANAGER_H
#define PLUGGIN_MANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_output(char* nom_plugin, const char* options, char output[1024]);

char* help(char* nom_plugin);

#endif //PLUGGIN_MANAGER_H

