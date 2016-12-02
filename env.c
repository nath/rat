#include <string.h>

#include "env.h"
#include "error.h"
#include "lexeme.h"
#include "types.h"

Lexeme *insertEnv(Lexeme *env, Lexeme *var, Lexeme *val) {
    Lexeme *table = car(env);
    setCar(table, cons(CONS, var, car(table)));
    setCdr(table, cons(CONS, val, cdr(table)));
    return val;
}

Lexeme *lookupEnv(Lexeme *env, Lexeme *var) {
    while (env != NULL) {
        Lexeme *table = car(env);
        Lexeme *vars = car(table);
        Lexeme *vals = cdr(table);
        while (vars != NULL) {
            if (!strcmp(var->sval, car(vars)->sval)) {
                return car(vals);
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = cdr(env);
    }

    fatalError("%s is undefined.\n", var->sval);

    return NULL;
}


Lexeme *updateEnv(Lexeme *env, Lexeme *var, Lexeme *val) {
    while (env != NULL) {
        Lexeme *table = car(env);
        Lexeme *vars = car(table);
        Lexeme *vals = cdr(table);
        while (vars != NULL) {
            if (!strcmp(var->sval, car(vars)->sval)) {
                setCar(vals, val);
                return car(vals);
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = cdr(env);
    }

    fatalError("%s is undefined.\n", var->sval);

    return NULL;
}

Lexeme *extendEnv(Lexeme *env, Lexeme *vars, Lexeme *vals) {
    return cons(ENV, makeTable(vars, vals), env);
}

Lexeme *createEnv() {
    return extendEnv(NULL, NULL, NULL);
}

Lexeme *makeTable(Lexeme *vars, Lexeme *vals) {
    return cons(TABLE, vars, vals);
}
