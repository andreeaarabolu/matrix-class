make: Matrix.o main.o test.o
	g++ Matrix.o main.o test.o -o matrix-class && ./matrix-class
Matrix.o: Matrix.hpp Matrix.cpp
	g++ Matrix.hpp Matrix.cpp -c 
main.o: main.cpp
	g++ main.cpp -c
test.o: test.cpp test.hpp
	g++ test.cpp test.hpp -c
