#include "LStateMachine.h"
#include "LState.h"
#include <SDL2/SDL.h>

LStateMachine::LStateMachine() {
    // Empty for now
}

void LStateMachine::free() {
    while(!mStates.empty()) {
        mStates.top()->exit();
        mStates.pop();
    }
}

bool LStateMachine::update() {
    mStates.top()->update();
    if(mStates.empty()) {
        return false;
    }
    return true;
}

void LStateMachine::render() {
    mStates.top()->render();
    SDL_Delay(16);
}

void LStateMachine::push(LState* state) {
    mStates.push(state);
    state->enter();
}

void LStateMachine::pop() {
    mStates.top()->exit();
    mStates.pop();
}