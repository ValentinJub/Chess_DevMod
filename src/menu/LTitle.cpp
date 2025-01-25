#include "LTitle.h"

LTitle::LTitle() {
    mStartupSound = Util::loadChunk(CHUNK_TITLE);
}

void LTitle::free() {
    Mix_FreeChunk(mStartupSound);
    mStartupSound = NULL;
    mTitleTexture[TITLE].free();
    mTitleTexture[AUTHOR].free();
}

bool LTitle::init() {
	return this->loadTitle();
}

bool LTitle::loadTitle() {
	bool success = true;
	if(!(mTitleTexture[TITLE].loadFromRenderedText(gFont64, TITLE_STR.c_str() , COLOR_BLACK))) {
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
    Mix_PlayChannel(-1, mStartupSound, 0);
    while(Mix_Playing(-1) > 0) {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		gBackgroundTexture.render();
		mTitleTexture[TITLE].render((SCREEN_WIDTH - mTitleTexture[TITLE].getWidth()) / 2, (SCREEN_HEIGHT - mTitleTexture[TITLE].getHeight()) /2);
		mTitleTexture[AUTHOR].render(SCREEN_WIDTH - mTitleTexture[AUTHOR].getWidth(), SCREEN_HEIGHT - mTitleTexture[AUTHOR].getHeight());
		SDL_RenderPresent(gRenderer);
        SDL_Delay(16);
    }
}