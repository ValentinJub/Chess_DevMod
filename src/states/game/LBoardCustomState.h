/*
playPVP.h
Fri Mar 25 2022
by Valentin
-----------
Function prototypes for playPVP.cpp 
*/

#ifndef HEADER_LBOARDCUSTSTATE
#define HEADER_LBOARDCUSTSTATE

#include "states/LState.h"
#include "utils/util.h"
#include "graphics/LTexture.h"
#include "game/board/LBoardCustom.h"

class LBoardCustomState : public LState {
    public:
        LBoardCustomState();
        ~LBoardCustomState();
        void enter(LObserver* observer) override;
        void exit() override;
        void update(Uint64 dt) override;
        void render() override;
    private:
        LBoardCustom* mBoard;
};

#endif 