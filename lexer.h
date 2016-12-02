#ifndef LEXER_H
#define LEXER_H

#include "lexeme.h"
#include "parser.h"

Lexeme* lex(Parser *p);
Lexeme* lexNumber(char ch, Parser *p);
Lexeme* lexString(Parser *p);
Lexeme* lexID(char ch, Parser *p);

char getChar(Parser *p);
char skipWhitespace(Parser *p);

#endif
