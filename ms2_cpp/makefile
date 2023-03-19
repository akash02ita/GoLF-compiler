.PHONY : clean

CC := g++
CCFLAGS := -std=c++14 -Wall

# note: you must set global.h in PARSER_MODE! SCANNER_MODE not supported here
golf : main.o parse.tab.o lex.yy.o
	$(CC) $(CCFLAGS) $^ -o golf
main.o: main.cpp parse.tab.h lex.h
lex.yy.cc: lex.l parse.tab.h lex.h
	flex lex.l
parse.tab.c parse.tab.h: parse.y
	bison -d parse.y
clean:
	-rm lex.yy.cc parse.tab.* location.hh *.o golf