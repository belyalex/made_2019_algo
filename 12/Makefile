#CC = /opt/intel/bin/icc
CC = g++
ccflags = -c -O2 -fno-stack-limit -x c++ -std=c++11

all: main

main: main.o
	$(CC) main.o -o main

main.o: main.cpp
	$(CC) $(ccflags) main.cpp

clean:
	rm -f *.o main