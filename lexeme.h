#ifndef LEXEME_H
#define LEXEME_H

typedef struct Lexeme {
    const char *type;

    char *sval;
    int ival;

    struct Lexeme *left, *right;
} Lexeme;

Lexeme* newLexeme(const char *type);
Lexeme* cons(const char *type, Lexeme *left, Lexeme *right);
Lexeme* car(Lexeme *l);
Lexeme* cdr(Lexeme *l);

void printLexeme(Lexeme *l);

#endif
