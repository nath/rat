#include <stdio.h>

#include "lexeme.h"
#include "prettyPrint.h"
#include "types.h"

void indent(int depth) {
    for (int i=0; i<depth; i++) {
        printf("\t");
    }
}

void prettyPrint(Lexeme *pt, int depth) {
    if (pt == NULL) {
        return;
    } else {
        if (pt->type != STATEMENT_LIST) {
            indent(depth);
        }
    }

    if (pt->type == NUMBER) {
        printf("%d", pt->ival);
    } else if (pt->type == STRING) {
        printf("\"%s\"", pt->sval);
    } else if (pt->type == ID) {
        printf("%s", pt->sval);
    } else if (pt->type == STATEMENT_LIST) {
        prettyPrint(pt->left, depth);
        if (pt->left->type != IF && pt->left->type != WHILE && pt->left->type != VAR && pt->left->type != DEF) {
            printf(";");
        }
        printf("\n");
        prettyPrint(pt->right, depth);
    } else if (pt->type == EXPRESSION_LIST || pt->type == ID_LIST) {
        prettyPrint(pt->left, 0);
        if (pt->right) {
            printf(", ");
            prettyPrint(pt->right, 0);
        }
    } else if (pt->type == FN_CALL) {
        prettyPrint(pt->left, 0);
        printf("(");
        prettyPrint(pt->right, 0);
        printf(")");
    } else if (pt->type == VAR) {
        printf("var ");
        prettyPrint(pt->left, 0);
        printf(" = ");
        prettyPrint(pt->right, 0);
        printf(";");
    } else if (pt->type == DEF) {
        printf("def ");
        prettyPrint(pt->left, 0);
        printf("(");
        prettyPrint(pt->right->left, 0);
        printf(") {\n");
        prettyPrint(pt->right->right, depth+1);
        indent(depth);
        printf("}");
    } else if (pt->type == LAMBDA) {
        printf("lambda (");
        prettyPrint(pt->right->left, 0);
        printf(") ");
        if (pt->right->right->type == STATEMENT_LIST) {
            printf("{\n");
            prettyPrint(pt->right->right, depth+1);
            indent(depth);
            printf("}");
        } else {
            prettyPrint(pt->right->right, 0);
            printf(";");
        }
    } else if (pt->type == IF) {
        printf("if (");
        prettyPrint(pt->left->left, 0);
        printf(")");
        if (pt->left->right->type == STATEMENT_LIST) {
            printf(" {\n");
            prettyPrint(pt->left->right, depth+1);
            indent(depth);
            printf("}");
        } else {
            printf("\n");
            prettyPrint(pt->left->right, depth+1);
            printf(";");
        }
        if (pt->right != NULL) {
            printf(" else");
            if (pt->right->left->type == STATEMENT_LIST) {
                printf(" {\n");
                prettyPrint(pt->right->left, depth+1);
                indent(depth);
                printf("}");
            } else {
                printf("\n");
                prettyPrint(pt->right->left, depth+1);
                printf(";");
            }
        }
    } else if (pt->type == WHILE) {
        printf("while (");
        prettyPrint(pt->left, 0);
        printf(")");
        if (pt->right->type == STATEMENT_LIST) {
            printf(" {\n");
            prettyPrint(pt->right, depth+1);
            indent(depth);
            printf("}");
        } else {
            printf("\n");
            prettyPrint(pt->right, depth+1);
            printf(";");
        }
    } else if (pt->type == OPAREN) {
        printf("(");
        prettyPrint(pt->right, 0);
        printf(")");
    } else if (pt->type == DOT) {
        prettyPrint(pt->left, 0);
        printf(".");
        prettyPrint(pt->right, 0);
    } else if (pt->type == PLUS) {
        prettyPrint(pt->left, 0);
        printf("+");
        prettyPrint(pt->right, 0);
    } else if (pt->type == MINUS) {
        prettyPrint(pt->left, 0);
        printf("-");
        prettyPrint(pt->right, 0);
    } else if (pt->type == TIMES) {
        prettyPrint(pt->left, 0);
        printf("*");
        prettyPrint(pt->right, 0);
    } else if (pt->type == DIVIDE) {
        prettyPrint(pt->left, 0);
        printf("/");
        prettyPrint(pt->right, 0);
    } else if (pt->type == EQUALS) {
        prettyPrint(pt->left, 0);
        printf("==");
        prettyPrint(pt->right, 0);
    } else if (pt->type == ASSIGN) {
        prettyPrint(pt->left, 0);
        printf("=");
        prettyPrint(pt->right, 0);
    } else if (pt->type == AND) {
        prettyPrint(pt->left, 0);
        printf("&");
        prettyPrint(pt->right, 0);
    } else if (pt->type == OR) {
        prettyPrint(pt->left, 0);
        printf("|");
        prettyPrint(pt->right, 0);
    } else if (pt->type == NOT) {
        printf("!");
        prettyPrint(pt->right, 0);
    } else if (pt->type == GT) {
        prettyPrint(pt->left, 0);
        printf(">");
        prettyPrint(pt->right, 0);
    } else if (pt->type == LT) {
        prettyPrint(pt->left, 0);
        printf("<");
        prettyPrint(pt->right, 0);
    } else if (pt->type == GTE) {
        prettyPrint(pt->left, 0);
        printf(">=");
        prettyPrint(pt->right, 0);
    } else if (pt->type == LTE) {
        prettyPrint(pt->left, 0);
        printf("<=");
        prettyPrint(pt->right, 0);
    } else {
        printf("%s", pt->type);
    }
}
