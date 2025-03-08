#include "CApp.h"

SDL_Renderer* gRenderer;
LStateMachine* gStateMachine;
LMediaFactory* gMediaFactory;
LMusicPlayer* gMusicPlayer;
LChunkPlayer* gChunkPlayer;
LTexture* gBackgroundTexture;
TTF_Font* gFont64;
TTF_Font* gFont32;
TTF_Font* gFont16;
extern std::shared_ptr<spdlog::logger> gLogger;

// Global music volume, between 0 and 128.
uint8_t gMusicVolume;


CApp::CApp(bool showTitleScreen) {
	spdlog::info("--------------A new debug session begins--------------");
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
	spdlog::debug("Freeing global variables...");
	gMediaFactory->free();
	gBackgroundTexture->free();
	gStateMachine->free();
	gMusicPlayer->free();
	gChunkPlayer->free();
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
		spdlog::error("SDL could not initialise! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	//Set texture filtering to linear
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
		spdlog::error( "Warning: Linear texture filtering not enabled!" );
	}

	//Initialize PNG loading
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		spdlog::error("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}
	if(TTF_Init() == -1) {
		spdlog::error("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
		spdlog::error( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return false;
	}
	// set volume for all channels
	Mix_Volume(-1, MIX_MAX_VOLUME / 2);
	return true;
}

bool CApp::initWindow() {
	if(!mWindow->init()) {
		spdlog::error( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	return true;
}

bool CApp::initMenus() {
	if(mShowTitleScreen) {
		gStateMachine->push(new LStartState);
	} else {
		gStateMachine->push(new LMainMenuState);
	}
	return true;
}

bool CApp::initGlobalVars() {
	gMediaFactory = LMediaFactory::Instance();
	gMusicPlayer = LMusicPlayer::Instance();
	gChunkPlayer = LChunkPlayer::Instance();
	gRenderer = mWindow->createRenderer();
	if( gRenderer == NULL ) {
		spdlog::error("Renderer could not be created! SDL Error: {}", SDL_GetError());
		return false;
	}
	spdlog::debug("Renderer created");
	gBackgroundTexture = gMediaFactory->getImg(SPRITE_BACKGROUND);
	if(gBackgroundTexture == NULL) {
		spdlog::error("Failed to load background texture!");
		return false;
	}
	spdlog::debug("Background texture created");
	gFont64 = gMediaFactory->getFont( FONT_BRANDA, 64 );
	if (gFont64 == NULL) {
		spdlog::error("Failed to load {} font! SDL_ttf Error: {}", FONT_BRANDA, TTF_GetError());
		return false;
	}
	gFont32 = gMediaFactory->getFont( FONT_BRANDA, 32 );
	if (gFont32 == NULL) {
		spdlog::error("Failed to load {} font! SDL_ttf Error: {}", FONT_BRANDA, TTF_GetError());
		return false;
	}gFont32 = gMediaFactory->getFont( FONT_BRANDA, 32 );
	if (gFont32 == NULL) {
		spdlog::error("Failed to load {} font! SDL_ttf Error: {}", FONT_BRANDA, TTF_GetError());
		return false;
	}
	gFont16 = gMediaFactory->getFont( FONT_BRANDA, 16 );
	if (gFont16 == NULL) {
		spdlog::error("Failed to load {} font! SDL_ttf Error: {}", FONT_BRANDA, TTF_GetError());
		return false;
	}
	spdlog::debug("Fonts created");
	gMusicVolume = MIX_MAX_VOLUME / 2;
	gStateMachine = new LStateMachine(this);
	return true;
}

bool CApp::init() {
	srand(time(0));
	return initSDL() && initWindow() && initGlobalVars() && initMenus();
}

void CApp::poll(LSubject* subject, int value) {
	// We don't use any of the params here, a signal is enough to stop the app
	mAppIsRunning = false;
}

// bool CApp::loadWindowIcon(std::string path) {
// 	SDL_Surface* icon = IMG_Load(path.c_str());
// 	if(icon == NULL) {
// 		spdlog::error("Unable to load icon! SDL_image Error: %s\n", IMG_GetError());
// 		return false;
// 	}
// 	try {
// 		SDL_SetWindowIcon(mWindow->getWindow(), icon);
// 		SDL_FreeSurface(icon);
// 		return true;
// 	}
// 	catch(const char* msg) {
// 		spdlog::error("%s\n", msg);
// 		return false;
// 	}
// }

int CApp::exec() {
	if(!this->init()) {
		spdlog::error("Critical failure: failed to initialize!");
	}
	else {
		Uint64 NOW = SDL_GetPerformanceCounter();
		Uint64 LAST = 0;
		while(this->mAppIsRunning) {
			LAST = NOW;
			NOW = SDL_GetPerformanceCounter();
			double dt = ((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
			gStateMachine->update(dt);
			SDL_RenderClear(gRenderer);
			gBackgroundTexture->render();
			gStateMachine->render();
			SDL_RenderPresent(gRenderer);
		}
	}
	spdlog::info("Exiting...");
	this->free();
	return 0;
}