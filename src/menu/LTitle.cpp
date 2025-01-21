#include "LTitle.h"

LTitle::LTitle() {
    mStartupSound = loadChunk("SoundEffects/gc.mp3");
}

void LTitle::free() {
    Mix_FreeChunk(mStartupSound);
    mStartupSound = NULL;
    mTitleTexture[TITLE].free();
    mTitleTexture[AUTHOR].free();
}

bool LTitle::loadTitle() {
	bool success = true;
	gFont64 = TTF_OpenFont( "resources/branda.ttf", 64 );
	gFont32 = TTF_OpenFont( "resources/branda.ttf", 32 );
	
	if( gFont64 == NULL  || gFont32 == NULL) {
			printf( "Failed to load resources/valentin font! SDL_ttf Error: %s\n", TTF_GetError() );
			success = false;
	}
	else if(!(mTitleTexture[TITLE].loadFromRenderedText(gFont64, TITLE_STR.c_str() , COLOR_BLACK))) {
		printf("Unable to load rendered text!");
		success = false;
	}
	else if(!(mTitleTexture[AUTHOR].loadFromRenderedText(gFont32, TITLE_AUTHOR_STR.c_str() , COLOR_BLACK))) {
		printf("Unable to load rendered text!");
		success = false;
	}
	return success;
}

void LTitle::render() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	gBackgroundTexture.render();
	mTitleTexture[TITLE].render((SCREEN_WIDTH - mTitleTexture[TITLE].getWidth()) / 2, (SCREEN_HEIGHT - mTitleTexture[TITLE].getHeight()) /2);
	mTitleTexture[AUTHOR].render(SCREEN_WIDTH - mTitleTexture[AUTHOR].getWidth(), SCREEN_HEIGHT - mTitleTexture[AUTHOR].getHeight());
	SDL_RenderPresent(gRenderer);

    Mix_PlayChannel(-1, mStartupSound, 0);
    while(Mix_Playing(-1) > 0) {
        SDL_Delay(16);
    }
}