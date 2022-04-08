TARGET = ./task.out

.PHONY: all build run clean

all: build run clean

build:
	$(CXX) -g -fdiagnostics-color -Wall -Wextra -Werror -o $(TARGET) $(SRC)

run:
	$(TARGET)

clean:
	rm *.out

memtest: build
	valgrind $(TARGET)
	rm *.out