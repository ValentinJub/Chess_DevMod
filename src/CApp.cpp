#include "CApp.h"

SDL_Renderer* gRenderer = NULL; 
LTexture gWIPTexture; 
LTexture gBackgroundTexture;
TTF_Font* gFont64 = NULL;
TTF_Font* gFont32 = NULL;

// Global music volume, between 0 and 128.
uint8_t gMusicVolume = MIX_MAX_VOLUME / 2;


CApp::CApp() {
	// responsible for running the program, pressing esc in the menu would close the program
	mAppIsRunning = true;
	// responsible for waiting for the title screen & music to finish before displaying the menu
	mShowTitleScreen = true;
	// Track when menu buttons are set
	mButtonsSet = false;
	// The window we'll be rendering to
	mWindow = new LWindow;
}

void CApp::close() {
	// Free the menu textures
	for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		mMenuTextures[i].free();
	}

	gWIPTexture.free();
	gBackgroundTexture.free();
	
	mLoadingScreen->Close();
	
	Mix_FreeChunk(mMenuClick);
	mMenuClick = NULL;
	
	Mix_FreeMusic(mMenuMusic);
	mMenuMusic = NULL;
	
	mWindow->free();
	mWindow = NULL;
	
	TTF_CloseFont(gFont64);
	gFont64 = NULL;
	TTF_CloseFont(gFont32);
	gFont32 = NULL;
	
	SDL_DestroyRenderer(gRenderer);
	
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

bool CApp::initSDL() {
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialise! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	//Set texture filtering to linear
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
		printf( "Warning: Linear texture filtering not enabled!" );
	}

	//Initialize PNG loading
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}
	if(TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return false;
	}
	return true;
}

bool CApp::initWindow() {
	if(!mWindow->init()) {
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	gRenderer = mWindow->createRenderer();
	if( gRenderer == NULL ) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	return true;
}

bool CApp::initChunks() {
	mMenuClick = loadChunk("SoundEffects/START.wav"); 
	mMenuMusic = loadMusic("Music/Menu/menu.mp3");
	return true;
}

bool CApp::init() {
	if(!initSDL()) {
		return false;
	}
	if(!initWindow()) {
		return false;
	}
	if(!initChunks()) {
		return false;
	}
	if (mShowTitleScreen) {
		mLoadingScreen = new LoadingScreen;
		mLoadingScreen->loadTitle();
	}
	srand(time(0));
	return true;
}

// bool CApp::loadWindowIcon(std::string path) {
// 	SDL_Surface* icon = IMG_Load(path.c_str());
// 	if(icon == NULL) {
// 		printf("Unable to load icon! SDL_image Error: %s\n", IMG_GetError());
// 		return false;
// 	}
// 	try {
// 		SDL_SetWindowIcon(mWindow->getWindow(), icon);
// 		SDL_FreeSurface(icon);
// 		return true;
// 	}
// 	catch(const char* msg) {
// 		printf("%s\n", msg);
// 		return false;
// 	}
// }

void CApp::handleEvents(SDL_Event* e) {
	if(e->type == SDL_QUIT) {
		mAppIsRunning = false;
	}
	else if(e->type == SDL_KEYDOWN) {
		switch(e->key.keysym.sym) {
			case SDLK_ESCAPE:
				mAppIsRunning = false;
				break;
			case SDLK_1:
				stopMusic();
				//Unset menu buttons and reset menuBool
				unsetButtons(mMenuButtons);
				mButtonsSet = false;
				playPVP();
				break;
			case SDLK_2:
				stopMusic();
				//Unset menu buttons and reset menuBool
				unsetButtons(mMenuButtons);
				mButtonsSet = false;
				playerVersusComputer();
				break;
			case SDLK_3:
				//Unset menu buttons and reset menuBool
				unsetButtons(mMenuButtons);
				mButtonsSet = false;
				SDL_Delay(200);
				//flush events to prevent clicks clicking the 
				//menu buttons when going back to menu from settings
				flushEvents();
				settings();
				break;
		}
	}
	for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		if(mMenuButtons[i].handleInside(e)) {
			loadMenuHighlight(mMenuTextures, i);
			if(mMenuButtons[i].handleClick(e)) {
				if(i == PLAY) {
					stopMusic();
					//Unset menu buttons and reset menuBool
					unsetButtons(mMenuButtons);
					mButtonsSet = false;
					playPVP();
					flushEvents();
				}
				else if(i == PLAY_AI) {
					stopMusic();
					//Unset menu buttons and reset menuBool
					unsetButtons(mMenuButtons);
					mButtonsSet = false;
					playerVersusComputer();
					flushEvents();
				}
				else if(i == SETTINGS) {
					//Unset menu buttons and reset menuBool
					unsetButtons(mMenuButtons);
					mButtonsSet = false;
					SDL_Delay(200);
					//flush events to prevent clicks clicking the 
					//menu buttons when going back to menu from settings
					flushEvents();
					settings();
				}
				else if(i == DEVMODE) {
					//Unset menu buttons and reset menuBool
					unsetButtons(mMenuButtons);
					mButtonsSet = false;
					SDL_Delay(200);
					//flush events to prevent clicks clicking the 
					//menu buttons when going back to menu from settings
					flushEvents();
				}
			}
		}
	}
}

void CApp::loop() {
	SDL_Event e;
	while(SDL_PollEvent(&e) > 0) {
		handleEvents(&e);
	}
	if(mShowTitleScreen) {
		mLoadingScreen->Show();
		mShowTitleScreen = false;
	}
	else { 
		playMusic();
		displayMenu(mMenuTextures);
		if(!(mButtonsSet)) {
			setButtons(mMenuButtons, mMenuTextures);
			mButtonsSet = true;
		}
	SDL_Delay(16);
	}
	
}

int CApp::Execute() {
	if(!init()) {
		printf("Failed to initialise!");
	}
	else {
		if(!loadMenu(mMenuTextures)) {
			printf("Failed to load menu media!");
		}
		else {
			while(mAppIsRunning) {
				loop();
			}
		}
	}
	CApp::close();
	return 0;
}

void CApp::playMusic() {
	if(Mix_PlayingMusic() == 0) {
		// loop infinitely menu music
		Mix_PlayMusic(mMenuMusic, -1);
		// set volume 
		Mix_Volume(-1, gMusicVolume);
		Mix_VolumeMusic(gMusicVolume);
	}
}

void CApp::stopMusic() {
	if(Mix_PlayingMusic()) {
		Mix_FadeOutMusic(300);
	}
}