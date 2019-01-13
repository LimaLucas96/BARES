Bares: token.o main.o
	g++ -Wall -std=c++11 token.o main.o -o Bares -lm
	-rm *.o
token.o:
	g++ -std=c++11 -o token.o -c src/token2.cpp
main.o:
	g++ -std=c++11 -o main.o -c src/main.cpp