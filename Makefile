CXX = g++
CXXFLAGS = -ggdb -std=c++20 -Werror -Wextra -Wfatal-errors -pedantic -I.

all: ladder dijkstras

ladder: src/ladder_main.o src/ladder.o
	$(CXX) $(CXXFLAGS) src/ladder_main.o src/ladder.o -o ladder

dijkstras: src/dijkstras_main.o src/dijkstras.o
	$(CXX) $(CXXFLAGS) src/dijkstras_main.o src/dijkstras.o -o dijkstras

src/ladder_main.o: src/ladder_main.cpp src/ladder.h
	$(CXX) $(CXXFLAGS) -c src/ladder_main.cpp -o src/ladder_main.o

src/ladder.o: src/ladder.cpp src/ladder.h
	$(CXX) $(CXXFLAGS) -c src/ladder.cpp -o src/ladder.o

src/dijkstras_main.o: src/dijkstras_main.cpp src/dijkstras.h
	$(CXX) $(CXXFLAGS) -c src/dijkstras_main.cpp -o src/dijkstras_main.o

src/dijkstras.o: src/dijkstras.cpp src/dijkstras.h
	$(CXX) $(CXXFLAGS) -c src/dijkstras.cpp -o src/dijkstras.o

clean:
	rm -f src/*.o ladder dijkstras