#pragma once

typedef enum {
    TKN_IDENTIFIER,
    TKN_LITERAL,
    TKN_EQUAL,
    TKN_SEMICOLON,
    TKN_PIPE,
    TKN_PLUS,
    TKN_LPAREN,
    TKN_RPAREN,
    TKN_LBRACKET,
    TKN_RBRACKET,
    TKN_LBRACE,
    TKN_RBRACE,
    TKN_EOF,
    TKN_ERROR
} Token;
