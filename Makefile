CXX = g++

CFLAGS = -Dmain=SDL_main -Wall

CPPFLAGS = -I src/include -L src/lib


all:
	$(CXX) $(CFLAGS) $(CPPFLAGS) src/*.cpp -o main -lmingw32 -lSDL2main -lSDL2 -mwindows -lSDL2_image -lSDL2_ttf -lSDL2_mixer -static -static-libgcc -lwinmm -lusp10 -lgdi32 -lrpcrt4 -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid