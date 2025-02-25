#include "LStartState.h"

extern TTF_Font* gFont64; 
extern TTF_Font* gFont32;
extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;
extern LMediaFactory* gMediaFactory;
extern LChunkPlayer* gChunkPlayer;
extern LStateMachine* gStateMachine;

using tweeny::easing;

LStartState::LStartState() {
	try {
		mLogger = spdlog::basic_logger_mt("LStartState", "logs/CApp.log");
	}
	catch(const spdlog::spdlog_ex& ex) {
		std::cerr << "Log init failed: " << ex.what() << std::endl;
	}
	mAlpha = 0;
	mRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
};

void LStartState::enter(LObserver* observer) {
	mLogger->info("Entering Start State");
	mObserver = observer;
	this->Attach(observer);
	this->init();
	gChunkPlayer->play(CHUNK_TITLE);
}

void LStartState::exit() {
	mLogger->info("Exiting Start State");
	this->Detach(mObserver);
	this->free();
}

void LStartState::update(Uint64 dt) {
	SDL_Event e;
	while(SDL_PollEvent(&e) > 0) {
		if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
			mLogger->info("Detected quit signal");
			this->Notify();
			gStateMachine->pop();
		}
	}
	// The title screen will only be displayed while the music is playing
	if(!gChunkPlayer->isPlaying()) {
		if(mAlpha == 0) {
			mTween = tweeny::from(0).to(255).during(2000).via(easing::linear).onStep([this](int alpha) {
				mAlpha = alpha;
				return false;
			});
		}
		Uint32 dt32 = static_cast<Uint32>(dt);
    	mTween.step(dt32);
		if(mAlpha == 255) {
			gStateMachine->pop();
			gStateMachine->push(new LMainMenuState);
		}
	}
}

void LStartState::render() {
		for(int i = 0; i < TOTAL_TITLE_ITEMS; i++) {
			mTitleTexture[i]->render();
		}
		if(mAlpha > 0) {
			SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, mAlpha);
			SDL_RenderFillRect(gRenderer, &mRect);
		}
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
	mTitleTexture[TITLE]->setPos(
		(SCREEN_WIDTH - mTitleTexture[TITLE]->w()) / 2,
		(SCREEN_HEIGHT - mTitleTexture[TITLE]->h()) / 2
	);
	mTitleTexture[AUTHOR] = gMediaFactory->getTxt(TITLE_AUTHOR_STR, gFont32, COLOR_BLACK);
	mTitleTexture[AUTHOR]->setPos(
		SCREEN_WIDTH - mTitleTexture[AUTHOR]->w(),
		SCREEN_HEIGHT - mTitleTexture[AUTHOR]->h()
	);
}

