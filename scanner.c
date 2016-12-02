#include <stdio.h>

#include "error.h"
#include "lexeme.h"
#include "lexer.h"
#include "parser.h"
#include "types.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fatalError("Usage: %s <file to scan>", argv[0]);
    }

    Parser *p = newParser(argv[1]);
    Lexeme *l = lex(p);
    while (l->type != END_OF_FILE) {
        printLexeme(l);
        l = lex(p);
    }

    return 0;
}
