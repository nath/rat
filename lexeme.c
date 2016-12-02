#include <stdio.h>
#include <stdlib.h>

#include "lexeme.h"
#include "types.h"

Lexeme* newLexeme(const char* type) {
    Lexeme *l = malloc(sizeof(Lexeme));
    l->type = type;
    return l;
}

Lexeme* cons(const char* type, Lexeme *left, Lexeme *right) {
    Lexeme *l = newLexeme(type);
    l->left = left;
    l->right = right;
    return l;
}

Lexeme* car(Lexeme *l) {
    return l->left;
}

Lexeme* cdr(Lexeme *l) {
    return l->right;
}

void printLexeme(Lexeme *l) {
    if (l->type == NUMBER) {
        printf("%s %d\n", NUMBER, l->ival);
    } else if (l->type == STRING || l->type == ID) {
        printf("%s %s\n", l->type, l->sval);
    } else {
        printf("%s\n", l->type);
    }
}
