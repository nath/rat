OBJS=env.o error.o eval.o lexeme.o lexer.o parser.o prettyPrint.o types.o 
rat: $(OBJS) rat.c
	gcc -Wall -g $(OBJS) rat.c -o rat

lex: $(OBJS) scanner.c
	gcc -Wall -g $(OBJS) scanner.c -o lex

env.o: env.c
	gcc -Wall -g -c env.c

eval.o: eval.c
	gcc -Wall -g -c eval.c

error.o: error.c
	gcc -Wall -g -c error.c

lexeme.o: lexeme.c types.c
	gcc -Wall -g -c lexeme.c

lexer.o: error.c lexeme.c lexer.c parser.c types.c
	gcc -Wall -g -c lexer.c

parser.o: parser.c
	gcc -Wall -g -c parser.c

prettyPrint.o: prettyPrint.c
	gcc -Wall -g -c prettyPrint.c

types.o: types.c
	gcc -Wall -g -c types.c

clean:
	rm *.o rat
