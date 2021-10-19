CXX = g++
CPPFLAGS = -g -Wall

all: doublet

doublet: doublet.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf doublet
