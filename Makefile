TARGET = ./task.out
SRCS = ./haffman/Huffman.cpp
HDRS = /haffman/Huffman.h

.PHONY: all check build run clean

all: check build run

check:
	./run_linters.sh $(SRCS)

build: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) -std=c++17 -g -fdiagnostics-color -O0 -Wall -Wextra -I $(HDRS) -o $(TARGET) $(SRCS)

run:
	$(TARGET)

clean:
	rm *.out

memtest: build
	valgrind $(TARGET)
	rm *.out