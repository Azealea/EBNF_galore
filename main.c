#include "ast.h"
#include "parser.h"

int main(int argc, char** argv) {
    if (argc != 2)
    {
    printf("usage: a.out PATH\n");
    return 1;
    }
    Grammar* grammar = parse(argv[1]);
    grammar_print(grammar);
    printf("FINISHED\n");
    return 0;
}

