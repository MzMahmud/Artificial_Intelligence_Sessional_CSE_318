project: n_puzzle

n_puzzle: state main
	g++ main.o state.o -o n_puzzle.out

state: State.cpp
	g++ -c State.cpp -o state.o

main: main.cpp state.o
	g++ -c main.cpp -o main.o

clean:
	rm -rf *.o *.out out.txt
