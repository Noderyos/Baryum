#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "draw.h"

#define BAR_HEIGHT 20
#define UPDATE_INTERVAL 1


int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "ERROR: Cannot open display.\n");
        exit(EXIT_FAILURE);
    }

    int screen = DefaultScreen(display);

    Window bar = XCreateSimpleWindow(
            display, RootWindow(display, screen),
            0, 0, 
            DisplayWidth(display, screen), BAR_HEIGHT,
            0, 0x000000, 0x101010
    );
    
    // Set window type to be a dock
    Atom wm_state = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
    Atom wm_state_dock = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
    XChangeProperty(display, bar, 
            wm_state, XA_ATOM, 
            32, PropModeReplace, 
            (unsigned char *)&wm_state_dock, 1);


    XSelectInput(display, bar, ExposureMask | ButtonPressMask);
    XMapWindow(display, bar);

    while (1) {
        XClearWindow(display, bar);
        DrawFormatedText(display, bar, 10, 15, "{0000FF}Hello {FFFFFF}from {FF0000}Baryum");
        
        while (XPending(display)) {
            XEvent event;
            XNextEvent(display, &event);
        }

        sleep(UPDATE_INTERVAL);
    }

    XDestroyWindow(display, bar);
    XCloseDisplay(display);

    return 0;
}

