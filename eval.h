#ifndef EVAL_H
#define EVAL_H

#include "lexeme.h"

Lexeme *eval(Lexeme *pt, Lexeme *env);
Lexeme *evalPlus(Lexeme *pt, Lexeme *env);
Lexeme *evalMinus(Lexeme *pt, Lexeme *env);
Lexeme *evalTimes(Lexeme *pt, Lexeme *env);
Lexeme *evalDivide(Lexeme *pt, Lexeme *env);
Lexeme *evalEquals(Lexeme *pt, Lexeme *env);
Lexeme *evalAssign(Lexeme *pt, Lexeme *env);
Lexeme *evalAnd(Lexeme *pt, Lexeme *env);
Lexeme *evalOr(Lexeme *pt, Lexeme *env);
Lexeme *evalNot(Lexeme *pt, Lexeme *env);
Lexeme *evalGt(Lexeme *pt, Lexeme *env);
Lexeme *evalLt(Lexeme *pt, Lexeme *env);
Lexeme *evalGte(Lexeme *pt, Lexeme *env);
Lexeme *evalLte(Lexeme *pt, Lexeme *env);
Lexeme *evalDot(Lexeme *pt, Lexeme *env);
Lexeme *evalWhile(Lexeme *pt, Lexeme *env);
Lexeme *evalIf(Lexeme *pt, Lexeme *env);
Lexeme *evalFuncDef(Lexeme *pt, Lexeme *env);
Lexeme *evalLambda(Lexeme *pt, Lexeme *env);
Lexeme *evalVarDef(Lexeme *pt, Lexeme *env);
Lexeme *evalStatementList(Lexeme *pt, Lexeme *env);
Lexeme *evalFuncCall(Lexeme *pt, Lexeme *env);
Lexeme *evalArgs(Lexeme *pt, Lexeme *env);

int isTrue(Lexeme *l);

#endif
