TARGET = ./task.out
SRC = ./7_3.cpp

.PHONY: all check build run clean

all: check build run

check:
	./run_linters.sh $(SRC)

build:
	$(CXX) -g -fdiagnostics-color -Wall -Wextra -Werror -std=c++17 -o $(TARGET) $(SRC)

run:
	$(TARGET)

clean:
	rm *.out

memtest: build
	valgrind $(TARGET)
	rm *.out