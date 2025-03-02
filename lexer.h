#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    T_IDENTIFIER, T_LITERAL, T_EQUAL, T_SEMICOLON, T_PIPE,
    T_LPAREN, T_RPAREN, T_LBRACKET, T_RBRACKET, T_LBRACE, T_RBRACE,
    T_EOF, T_ERROR
} Token;

typedef struct {
    const char* input;
    size_t pos;
    Token current_token;
    char lexeme[256]; 
} Lexer;

Token lexer_peek(Lexer* lexer);
Token lexer_pop(Lexer* lexer);
