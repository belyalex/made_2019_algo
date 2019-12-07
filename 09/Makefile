CC = g++
ccflags = -c -std=c++14 -O2

all: main

main: main.o
	$(CC) main.o -o main

main.o: main.cpp Huffman.h
	$(CC) $(ccflags) main.cpp

clean:
	rm -f *.o main