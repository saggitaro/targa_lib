INC = -I ./include/

executable: source.o tgaimage.o
	g++ -g source.o tgaimage.o

source.o: ./src/source.cpp ./include/tgaimage.hpp
	g++ -g $(INC) -c ./src/source.cpp

tgaimage.o: ./src/tgaimage.cpp ./include/tgaimage.hpp
	g++ -g $(INC) -c ./src/tgaimage.cpp

clean:
	rm tgaimage.o source.o a.out