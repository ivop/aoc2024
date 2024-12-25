CFLAGS=-O0 -g3 -march=native -Wall -Wextra

all: both

%: %.cpp
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm -f both *~
