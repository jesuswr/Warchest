play :
	g++ -std=c++2a -o warchest src/warchest.cpp src/board.cpp tests/test_board.hpp
	./warchest

test :
	@echo "It takes a bit to compile the test library"
	g++ -std=c++2a -o test_warchest tests/test_board.cpp tests/catch_amalgamated.cpp src/board.cpp
	./test_warchest
	rm test_warchest

clean :
	rm  warchest