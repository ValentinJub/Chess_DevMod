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
#include "menu/LoadingScreen.h"
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
	int Execute();
	
private:
	bool init();
	bool initSDL();
	bool initWindow();
	bool initChunks();
	void loop();
	void handleEvents(SDL_Event* e);
	void close();
	void playMusic();
	void stopMusic();
	bool loadWindowIcon(std::string path);

	bool mAppIsRunning, mShowTitleScreen, mButtonsSet;
	LoadingScreen* mLoadingScreen;
	LButton mMenuButtons[TOTAL_MENU_ITEMS];
	LWindow* mWindow;
	LTexture mMenuTextures[TOTAL_MENU_ITEMS];
	Mix_Chunk* mMenuClick;
	Mix_Music* mMenuMusic;
};

#endif