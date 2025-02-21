/*
playPVP.h
Fri Mar 25 2022
by Valentin
-----------
Function prototypes for playPVP.cpp 
*/

#ifndef HEADER_LBOARDSTATE
#define HEADER_LBOARDSTATE

#include "states/LState.h"
#include "utils/util.h"
#include "graphics/LTexture.h"
#include "game/board/LBoardPVP.h"

class LBoardState : public LState {
    public:
        LBoardState();
        ~LBoardState();
        void enter(LObserver* observer) override;
        void exit() override;
        void update() override;
        void render() override;
    private:
        LBoardPVP* mBoard;
};

void playPVP();

#endif 