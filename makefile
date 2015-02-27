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
	rm -f *.o core *.exe *.out *.stackdump *.gen generated.ibtl test.ibtl translated.forth
	ls

stutest.out: compiler
	cat stutest1.in
	-rm -f tree.out
	-./compiler $(RUNFLAGS) -b mod.grammar stutest1.in > stutest1.out 2>&1
	cat stutest1.out
	-cat tree.out >> stutest1.out

fsa: compiler
	./compiler -f
	cat fsa.out

grammar: compiler
	./compiler -g

builder: compiler
	./compiler -g -b mod.grammar

gtest: compiler test.ibtl
	./compiler -b mod.grammar test.ibtl

generate: compiler mod.grammar
	./compiler -t -b mod.grammar
	echo "" >> test.ibtl
	cat generated.ibtl >> test.ibtl

dtest: compiler stutest2.in
	./compiler -b mod.grammar stutest2.in

proftest.out: compiler
	cat $(PROFTEST)
	compiler $(PROFTEST) > proftest.out
	cat proftest.out