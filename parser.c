#include "ast.h"
#include "parser.h"
#include "lexer.h"

#include <stdbool.h>
#include "array_builder.h"
#include "log.h"

#define PARSER_ERR parser_err(parser)
#define PEEK lexer_peek(parser->lexer)
#define POP lexer_pop(parser->lexer)
#define EAT(tkn) parser_eat(parser, tkn)
#define LEXER_GET_STRING strdup(parser->lexer->lexeme)

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
    LOG_INFO("syntax");
    ARRAY_BUILDER(rules, GrammarRule*);
    while (PEEK == TKN_IDENTIFIER) {
        ARRAY_BUILDER_PUSH(rules, Parser_rule());
    }
    return Grammar_ctor(ARRAY_BUILDER_FINALIZE(rules));
}

/*
** rule = identifier "=" expression ";" ;
*/
GrammarRule* parser_rule(Parser* parser) {
    LOG_INFO("rule");
    if (!EAT(TKN_IDENTIFIER)) PARSER_ERR;
    char* id = LEXER_GET_STRING;

    if (!EAT(TKN_EQUAL)) PARSER_ERR;

    ASTNode* expr = Parser_expression();
    if (!EAT(TKN_SEMICOLON)) PARSER_ERR;

    return GrammarRule_ctor(id, expr );
}

/*
** expression = term { "|" term } ;
*/
ASTNode* parser_expression(Parser* parser) {
    LOG_INFO("expression");
    ARRAY_BUILDER(terms, ASTNode*);
    ARRAY_BUILDER_PUSH(terms, Parser_term());
    while (EAT(TKN_PIPE)) {
        ARRAY_BUILDER_PUSH(terms, Parser_term());
    }
    return ASTNode_ctor_nodes(AST_CHOICE,  ARRAY_BUILDER_FINALIZE(terms));
}

/*
** term = factor { factor } ;
*/
ASTNode* parser_term(Parser* parser) {
    // term is a group of factor
    LOG_INFO("term");
    ARRAY_BUILDER(factors, ASTNode*);
    ARRAY_BUILDER_PUSH(factors, Parser_factor());
    while (PEEK == TKN_IDENTIFIER || PEEK == TKN_LITERAL || PEEK == TKN_LBRACE || PEEK == TKN_LBRACKET || PEEK == TKN_LPAREN) {
        ARRAY_BUILDER_PUSH(factors, Parser_factor());
    }
    return ASTNode_ctor_nodes(AST_FACTORS, ARRAY_BUILDER_FINALIZE(factors));
}

/*
** factor = identifier
**        | literal
**        | "(" expression ")"
**        | "[" expression "]"
**        | "{" expression "}" ;
*/
ASTNode* parser_factor(Parser* parser) {
    LOG_INFO("factor");
    if (EAT(TKN_IDENTIFIER))
        return ASTNode_ctor_value(AST_IDENTIFIER, LEXER_GET_STRING);
    else if (EAT(TKN_LITERAL))
        return ASTNode_ctor_value(AST_LITERAL, LEXER_GET_STRING);
    else if (EAT(TKN_LBRACE))
    {
        ASTNode *expr = Parser_expression();
        if (!EAT(TKN_RBRACE)) PARSER_ERR;
        return ASTNode_ctor_node(AST_REPETITION, expr);
    }
    else if (EAT(TKN_LBRACKET))
    {
        ASTNode *expr = Parser_expression();
        if (!EAT(TKN_RBRACKET)) PARSER_ERR;
        return ASTNode_ctor_node(AST_OPTIONAL, expr);
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
    fprintf(stderr, "Parsing error!\n");
    exit(EXIT_FAILURE);
}

