.PHONY: all

all: sol1.out sol2.out


sol1.out: sol1.l
	flex -o sol1.yy.c sol1.l
	gcc -g sol1.yy.c -o sol1.out

sol2.out: sol2.l
	flex -o sol2.yy.c sol2.l
	gcc -g sol2.yy.c -o sol2.out
	

clean:
	-rm *.out *.yy.c