#pragma once

#include "parser.h"

void parser_init(Parser* p, Lexer* lexer) {
    p->lexer = lexer;
    p->status = 0;
    p->root = parser_syntax(p);
}

ASTNode* parser_syntax(Parser* p) {
    ASTNode* root = parser_rule(p);
    while (lexer_peek(p->lexer) == T_IDENTIFIER) {
        root = create_ast_node(AST_RULE, NULL, root, parser_rule(p));
    }
    return root;
}

ASTNode* parser_rule(Parser* p) {
    if (lexer_peek(p->lexer) != T_IDENTIFIER) parser_err(p);
    char* id = strdup(p->lexer->lexeme);
    lexer_pop(p->lexer);

    if (lexer_pop(p->lexer) != T_EQUAL) parser_err(p);
    
    ASTNode* expr = parser_expression(p);
    if (lexer_pop(p->lexer) != T_SEMICOLON) parser_err(p);

    return create_ast_node(AST_RULE, id, expr, NULL);
}

ASTNode* parser_expression(Parser* p) {
    ASTNode* left = parser_term(p);
    while (lexer_peek(p->lexer) == T_PIPE) {
        lexer_pop(p->lexer);
        left = create_ast_node(AST_EXPRESSION, "|", left, parser_term(p));
    }
    return left;
}

ASTNode* parser_term(Parser* p) {
    ASTNode* left = parser_factor(p);
    while (lexer_peek(p->lexer) == T_IDENTIFIER || lexer_peek(p->lexer) == T_LITERAL) {
        left = create_ast_node(AST_TERM, NULL, left, parser_factor(p));
    }
    return left;
}

ASTNode* parser_factor(Parser* p) {
    if (lexer_peek(p->lexer) == T_IDENTIFIER || lexer_peek(p->lexer) == T_LITERAL) {
        return create_ast_node(AST_FACTOR, strdup(p->lexer->lexeme), NULL, NULL);
    }
    parser_err(p);
    return NULL;
}

void parser_err(Parser* p) {
    p->status = 1;
    fprintf(stderr, "Parsing error!\n");
    exit(EXIT_FAILURE);
}

