CC = gcc
CFLAGS = -O2 -Wall

all: bin/sat_gen

bin/sat_gen: src/sat_gen.c
	mkdir -p bin
	$(CC) $(CFLAGS) src/sat_gen.c -o bin/sat_gen

clean:
	rm -rf bin/sat_gen
