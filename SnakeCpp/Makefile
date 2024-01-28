all : main

main : main.o
	g++ $(CFLAGS) -o snake main.o -std=c++17 -lncurses

main.o : main.cpp
	g++ $(CFLAGS) -c main.cpp -std=c++17

release: release.o
	g++ $(CFLAGS) -o snake main.o -std=c++17 -lncurses -O2

release.o: 
	g++ $(CFLAGS) -o snake main.cpp -std=c++17 -lncurses -O2

clean_build :
	rm -f *.o

clean : clean_build
	rm snake
