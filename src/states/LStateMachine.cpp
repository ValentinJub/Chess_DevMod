#include "LStateMachine.h"
#include "LState.h"

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
    if(mStates.empty()) {
        return false;
    }
    mStates.top()->update();
    return true;
}

void LStateMachine::render() {
    mStates.top()->render();
}

void LStateMachine::push(LState* state) {
    mStates.push(state);
    state->enter();
}

void LStateMachine::pop() {
    mStates.top()->exit();
    mStates.pop();
}