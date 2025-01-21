#include "util.h"

Mix_Music* loadMusic(const char *src) {
	Mix_Music* music = Mix_LoadMUS(src);
	if(music == NULL) {
		printf( "Failed to load music %s! SDL_mixer Error: %s\n", src, Mix_GetError() );
	}
	return music;
}

Mix_Chunk* loadChunk(const char *src) {
	Mix_Chunk* chunk = Mix_LoadWAV(src);
	if(chunk == NULL) {
		printf( "Failed to load chunk sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	return chunk;
}