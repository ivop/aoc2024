CFLAGS=-O0 -g3 -march=native -Wall -Wextra -Wno-char-subscripts

all: both

both: both.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f both *~
