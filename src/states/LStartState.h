/* File: LStartState.h

This is a crossover between a state and a non interactive menu
for the sake of simplicity I bundled both in this file

*/

#ifndef LSTARTSTATE_H 
#define LSTARTSTATE_H   

#include "utils/util.h"
#include "com/constantes.h"
#include "graphics/LTexture.h"
#include "LState.h"
#include "LStateMachine.h"
#include "factories/LMediaFactory.h"
#include "com/LObserver.h"
#include "com/LSubject.h"
#include "sound/LChunkPlayer.h"

const std::string TITLE_STR = "Chess";
const std::string TITLE_AUTHOR_STR = "by Valentin with Love";


class LStartState : public LState, public LSubject {

public:
    LStartState();
    void enter(LObserver* observer);
    void exit();
    void update();
    void render();
private:
    void free();
    void init();
    void loadTexture();
    LTexture* mTitleTexture[TOTAL_TITLE_ITEMS];
    LObserver* mObserver;
    std::shared_ptr<spdlog::logger> mLogger;
};

#endif
