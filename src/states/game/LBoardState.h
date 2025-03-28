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
#include "game/board/LBoard.h"

class LBoardState : public LState {
    public:
        LBoardState(PlayMode mode);
        ~LBoardState();
        void enter(LObserver* observer) override;
        void exit() override;
        void update(Uint64 dt) override;
        void render() override;
    private:
        PlayMode mPlayMode;
        LBoard* mBoard;
};

#endif 