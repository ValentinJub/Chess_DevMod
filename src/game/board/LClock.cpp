#include "game/board/LClock.h"

LClock::LClock(int timeLimit) : mTimeLimit(timeLimit) {
    mWhiteTimer = std::make_unique<LTimer>();
    mBlackTimer = std::make_unique<LTimer>();
}

LClock::~LClock() {
    
}

void LClock::start() {
    mWhiteTimer->start();
}

void LClock::next() {
    if(!mWhiteTimer->isPaused()) {
        mWhiteTimer->pause();
        if(!mBlackTimer->isStarted()) {
            mBlackTimer->start();
        }
        else {
            mBlackTimer->unpause();
        }
    }
    else {
        mBlackTimer->pause();
        mWhiteTimer->unpause();
    }
}

void LClock::pause() {
    mWhiteTimer->pause();
    mBlackTimer->pause();
}

void LClock::unpause() {
    mWhiteTimer->unpause();
    mBlackTimer->unpause();
}

int LClock::white() {
    return mTimeLimit - (mWhiteTimer->getTicks() / 1000);
}

int LClock::black() {
    return mTimeLimit - (mBlackTimer->getTicks() / 1000);
}