CXX = g++

CFLAGS = -Wall -std=c++11 -ferror-limit=100

CPPFLAGS = -I src/com

all:
	$(CXX) $(CFLAGS) $(CPPFLAGS) src/*.cpp src/com/*.cpp src/menu/*.cpp src/states/*.cpp src/utils/*.cpp -o build/main -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 