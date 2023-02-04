.PHONY: clean

CXX=g++
CXXFLAGS=-Wall

golf: main.o lex.yy.o
	$(CXX) $(CXXFLAGS) -o golf main.o lex.yy.o && cp golf tests/golf
main.o: main.cpp lex.h
	$(CXX) $(CXXFLAGS) -c main.cpp

lex.yy.o: lex.yy.cc
	$(CXX) $(CXXFLAGS) -c lex.yy.cc

lex.yy.cc: lex.l lex.h
	flex lex.l

clean:
	-rm *.o *.yy.cc golf tests/golf
