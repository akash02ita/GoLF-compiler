test:
	bison -Wcounterexamples -d parse.y
	flex lex.l
	gcc -Wall -c parse.tab.c
	gcc -Wall -c lex.yy.c
	gcc -Wall -c main.c
	gcc -Wall *.o

clean:
	rm parse.tab.* lex.yy.* *.o a.out