#pragma once

#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer* lexer;
    int status;
    ASTNode* root;
} Parser;

void parser_init(Parser* p, Lexer* lexer);
ASTNode* parser_syntax(Parser* p);
ASTNode* parser_rule(Parser* p);
ASTNode* parser_expression(Parser* p);
ASTNode* parser_term(Parser* p);
ASTNode* parser_factor(Parser* p);
void parser_err(Parser* p);
