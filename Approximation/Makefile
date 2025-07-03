OBJS = functions.o readFile.o structDFA.o DFA.o learnDFA.o main.o
CXXFLAGS = -pedantic -ansi -std=c++20 -I ./eigen-3.4.0 
# -Wall

make: $(OBJS) 
	g++ $(OBJS) -o main

clean:
	rm -f *.o
	rm -f main
	rm -f Matrix
	rm -f readFile
	rm -f *.gch
