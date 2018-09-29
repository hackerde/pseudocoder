graphics: main.o Block.o
	g++ main.o Block.o -o PseudoCoder -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp

Block.o:  Block.cpp Block.h
	g++ -g -Wall -std=c++11 -c Block.cpp

clean:
	rm *.o