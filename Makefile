CXX = g++
CXXFLAGS = -std=c++11

part3: part3.o
	$(CXX) part3.o -o part3

part3.o: part3.cpp
	$(CXX) -c part3.cpp -o part3.o

clean:
	rm -rf *.o part3

