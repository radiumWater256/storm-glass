main:main.o invman.o
	g++ main.o invman.o -o main

main.o:main.cpp invman.h
	g++ -c -std=c++11 main.cpp

invman.o:invman.cpp invman.h
	g++ -c -std=c++11 invman.cpp

.PHONY: clean
clean:
	rm *.o