#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "types.h"

Parser* newParser(const char* fileName) {
   Parser *p = malloc(sizeof(Parser)); 
   p->in = fopen(fileName, "r");
   if (p->in == NULL) {
       fatalError("Error opening %s", fileName);
   }
   p->line = 1;
   p->col = 1;
   p->charSaved = 0;
   p->currentLexeme = NULL;
   return p;
}

void saveChar(char ch, Parser *p) {
    p->charSaved = 1;
    p->savedChar = ch;
}

int check(Parser *p, const char *type) {
    return type == p->currentLexeme->type;
}

void advance(Parser *p) {
    p->currentLexeme = lex(p);
}

Lexeme *match(Parser *p, const char *type) {
    Lexeme *l = p->currentLexeme;
    matchNoAdvance(p, type);
    advance(p);
    return l;
}

void matchNoAdvance(Parser *p, const char *type) {
    if (!check(p, type)) {
        fatalError("Error, unexpected %s. Line %d col %d", p->currentLexeme->type, p->line, p->col);
    }
}

Lexeme *parse(Parser *p) {
    advance(p); //read in first lexeme
    Lexeme *pt = statementList(p);
    match(p, END_OF_FILE);
    return pt;
}

Lexeme *statementList(Parser *p) {
    Lexeme *l, *r = NULL;
    l = statement(p);
    if (statementListPending(p)) {
        r = statementList(p);
    }
    return cons(STATEMENT_LIST, l, r);
}

int statementListPending(Parser *p) {
    return statementPending(p);
}

Lexeme *statement(Parser *p) {
    if (expressionPending(p)) {
        Lexeme *l = expression(p);
        match(p, SEMI);
        return l;
    } else if (ifStatementPending(p)) {
        return ifStatement(p);
    } else if (whileLoopPending(p)) {
        return whileLoop(p);
    } else if (variableDefPending(p)) {
        return variableDef(p);
    } else {
        return functionDef(p);
    }
}

int statementPending(Parser *p) {
    return expressionPending(p) || ifStatementPending(p) || whileLoopPending(p) 
        || variableDefPending(p) || functionDefPending(p);
}

Lexeme *expression(Parser *p) {
    Lexeme *l = unary(p);
    if (operatorPending(p)) {
        Lexeme *op, *r;
        op = operator(p);
        r = expression(p);
        op->left = l;
        op->right = r;
        return op;
    }
    return l;
}

int expressionPending(Parser *p) {
    return unaryPending(p);
}

Lexeme *operator(Parser *p) {
    if (check(p, PLUS)) {
        return match(p, PLUS);
    } else if (check(p, MINUS)) {
        return match(p, MINUS);
    } else if (check(p, TIMES)) {
        return match(p, TIMES);
    } else if (check(p, DIVIDE)) {
        return match(p, DIVIDE);
    } else if (check(p, EQUALS)) {
        return match(p, EQUALS);
    } else if (check(p, ASSIGN)) {
        return match(p, ASSIGN);
    }else if (check(p, AND)) {
        return match(p, AND);
    } else if (check(p, OR)) {
        return match(p, OR);
    } else if (check(p, GT)) {
        return match(p, GT);
    } else if (check(p, GTE)) {
        return match(p, GTE);
    } else if (check(p, LT)) {
        return match(p, LT);
    } else {
        return match(p, LTE);
    }
}

int operatorPending(Parser *p) {
    return check(p, PLUS) || check(p, MINUS) || check(p, TIMES) || check(p, DIVIDE) || check(p, EQUALS) || check(p, ASSIGN)
        || check(p, AND) || check(p, OR) || check(p, GT) || check(p, GTE) || check(p, LT) || check(p, LTE);
}

Lexeme *unary(Parser *p) {
    if (check(p, NUMBER)) {
        return match(p, NUMBER);
    } else if (check(p, STRING)) {
        return match(p, STRING);
    } else if (variablePending(p)) {
        Lexeme *l = variable(p);
        if (argsPending(p)) {
            Lexeme *r = args(p);
            return cons(FN_CALL, l, r);
        }
        return l;
    } else if (check(p, OPAREN)) {
        Lexeme *l = match(p, OPAREN);
        l->right = expression(p);
        match(p, CPAREN);
        if (argsPending(p)) {
            Lexeme *r = args(p);
            return cons(FN_CALL, l, r);
        }
        return l;
    } else if (check(p, MINUS)) {
        Lexeme *l = match(p, MINUS);
        l->right = unary(p);
        return l;
    } else if (check(p, DELAY)) {
        Lexeme *l = match(p, DELAY);
        l->right = unary(p);
        return l;
    } else {
        Lexeme *l = match(p, NOT);
        l->right = unary(p);
        return l;
    }
}

int unaryPending(Parser *p) {
    return check(p, NUMBER) || check(p, STRING) || variablePending(p) || check(p, OPAREN)
        || check(p, MINUS) || check(p, NOT) || check(p, DELAY);
}

Lexeme *variable(Parser *p) {
    Lexeme *l = match(p, ID);
    if (check(p, DOT)) {
        Lexeme *dot, *r;
        dot = match(p, DOT);
        r = variable(p);
        dot->left = l;
        dot->right = r;
        return dot;
    }
    return l;
}

int variablePending(Parser *p) {
    return check(p, ID);
}

Lexeme *args(Parser *p) {
    match(p, OPAREN);
    Lexeme *l = optExpressionList(p);
    match(p, CPAREN);
    return l;
}

int argsPending(Parser *p) {
    return check(p, OPAREN);
}

Lexeme *optExpressionList(Parser *p) {
    if (expressionListPending(p)) {
        return expressionList(p);
    }
    return NULL;
}

Lexeme *expressionList(Parser *p) {
    Lexeme *l, *r = NULL;
    l = expression(p);
    if (check(p, COMMA)) {
        match(p, COMMA);
        r = expressionList(p);
    }
    return cons(EXPRESSION_LIST, l, r);
}

int expressionListPending(Parser *p) {
    return expressionPending(p);
}

Lexeme *block(Parser *p) {
    if (check(p, OBRACE)) {
        match(p, OBRACE);
        Lexeme *l = statementList(p);
        match(p, CBRACE);
        return l;
    } else {
        return statement(p);
    }
}

Lexeme *conditional(Parser *p) {
    match(p, OPAREN);
    Lexeme *l = expression(p);
    match(p, CPAREN);
    return l;
}

Lexeme *ifStatement(Parser *p) {
    Lexeme *l, *cond, *blk, *el;
    l = match(p, IF);
    cond = conditional(p);
    blk = block(p);
    el = optElse(p);
    l->left = cons(CONS, cond, blk);
    l->right = el;
    return l;
}

int ifStatementPending(Parser *p) {
    return check(p, IF);
}

Lexeme *optElse(Parser *p) {
    if (check(p, ELSE)) {
        match(p, ELSE);
        return block(p);
    }
    return NULL;
}

Lexeme *whileLoop(Parser *p) {
    Lexeme *l = match(p, WHILE);
    l->left = conditional(p);
    l->right = block(p);
    return l;
}

int whileLoopPending(Parser *p) {
    return check(p, WHILE);
}

Lexeme *variableDef(Parser *p) {
    Lexeme *l = match(p, VAR);
    l->left = variable(p);
    match(p, ASSIGN);
    l->right = expression(p);
    match(p, SEMI);
    return l;
}

int variableDefPending(Parser *p) {
    return check(p, VAR);
}

Lexeme *functionDef(Parser *p) {
    Lexeme *l, *r, *params, *body;
    l = match(p, DEF);
    l->left = variable(p);
    match(p, OPAREN);
    params = optIdList(p);
    match(p, CPAREN);
    body = block(p);
    r = cons(CONS, params, body);
    l->right = r;
    return l;
}

int functionDefPending(Parser *p) {
    return check(p, DEF);
}

Lexeme *optIdList(Parser *p) {
    if (idListPending(p)) {
        return idList(p);
    }
    return NULL;
}

Lexeme *idList(Parser *p) {
    Lexeme *l, *r = NULL;
    l = match(p, ID);
    if (check(p, COMMA)) {
        match(p, COMMA);
        r = idList(p);
    }
    return cons(ID_LIST, l, r);
}

int idListPending(Parser *p) {
    return check(p, ID);
}
