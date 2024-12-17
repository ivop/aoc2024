CFLAGS=-O3 -march=native -Wall -Wextra

all: both vis

%: %.cpp
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm -f vis both *~
