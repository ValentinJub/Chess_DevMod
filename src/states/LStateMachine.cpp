#include "LStateMachine.h"
#include "states/LState.h"
#include <SDL2/SDL.h>
#include "com/constantes.h"

extern SDL_Renderer* gRenderer;
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
    if(mStates.empty()) return;
    mStates.back()->update(dt);
}

// This renders all the states in the state machine
void LStateMachine::render() {
    SDL_RenderClear(gRenderer);
    SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	SDL_SetRenderDrawColor(gRenderer, 0x46, 0x46, 0x4c, 0xFF);
	SDL_RenderFillRect(gRenderer, &rect);
    if(mStates.empty()) return;
    for(auto state : mStates) {
        state->render();
    }
    SDL_RenderPresent(gRenderer);
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