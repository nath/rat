OBJS=error.o lexeme.o lexer.o parser.o types.o
prettyPrint: $(OBJS) recognizer.c prettyPrint.c
	gcc -Wall $(OBJS) recognizer.c prettyPrint.c -o rat

lex: $(OBJS) scanner.c
	gcc -Wall $(OBJS) scanner.c -o rat

error.o: error.c
	gcc -Wall -c error.c

lexeme.o: lexeme.c types.c
	gcc -Wall -c lexeme.c

lexer.o: error.c lexeme.c lexer.c parser.c types.c
	gcc -Wall -c lexer.c

parser.o: error.c parser.c
	gcc -Wall -c parser.c

scanner.o: error.c lexeme.c lexer.c parser.c types.c
	gcc -Wall -c scanner.c

types.o: types.c
	gcc -Wall -c types.c

clean:
	rm *.o rat
