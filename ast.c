#include "ast.h"
#include <stdlib.h>

static inline ASTNode* ASTNode_ctor(ASTNodeType type)
{
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

ASTNode* ASTNode_ctor_value(ASTNodeType type, char *value)
{
    ASTNode *node = ASTNode_ctor(type);
    node->data.value = value;
    return node;
}

ASTNode* ASTNode_ctor_nodes(ASTNodeType type, ASTNode **nodes)
{
    ASTNode *node = ASTNode_ctor(type);
    node->data.nodes = nodes;
    return node;
}

ASTNode* ASTNode_ctor_node(ASTNodeType type, ASTNode *child)
{
    ASTNode *node = ASTNode_ctor(type);
    node->data.child = child;
    return node;
}

GrammarRule *GrammarRule_ctor(char *identifier, ASTNode* node)
{
    GrammarRule *rule = malloc(sizeof(GrammarRule));
    rule->identifier = identifier;
    rule->node = node;
    return rule;
}

Grammar * Grammar_ctor(GrammarRule **rules)
{
    Grammar *grammar = malloc(sizeof(Grammar));
    grammar->rules = rules;
    return grammar;
}


void free_ast(ASTNode* node) {
    if (!node) return;
    free(node);
}
