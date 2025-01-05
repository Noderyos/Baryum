#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "draw.h"
#include "section.c"
#include "plugins_manager.h"

#define BAR_HEIGHT 20
#define UPDATE_INTERVAL 1


int main() {
    EntryList mods[ISLE] = {0};
    section_main(&mods[0], &mods[1], &mods[2]);

    printf("Parsed Entries:\n");
    for (int n = 0; n < ISLE; n++) {
        printf("%s %d \n","section ", n );
        for (int i = 0; i < mods[n].count; i++) {
            printf("Key: %s, Data: %s, \n",
               mods[n].entries[i].key, mods[n].entries[i].data);
        }
    }
    for(int n = 0; n < ISLE; n++) {
        free_entry_list(&mods[n]);
    }

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
        
        char output[1024];
        get_output("time.sh", "", output);
        size_t text_size = TextSize(display, output);
        DrawFormatedText(display, bar, 
                (DisplayWidth(display, screen) - text_size)/2, 15, output);
        
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

