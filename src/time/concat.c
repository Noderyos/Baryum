#include "concat.h"

char *concat2(const char *a, const char *b) {
    size_t alen = strlen(a);
    size_t blen = strlen(b);
    char *res = malloc(alen + blen + 1);
    if (res) {
        memcpy(res, a, alen);
        memcpy(res + alen, b, blen + 1);
    }
    return res;
}

char *concat(const char *a, const char *b, const char *c) {
    size_t alen = strlen(a);
    size_t blen = strlen(b);
    size_t clen = strlen(c);
    char *res = malloc(alen + blen + clen + 1);
    if (res) {
        memcpy(res, a, alen);
        memcpy(res + alen, b, blen);
        memcpy(res + alen + blen, c, clen + 1);
    }
    return res;
}
