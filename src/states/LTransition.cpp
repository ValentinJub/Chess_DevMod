#include "states/LTransition.h"

extern TTF_Font* gFont64; 
extern TTF_Font* gFont32;
extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;
extern LMediaFactory* gMediaFactory;
extern LStateMachine* gStateMachine;

using tweeny::easing;

LTransition::LTransition(TransitionState ts, LState* nextState) : mNextState(nextState), mTransitionState(ts) {
	try {
		mLogger = spdlog::basic_logger_mt("LTransition", "logs/CApp.log");
	}
	catch(const spdlog::spdlog_ex& ex) {
		std::cerr << "Log init failed: " << ex.what() << std::endl;
	}
};

void LTransition::enter(LObserver* observer) {
    mAppObserver = observer;
    mRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    switch(mTransitionState) {
        case FADE_OUT:
            mAlpha = 0;
            mTween = tweeny::from(0).to(255).during(2000).via(easing::linear).onStep([this](int alpha) {
                mAlpha = alpha;
                return false;
            });
            break;
        case FADE_IN:
            mAlpha = 255;
            mTween = tweeny::from(255).to(0).during(2000).via(easing::linear).onStep([this](int alpha) {
                mAlpha = alpha;
                return false;
            });
            break;
    }
    
}

void LTransition::exit() {
    mLogger->info("Exiting LTransition State");
    spdlog::drop("LTransition");
}

void LTransition::update(Uint64 dt) {
    Uint32 dt32 = static_cast<Uint32>(dt);
    mTween.step(dt32);
    const int ts = mTransitionState == FADE_OUT ? 255 : 0;
    if(mAlpha == ts) {
        gStateMachine->pop();
        if(mNextState != NULL) {
            gStateMachine->pop();
            gStateMachine->push(mNextState);
        }
        return;
    }
	SDL_Event e;
	while(SDL_PollEvent(&e) > 0) {
		if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
			mLogger->info("Detected quit signal");
			this->Notify(0);
			gStateMachine->pop();
		}
	}
}

void LTransition::render() {
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, mAlpha);
    SDL_RenderFillRect(gRenderer, &mRect);
}





	


