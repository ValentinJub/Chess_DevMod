#include "LStateMachine.h"
#include "LState.h"

LStateMachine::LStateMachine() {
    // Empty for now
}

LStateMachine::~LStateMachine() {
    while(!mStates.empty()) {
        mStates.top()->exit();
        mStates.pop();
    }
}

void LStateMachine::update() {
    mStates.top()->update();
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