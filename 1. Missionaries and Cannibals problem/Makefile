all: final 

final: state.o main.o
	g++ -c main.o state.o -o missionaries_cannibals.out

state.o: State.cpp state.h
	g++ -c State.cpp -o state.o

main.o: main.cpp state.h
	g++ -c main.cpp -o main.o
	
clean:
	rm -rf *.o *.out