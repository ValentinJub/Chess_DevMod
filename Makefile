CXX = g++

CFLAGS = -Wall -std=c++11

CPPFLAGS = -I src/include


all:
	$(CXX) $(CFLAGS) $(CPPFLAGS) src/*.cpp -o main -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 