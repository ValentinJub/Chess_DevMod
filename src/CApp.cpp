#include "CApp.h"

SDL_Renderer* gRenderer;
LStateMachine* gStateMachine;
LMediaFactory* gMediaFactory;
LTexture* gBackgroundTexture;
TTF_Font* gFont64;
TTF_Font* gFont32;

// Global music volume, between 0 and 128.
uint8_t gMusicVolume;


CApp::CApp(bool showTitleScreen) {
	mAppIsRunning = true;
	mShowTitleScreen = showTitleScreen;
	mWindow = new LWindow;
}

void CApp::free() {
	this->freeGlobalVars();
	mWindow->free();
	this->closeSDL();	
}

void CApp::freeGlobalVars() {
	gMediaFactory->free();
	gBackgroundTexture->free();
	gStateMachine->free();
	if(gFont64 != NULL) {
		TTF_CloseFont(gFont64);
		gFont64 = NULL;
	}
	if(gFont32 != NULL) {
		TTF_CloseFont(gFont32);
		gFont32 = NULL;
	}
	if (gRenderer != NULL) {
		SDL_DestroyRenderer(gRenderer);
		gRenderer = NULL;
	}
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
	// set volume for all channels
	Mix_Volume(-1, MIX_MAX_VOLUME / 2);
	return true;
}

bool CApp::initWindow() {
	if(!mWindow->init()) {
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	return true;
}

bool CApp::initMenus() {
	gStateMachine->push(new LMainMenuState);
	if(mShowTitleScreen) {
		gStateMachine->push(new LStartState);
	}
	return true;
}

bool CApp::initGlobalVars() {
	gMediaFactory = new LMediaFactory;
	gRenderer = mWindow->createRenderer();
	if( gRenderer == NULL ) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	gBackgroundTexture = gMediaFactory->getImg(SPRITE_BACKGROUND);
	if(gBackgroundTexture == NULL) {
		return false;
	}
	gFont64 = TTF_OpenFont( FONT_BRANDA, 64 );
	if (gFont64 == NULL) {
		printf( "Failed to load resources/valentin font! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}
	gFont32 = TTF_OpenFont( FONT_BRANDA, 32 );
	if (gFont32 == NULL) {
		printf( "Failed to load resources/valentin font! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}
	gMusicVolume = MIX_MAX_VOLUME / 2;
	gStateMachine = new LStateMachine;
	return true;
}

bool CApp::init() {
	srand(time(0));
	return initSDL() && initWindow() && initGlobalVars() && initMenus();
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

int CApp::exec() {
	if(!this->init()) {
		printf("Failed to initialise!");
	}
	else {
		while(gStateMachine->update()) {
			gStateMachine->render();
		}
	}
	this->free();
	return 0;
}