.PHONY: clean transfer_to_ms2

CXX=g++
CXXFLAGS=-Wall

golf: main.o lex.yy.o
	$(CXX) $(CXXFLAGS) -o golf main.o lex.yy.o && cp golf tests/golf
main.o: main.cpp lex.h global.h
	$(CXX) $(CXXFLAGS) -c main.cpp

lex.yy.o: lex.yy.cc
	$(CXX) $(CXXFLAGS) -c lex.yy.cc

lex.yy.cc: lex.l lex.h global.h
	flex lex.l

clean:
	-rm *.o *.yy.cc golf tests/golf

transfer_to_ms2_cpp: lex.h lex.l # transfer files to milestone 2
	@echo "\t\tattempting transfer to ms2_cpp folder" && sleep 1 && echo
	ls ../ms2_cpp
	cp lex.h lex.l ../ms2_cpp
	@echo "\n\t\ttransfer SUCCESSFUL. See contents at ../ms2_cpp" && sleep 1 && echo
	ls ../ms2_cpp