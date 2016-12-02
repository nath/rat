#ifndef ENV_H
#define ENV_H

#include "lexeme.h"

Lexeme *insertEnv(Lexeme *env, Lexeme *var, Lexeme *val);
Lexeme *lookupEnv(Lexeme *env, Lexeme *var);
Lexeme *updateEnv(Lexeme *env, Lexeme *var, Lexeme *val);
Lexeme *extendEnv(Lexeme *env, Lexeme *vars, Lexeme *vals);
Lexeme *createEnv();

Lexeme *makeTable(Lexeme *var, Lexeme *val);

#endif
