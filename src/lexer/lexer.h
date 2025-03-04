#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/log.h"
#include "token.h"

#define SIZE_INPUT_BUFF 100000


typedef struct {
    size_t pos;
    char input[SIZE_INPUT_BUFF];
    Token current_token;
    char lexeme[256];
    FILE* file;
} Lexer;

Token lexer_peek(Lexer* lexer);
Token lexer_pop(Lexer* lexer);
void lexer_init(Lexer* lexer, const char* path);
