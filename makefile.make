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
CCFLAGS = -Wall
OBJS = lexer.o state.o string.o symboltable.o token.o vector.o
SOURCE = lexer.cpp state.cpp string.cpp symboltable.cpp token.cpp vector.cpp
RUNFLAGS =

$(OBJS): $(SOURCE)
	$(CCC) $(CCFLAGS) -c $(SOURCE)

compiler: $(OBJS)
	$(CCC) $(CCFLAGS) -o compiler $(OBJS)

clean:
	rm -f *.o core *.exe
	ls

stutest.out: compiler
	cat stutest1.in
	-compiler $(RUNFLAGS) stutest1.in > stutest1.out
	cat stutest1.out
	cat stutest2.in
	-compiler stutest2.in > stutest2.out
	cat stutest2.out

proftest.out: compiler
	cat $(PROFTEST)
	compiler $(PROFTEST) > proftest.out
	cat proftest.out