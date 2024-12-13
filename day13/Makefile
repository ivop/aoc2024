CFLAGS=-O3 -march=native -Wall -Wextra

all: part1 both

part1: part1.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

both: both.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean:
	rm -f part1 both *~
