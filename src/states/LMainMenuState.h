#ifndef LMAINMENUSTATE_H
#define LMAINMENUSTATE_H

#include "../utils/util.h"
#include "../LTexture.h"
#include "../LButton.h"
#include "../settings.h"
#include "LState.h"
#include "LStateMachine.h"
#include "../factories/LMediaFactory.h"
#include "../com/constantes.h"
#include "../playPVAI.h"
#include "../playPVP.h"

extern TTF_Font* gFont64; 
extern TTF_Font* gFont32;
extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;
extern LMediaFactory* gMediaFactory;
extern LStateMachine* gStateMachine;
extern uint8_t gMusicVolume;

const std::string MENU_PLAY_STR = "Play PvP";
const std::string MENU_PLAY_AI_STR = "Play vs AI";
const std::string MENU_SETTINGS_STR = "Game Settings";
const std::string MENU_DEVMODE_STR = "Developer Mode";

class LMainMenuState : public LState {

public:
    LMainMenuState();
    void enter();
    void exit();
    void update();
    void render();

private:
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
};


#endif