#ifndef LMAINMENU_H
#define LMAINMENU_H

#include "com/headers.h"
#include "com/constantes.h"
#include "LButton.h"
#include "LTexture.h"
#include "com/LSubject.h"
#include "states/LStateMachine.h"
#include "states/LSettingsState.h"
#include "factories/LMediaFactory.h"
#include "playPVAI.h"
#include "playPVP.h"

class LMainMenu : public LSubject {
public:
    LMainMenu(LObserver* observer);
    void update();
    void render();
    bool init();
    void free();
    void handleEvents(SDL_Event* e);
    void handleKeyEvents(SDL_Event* e);
    void handleMouseEvents(SDL_Event* e);
    void highlightSelected(int position = 3);
    void setButtons();
    void setTexturePositions();
    void unsetButtons();
    void flushEvents();
    void playMusic();
	void stopMusic();

    bool mButtonsAreSet;
    LTexture* mMenuTextures[TOTAL_MENU_ITEMS];
    LButton* mMenuButtons[TOTAL_MENU_ITEMS];
	Mix_Chunk* mMenuClick;
	Mix_Music* mMenuMusic;
    LObserver* mAppObserver;
};

#endif

