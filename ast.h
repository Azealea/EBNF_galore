#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum {
        AST_IDENTIFIER,
        AST_LITERAL,
        AST_EXPRESSION,
        AST_TERM,
        AST_FACTOR,
        AST_GROUP,
        AST_OPTION,
        AST_REPETITION,
	AST_RULE
    } ASTNodeType;


// AST Node Structure
typedef struct ASTNode {
    ASTNodeType type;
    const char* value;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

ASTNode* create_ast_node(ASTNodeType type, const char* value, ASTNode* left, ASTNode* right);
void free_ast(ASTNode* node);
