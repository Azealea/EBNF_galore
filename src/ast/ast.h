#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    AST_CHOICE,
    AST_FACTORS,
    AST_CONCAT,
    AST_GROUP,
    AST_OPTIONAL,
    AST_REPETITION,
    AST_LITERAL,
    AST_IDENTIFIER
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union
    {
        struct ASTNode **nodes;
        struct ASTNode *child;
        char* value;
    } data;
} ASTNode;

typedef struct
{
    char *identifier;
    ASTNode *node;
} GrammarRule;

typedef struct
{
    GrammarRule **rules;
} Grammar;

void free_ast(ASTNode* node);
ASTNode* ASTNode_ctor_value(ASTNodeType type, char *value);
ASTNode* ASTNode_ctor_nodes(ASTNodeType type, ASTNode **nodes);
ASTNode* ASTNode_ctor_node(ASTNodeType type, ASTNode *nodes);
GrammarRule *GrammarRule_ctor(char *identifier, ASTNode* node);
Grammar * Grammar_ctor(GrammarRule **rules);
void grammar_print(Grammar *grammar);
