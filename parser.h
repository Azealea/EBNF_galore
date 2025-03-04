#pragma once

#include <stdio.h>

#include "ast.h"
#include "lexer.h"


typedef struct {
    Lexer* lexer;
    int status;
} Parser;

Grammar* parse(const char* path);

Grammar* parser_syntax(Parser* p);
GrammarRule* parser_rule(Parser* p);
ASTNode* parser_expression(Parser* p);
ASTNode* parser_term(Parser* p);
ASTNode* parser_factor(Parser* p);

#define Parser_syntax() parser_syntax(parser)
#define Parser_rule() parser_rule(parser)
#define Parser_expression() parser_expression(parser)
#define Parser_term() parser_term(parser)
#define Parser_factor() parser_factor(parser)

void parser_err(Parser* p);

/*
** syntax = { rule } ;
**
** rule = identifier "=" expression ";" ;
**
** expression = term { "|" term } ;
**
** term = factor { factor } ;
**
** factor = identifier
**        | literal
**        | "(" expression ")"
**        | "[" expression "]"
**        | "{" expression "}" ;
**
** (* lexer job VVV *)
**
** identifier = letter { letter | digit | "_" } ;
**
** literal = '"' character { character } '"'
**         | "'" character { character } "'" ;
**
** character = ? [^'"] ? ;
**
** letter = ? [A-Za-z] ? ;
**
** digit = ? [0-9] ? ;
*/
