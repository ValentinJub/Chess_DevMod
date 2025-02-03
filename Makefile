CXX = g++

CFLAGS = -Wall -std=c++11 -ferror-limit=100

LDFLAGS = -lfmt -lspdlog

CPPFLAGS = -I/Users/valentin/Dev/Chess_DevMod/src \
           -I/usr/local/Cellar/fmt/11.1.3/include \
           -I/usr/local/Cellar/spdlog/1.15.1/include

all:
	$(CXX) $(CFLAGS) $(CPPFLAGS) src/*.cpp src/com/*.cpp src/sound/*.cpp src/states/*.cpp src/menu/*.cpp src/utils/*.cpp src/factories/*.cpp \
	-o build/main \
	-lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer \
	