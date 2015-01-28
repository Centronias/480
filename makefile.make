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
OBJS = comString.o token.o symbolTable.o state.o lexer.o
SOURCE = lexer.cpp state.cpp comString.cpp symboltable.cpp token.cpp
RUNFLAGS = -d

$(OBJS): $(SOURCE)
	$(CCC) $(CCFLAGS) -c $(SOURCE)

compiler: $(OBJS)
	$(CCC) $(CCFLAGS) -o compiler $(OBJS)

clean:
	rm -f *.o core *.exe
	ls

stutest.out: compiler
	cat stutest1.in
	-./compiler $(RUNFLAGS) stutest1.in > stutest1.out
	cat stutest1.out
#	cat stutest2.in
#	-./compiler stutest2.in > stutest2.out
#	cat stutest2.out

proftest.out: compiler
	cat $(PROFTEST)
	compiler $(PROFTEST) > proftest.out
	cat proftest.out