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
OBJS = comString.o token.o symbolTable.o state.o lexer.o grammar.o parser.o global.o
SOURCE = lexer.cpp state.cpp comString.cpp symboltable.cpp token.cpp grammar.cpp parser.cpp global.cpp
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
	-./compiler $(RUNFLAGS) stutest1.in > stutest1.out 2>&1
	cat stutest1.out
#	cat stutest2.in
#	-./compiler stutest2.in > stutest2.out
#	cat stutest2.out

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
	./compiler -g -b Grammar
#	cat grammar.out

tree: compiler
	./compiler -t
	cat source.out >> gen.out
	rm -f source.out

proftest.out: compiler
	cat $(PROFTEST)
	compiler $(PROFTEST) > proftest.out
	cat proftest.out