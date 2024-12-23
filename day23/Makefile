CFLAGS=-O3 -march=native -Wall -Wextra

all: both

%: %.cpp
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm -f both *~
