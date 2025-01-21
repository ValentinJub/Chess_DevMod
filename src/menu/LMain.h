#ifndef MAINMENU_H
#define MAINMENU_H

#include "../utils/util.h"
#include "../LTexture.h"
#include "../LButton.h"
#include "../settings.h"
#include "../com/constantes.h"
#include "../playPVAI.h"
#include "../playPVP.h"

extern TTF_Font* gFont64; 
extern TTF_Font* gFont32;
extern SDL_Renderer* gRenderer;
extern LTexture gBackgroundTexture;
extern uint8_t gMusicVolume;

const std::string MENU_PLAY_STR = "Play PvP";
const std::string MENU_PLAY_AI_STR = "Play vs AI";
const std::string MENU_SETTINGS_STR = "Game Settings";
const std::string MENU_DEVMODE_STR = "Developer Mode";

class LMain {

public:
    LMain();
    void free();
    bool handleEvents(SDL_Event* e);
    bool handleKeyEvents(SDL_Event* e);
    void handleMouseEvents(SDL_Event* e);
    bool init();
    bool loadMenu(LTexture menuTextures[]);
    void highlightSelected(int position = 3);
    void render();
    void setButtons();
    void setTexturePositions();
    void unsetButtons();
    void flushEvents();
    void playMusic();
	void stopMusic();

private:
    bool mButtonsAreSet;
    LTexture mMenuTextures[TOTAL_MENU_ITEMS];
    LButton mMenuButtons[TOTAL_MENU_ITEMS];
	Mix_Chunk* mMenuClick;
	Mix_Music* mMenuMusic;
};


#endif