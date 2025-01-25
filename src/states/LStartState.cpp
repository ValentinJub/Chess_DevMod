#include "LStartState.h"

LStartState::LStartState() {
    
}

void LStartState::enter() {
	this->init();
	Mix_PlayChannel(-1, mStartupSound, 0);
}

void LStartState::exit() {
	this->free();
}

void LStartState::update() {
	SDL_Event e;
	while(SDL_PollEvent(&e) > 0) {
		if(e.type == SDL_QUIT) {
			gStateMachine->pop();
		}
	}
	if(!Mix_Playing(-1)) {
		gStateMachine->pop();
	}
}

void LStartState::render() {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		gBackgroundTexture.render();
		mTitleTexture[TITLE].render((SCREEN_WIDTH - mTitleTexture[TITLE].getWidth()) / 2, (SCREEN_HEIGHT - mTitleTexture[TITLE].getHeight()) /2);
		mTitleTexture[AUTHOR].render(SCREEN_WIDTH - mTitleTexture[AUTHOR].getWidth(), SCREEN_HEIGHT - mTitleTexture[AUTHOR].getHeight());
		SDL_RenderPresent(gRenderer);
        SDL_Delay(16);
}

void LStartState::free() {
    Mix_FreeChunk(mStartupSound);
    mStartupSound = NULL;
    mTitleTexture[TITLE].free();
    mTitleTexture[AUTHOR].free();
}

bool LStartState::init() {
	mStartupSound = Util::loadChunk(CHUNK_TITLE);
	return this->loadTitle();
}

bool LStartState::loadTitle() {
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

