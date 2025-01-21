#include "CApp.h"

SDL_Renderer* gRenderer = NULL; 
LTexture gBackgroundTexture;
TTF_Font* gFont64 = NULL;
TTF_Font* gFont32 = NULL;

// Global music volume, between 0 and 128.
uint8_t gMusicVolume = MIX_MAX_VOLUME / 2;


CApp::CApp() {
	mAppIsRunning = true;
	mShowTitleScreen = false;
	mWindow = new LWindow;
}

void CApp::close() {
	gBackgroundTexture.free();
	
	mLoadingScreen->free();
	mMainMenu->free();
	mWindow->free();
	mWindow = NULL;
	
	TTF_CloseFont(gFont64);
	gFont64 = NULL;
	TTF_CloseFont(gFont32);
	gFont32 = NULL;
	
	SDL_DestroyRenderer(gRenderer);
	this->closeSDL();	
}

void CApp::closeSDL() {
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

bool CApp::initMenu(bool initTitle) {
	if(initTitle) {
		mLoadingScreen = new LTitle;
		if(!mLoadingScreen->loadTitle()) {
			return false;
		}
	}
	mMainMenu = new LMain;
	if(!mMainMenu->init()) {
		return false;
	}
	return true;
}

bool CApp::init() {
	srand(time(0));
	return initSDL() && initWindow() && initMenu();
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
	if(mMainMenu->handleEvents(e)) {
		mAppIsRunning = false;
	}
}

void CApp::loop() {
	SDL_Event e;
	while(SDL_PollEvent(&e) > 0) {
		handleEvents(&e);
	}
	if(mShowTitleScreen) {
		mLoadingScreen->render();
		mShowTitleScreen = false;
	}
	else { 
		mMainMenu->playMusic();
		mMainMenu->render();
		SDL_Delay(16);
	}
	
}

int CApp::Execute() {
	if(!init()) {
		printf("Failed to initialise!");
	}
	else {
		while(mAppIsRunning) {
			loop();
		}
	}
	CApp::close();
	return 0;
}