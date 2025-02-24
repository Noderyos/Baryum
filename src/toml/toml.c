#include "toml/toml.h"

#define TOK_ERROR(x, t, s) if(!(x)){fprintf(stderr, "Config error at %ld:%ld : %s\n", t.row, t.col, s);exit(-1);}
#define CHECK_TOKEN(tok, typ) if((tok).type != typ){ \
                                   fprintf(stderr, \
                                       "TOML ERROR: Invalid token at line %ld:%ld", \
                                       tok.row+1, tok.col+1); \
                                   exit(-1); \
                               };

struct {
    char key[64];
    Toml_Type type;
    void *value;
    size_t size;
} keys[128] = {0};

size_t key_count = 0;



char *read_file(const char *filename){
    FILE *f = fopen(filename, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);
    string[fsize] = 0;

    return string;
}

char *toml_get_str(char *key) {
    for (size_t i = 0; i < key_count; i++) {
        if (strcmp(keys[i].key, key) == 0) {
            ASSERT(keys[i].type == TOML_STRING, "Key %s is not a string", key);
            return keys[i].value;
        }
    }
    ASSERT(0, "Cannot find key %s", key);
}

int toml_get_int(char *key) {
    for (size_t i = 0; i < key_count; i++) {
        if (strcmp(keys[i].key, key) == 0) {
            ASSERT(keys[i].type == TOML_NUMBER, "Key %s is not a number", key);
            return *(int*)keys[i].value;
        }
    }
    ASSERT(0, "Cannot find key %s", key);
}

int *toml_get_int_list(char *key, size_t *size) {
    for (size_t i = 0; i < key_count; i++) {
        if (strcmp(keys[i].key, key) == 0) {
            ASSERT(keys[i].type == TOML_INT_LIST, "Key %s is not a number list", key);
            *size = keys[i].size;
            return (int *)keys[i].value;
        }
    }
    ASSERT(0, "Cannot find key %s", key);
}

char **toml_get_str_list(char *key, size_t *size) {
    for (size_t i = 0; i < key_count; i++) {
        if (strcmp(keys[i].key, key) == 0) {
            ASSERT(keys[i].type == TOML_STR_LIST, "Key %s is not a string list", key);
            *size = keys[i].size;
            return (char **)keys[i].value;
        }
    }
    ASSERT(0, "Cannot find key %s", key);

}

void toml_parse(char *filename) {
    char *text = read_file(filename);
    Toml_Lexer lexer = _toml_lexer_init(text, strlen(text));
    
    char cur_sec[64];

    Toml_Token t = _toml_lexer_next(&lexer);
    while (t.type != TOKEN_INVALID) { 
        if (t.type == TOKEN_OPEN_BRACKET) {
            Toml_Token sec = _toml_lexer_next(&lexer);
            CHECK_TOKEN(sec, TOKEN_SYMBOL);
            TOK_ERROR(sec.text_len < 64, sec, "Section name is too long");
            
            memset(cur_sec, 0, 64);
            strncpy(cur_sec, sec.text, sec.text_len);
            
            Toml_Token close = _toml_lexer_next(&lexer);
            CHECK_TOKEN(close, TOKEN_CLOSE_BRACKET);

        }

        else if(t.type == TOKEN_SYMBOL){
            Toml_Token equal = _toml_lexer_next(&lexer);
            CHECK_TOKEN(equal, TOKEN_EQUAL);
            ASSERT(key_count < 128, "Too many keys");
            ASSERT(strlen(cur_sec) + t.text_len + 1 < 64, "Key is too long at line ");
           
            Toml_Token value = _toml_lexer_next(&lexer);

            strcpy(keys[key_count].key, cur_sec);
            strcat(keys[key_count].key, ".");
            strncat(keys[key_count].key, t.text, t.text_len);

            if (value.type == TOKEN_STRING) {
                keys[key_count].type = TOML_STRING;
                keys[key_count].value = strndup(value.text, value.text_len);
                keys[key_count].size = value.text_len;
            } 

            else if (value.type == TOKEN_NUMBER) {
                keys[key_count].type = TOML_NUMBER;
                keys[key_count].value = malloc(sizeof(int));
                keys[key_count].size = 1;
                *(int*)keys[key_count].value = strtol(value.text, NULL, 10);

            } 

            else if (value.type == TOKEN_OPEN_BRACKET) { // List
                Toml_Token value = _toml_lexer_next(&lexer);
                if (value.type == TOKEN_NUMBER) { // Number list
                    keys[key_count].type = TOML_INT_LIST;
                    keys[key_count].value = malloc(256*sizeof(int));
                    keys[key_count].size = 0;
                   
                    Toml_Token next;
                    int *ints = keys[key_count].value;
                    size_t idx = 0;

                    ints[idx++] = strtol(value.text, NULL, 10);

                    while ((next = _toml_lexer_next(&lexer)).type == TOKEN_COMMA) {
                        value = _toml_lexer_next(&lexer); 
                        CHECK_TOKEN(value, TOKEN_NUMBER);
                        
                        ASSERT(idx < 256, "List is too long");
                        ints[idx++] = strtol(value.text, NULL, 10);           
                    }
                    CHECK_TOKEN(next, TOKEN_CLOSE_BRACKET);
                    
                    keys[key_count].size = idx;

                }

                else if (value.type == TOKEN_STRING) { // String list
                    keys[key_count].type = TOML_STR_LIST;
                    keys[key_count].value = malloc(256*sizeof(char *));
                    keys[key_count].size = 0;
                   
                    Toml_Token next;
                    char **strs = keys[key_count].value;
                    size_t idx = 0;

                    strs[idx] = strndup(value.text, value.text_len);
                    strs[idx++][value.text_len] = 0;
                    
                    while ((next = _toml_lexer_next(&lexer)).type == TOKEN_COMMA) {
                        value = _toml_lexer_next(&lexer); 
                        CHECK_TOKEN(value, TOKEN_STRING);
                    
                        ASSERT(idx < 256, "List is too long");
                        strs[idx] = strndup(value.text, value.text_len);
                        strs[idx++][value.text_len] = 0;
                    }
                    CHECK_TOKEN(next, TOKEN_CLOSE_BRACKET);
                    
                    keys[key_count].size = idx; 
                } 

                else {
                    ASSERT(0, "Invalid type in list");
                }
            } 

            else {
                ASSERT(0, "Invalid value");
            }
            key_count++;
        
        } 

        else {
            ASSERT(0, "Invalid Token");
        }
        
        t = _toml_lexer_next(&lexer);
    }
    free(text);
}

void toml_free(void) {
    for (size_t i = 0; i < key_count; i++){

        if (keys[i].type == TOML_NUMBER || keys[i].type == TOML_STRING || keys[i].type == TOML_INT_LIST) {
            free(keys[i].value);
        } else if (keys[i].type == TOML_STR_LIST) {
            for (size_t j = 0; j < keys[i].size; j++) 
                free(((char**)keys[i].value)[j]);
            free(keys[i].value);
        }
    }
    key_count = 0;
}
