all : warchest
	./warchest

warchest : src/warchest.cpp src/warchest.hpp src/board.o 
	g++ -std=c++17 -Wall -O2 -o warchest src/warchest.cpp src/board.o 

board : src/board.cpp src/board.hpp
	g++ -std=c++17 -Wall -O2 -c src/board.cpp

clean :
	rm src/*.o warchest