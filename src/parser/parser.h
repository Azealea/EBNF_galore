#pragma once

#include <stdio.h>

#define _GNU_SOURCE
#include <string.h>
#include "ast/ast.h"
#include "lexer/lexer.h"

#include <stdbool.h>
#include "utils/array_builder.h"
#include "utils/log.h"
#include "lexer/token.h"


typedef struct {
    Lexer* lexer;
    int status;
} Parser;

Grammar* parse(const char* path);

Grammar* parser_syntax(Parser* p);
GrammarRule* parser_rule(Parser* p);
ASTNode* parser_expression(Parser* p);
ASTNode* parser_term(Parser* p);
ASTNode* parser_concat(Parser* p);
ASTNode* parser_factor(Parser* p);

#define Parser_syntax() parser_syntax(parser)
#define Parser_rule() parser_rule(parser)
#define Parser_expression() parser_expression(parser)
#define Parser_term() parser_term(parser)
#define Parser_concat() parser_concat(parser)
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
** concat = factor { "+" factor }
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
