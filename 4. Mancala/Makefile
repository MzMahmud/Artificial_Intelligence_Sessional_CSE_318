all: run

run: main.out
	./main.out

main.out: main.o state.o
	g++ main.o state.o -o main.out

main.o: main.cpp
	g++ -c main.cpp -o main.o

state.o: state.h state.cpp
	g++ -c state.cpp -o state.o

clean:
	rm -rf *.out *.o
