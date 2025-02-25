#ifndef LTRANSITION_H
#define LTRANSITION_H

#include "utils/util.h"
#include "states/LState.h"
#include "states/LStateMachine.h"
#include "factories/LMediaFactory.h"
#include "graphics/LTexture.h"
#include "tweeny/include/tweeny.h"
#include "com/logger.h"
#include "com/LSubject.h"

extern TTF_Font* gFont64; 
extern TTF_Font* gFont32;
extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;
extern LMediaFactory* gMediaFactory;
extern LStateMachine* gStateMachine;
extern uint8_t gMusicVolume;

enum TransitionState {
    FADE_IN,
    FADE_OUT
};

class LTransition : public LState, public LSubject {

public:
    LTransition(TransitionState ts, LState* nextState);
    void enter(LObserver* observer);
    void exit();
    void update(Uint64 dt);
    void render();

private:
    SDL_Rect mRect; // rectangle to cover the screen
    int mAlpha; // alpha value for the rectangle rendering
    tweeny::tween<int> mTween;
    std::shared_ptr<spdlog::logger> mLogger;
    LObserver* mAppObserver;
    LState* mNextState;
    TransitionState mTransitionState;
};


#endif