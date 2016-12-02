#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "lexeme.h"
#include "lexer.h"
#include "parser.h"
#include "types.h"

Lexeme* lex(Parser *p) {
    char ch;
    ch = skipWhitespace(p);

    switch(ch) {
        case ';':
            return newLexeme(SEMI);
            break;
        case '(':
            return newLexeme(OPAREN);
            break;
        case ')':
            return newLexeme(CPAREN);
            break;
        case '{':
            return newLexeme(OBRACE);
            break;
        case '}':
            return newLexeme(CBRACE);
            break;
        case ',':
            return newLexeme(COMMA);
            break;
        case '.':
            return newLexeme(DOT);
            break;
        case '+':
            return newLexeme(PLUS);
            break;
        case '-':
            return newLexeme(MINUS);
            break;
        case '*':
            return newLexeme(TIMES);
            break;
        case '/':
            return newLexeme(DIVIDE);
            break;
        case '&':
            return newLexeme(AND);
            break;
        case '|':
            return newLexeme(OR);
            break;
        case '!':
            return newLexeme(NOT);
            break;
        case '@':
            return newLexeme(DELAY);
            break;
        case '>':
            ch = getChar(p);
            if (ch == '=') {
                return newLexeme(GTE);
            }
            saveChar(ch, p);
            return newLexeme(GT);
            break;
        case '<':
            ch = getChar(p);
            if (ch == '=') {
                return newLexeme(LTE);
            }
            saveChar(ch, p);
            return newLexeme(LT);
            break;
        case '=':
            ch = getChar(p);
            if (ch == '=') {
                return newLexeme(EQUALS);
            }
            saveChar(ch, p);
            return newLexeme(ASSIGN);
            break;
        case EOF:
            return newLexeme(END_OF_FILE);
            break;
        default:
            if (isdigit(ch)) {
                return lexNumber(ch, p);
            } else if (ch == '"') {
                return lexString(p);
            } else {
                return lexID(ch, p);
            }
    }

    //never reached, silence warning
    return NULL;
}

Lexeme* lexNumber(char ch, Parser *p) {
    Lexeme *l = newLexeme(NUMBER);
    int n = ch - '0';

    ch = getChar(p);
    while (isdigit(ch)) {
        n *= 10;
        n += ch - '0';
        ch = getChar(p);
    }

    saveChar(ch, p);

    l->ival = n;
    return l;
}

Lexeme* lexString(Parser *p) {
    Lexeme *l = newLexeme(STRING);
    char ch;
    char buf[1024];
    int len = 0;
    ch = getChar(p);
    while (ch != '"') {
        if (ch == '\\') {
            ch = getChar(p);
            switch (ch) {
                case 'n':
                    buf[len] = '\n';
                    break;
                case '"':
                    buf[len] = '"';
                    break;
            }
        } else if (ch == EOF) {
            fatalError("Unexpected EOF in string");
        } else {
            buf[len] = ch;
        }
        ch = getChar(p);
        len++;
        if (len == 1024) {
            fatalError("Strings are limited to 1024 characters.");
        }
    }

    buf[len] = '\0';

    l->sval = malloc(strlen(buf) + 1);
    strcpy(l->sval, buf);

    return l;
}

Lexeme* lexID(char ch, Parser *p) {
    Lexeme *l = newLexeme(ID);
    char buf[64];
    int len = 1;
    buf[0] = ch;

    ch = getChar(p);
    while (!isspace(ch) && ch != EOF && strchr(SYMBOLS, ch) == 0) {
        buf[len] = ch;
        len++;
        if (len == 64) {
            fatalError("Identifiers can only be 64 charachters long");
        }
        ch = getChar(p);
    }

    buf[len] = '\0';

    saveChar(ch, p);

    if (strcmp(buf, "while") == 0) {
        l->type = WHILE;
    } else if (strcmp(buf, "if") == 0) {
        l->type = IF;
    } else if (strcmp(buf, "else") == 0) {
        l->type = ELSE;
    } else if (strcmp(buf, "def") == 0) {
        l->type = DEF;
    } else if (strcmp(buf, "var") == 0) {
        l->type = VAR;
    } else if (strcmp(buf, "lambda") == 0) {
        l->type = LAMBDA;
    } else {
        l->sval = malloc(strlen(buf) + 1);
        strcpy(l->sval, buf);
    }

    return l;
}

char getChar(Parser *p) {
    if (p->charSaved) {
        p->charSaved = 0;
        return p->savedChar;
    } else {
        char ch = fgetc(p->in);
        p->col++;
        if (ch == '\n') {
            p->line++;
            p->col = 0;
        }
        return ch;
    }
}

char skipWhitespace(Parser *p) {
    char ch = getChar(p);

    while (isspace(ch) || ch == '#') {
        if (ch == '#') {
            while (ch != '\n' && ch != EOF) {
                ch = getChar(p);
            }
            if (ch == EOF) {
                return EOF;
            }
        }
        ch = getChar(p);
    }

    return ch;
}
