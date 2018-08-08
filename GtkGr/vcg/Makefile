

all:
	gcc -g ext_main.cpp $(wildcard *.c) -lm



lex:
	flex -d --nounistd --noline grammar.l        # lex.yy.c

yacc:
	bison -ydvt --no-lines grammar.y    # y.tab.h y.tab.c y.output

.PHONY: clean
clean:
	rm *.o a.out
