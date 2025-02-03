#include "LStartState.h"

extern TTF_Font* gFont64; 
extern TTF_Font* gFont32;
extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;
extern LMediaFactory* gMediaFactory;
extern LChunkPlayer* gChunkPlayer;
extern LStateMachine* gStateMachine;

LStartState::LStartState() {};

void LStartState::enter(LObserver* observer) {
	mObserver = observer;
	this->Attach(observer);
	this->init();
	gChunkPlayer->play(CHUNK_TITLE);
}

void LStartState::exit() {
	this->Detach(mObserver);
	this->free();
}

void LStartState::update() {
	SDL_Event e;
	while(SDL_PollEvent(&e) > 0) {
		if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
			this->Notify();
			gStateMachine->pop();
		}
	}
	// The title screen will only be displayed while the music is playing
	if(!gChunkPlayer->isPlaying()) {
		gStateMachine->pop();
	}
}

void LStartState::render() {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		gBackgroundTexture->render();
		mTitleTexture[TITLE]->render(
			(SCREEN_WIDTH - mTitleTexture[TITLE]->getWidth()) / 2,
			(SCREEN_HEIGHT - mTitleTexture[TITLE]->getHeight()) / 2
		);
		mTitleTexture[AUTHOR]->render(
			SCREEN_WIDTH - mTitleTexture[AUTHOR]->getWidth(),
			SCREEN_HEIGHT - mTitleTexture[AUTHOR]->getHeight()
		);
		SDL_RenderPresent(gRenderer);
}

void LStartState::free() {
	for (int i = 0; i < TOTAL_TITLE_ITEMS; i++) {
		mTitleTexture[i]->free();
	}
}

void LStartState::init() {
	this->loadTexture();
}

void LStartState::loadTexture() {
	mTitleTexture[TITLE] = gMediaFactory->getTxt(TITLE_STR, gFont64, COLOR_BLACK);
	mTitleTexture[AUTHOR] = gMediaFactory->getTxt(TITLE_AUTHOR_STR, gFont32, COLOR_BLACK);
}

