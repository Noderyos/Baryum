#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

typedef enum {
    TOKEN_INVALID,
    TOKEN_SYMBOL,
    TOKEN_OPEN_BRACKET,
    TOKEN_CLOSE_BRACKET,
    TOKEN_COMMA,
    TOKEN_EQUAL,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_COMMENT
} Toml_Token_Type;

typedef struct {
    Toml_Token_Type type;
    const char *text;
    size_t text_len;
    size_t row;
    size_t col;
} Toml_Token;

typedef struct {
    char *content;
    size_t content_len;
    size_t cursor;
    size_t line;
    size_t bol;
} Toml_Lexer;

Toml_Lexer _toml_lexer_init(char *content, size_t content_len);
Toml_Token _toml_lexer_next(Toml_Lexer *l);

#endif
