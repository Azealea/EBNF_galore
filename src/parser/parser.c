#include "parser.h"
#include "ast/ast.h"
#include "lexer/token.h"
#include "utils/log.h"
#include <stdbool.h>
#include <stdlib.h>

#define PARSER_ERR parser_err(parser)
#define PEEK lexer_peek(parser->lexer)
#define POP lexer_pop(parser->lexer)
#define EAT(tkn) parser_eat(parser, tkn)
#define LEXER_GET_STRING strdup(parser->lexer->lexeme)
#define LEXER_GET_NUMBER parser_take_number(parser)

int parser_take_number(Parser *parser)
{
    if (PEEK == TKN_WEIGHT)
    {
        int res = atoi(parser->lexer->lexeme);
        POP;
        return res;
    }
    else {
        return 1;
    }
}

bool parser_eat(Parser *parser, Token tkn)
{
    if(PEEK == tkn)
    {
        POP;
        return true;
    }
    return false;
}

void parser_init(Parser* parser, Lexer* lexer) {
    parser->lexer = lexer;
    parser->status = 0;
}

Grammar* parse(const char* path) {
    Lexer lexer;
    lexer_init(&lexer, path);
    Parser parser;
    parser_init(&parser, &lexer);
    return parser_syntax(&parser);
}

/*
** syntax = { rule } ;
*/
Grammar* parser_syntax(Parser* parser) {
    LOG_TRACE("syntax");
    ARRAY_BUILDER(rules, GrammarRule*);
    while (PEEK == TKN_IDENTIFIER || PEEK == TKN_EXCLEMATION) {
        ARRAY_BUILDER_PUSH(rules, Parser_rule());
    }
    return Grammar_ctor(ARRAY_BUILDER_FINALIZE(rules));
}

/*
** rule = [ '#' ]  identifier "=" expression ";" ;
*/
GrammarRule* parser_rule(Parser* parser) {
    LOG_TRACE("rule");

    bool is_root = EAT(TKN_EXCLEMATION);
    if ( PEEK !=TKN_IDENTIFIER) PARSER_ERR;
    char* id = LEXER_GET_STRING;
    POP;

    if (!EAT(TKN_EQUAL)) PARSER_ERR;

    ASTNode* expr = Parser_expression();
    if (!EAT(TKN_SEMICOLON)) PARSER_ERR;

    return GrammarRule_ctor(id, expr, is_root);
}

/*
** expression = term { "|" term } ;
*/
ASTNode* parser_expression(Parser* parser) {
    LOG_TRACE("expression");
    ARRAY_BUILDER(terms, ASTNode*);
    ARRAY_BUILDER_PUSH(terms, Parser_term());
    while (EAT(TKN_PIPE)) {
        ARRAY_BUILDER_PUSH(terms, Parser_term());
    }
    return ASTNode_ctor_nodes(AST_CHOICE,  ARRAY_BUILDER_FINALIZE(terms));
}

/*
** term = ? [0-9]* ? concat { concat } ;
*/
ASTNode* parser_term(Parser* parser) {
    // term is a group of factor
    LOG_TRACE("term");
    int weight = LEXER_GET_NUMBER;
    ARRAY_BUILDER(concats, ASTNode*);
    ARRAY_BUILDER_PUSH(concats, Parser_concat());
    while (PEEK == TKN_IDENTIFIER || PEEK == TKN_LITERAL || PEEK == TKN_LBRACE || PEEK == TKN_LBRACKET || PEEK == TKN_LPAREN) {
        ARRAY_BUILDER_PUSH(concats, Parser_concat());
    }
    return ASTNode_ctor_factors(ARRAY_BUILDER_FINALIZE(concats), weight);
}

/*
** concat = factor { "+" factor } ;
*/
ASTNode* parser_concat(Parser* parser)
{
    LOG_TRACE("expression");
    ARRAY_BUILDER(factors, ASTNode*);
    ARRAY_BUILDER_PUSH(factors, Parser_factor());
    while (EAT(TKN_PLUS)) {
        ARRAY_BUILDER_PUSH(factors, Parser_factor());
    }
    return ASTNode_ctor_nodes(AST_CONCAT,  ARRAY_BUILDER_FINALIZE(factors));
}

/*
** factor = identifier
**        | literal
**        | "(" expression ")"
**        | "["  [ ? [0-9]+<space>[0-9]+ ? ] expression "]"
**        | "{" ? [0-9]* ? expression "}" ;
*/
ASTNode* parser_factor(Parser* parser) {
    LOG_TRACE("factor");
    if (PEEK == TKN_IDENTIFIER)
    {
        char * tmp = LEXER_GET_STRING;
        POP;
        return ASTNode_ctor_value(AST_IDENTIFIER, tmp);
    }
    else if (PEEK ==TKN_LITERAL)
    {
        char * tmp = LEXER_GET_STRING;
        POP;
        return ASTNode_ctor_value(AST_LITERAL, tmp);
    }
    else if (EAT(TKN_LBRACE))
    {
        int max_rep = PEEK == TKN_WEIGHT ? LEXER_GET_NUMBER : 3 ;
        ASTNode *expr = Parser_expression();
        if (!EAT(TKN_RBRACE)) PARSER_ERR;
        return ASTNode_ctor_repition(expr, max_rep);
    }
    else if (EAT(TKN_LBRACKET))
    {
        int w_yes = 1;
        int w_no = 1;
        if (PEEK == TKN_WEIGHT)
        {
            w_yes = LEXER_GET_NUMBER;
            w_no = LEXER_GET_NUMBER;
        }
        ASTNode *expr = Parser_expression();
        if (!EAT(TKN_RBRACKET)) PARSER_ERR;
        return ASTNode_ctor_optional( expr, w_yes, w_no);
    }
    else if (EAT(TKN_LPAREN))
    {
        ASTNode *expr = Parser_expression();
        if (!EAT(TKN_RPAREN)) PARSER_ERR;
        return ASTNode_ctor_node(AST_GROUP, expr);
    }
    PARSER_ERR;
    return NULL;
}

void parser_err(Parser* parser) {
    parser->status = 1;
    const char *error_ptr = parser->lexer->input + parser->lexer->pos;
    const char *line_start = error_ptr;
    while (line_start > parser->lexer->input && *(line_start - 1) != '\n') {
        line_start--;
    }

    const char *line_end = error_ptr;
    while (*line_end && *line_end != '\n') {
        line_end++;
    }

    int col = error_ptr - line_start;

    fprintf(stderr, "parser: Unexpected token:\n%.*s\n%*s%s\n",
            (int)(line_end - line_start), line_start, col + 1, "", "^");
    exit(1);
}

