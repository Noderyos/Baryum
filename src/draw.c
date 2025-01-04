#include "draw.h"

#define DEFAULT_FOREGROUND ((XRenderColor){65535, 65535, 65535, 0})


// Color code format : '{RRGGBB}' in hex, {-} for reset
void DrawFormatedText(Display *display, Window bar, int x, int y, char *text) {
    int screen = DefaultScreen(display);

    XftFont *font = XftFontOpenName(display, screen, "monospace-10");
    if (!font) {
        fprintf(stderr, "ERROR: Cannot load font\n");
        return;
    }

    Visual *visual = DefaultVisual(display, screen);
    Colormap colormap = DefaultColormap(display, screen);

    XftDraw *draw = XftDrawCreate(display, bar, visual, colormap);
    XftColor color;
    XRenderColor render_color = DEFAULT_FOREGROUND;
    XftColorAllocValue(display, visual, colormap, &render_color, &color);

    char* cursor = text;
    char *token = cursor;
    int token_len = 0;
    int cur_x = x;

    XGlyphInfo extents;
    
    while (*cursor) {
        if (*cursor == '{') { // Color code
            if (token_len) {
                XftDrawStringUtf8(draw, &color, font, cur_x, y, (XftChar8 *)token, token_len);
                XftTextExtentsUtf8(display, font, (XftChar8 *)token, token_len, &extents);
                cur_x += extents.xOff;
            }

            cursor++;
            ASSERT(*cursor, "Unexpected EOL");
            
            if (cursor[0] == '-' && cursor[1] == '}') {
                render_color = DEFAULT_FOREGROUND;
                cursor += 2;
            } else {
                char *end;
                
                long i_color = strtol(cursor, &end, 16);
                ASSERT(*end == '}', "Invalid Hex");
                ASSERT(end-cursor == 6, "Hex is incomplete");
                
                cursor += 7;


                render_color.red   = ((i_color >> 16) & 0xff) * 257;
                render_color.green = ((i_color >>  8) & 0xff) * 257;
                render_color.blue  = ((i_color >>  0) & 0xff) * 257;
            }
            
            XftColorAllocValue(display, visual, colormap, &render_color, &color);

            token = cursor;
            token_len = 0;

        } else if (*cursor == '\\') { // Escape caracter
            XftDrawStringUtf8(draw, &color, font, cur_x, y, (XftChar8 *)token, token_len);
            XftTextExtentsUtf8(display, font, (XftChar8 *)token, token_len, &extents);
            cur_x += extents.xOff;
            token = ++cursor;
            ASSERT(*cursor, "Invalid espace");
            token_len = 1;
            cursor++;
        } else { // Simply text
            token_len++;
            cursor++;
        }
    }

    if (token_len) {
        XftDrawStringUtf8(draw, &color, font, cur_x, y, (XftChar8 *)token, token_len);
    }

    XftDrawDestroy(draw);
    XftFontClose(display, font);
}
