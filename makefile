play :
	g++ -std=c++2a -o warchest src/warchest.cpp src/board.cpp
	./warchest

clean :
	rm  warchest