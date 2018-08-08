

all:
	gcc -g -lm ext_main.cpp $(wildcard *.c)



lex:
	flex -d --nounistd --noline grammar.l        # lex.yy.c

yacc:
	bison -ydvt --no-lines grammar.y    # y.tab.h y.tab.c y.output

.PHONY: clean
clean:
	rm *.o a.out
