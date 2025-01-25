#include "util.h"

Mix_Music* Util::loadMusic(const char *src) {
	Mix_Music* music = Mix_LoadMUS(src);
	if(music == NULL) {
		printf( "Failed to load music %s! SDL_mixer Error: %s\n", src, Mix_GetError() );
	}
	return music;
}

Mix_Chunk* Util::loadChunk(const char *src) {
	Mix_Chunk* chunk = Mix_LoadWAV(src);
	if(chunk == NULL) {
		printf( "Failed to load chunk sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	return chunk;
}

bool Util::showTitleScreen(int argc, char *argv[]) {
	bool showTitle = true;
	if(argc > 1) {
		for (int i = 0; i < argc; i++) {
			if(strcmp(argv[i], "-h") == 0) {
				showTitle = false;
			}
		}
		
	}
	return showTitle;
}
