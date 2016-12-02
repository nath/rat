#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "error.h"
#include "eval.h"
#include "lexeme.h"
#include "lexer.h"
#include "parser.h"
#include "prettyPrint.h"
#include "types.h"

void installBuiltins(Lexeme *env);

Lexeme *squeek(Lexeme *args);
Lexeme *squeekln(Lexeme *args);
Lexeme *array(Lexeme *args);

int main(int argc, char *argv[]) {
    Parser *p = newParser(argv[1]);

    Lexeme *pt = parse(p);

    if (argc > 2)
        prettyPrint(pt, 0);


    Lexeme *globalEnv = createEnv();
    installBuiltins(globalEnv);
    eval(pt, globalEnv);

    return 0;
}

void installBuiltins(Lexeme *env) {
    Lexeme *var = newLexeme(ID);
    var->sval = "squeek";
    Lexeme *val = newLexeme(BUILTIN);
    val->fp = squeek;
    insertEnv(env, var, val);

    var = newLexeme(ID);
    var->sval = "squeekln";
    val = newLexeme(BUILTIN);
    val->fp = squeekln;
    insertEnv(env, var, val);

    var = newLexeme(ID);
    var->sval = "array";
    val = newLexeme(BUILTIN);
    val->fp = array;
    insertEnv(env, var, val);
}

Lexeme *squeek(Lexeme *args) {
    Lexeme *arg;
    while (args != NULL) {
       arg = car(args);
       if (arg->type == NUMBER) {
           printf("%d", arg->ival);
       } else if (arg->type == STRING) {
           printf("%s", arg->sval);
       } else {
           fatalError("RuntimeError, can't print a %s\n", arg->type);
       }
       args = cdr(args);
    }
    return arg;
}

Lexeme *squeekln(Lexeme *args) {
    Lexeme *result = squeek(args);
    printf("\n");
    return result;
}

Lexeme *array(Lexeme *args) {
    if (args == NULL || cdr(args) != NULL) {
        fatalError("Incorrect number of arguments to array()\n");
    }

    Lexeme *size = car(args);
    if (size->type != NUMBER || size->ival <= 0) {
        fatalError("Argument to array() must be a number > 0\n");
    }

    Lexeme *result = newLexeme(ARRAY);
    result->ival = size->ival;
    result->arr = malloc(size->ival * sizeof(Lexeme *));
    return result;
}
