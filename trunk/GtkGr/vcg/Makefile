

all:
	gcc -g -lstdc++ ext_main.cpp lex.yy.c y.tab.c main.c alloc.c timelim.c options.c folding.c step0.c step1.c step2.c step3.c step4.c tree.c prepare.c drawlib.c drawstr.c



lex:
	flex -d --nounistd --noline grammar.l        # lex.yy.c

yacc:
	bison -ydvt --no-lines grammar.y    # y.tab.c y.tab.c

clean:
	rm *.o a.out
