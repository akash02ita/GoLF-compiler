.PHONY: clean transfer_to_ms2

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

transfer_to_ms2: lex.h lex.l # transfer files to milestone 2
	@echo "\t\tattempting transfer to ms2 folder" && sleep 1 && echo
	ls ../ms2
	cp lex.h lex.l ../ms2
	@echo "\n\t\ttransfer SUCCESSFUL. See contents at ../ms2" && sleep 1 && echo
	ls ../ms2