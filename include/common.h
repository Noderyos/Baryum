#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(cond, ...)             \
    if(!(cond)) {                     \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);                      \
    }

#endif
