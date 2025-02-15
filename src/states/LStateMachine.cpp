#include "LStateMachine.h"
#include "states/LState.h"
#include <SDL2/SDL.h>

LStateMachine::LStateMachine(LObserver* appObserver) : mAppObserver(appObserver) {}

void LStateMachine::free() {
    while(!mStates.empty()) {
        mStates.top()->exit();
        mStates.pop();
    }
}

void LStateMachine::update() {
    mStates.top()->update();
}

void LStateMachine::render() {
    if(mStates.empty()) return;
    mStates.top()->render();
    SDL_Delay(16);
}

void LStateMachine::push(LState* state) {
    mStates.push(state);
    state->enter(mAppObserver);
}

void LStateMachine::pop() {
    mStates.top()->exit();
    mStates.pop();
}