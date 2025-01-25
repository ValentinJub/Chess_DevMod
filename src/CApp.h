/*
CApp.h
Sun Mar 20 2022
by Valentin
-----------
headers for CApp
*/

#ifndef H_CAPP
#define H_CAPP

#include "utils/util.h"
#include "LTexture.h"
#include "menu/headers.h"
#include "LButton.h"
#include "LWindow.h"
#include "LTimer.h"

class CApp {
public:
	CApp(bool showTitleScreen);
	int exec();
	
private:
	bool init();
	bool initSDL();
	bool initWindow();
	bool initMenus();
	bool initGlobalVars();
	void free();
	void closeSDL();
	void freeGlobalVars();
	void loop();
	void handleEvents(SDL_Event* e);
	// bool loadWindowIcon(std::string path);

	bool mAppIsRunning, mShowTitleScreen;
	LTitle * mLoadingScreen;
	LMain * mMainMenu;
	LWindow* mWindow; // This is the program's window
};

#endif