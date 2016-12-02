OBJS=env.o error.o eval.o lexeme.o lexer.o parser.o prettyPrint.o types.o 
FLAGS=-Wall
rat: $(OBJS) rat.c
	gcc $(FLAGS) $(OBJS) rat.c -o rat

lex: $(OBJS) scanner.c
	gcc $(FLAGS) $(OBJS) scanner.c -o lex

env.o: env.c
	gcc $(FLAGS) -c env.c

eval.o: eval.c
	gcc $(FLAGS) -c eval.c

error.o: error.c
	gcc $(FLAGS) -c error.c

lexeme.o: lexeme.c types.c
	gcc $(FLAGS) -c lexeme.c

lexer.o: error.c lexeme.c lexer.c parser.c types.c
	gcc $(FLAGS) -c lexer.c

parser.o: parser.c
	gcc $(FLAGS) -c parser.c

prettyPrint.o: prettyPrint.c
	gcc $(FLAGS) -c prettyPrint.c

types.o: types.c
	gcc $(FLAGS) -c types.c

clean:
	rm *.o rat

error1:
	cat error1.rat

error1x:
	./rat error1.rat

error2:
	cat error2.rat

error2x:
	./rat error2.rat

error3:
	cat error3.rat

error3x:
	./rat error3.rat

arrays:
	cat arrays.rat

arraysx:
	./rat arrays.rat

conditionals:
	cat conditionals.rat

conditionalsx:
	./rat conditionals.rat

recursion:
	cat recursion.rat

recursionx:
	./rat recursion.rat

iteration:
	cat iteration.rat

iterationx:
	./rat iteration.rat

functions:
	cat functions.rat

functionsx:
	./rat functions.rat

problem:
	cat problem.rat

problemx:
	./rat problem.rat

grad:
	cat grad.rat

gradx:
	./rat grad.rat
