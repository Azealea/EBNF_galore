#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_INPUT_BUFF 100000

typedef enum {
    TKN_IDENTIFIER,
    TKN_LITERAL,
    TKN_EQUAL,
    TKN_SEMICOLON,
    TKN_PIPE,
    TKN_LPAREN,
    TKN_RPAREN,
    TKN_LBRACKET,
    TKN_RBRACKET,
    TKN_LBRACE,
    TKN_RBRACE,
    TKN_EOF,
    TKN_ERROR
} Token;

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
void lexer_advance(Lexer* lexer);
Token lexer_next_token(Lexer* lexer);
void lexer_skip_whitespace(Lexer* lexer);
Token lexer_identifier(Lexer* lexer);
Token lexer_literal(Lexer* lexer);
