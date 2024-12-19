CFLAGS=-O3 -march=native -Wall -Wextra -std=c++20

all: both

%: %.cpp
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm -f both *~
