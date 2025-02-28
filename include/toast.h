#ifndef TOAST_H
#define TOAST_H

#include <X11/Xlib.h>
#include <stdlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>

#include <wchar.h>
#include <time.h> 
#include <unistd.h>

// create toast with specified duration, and text
void create_toast(Display *display,
        int screen, int height, int width,int bar_height,
        int time_s, char *text);

#endif
