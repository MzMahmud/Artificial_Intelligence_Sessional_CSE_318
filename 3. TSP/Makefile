all: run

run: main.out
	./main.out

main.out:  main.o node.o
	g++ main.o node.o -o main.out

main.o: main.cpp node.o
	g++ -c main.cpp -o main.o

node.o: node.h node.cpp
	g++ -c node.cpp -o node.o

clean:
	rm -rf *.out *.o
