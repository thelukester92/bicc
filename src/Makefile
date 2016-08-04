CC=g++-5
CFLAGS=-Wall -fopenmp -std=c++11

all:
	$(CC) -O3 -o ../bin/bicc $(CFLAGS) *.cpp

dbg:
	$(CC) -g -o ../bin/bicc $(CFLAGS) *.cpp

clean:
	rm -rf ../bin/*
