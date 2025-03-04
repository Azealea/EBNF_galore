#pragma once

#include <stdbool.h>
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
        struct {
            struct ASTNode **choices;
            int weigth;
        } factors;
        struct ASTNode **nodes;
        struct ASTNode *child;
        char* value;
    } data;
} ASTNode;

typedef struct
{
    char *identifier;
    ASTNode *node;
    bool is_root;
} GrammarRule;

typedef struct
{
    GrammarRule **rules;
} Grammar;

void free_ast(ASTNode* node);
ASTNode* ASTNode_ctor_factors(ASTNodeType type, ASTNode** nodes, int weigth);
ASTNode* ASTNode_ctor_value(ASTNodeType type, char *value);
ASTNode* ASTNode_ctor_nodes(ASTNodeType type, ASTNode **nodes);
ASTNode* ASTNode_ctor_node(ASTNodeType type, ASTNode *nodes);
GrammarRule *GrammarRule_ctor(char *identifier, ASTNode* node, bool is_root);
Grammar * Grammar_ctor(GrammarRule **rules);
void grammar_print(Grammar *grammar);
