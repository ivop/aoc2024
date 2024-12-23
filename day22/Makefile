CFLAGS=-O3 -g3 -march=native -Wall -Wextra

all: p1 p2 p2mt p2b p2bmt

%: %.c
	$(CC) $(CFLAGS) -o $@ $^

%: %.cpp
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm -f p1 p2 p2mt p2b *~
