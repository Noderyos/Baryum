#ifndef DRAW_H
#define DRAW_H

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

void DrawFormatedText(Display *display, Window bar, int x, int y, char *text);

#endif
