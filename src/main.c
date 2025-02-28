#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "draw.h"
#include "toml/toml.h"
#include "plugins_manager.h"
#include "toast.h"

#define BAR_HEIGHT 20
#define UPDATE_INTERVAL 1

#define POPUP_WIDTH 500
#define POPUP_HEIGHT 200

int main(int argc, char** argv) {

    struct {
        size_t size;
        char **modules;
    }sections[3];

    toml_parse("config.toml");
    sections[0].modules = toml_get_str_list("main.module-left", &sections[0].size);
    sections[1].modules = toml_get_str_list("main.module-center", &sections[1].size);
    sections[2].modules = toml_get_str_list("main.module-right", &sections[2].size);

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

    if(argc > 1){
        if(strcmp(argv[1], "t") == 0){
            create_toast(display, screen, POPUP_HEIGHT, POPUP_WIDTH, BAR_HEIGHT, 10, "hello world");
        }
    }

    while (1) {
        XClearWindow(display, bar);
        
        char left[1024] = {0}, middle[1024] = {0}, right[1024] = {0};
        char output[1024];

        for (size_t i = 0; i < sections[0].size; i++) {
            get_output(sections[0].modules[i], "", output);
            strcat(left, output);
        }
        for (size_t i = 0; i < sections[1].size; i++) {
            get_output(sections[1].modules[i], "", output);
            strcat(middle, output);
        }
       for (size_t i = 0; i < sections[2].size; i++) {
            get_output(sections[2].modules[i], "", output);
            strcat(right, output);
        }

        DrawFormatedText(display, bar, 5, 15, left);
        
        DrawFormatedText(display, bar, 
                (DisplayWidth(display, screen) - TextSize(display, middle))/2, 15, 
                middle);
        DrawFormatedText(display, bar, 
                DisplayWidth(display, screen) - TextSize(display, right) - 5, 15, 
                right);
        
 
        while (XPending(display)) {
            XEvent event;
            XNextEvent(display, &event);
        }

        sleep(UPDATE_INTERVAL);
    }

    XDestroyWindow(display, bar);
    XCloseDisplay(display);
    toml_free();
    return 0;
}

