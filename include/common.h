#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(x, s) if(!(x)){fprintf(stderr, "Assertion failed at %s:%d : %s\n", __FILE__, __LINE__, s);exit(-1);}

#endif
