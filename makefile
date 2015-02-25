# *****************************************************************************
# *****************************************************************************
# makefile
# *****************************************************************************
# Charles Santos
# 931-928-349
# CS480 W15
# *****************************************************************************
# *****************************************************************************



CCC = g++
CCFLAGS = -fmax-errors=10 -Wall -std=gnu++11
OBJS = comString.o token.o symbolTable.o state.o lexer.o grammar.o parser.o global.o tu.o translator.o
SOURCE = lexer.cpp state.cpp comString.cpp symboltable.cpp token.cpp grammar.cpp parser.cpp global.cpp tu.cpp translator.cpp
RUNFLAGS =

$(OBJS): $(SOURCE)
	$(CCC) $(CCFLAGS) -c $(SOURCE)

compiler: $(OBJS)
	$(CCC) $(CCFLAGS) -o compiler $(OBJS)

clean:
	rm -f *.o core *.exe *.out *.stackdump
	ls

stutest.out: compiler
	cat stutest1.in
	-rm -f tree.out
	-./compiler $(RUNFLAGS) -b mod.grammar stutest1.in > stutest1.out 2>&1
	cat stutest1.out
	-cat tree.out >> stutest1.out

#	cat bad.ibtl
	-rm -f tree.out
	-./compiler -b mod.grammar bad.ibtl > stutest2.out
	cat stutest2.out
	-cat tree.out >> stutest2.out

#	cat test.ibtl
	-rm -f tree.out
	-./compiler -b mod.grammar test.ibtl > stutest3.out
	cat stutest3.out
	-cat tree.out >> stutest3.out

debug: compiler
	cat stutest2.in
	-./compiler $(RUNFLAGS) stutest2.in > debug.out
	cat debug.out

fsa: compiler
	./compiler -f
	cat fsa.out

grammar: compiler
	./compiler -g

builder: compiler
	./compiler -g -b mod.grammar
#	cat grammar.out

tree: compiler
	./compiler -t
	cat source.out >> gen.out
	rm -f source.out

gperfect: compiler given.grammar
	./compiler -t -b given.grammar
	echo "" >> test.ibtl
	cat source.out >> test.ibtl

tperfect: compiler test.ibtl
	./compiler -b mod.grammar test.ibtl

dtest: compiler stutest2.in
	./compiler -b mod.grammar stutest2.in

proftest.out: compiler
	cat $(PROFTEST)
	compiler $(PROFTEST) > proftest.out
	cat proftest.out