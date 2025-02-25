#include "LStateMachine.h"
#include "states/LState.h"
#include <SDL2/SDL.h>

LStateMachine::LStateMachine(LObserver* appObserver) : mAppObserver(appObserver) {}

// This frees all the states in the state machine
void LStateMachine::free() {
    while(!mStates.empty()) {
        mStates.back()->exit();
        mStates.pop_back();
    }
}

// This updates the top most state in the state machine
void LStateMachine::update(Uint64 dt) {
    mStates.back()->update(dt);
}

// This renders all the states in the state machine
void LStateMachine::render() {
    if(mStates.empty()) return;
    for(auto state : mStates) {
        state->render();
    }
    SDL_Delay(16);
}

// This adds a new state to the state machine
void LStateMachine::push(LState* state) {
    mStates.push_back(state);
    state->enter(mAppObserver);
}

// This removes the last state from the state machine
void LStateMachine::pop() {
    if(!mStates.empty()) {
        mStates.back()->exit();
        mStates.pop_back();
    }
}

// This removes the first state from the state machine
void LStateMachine::cut() {
    if(!mStates.empty()) {
        mStates.front()->exit();
        mStates.erase(mStates.begin());
    }
}