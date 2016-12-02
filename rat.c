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

void installBuiltin(char *name, Lexeme *(*fp)(Lexeme*), Lexeme *env);
void installBuiltins(Lexeme *env);

Lexeme *squeek(Lexeme *args);
Lexeme *squeekln(Lexeme *args);
Lexeme *array(Lexeme *args);
Lexeme *include(Lexeme *args);
Lexeme *isNumber(Lexeme *args);

Lexeme *globalEnv;

int main(int argc, char *argv[]) {
    Parser *p = newParser(argv[1]);

    Lexeme *pt = parse(p);

    if (argc > 2)
        prettyPrint(pt, 0);


    globalEnv = createEnv();
    installBuiltins(globalEnv);
    eval(pt, globalEnv);

    return 0;
}

void installBuiltin(char *name, Lexeme *(*fp)(Lexeme*), Lexeme *env) {
    Lexeme *var = newLexeme(ID);
    var->sval = name;
    Lexeme *val = newLexeme(BUILTIN);
    val->fp = fp;
    insertEnv(env, var, val);
}

void installBuiltins(Lexeme *env) {
    installBuiltin("squeek", squeek, env);
    installBuiltin("squeekln", squeekln, env);
    installBuiltin("array", array, env);
    installBuiltin("include", include, env);
    installBuiltin("isNumber", isNumber, env);
}

Lexeme *squeek(Lexeme *args) {
    Lexeme *arg;
    while (args != NULL) {
       arg = car(args);
       if (arg->type == NUMBER) {
           printf("%d", arg->ival);
       } else if (arg->type == STRING) {
           printf("%s", arg->sval);
       } else if (arg->type == NIL) {
           printf("NIL");
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

Lexeme *include(Lexeme *args) {
    if (args == NULL || cdr(args) != NULL) {
        fatalError("Incorrect number of arguments to array()\n");
    }

    Lexeme *file = car(args);
    if (file->type != STRING) {
        fatalError("Argument to include() must be a string\n");
    }

    Parser *p = newParser(file->sval);
    Lexeme *pt = parse(p);
    return eval(pt, globalEnv);
}

Lexeme *isNumber(Lexeme *args) {
    if (args == NULL || cdr(args) != NULL) {
        fatalError("Incorrect number of arguments to array()\n");
    }

    Lexeme *result = newLexeme(NUMBER);
    result->ival = car(args)->type == NUMBER;
    return result;
}
