#include "lexer.h"
#include "token.h"
#include "utils/log.h"
#include <ctype.h>

static void lexer_read_file(Lexer* lexer) {
    LOG_TRACE("read_file");
    fread(lexer->input, sizeof(*lexer->input), SIZE_INPUT_BUFF, lexer->file);
}

static void lexer_advance(Lexer* lexer) {
    if (lexer->input[lexer->pos] != '\0') {
        lexer->pos++;
    }
}

static void lexer_skip_whitespace(Lexer* lexer) {
    while (isspace(lexer->input[lexer->pos])) {
        lexer_advance(lexer);
    }
}

static Token lexer_weight(Lexer *lexer)
{
    LOG_TRACE("weigth");
    size_t start = lexer->pos;
    while (isdigit(lexer->input[lexer->pos]))
        lexer_advance(lexer);
    strncpy(lexer->lexeme, lexer->input + start, lexer->pos - start);
    lexer->lexeme[lexer->pos - start] = '\0';
    return TKN_WEIGHT;
}

static Token lexer_identifier(Lexer* lexer) {
    LOG_TRACE("identifier");
    size_t start = lexer->pos;
    while (isalnum(lexer->input[lexer->pos]) || lexer->input[lexer->pos] == '_')
        lexer_advance(lexer);
    strncpy(lexer->lexeme, lexer->input + start, lexer->pos - start);
    lexer->lexeme[lexer->pos - start] = '\0';
    return TKN_IDENTIFIER;
}

static Token lexer_literal(Lexer* lexer) {
    LOG_TRACE("literal");
    char quote = lexer->input[lexer->pos++];
    size_t start = lexer->pos;
    while (lexer->input[lexer->pos] != quote && lexer->input[lexer->pos] != '\0')
    {
        lexer_advance(lexer);
    }
    strncpy(lexer->lexeme, lexer->input + start, lexer->pos - start);
    lexer->lexeme[lexer->pos - start] = '\0';
    lexer_advance(lexer);
    return TKN_LITERAL;
}

static Token lexer_next_token(Lexer* lexer) {
    LOG_TRACE("next_token");
    lexer_skip_whitespace(lexer);
    char c = lexer->input[lexer->pos];

    if (c == '\0') return TKN_EOF;
    if (isdigit(c)) return lexer_weight(lexer);
    if (isalpha(c)) return lexer_identifier(lexer);
    if (c == '"' || c == '\'') return lexer_literal(lexer);

    static Token char_to_token[256]  = {
        ['='] = TKN_EQUAL,
        [';'] = TKN_SEMICOLON,
        ['|'] = TKN_PIPE,
        ['+'] = TKN_PLUS,
        ['!'] = TKN_EXCLEMATION,
        ['('] = TKN_LPAREN,
        [')'] = TKN_RPAREN,
        ['['] = TKN_LBRACKET,
        [']'] = TKN_RBRACKET,
        ['{'] = TKN_LBRACE,
        ['}'] = TKN_RBRACE,
    };

    Token new_token = char_to_token[(int) c];
    if (new_token == TKN_ERROR)
        return TKN_ERROR;

    lexer_advance(lexer);
    return new_token;
}


void lexer_init(Lexer* lexer, const char* path) {
    LOG_TRACE("init");
    lexer->file = fopen(path, "r");
    lexer_read_file(lexer);
    lexer->pos = 0;
    lexer->current_token = lexer_next_token(lexer);
}

static void lexer_print_current_token(Lexer *lexer)
{
    static char *token_to_string[] =
    {
        [TKN_ERROR] = "ERROR",
        [TKN_EOF] = "EOF",
        [TKN_LITERAL] = "[literal]",
        [TKN_IDENTIFIER] = "[identifier]",
        [TKN_WEIGHT] = "[weigth]",
        [TKN_PIPE] = "|",
        [TKN_PLUS] = "+",
        [TKN_EXCLEMATION] = "!",
        [TKN_SEMICOLON] = ";",
        [TKN_EQUAL] = "=",
        [TKN_RPAREN] = ")",
        [TKN_LPAREN] = "(",
        [TKN_LBRACKET] = "[",
        [TKN_RBRACKET] = "]",
        [TKN_RBRACE] = "}",
        [TKN_LBRACE] = "{",
    };
    printf("\ttoken : %s", token_to_string[lexer->current_token]);
    if (lexer->current_token == TKN_IDENTIFIER)
    {
        printf(": %%%s%%", lexer->lexeme);
    }
    else if (lexer->current_token == TKN_LITERAL)
    {
        printf(": \"%s\"", lexer->lexeme);
    }
    else if (lexer->current_token == TKN_WEIGHT)
    {
        printf(": %s", lexer->lexeme);
    }
    printf("\n");
}

Token lexer_peek(Lexer* lexer) {
    LOG_DEBUG("peek");
    if (CURRENT_LOG_LEVEL >= DEBUG)
        lexer_print_current_token(lexer);
    return lexer->current_token;
}

Token lexer_pop(Lexer* lexer) {
    LOG_DEBUG("pop");
    if (CURRENT_LOG_LEVEL >= DEBUG)
        lexer_print_current_token(lexer);
    Token token = lexer->current_token;
    lexer->current_token = lexer_next_token(lexer);
    return token;
}

