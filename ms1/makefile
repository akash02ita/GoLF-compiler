.PHONY: clean

def: scanner.c
	gcc scanner.c -o golf && cp golf tests
scanner.c: scanner.l
	flex -o scanner.c scanner.l
	
clean:
	-rm scanner.c golf tests/golf
