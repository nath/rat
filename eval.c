#include <stdio.h>
#include <string.h>

#include "env.h"
#include "error.h"
#include "eval.h"
#include "lexeme.h"
#include "types.h"

Lexeme *eval(Lexeme *pt, Lexeme *env) {
    if (pt->type == NUMBER || pt->type == STRING)
        return pt;

    if (pt->type == ID) {
        if (!strcmp(pt->sval, "this"))
            return env;

        Lexeme *result = lookupEnv(env, pt);
        if (result->type == DELAY)
            return eval(cdr(result), car(result));
        return result;
    }

    if (pt->type == PLUS)
        return evalPlus(pt, env);

    if (pt->type == MINUS)
        return evalMinus(pt, env);

    if (pt->type == TIMES)
        return evalTimes(pt, env);

    if (pt->type == DIVIDE)
        return evalDivide(pt, env);

    if (pt->type == EQUALS)
        return evalEquals(pt, env);

    if (pt->type == ASSIGN)
        return evalAssign(pt, env);

    if (pt->type == AND)
        return evalAnd(pt, env);

    if (pt->type == OR)
        return evalOr(pt, env);

    if (pt->type == NOT)
        return evalNot(pt, env);

    if (pt->type == DELAY)
        return eval(cdr(pt), car(pt));

    if (pt->type == GT)
        return evalGt(pt, env);

    if (pt->type == LT)
        return evalLt(pt, env);

    if (pt->type == GTE)
        return evalGte(pt, env);

    if (pt->type == LTE)
        return evalLte(pt, env);

    if (pt->type == DOT)
        return evalDot(pt, env);

    if (pt->type == ARRGET)
        return evalArrGet(pt, env);

    if (pt->type == OPAREN)
        return eval(cdr(pt), env);

    if (pt->type == WHILE)
        return evalWhile(pt, env);

    if (pt->type == IF)
        return evalIf(pt, env);

    if (pt->type == DEF)
        return evalFuncDef(pt, env);

    if (pt->type == LAMBDA)
        return evalLambda(pt, env);

    if (pt->type == VAR)
        return evalVarDef(pt, env);

    if (pt->type == STATEMENT_LIST)
        return evalStatementList(pt, env);

    if (pt->type == FN_CALL)
        return evalFuncCall(pt, env);

    fatalError("Unhandled type eval'd: %s", pt->type);

    return NULL;
}

Lexeme *evalPlus(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *left = eval(car(pt), env);
    Lexeme *right = eval(cdr(pt), env);
    if (left->type == NUMBER && right->type == NUMBER) {
        result->ival = left->ival + right->ival;
        return result;
    }

    fatalError("Can only add numbers, %s, %s\n", left->type, right->type);
    return NULL;
}

Lexeme *evalMinus(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *right = eval(cdr(pt), env);
    if (car(pt) == NULL) {
        if (right->type == NUMBER) {
            result->ival = -1*right->ival;
            return result;
        }
        fatalError("Can only negate numbers\n");
        return NULL;
    }
    Lexeme *left = eval(car(pt), env);
    if (left->type == NUMBER && right->type == NUMBER) {
        result->ival = left->ival - right->ival;
        return result;
    }

    fatalError("Can only subtract numbers\n");
    return NULL;
}

Lexeme *evalTimes(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *left = eval(car(pt), env);
    Lexeme *right = eval(cdr(pt), env);
    if (left->type == NUMBER && right->type == NUMBER) {
        result->ival = left->ival * right->ival;
        return result;
    }

    fatalError("Can only multiply numbers\n");
    return NULL;
}

Lexeme *evalDivide(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *left = eval(car(pt), env);
    Lexeme *right = eval(cdr(pt), env);
    if (left->type == NUMBER && right->type == NUMBER) {
        result->ival = left->ival / right->ival;
        return result;
    }

    fatalError("Can only divide numbers\n");
    return NULL;
}

Lexeme *evalEquals(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *left = eval(car(pt), env);
    Lexeme *right = eval(cdr(pt), env);
    if (left->type == NUMBER && right->type == NUMBER) {
        result->ival = left->ival == right->ival;
        return result;
    }
    if (left->type == STRING && right->type == STRING) {
        result->ival = !strcmp(left->sval, right->sval);
        return result;
    }

    fatalError("Can only same types\n");
    return NULL;
}

Lexeme *evalAssign(Lexeme *pt, Lexeme *env) {
    Lexeme *id = car(pt);
    Lexeme *val = eval(cdr(pt), env);
    if (id->type == DOT) {
        Lexeme *obj = eval(car(id), env);
        updateEnv(obj, cdr(id), val);
    } else if (id->type == ARRGET) {
        Lexeme *arr = eval(car(id), env);
        if (arr->type != ARRAY) {
            fatalError("Can't access sub-element of non-array.\n");
        }
        Lexeme *index = eval(cdr(id), env);
        if (index->type != NUMBER) {
            fatalError("Array indicies must be numbers\n");
        }
        if (index->ival < 0 || index->ival >= arr->ival) {
            fatalError("Array index out of bounds: %d\n", index->ival);
        }
        arr->arr[index->ival] = val;
    } else {
        updateEnv(env, id, val);
    }
    return val;
}

Lexeme *evalAnd(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *left = eval(car(pt), env);
    Lexeme *right = eval(cdr(pt), env);
    if (left->type == NUMBER && right->type == NUMBER) {
        result->ival = left->ival && right->ival;
        return result;
    }

    fatalError("Can only and numbers\n");
    return NULL;
}

Lexeme *evalOr(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *left = eval(car(pt), env);
    Lexeme *right = eval(cdr(pt), env);
    if (left->type == NUMBER && right->type == NUMBER) {
        result->ival = left->ival || right->ival;
        return result;
    }

    fatalError("Can only or numbers\n");
    return NULL;
}

Lexeme *evalNot(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *right = eval(cdr(pt), env);
    if (right->type == NUMBER) {
        result->ival = !right->ival;
        return result;
    }

    fatalError("Can only not numbers\n");
    return NULL;
}

Lexeme *evalGt(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *left = eval(car(pt), env);
    Lexeme *right = eval(cdr(pt), env);
    if (left->type == NUMBER && right->type == NUMBER) {
        result->ival = left->ival > right->ival;
        return result;
    }

    fatalError("Can only greater than numbers\n");
    return NULL;
}

Lexeme *evalLt(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *left = eval(car(pt), env);
    Lexeme *right = eval(cdr(pt), env);
    if (left->type == NUMBER && right->type == NUMBER) {
        result->ival = left->ival < right->ival;
        return result;
    }

    fatalError("Can only less than numbers\n");
    return NULL;
}

Lexeme *evalGte(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *left = eval(car(pt), env);
    Lexeme *right = eval(cdr(pt), env);
    if (left->type == NUMBER && right->type == NUMBER) {
        result->ival = left->ival >= right->ival;
        return result;
    }

    fatalError("Can only greater than equals numbers\n");
    return NULL;
}

Lexeme *evalLte(Lexeme *pt, Lexeme *env) {
    Lexeme *result = newLexeme(NUMBER);
    Lexeme *left = eval(car(pt), env);
    Lexeme *right = eval(cdr(pt), env);
    if (left->type == NUMBER && right->type == NUMBER) {
        result->ival = left->ival <= right->ival;
        return result;
    }

    fatalError("Can only divide numbers\n");
    return NULL;
}

Lexeme *evalDot(Lexeme *pt, Lexeme *env) {
    Lexeme *left = eval(car(pt), env);
    if (left->type == ENV) {
        return lookupEnv(left, cdr(pt));
    }

    fatalError("Can only use dot on environments\n");
    return NULL;
}

Lexeme *evalArrGet(Lexeme *pt, Lexeme *env) {
    Lexeme *arr = eval(car(pt), env);
    if (arr->type != ARRAY) {
        fatalError("Can't access sub-element of non-array.\n");
    }
    Lexeme *index = eval(cdr(pt), env);
    if (index->type != NUMBER) {
        fatalError("Array indicies must be numbers\n");
    }
    if (index->ival < 0 || index->ival >= arr->ival) {
        fatalError("Array index out of bounds: %d\n", index->ival);
    }
    return arr->arr[index->ival];
}

Lexeme *evalWhile(Lexeme *pt, Lexeme *env) {
    Lexeme *result = NULL;
    while (isTrue(eval(car(pt), env))) {
        result = eval(cdr(pt), env);
    }
    return result;
}

Lexeme *evalIf(Lexeme *pt, Lexeme *env) {
    if (isTrue(eval(car(car(pt)), env))) {
        return eval(cdr(car(pt)), env);
    }
    if (cdr(pt) != NULL) {
        return eval(cdr(pt), env);
    }
    return NULL;
}

Lexeme *evalFuncDef(Lexeme *pt, Lexeme *env) {
    return insertEnv(env, car(pt), cons(CLOSURE, env, cdr(pt)));
}

Lexeme *evalLambda(Lexeme *pt, Lexeme *env) {
    return cons(CLOSURE, env, cdr(pt));
}

Lexeme *evalVarDef(Lexeme *pt, Lexeme *env) {
    Lexeme *val = eval(cdr(pt), env);
    insertEnv(env, car(pt), val);
    return val;
}

Lexeme *evalStatementList(Lexeme *pt, Lexeme *env) {
    Lexeme *result = NULL;
    while (pt != NULL) {
        result = eval(car(pt), env);
        pt = cdr(pt);
    }
    return result;
}

Lexeme *evalFuncCall(Lexeme *pt, Lexeme *env) {
    Lexeme *closure = eval(car(pt), env);
    Lexeme *args = cdr(pt);
    Lexeme *eargs = evalArgs(args, env);
    if (closure->type == BUILTIN) {
        return closure->fp(eargs);
    }
    Lexeme *params = car(cdr(closure));
    Lexeme *denv = car(closure);
    Lexeme *xenv = extendEnv(denv, params, eargs);
    return eval(cdr(cdr(closure)), xenv);
}

Lexeme *evalArgs(Lexeme *pt, Lexeme *env) {
    if (pt == NULL)
        return NULL;

    Lexeme *earg = car(pt);
    if (earg->type != DELAY)
        earg = eval(earg, env);
    else
        setCar(earg, env);

    return cons(CONS, earg, evalArgs(cdr(pt), env));
}

int isTrue(Lexeme *l) {
    if (l->type == NUMBER)
        return l->ival;

    if (l->type == STRING)
        return strlen(l->sval);

    fatalError("Tried to see if a %s was true\n", l->type);
    return 1;
}
