#include "parser.h"

#define PEEK lexer_peek(parser->lexer)
#define POP lexer_pop(parser->lexer)
#define GET_IDENTIFIER strdup(parser->lexer->lexeme)
#define PARSER_LOG(rule) printf("%s\n", rule)

void parser_init(Parser* parser, Lexer* lexer) {
    parser->lexer = lexer;
    parser->status = 0;
    parser->vebose = VERBOSITY_ALL;
}

ASTNode* parse(const char* path) {
    Lexer lexer;
    lexer_init(&lexer, path);
    Parser parser;
    parser_init(&parser, &lexer);
    return parser_syntax(&parser);
}

/*
** syntax = { rule } ;
*/
ASTNode* parser_syntax(Parser* parser) {
    PARSER_LOG("syntax");
    ASTNode* root = Parser_rule();
    while (PEEK == TKN_IDENTIFIER) {
        root = create_ast_node(AST_RULE, NULL, root, Parser_rule());
    }
    return root;
}

/*
** rule = identifier "=" expression ";" ;
*/
ASTNode* parser_rule(Parser* parser) {
    PARSER_LOG("rule");
    if (PEEK != TKN_IDENTIFIER) parser_err(parser);
    char* id = GET_IDENTIFIER;
    POP;

    if (POP != TKN_EQUAL) parser_err(parser);

    ASTNode* expr = Parser_expression();
    if (POP != TKN_SEMICOLON) parser_err(parser);

    return create_ast_node(AST_RULE, id, expr, NULL);
}

/*
** expression = term { "|" term } ;
*/
ASTNode* parser_expression(Parser* parser) {
    PARSER_LOG("expression");
    ASTNode* left = Parser_term();
    while (PEEK == TKN_PIPE) {
        POP;
        left = create_ast_node(AST_EXPRESSION, "|", left, Parser_term());
    }
    return left;
}

/*
** term = factor { factor } ;
*/
ASTNode* parser_term(Parser* parser) {
    PARSER_LOG("term");
    ASTNode* left = Parser_factor();
    while (PEEK == TKN_IDENTIFIER || PEEK == TKN_LITERAL) {
        left = create_ast_node(AST_TERM, NULL, left, Parser_factor());
    }
    return left;
}

/*
** factor = identifier
**        | literal
**        | "(" expression ")"
**        | "[" expression "]"
**        | "{" repetition "}" ;
*/
ASTNode* parser_factor(Parser* parser) {
    PARSER_LOG("factor");
    if (PEEK == TKN_IDENTIFIER || PEEK == TKN_LITERAL) {
        return create_ast_node(AST_FACTOR, GET_IDENTIFIER, NULL, NULL);
    }
    parser_err(parser);
    return NULL;
}

void parser_err(Parser* parser) {
    parser->status = 1;
    fprintf(stderr, "Parsing error!\n");
    exit(EXIT_FAILURE);
}

