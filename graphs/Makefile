.PHONY: all check build_tests run_tests clean

all: check build_tests memtest

check:
	./run_linters.sh

build_tests:
	./build.sh

run_tests:
	./build/GraphsTests

clean:
	rm -rf build/

memtest: build_tests
	valgrind --error-exitcode=1 ./build/GraphsTests