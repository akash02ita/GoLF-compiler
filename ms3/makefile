# source: jared's code.
.PHONY: clean

CC=gcc
CFLAGS=-g -Wall
OBJ=main.o parse.tab.o lex.yy.o astC.o hashmap.o stack.o
YACC=bison
YFLAGS=-Wall -d
LEX=flex
LFLAGS=

golf: $(OBJ)
	$(CC) $(CFLAGS) -o golf $^
	cp golf TESTS

main.o: main.c parse.tab.h astC.h
hashmap.o: hashmap.c hashmap.h
stack.o: stack.c stack.h

astC.o: astC.c astC.h

lex.yy.c: lex.l parse.tab.h
	$(LEX) $(LFLAGS) $<

parse.tab.c parse.tab.h: parse.y astC.h
	$(YACC) $(YFLAGS) $<

clean:
	-rm *.yy.c
	-rm *.tab.h
	-rm *.tab.c
	-rm *.o
	-rm golf
	-rm TESTS/golf
