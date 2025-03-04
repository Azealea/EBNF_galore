#include "ast/ast.h"
#include "parser/parser.h"
#include "maker/maker.h"
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 2)
    {
    printf("usage: a.out PATH\n");
    return 1;
    }
    Grammar* grammar = parse(argv[1]);
    printf("PARSING FINISHED\n");
    grammar_print(grammar);
    printf("PRINTING FINISHED\n");
    srand((unsigned)time(NULL));
    if (grammar->rules)
        generate_script(grammar, grammar->rules[0]->node);
    printf("FINISHED\n");
    return 0;
}

