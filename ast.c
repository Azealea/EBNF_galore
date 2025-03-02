#include "ast.h"

ASTNode* create_ast_node(ASTNodeType type, const char* value, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->left = left;
    node->right = right;
    node->value = value;
    return node;

}

void free_ast(ASTNode* node) {
    if (!node) return;
    free(node);
}
