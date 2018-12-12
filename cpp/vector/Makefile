build: *.cpp
	g++ -o test -std=c++14 -g -Wall *.cpp

valtest: build
	valgrind --leak-check=full ./test

test: build
	./test
