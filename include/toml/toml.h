#ifndef TOML_H
#define TOML_H

#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "toml/toml_lexer.h"

typedef enum {
    TOML_STRING,
    TOML_NUMBER,
    TOML_INT_LIST,
    TOML_STR_LIST
} Toml_Type;

void toml_parse(char *filename);
char *toml_get_str(char *key);
int toml_get_int(char *key);
int *toml_get_int_list(char *key, size_t *size);
char **toml_get_str_list(char *key, size_t *size);

void toml_free();

#endif
