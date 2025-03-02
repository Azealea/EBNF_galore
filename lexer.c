#include "lexer.h"

void lexer_init(Lexer* lexer, const char* input) {
    lexer->input = input;
    lexer->pos = 0;
    lexer->current_token = lexer_next_token(lexer);
}

Token lexer_peek(Lexer* lexer) {
    return lexer->current_token;
}

Token lexer_pop(Lexer* lexer) {
    Token token = lexer->current_token;
    lexer->current_token = lexer_next_token(lexer);
    return token;
}

void lexer_advance(Lexer* lexer) {
    if (lexer->input[lexer->pos] != '\0') {
        lexer->pos++;
    }
}

Token lexer_next_token(Lexer* lexer) {
    lexer_skip_whitespace(lexer);
    char c = lexer->input[lexer->pos];

    if (c == '\0') return T_EOF;
    if (isalpha(c)) return lexer_identifier(lexer);
    if (c == '"' || c == '\'') return lexer_literal(lexer);

    switch (c) {
        case '=': lexer_advance(lexer); return T_EQUAL;
        case ';': lexer_advance(lexer); return T_SEMICOLON;
        case '|': lexer_advance(lexer); return T_PIPE;
        case '(': lexer_advance(lexer); return T_LPAREN;
        case ')': lexer_advance(lexer); return T_RPAREN;
        case '[': lexer_advance(lexer); return T_LBRACKET;
        case ']': lexer_advance(lexer); return T_RBRACKET;
        case '{': lexer_advance(lexer); return T_LBRACE;
        case '}': lexer_advance(lexer); return T_RBRACE;
        default: return T_ERROR;
    }
}

void lexer_skip_whitespace(Lexer* lexer) {
    while (isspace(lexer->input[lexer->pos])) {
        lexer_advance(lexer);
    }
}

Token lexer_identifier(Lexer* lexer) {
    size_t start = lexer->pos;
    while (isalnum(lexer->input[lexer->pos])) {
        lexer_advance(lexer);
    }
    strncpy(lexer->lexeme, lexer->input + start, lexer->pos - start);
    lexer->lexeme[lexer->pos - start] = '\0';
    return T_IDENTIFIER;
}

Token lexer_literal(Lexer* lexer) {
    char quote = lexer->input[lexer->pos++];
    size_t start = lexer->pos;
    while (lexer->input[lexer->pos] != quote && lexer->input[lexer->pos] != '\0') {
        lexer_advance(lexer);
    }
    strncpy(lexer->lexeme, lexer->input + start, lexer->pos - start);
    lexer->lexeme[lexer->pos - start] = '\0';
    lexer_advance(lexer);
    return T_LITERAL;
}
