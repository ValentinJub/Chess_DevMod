/*
CApp.h
Sun Mar 20 2022
by Valentin
-----------
headers for CApp
*/

#ifndef H_CAPP
#define H_CAPP

#include "headers.h"
#include "LTexture.h"
#include "LButton.h"
#include "LWindow.h"
#include "LTimer.h"
#include "funct_headers.h"
#include "settings.h"
#include "playPVAI.h"
#include "playPVP.h"

class CApp {
public:
	CApp();
	int execute();
	void loop();
	void handleEvents(SDL_Event* e);
	bool init();
	void close();
	void playMusic();
	void stopMusic();
private:
	bool mIsRunning, mWaitBool, mButtonsSet;
	LButton mMenuButtons[TOTAL_MENU_ITEMS];
	LWindow* mWindow;
	LTexture mTitleTextures[TOTAL_TITLE_ITEMS];
	LTexture mMenuTextures[TOTAL_MENU_ITEMS];
	Mix_Chunk* mStartupSound;
	Mix_Chunk* mMenuClick;
	Mix_Music* mMenuMusic;
};

#endif