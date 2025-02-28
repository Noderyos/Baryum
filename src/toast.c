#include "toast.h"
#include "draw.h"

typedef struct {
    Display *display;
    int screen;
    int height;
    int width;
    int bar_height;
    int time_s;
    char *text;
} toast_args_t;


static void *thread_toast(void *arg){
    toast_args_t *args = (toast_args_t *)arg;
   
    Display *display = args->display;
    int screen = args->screen;
    int height = args->height;
    int width = args->width;
    int bar_height = args->bar_height;
    int time_s = args->time_s;
    char *text = args->text;

    XSetWindowAttributes toast_attr;
    toast_attr.override_redirect = True;
    
    Window toast = XCreateSimpleWindow(
        display,
        RootWindow(display, screen),
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
    DrawFormatedText(display, toast, 5, 15, text);
    XFlush(display);
    
    sleep(time_s);
    
    XDestroyWindow(display, toast);

    free(args);
    return NULL;
}

//asycronous function that crate the toast
void create_toast(Display *display,
                  int screen,
                  int height,
                  int width,
                  int bar_height,
                  int time_s,
                  char *text) {

    toast_args_t *args = malloc(sizeof(toast_args_t));
    args->display   = display;
    args->screen    = screen;
    args->height    = height;
    args->width     = width;
    args->bar_height = bar_height;
    args->time_s    = time_s;
    args->text      = text;     
    pthread_t thread;

    if (pthread_create(&thread, NULL, thread_toast, args) != 0) {
        free(args);
        return;
    }
    pthread_detach(thread);
}
