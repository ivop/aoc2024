CFLAGS=-O0 -g3 -march=native -Wall -Wextra

all: p1 p2test disasm p2

%: %.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean:
	rm -f p1 p2 p2test disasm *~
