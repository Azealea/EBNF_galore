#include "ast.h"
#include <stdbool.h>
#include <stdlib.h>

static inline ASTNode* ASTNode_ctor(ASTNodeType type)
{
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

ASTNode* ASTNode_ctor_factors(ASTNode** nodes, int weigth)
{
    ASTNode *node = ASTNode_ctor(AST_FACTORS);
    node->data.factors.choices = nodes;
    node->data.factors.weigth = weigth;
    return node;
}

ASTNode* ASTNode_ctor_repition(ASTNode* child, int max_rep)
{
    ASTNode *node = ASTNode_ctor(AST_REPETITION);
    node->data.repetition.child = child;
    node->data.repetition.max_rep = max_rep;
    return node;
}
ASTNode* ASTNode_ctor_optional(ASTNode* child, int yes_weight, int no_weight)
{
    ASTNode *node = ASTNode_ctor(AST_OPTIONAL);
    node->data.optional.child = child;
    node->data.optional.no_weight = no_weight;
    node->data.optional.yes_weight = yes_weight;
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

GrammarRule *GrammarRule_ctor(char *identifier, ASTNode* node, bool is_root)
{
    GrammarRule *rule = malloc(sizeof(GrammarRule));
    rule->identifier = identifier;
    rule->node = node;
    rule->is_root = is_root;
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
