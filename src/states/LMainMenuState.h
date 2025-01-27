#ifndef LMAINMENUSTATE_H
#define LMAINMENUSTATE_H

#include "utils/util.h"
#include "states/LSettingsState.h"
#include "LState.h"
#include "LStateMachine.h"
#include "com/constantes.h"
#include "playPVAI.h"
#include "playPVP.h"
#include "menu/LMainMenu.h"

extern TTF_Font* gFont64; 
extern TTF_Font* gFont32;
extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;
extern LMediaFactory* gMediaFactory;
extern LStateMachine* gStateMachine;
extern uint8_t gMusicVolume;

class LMainMenuState : public LState {

public:
    LMainMenuState();
    void enter(LObserver* observer);
    void exit();
    void update();
    void render();

private:
    LMainMenu* mMainMenu;
};


#endif