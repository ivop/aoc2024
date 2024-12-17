CFLAGS=-O3 -march=native -Wall -Wextra -Wno-unused-result

all: p1 disasm p2

%: %.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean:
	rm -f p1 p2 p2test disasm *~
