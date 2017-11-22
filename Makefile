CC = g++
CFLAGS = -Wall -O2
SRCS := $(wildcard *.cpp)
TARGET := $(patsubst %.cpp, %, $(SRCS))

all: $(TARGET) clean
gen: gen.o
	$(CC) $(CFLAGS) -o $@ $<
calc: calc.o
	$(CC) $(CFLAGS) -o $@ $<
atk: atk.o
	$(CC) $(CFLAGS) -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c $@ $<
clean:
	rm -rf *.o
