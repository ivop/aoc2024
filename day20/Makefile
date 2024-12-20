CFLAGS=-O3 -g3 -march=native -Wall -Wextra -Wno-char-subscripts

all: both

%: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f both *~
