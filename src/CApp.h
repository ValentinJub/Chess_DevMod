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
#include <memory>
#include "graphics/LTexture.h"
#include "com/LButton.h"
#include "com/LWindow.h"
#include "com/LTimer.h"
#include "states/LStateMachine.h"
#include "states/menu/LMainMenuState.h"
#include "states/menu/LStartState.h"
#include "factories/LMediaFactory.h"
#include "com/LObserver.h"
#include "sound/LMusicPlayer.h"
#include "sound/LChunkPlayer.h"
#include "com/logger.h"

class CApp : public LObserver {
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
	void poll(LSubject* subject);
	// bool loadWindowIcon(std::string path);

	bool mAppIsRunning, mShowTitleScreen;
	LWindow* mWindow; // This is the program's window
};

#endif