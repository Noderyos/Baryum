#include "toast.h"
#include "draw.h"

void create_toast(Display *display, 
        int screen, int height, int width, int bar_height,
        int time_s, char *text) {

    XSetWindowAttributes toast_attr;
    toast_attr.override_redirect = True;
    
    Window toast = XCreateSimpleWindow(
        display, RootWindow(display, screen),
        DisplayWidth(display, screen) - width, bar_height + 10,
        width, height,
        0, 0x000000, 0x303030
    );

    XChangeWindowAttributes(display, toast, CWOverrideRedirect, &toast_attr);
    XCompositeRedirectWindow(display, toast, CompositeRedirectAutomatic);
    Atom opacity = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", False);
    unsigned long opacity_value = (unsigned long)(0.9 * 0xFFFFFFFF);
    XChangeProperty(display, toast, opacity, XA_CARDINAL, 32, PropModeReplace,
                    (unsigned char *)&opacity_value, 1);

    XMapWindow(display, toast);
    DrawFormatedText(display, toast, 5, 15, (char *)text);
    XFlush(display);
    sleep(time_s);
    XDestroyWindow(display, toast);    
}
