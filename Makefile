CC      = g++
CFLAGS  = --std=c++11
CINCLUDES = -I.

all: MemoryManagement_test clean

MemoryManagement_test: memoryManagement.o test.o
	$(CC) -o $@ $^ $(LDFLAGS)

test.o: test.cpp
	$(CC) -c $(CFLAGS) $<

memoryManagement.o: memoryManagement.cpp memoryManagement.hpp
	$(CC) -c $(CFLAGS) $(CINCLUDES) $<

.PHONY: clean cleanest

clean:
	rm -f *.o *.gch

cleanest: clean
	rm -f MemoryManagement_test
