/*

Author: Valentin Wissler
Date: 2024-01-24 21:14

This class is used handle states in the game. It is used to switch between different states of the game, 
such as the title screen, the main menu, the game itself, etc.

StateMachine is a stack of states, the top state is the one that is currently active. 
When a new state is pushed, the current state is paused and the new state is pushed on top of the stack. 
When a state is popped, the current state is removed from the stack and the previous state is resumed.

It would be desirable to keep some states in memory such as the main menu, the settings menu, etc.

*/

#ifndef LSTATEMACHINE_H
#define LSTATEMACHINE_H

#include <vector>
#include "com/LObserver.h"
#include "SDL2/SDL.h"

class LState;

class LStateMachine {
    public:
        LStateMachine(LObserver *appObserver);
        void free();
        void update(Uint64 dt);
        void render();
        void push(LState* state);
        void pop();
        void cut();
    private:
        std::vector<LState*> mStates;
        LObserver* mAppObserver;
};

#endif