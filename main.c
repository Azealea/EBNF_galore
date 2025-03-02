#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "parser.c"
#include "lexer.c"

int main() {
    Lexer lexer;
    lexer_init(&lexer, "rule = 'hello' | identifier;");
    
    Parser parser;
    parser_init(&parser, &lexer);

    if (parser.status == 0) {
        printf("Parsing succeeded.\n");
    } else {
        printf("Parsing failed.\n");
    }

    return 0;
}

