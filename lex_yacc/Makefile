

all:
	gcc -g ext_main.cpp lex.yy.c y.tab.c main.c alloc.c timelim.c options.c folding.c step0.c -lstdc++ 



lex:
	flex -d grammar.l        # lex.yy.c

yacc:
	bison -ydvt grammar.y    # y.tab.c y.tab.c

clean:
	rm *.o a.out
