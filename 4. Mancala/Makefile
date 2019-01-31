all: run

run: main.out
	./main.out

main.out: main.o proc.o state.o
	g++ main.o state.o proc.o -o main.out

state.o: state.h state.cpp
	g++ -c state.cpp -o state.o

proc.o: proc.cpp proc.h
	g++ -c proc.cpp -o proc.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm -rf *.out *.o
