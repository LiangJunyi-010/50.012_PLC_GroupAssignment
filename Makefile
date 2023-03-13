CXX = g++
testFLAGS = -ansi -pedantic -Wall -Werror

assign1: part1 part2 part3

part1: part1.o
	gcc part1.o -o part1

part1.o: part1.c
	gcc -c part1.c $(testFLAGS) -o part1.o

part2: part2.o
	$(CXX) part2.o -o part2

part2.o: part2.cpp
	$(CXX) -c part2.cpp -o part2.o

part3: part3.o
	$(CXX) part3.o -o part3

part3.o: part3.cpp
	$(CXX) -c part3.cpp -o part3.o

clean:
	rm -rf *.o part1 part2 part3

