CXX = g++

CFLAGS = -Wall -std=c++14 -ferror-limit=100

LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lfmt -lspdlog

CPPFLAGS = -I/Users/valentin/Dev/Chess_DevMod/src \
           -I/usr/local/Cellar/fmt/11.1.3/include \
           -I/usr/local/Cellar/spdlog/1.15.1/include \
           -I/usr/local/include/tweeny/include

BUILDDIR = build
PROG = main

SRCS = src/*.cpp src/com/*.cpp src/sound/*.cpp src/states/*.cpp src/states/game/*.cpp src/states/menu/*.cpp src/menu/*.cpp src/utils/*.cpp src/factories/*.cpp src/graphics/*.cpp src/game/board/*.cpp src/game/logic/*.cpp

all:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(SRCS) -o $(BUILDDIR)/$(PROG) $(LDFLAGS)
	