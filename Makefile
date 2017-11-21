CC = g++
CFLAGS = -Wall -O2

gen: gen.o
	$(CC) $(CFLAGS) -o gen gen.o

calc: calc.o
	$(CC) $(CFLAGS) -o calc calc.o

atk: atk.o
	$(CC) $(CFLAGS) -o atk atk.o

gen.o: gen.cpp
	$(CC) $(CFLAGS) -c gen.cpp

calc.o: calc.cpp
	$(CC) $(CFLAGS) -c calc.cpp

atk.o: atk.cpp
	$(CC) $(CFLAGS) -c atk.cpp

clean:
	rm gen calc atk gen.o calc.o atk.o
