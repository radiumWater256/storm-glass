main:main.o invMan.o
	g++ main.o invMan.o -o main

main.o:main.cpp invMan.h
	g++ -c -std=c++11 main.cpp

invMan.o:invMan.cpp invMan.h
	g++ -c -std=c++11 invMan.cpp

.PHONY: clean
clean:
	rm *.o