TARGET = ./task.out
SRCS = 3/3.cpp

.PHONY: all check build run clean

all: check build run

check:
	./run_linters.sh $(SRCS)

build: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) -std=c++17 -g -fdiagnostics-color -O0 -Wall -Wextra -o $(TARGET) $(SRCS)

run:
	$(TARGET)

clean:
	rm *.out

memtest: build
	valgrind --error-exitcode=1 $(TARGET)
	rm *.out