#include "parser.h"

int main(int argc, char** argv) {
    ASTNode* node = parse(argv[1]);
    printf("FINISHED\n");
    return 0;
}

