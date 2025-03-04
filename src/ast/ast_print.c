#include "ast.h"
#include <endian.h>
#include <stdio.h>

static void ast_print(ASTNode* node, int rec_level)
{
    if (!node) return;

    switch (node->type) {
        case AST_IDENTIFIER:
            printf("%%%s%%", node->data.value);
            break;

        case AST_LITERAL:
            printf("\"%s\"", node->data.value);
            break;

        case AST_GROUP:
            printf("( ");
            ast_print(node->data.child,1);
            printf(" )");
            break;

        case AST_OPTIONAL:
            printf("yes-no : %d-%d >", node->data.optional.yes_weight,node->data.optional.no_weight);
            printf("[ ");
            ast_print(node->data.child, 1);
            printf(" ]");
            break;

        case AST_REPETITION:
            printf("max repetition : %d >", node->data.repetition.max_rep);
            printf("{ ");
            ast_print(node->data.child, 1);
            printf(" }");
            break;

        case AST_CONCAT: {
            for (int i = 0; node->data.nodes[i] != NULL; i++) {
                ast_print(node->data.nodes[i], 1);
            }
            break;
        }
        case AST_FACTORS: {
            printf("WEIGHT: %i > ", node->data.factors.weigth);
            for (int i = 0; node->data.nodes[i] != NULL; i++) {
                if (i > 0) printf(" ");
                ast_print(node->data.nodes[i], 1);
            }
            break;
        }

        case AST_CHOICE: {
            if (rec_level == 0)
            {
            for (int i = 0; node->data.nodes[i] != NULL; i++) {
                printf(i == 0 ? "\t  " :"\t| ");
                ast_print(node->data.nodes[i],rec_level+1);
                printf("\n");
            }
                printf("\t;\n");
                break;
            }
            for (int i = 0; node->data.nodes[i] != NULL; i++) {
                if (i > 0) printf(" | ");
                ast_print(node->data.nodes[i], 1);
            }
            break;
        }
    }
}

static void grammar_rule_print(GrammarRule *rule)
{
    if (rule->is_root)
        printf("!");
    printf("%s =\n", rule->identifier);
    ast_print(rule->node, 0);
}
void grammar_print(Grammar *grammar)
{
    for (GrammarRule** curr_rule = grammar->rules; *curr_rule; curr_rule++)
    {
        grammar_rule_print(*curr_rule);
    }
}
