#ifndef PARSER_H
#define PARSER_H

typedef struct Parser {
    FILE *in;
    int line;
    int col;
    int charSaved;
    char savedChar;
    Lexeme *currentLexeme;
} Parser;

Parser* newParser(const char* fileName);

void saveChar(char ch, Parser *p);

int check(Parser *p, const char *type);
void advance(Parser *p);
Lexeme *match(Parser *p, const char *type);
void matchNoAdvance(Parser *p, const char *type);

Lexeme *parse(Parser *p);

Lexeme *statementList(Parser *p);
int statementListPending(Parser *p);
Lexeme *statement(Parser *p);
int statementPending(Parser *p);
Lexeme *expression(Parser *p);
int expressionPending(Parser *p);
Lexeme *operator(Parser *p);
int operatorPending(Parser *p);
Lexeme *unary(Parser *p);
int unaryPending(Parser *p);
Lexeme *variable(Parser *p);
int variablePending(Parser *p);
Lexeme *args(Parser *p);
int argsPending(Parser *p);
Lexeme *optExpressionList(Parser *p);
Lexeme *expressionList(Parser *p);
int expressionListPending(Parser *p);
Lexeme *block(Parser *p);
Lexeme *conditional(Parser *p);
Lexeme *ifStatement(Parser *p);
int ifStatementPending(Parser *p);
Lexeme *optElse(Parser *p);
Lexeme *whileLoop(Parser *p);
int whileLoopPending(Parser *p);
Lexeme *variableDef(Parser *p);
int variableDefPending(Parser *p);
Lexeme *functionDef(Parser *p);
int functionDefPending(Parser *p);
Lexeme *lambda(Parser *p);
int lambdaPending(Parser *p);
Lexeme *optIdList(Parser *p);
Lexeme *idList(Parser *p);
int idListPending(Parser *p);

#endif
