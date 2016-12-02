#include <stdio.h>
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
