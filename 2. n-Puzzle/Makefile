all: n_puzzle

n_puzzle: state.o main.o
	g++ main.o state.o -o n_puzzle.out

state.o: State.cpp state.h
	g++ -c State.cpp -o state.o

main.o: main.cpp state.h state.o
	g++ -c main.cpp -o main.o

clean:
	rm -rf *.o *.out out.txt
