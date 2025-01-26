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
		if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
			gStateMachine->pop();
		}
	}
	// The title screen will only be displayed while the music is playing
	if(!Mix_Playing(-1)) {
		gStateMachine->pop();
	}
}

void LStartState::render() {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		gBackgroundTexture->render();
		mTitleTexture[TITLE]->render(
			(SCREEN_WIDTH - mTitleTexture[TITLE]->getWidth()) / 2,
			(SCREEN_HEIGHT - mTitleTexture[TITLE]->getHeight()) /2
		);
		mTitleTexture[AUTHOR]->render(
			SCREEN_WIDTH - mTitleTexture[AUTHOR]->getWidth(),
			SCREEN_HEIGHT - mTitleTexture[AUTHOR]->getHeight()
		);
		SDL_RenderPresent(gRenderer);
}

void LStartState::free() {
    Mix_FreeChunk(mStartupSound);
    mStartupSound = NULL;
    mTitleTexture[TITLE]->free();
    mTitleTexture[AUTHOR]->free();
}

void LStartState::init() {
	this->loadTexture();
	this->loadSound();
}

void LStartState::loadTexture() {
	mTitleTexture[TITLE] = gMediaFactory->getTxt(TITLE_STR, gFont64, COLOR_BLACK);
	mTitleTexture[AUTHOR] = gMediaFactory->getTxt(TITLE_AUTHOR_STR, gFont32, COLOR_BLACK);
}

void LStartState::loadSound() {
	mStartupSound = gMediaFactory->getChunk(CHUNK_TITLE);
}

