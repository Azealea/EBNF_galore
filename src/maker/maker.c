#include "maker.h"
#include <stddef.h>
#include <stdlib.h>

typedef void (*GenFunc)(Grammar *grammar, ASTNode *node);
void generate_script(Grammar *grammar, ASTNode *node);

void gen_literal([[maybe_unused]] Grammar *grammar, ASTNode *node) {
    if (!node->data.value)
        return;

    if (strcmp("\\n", node->data.value) == 0)
        printf("\n");
    else
        printf("%s", node->data.value);
}

void gen_identifier(Grammar *grammar, ASTNode *node) {
    if (!node->data.value)
        return;
    for (int i = 0; grammar->rules[i] != NULL; i++) {
        if (strcmp(grammar->rules[i]->identifier, node->data.value) == 0) {
            generate_script(grammar, grammar->rules[i]->node);
            return;
        }
    }
    printf("==============================COULD NOT FIND %s ===============\n", node->data.value);
}

void gen_group(Grammar *grammar, ASTNode *node) {
    if (node->data.child)
        generate_script(grammar, node->data.child);
}

void gen_optional(Grammar *grammar, ASTNode *node) {
    if (!node->data.child)
        return;
    int weigths = node->data.optional.no_weight + node->data.optional.yes_weight;
    if (rand() % weigths >= node->data.optional.yes_weight) {
        generate_script(grammar, node->data.child);
    }
}

void gen_repetition(Grammar *grammar, ASTNode *node) {
    if (!node->data.child)
        return;
    int count = rand() % node->data.repetition.max_rep;
    for (int i = 0; i < count; i++) {
        generate_script(grammar, node->data.child);
    }
}

void gen_choice(Grammar *grammar, ASTNode *node) {
    if (!node->data.nodes)
        return;
    int count = 0;
    for (size_t i = 0 ; node->data.nodes[i]; i++)
    {
        count+= node->data.nodes[i]->data.factors.weigth;
    }
    if (!count)
        return;
    int choice = rand() % count;
    for (size_t i = 0; node->data.nodes[i]; i++)
    {
        if (choice < node->data.nodes[i]->data.factors.weigth)
        {
            generate_script(grammar, node->data.nodes[i]);
            return;
        }
        choice -= node->data.nodes[i]->data.factors.weigth;
    }

}

void gen_factors(Grammar *grammar, ASTNode *node) {
    if (!node->data.nodes)
        return;
    for (int i = 0; node->data.nodes[i] != NULL; i++) {
        if (i)
            printf(" ");
        generate_script(grammar, node->data.nodes[i]);
    }
}

void gen_concat(Grammar *grammar, ASTNode *node) {
    if (!node->data.nodes)
        return;
    for (int i = 0; node->data.nodes[i] != NULL; i++) {
        generate_script(grammar, node->data.nodes[i]);
    }
}

void generate_script(Grammar *grammar, ASTNode *node) {
    if (!node)
        return;
    static GenFunc gen_funcs[] = {
        [AST_CHOICE]  = gen_choice,
        [AST_FACTORS] =  gen_factors,
        [AST_CONCAT] = gen_concat,
        [AST_GROUP] = gen_group,
        [AST_OPTIONAL]  =gen_optional,
        [AST_REPETITION]= gen_repetition,
        [AST_LITERAL]= gen_literal,
        [AST_IDENTIFIER]= gen_identifier
    };
    if (node->type >= AST_CHOICE && node->type <= AST_IDENTIFIER) {
        gen_funcs[node->type](grammar, node);
    }
}


void generate(Grammar *grammar) {
    for (GrammarRule** rule = grammar->rules; *rule ; rule++)
    {
        if ((*rule)->is_root)
        {
            generate_script(grammar, (*rule)->node);
            printf("\n");
        }
    }
}
