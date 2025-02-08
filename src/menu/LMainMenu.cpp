#include "menu/LMainMenu.h"

extern TTF_Font* gFont64; 
extern TTF_Font* gFont32;
extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;
extern LMediaFactory* gMediaFactory;
extern LStateMachine* gStateMachine;
extern LMusicPlayer* gMusicPlayer;
extern uint8_t gMusicVolume;

const std::string MENU_PLAY_STR = "Play PvP";
const std::string MENU_PLAY_AI_STR = "Play vs AI";
const std::string MENU_SETTINGS_STR = "Game Settings";
const std::string MENU_DEVMODE_STR = "Developer Mode";

std::string MenuItem[] = {
	MENU_PLAY_STR,
    MENU_PLAY_AI_STR,
    MENU_SETTINGS_STR,
    MENU_DEVMODE_STR
};

LMainMenu::LMainMenu(LObserver* observer) {
    mAppObserver = observer;
	try {
		mLogger = spdlog::basic_logger_mt("LMainMenu", "logs/CApp.log");
	}
	catch(const spdlog::spdlog_ex& ex) {
		std::cerr << "Log init failed: " << ex.what() << std::endl;
	}
    this->Attach(observer);
    if(!this->init()) {
        mLogger->error("Failed to initialize main menu");
    }
}

void LMainMenu::update() {
    if(!gMusicPlayer->isPlaying()) {
		gMusicPlayer->play(MUSIC_MENU);
	}
    SDL_Event e;
    while(SDL_PollEvent(&e) > 0) {
        this->handleEvents(&e);
    }
}

void LMainMenu::render() {
	SDL_RenderClear(gRenderer);
	gBackgroundTexture->render();
	for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		mMenuTextures[i]->renderAuto();
	}
	SDL_RenderPresent(gRenderer);
}

void LMainMenu::free() {
	spdlog::drop("LMainMenu");
    this->Detach(mAppObserver);
    for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
        mMenuTextures[i]->free();
    }
}

bool LMainMenu::init() {
	for(int(i) = 0; i < TOTAL_MENU_ITEMS; i++) {
		mMenuTextures[i] = gMediaFactory->getTxtClickable(MenuItem[i].c_str(), gFont64, COLOR_BLACK);
		if(mMenuTextures[i] == NULL) {
			mLogger->error("Failed to load menu texture: {}", MenuItem[i]);
			return false;
		}
	}
    this->setTexturePositions();
    this->setButtons();
	return true;
}

void LMainMenu::setTexturePositions() {
	mMenuTextures[PLAY_AI]->setPos(
		(SCREEN_WIDTH - mMenuTextures[PLAY_AI]->w()) / 2, 
		(SCREEN_HEIGHT / 2) - (mMenuTextures[PLAY_AI]->h())
	); 
	mMenuTextures[SETTINGS]->setPos(
		(SCREEN_WIDTH - mMenuTextures[SETTINGS]->w()) / 2, 
		mMenuTextures[PLAY_AI]->y() + mMenuTextures[PLAY_AI]->h()
	);
	mMenuTextures[PLAY]->setPos(
		(SCREEN_WIDTH - mMenuTextures[PLAY]->w()) / 2,
		mMenuTextures[PLAY_AI]->y() - mMenuTextures[PLAY]->h()
	);
	mMenuTextures[DEVMODE]->setPos(
		(SCREEN_WIDTH - mMenuTextures[DEVMODE]->w()) / 2,
		mMenuTextures[SETTINGS]->y() + mMenuTextures[SETTINGS]->h()
	);
}

void LMainMenu::isNowSelected(int selected) {
    mMenuTextures[selected]->createImg(gMediaFactory->getTxtSDL_Texture(
        MenuItem[selected].c_str(),
        gFont64, 
        COLOR_RED
    ));
	for(int i = 0; i < TOTAL_MENU_ITEMS; i++) {
        if(i != selected) {
            mMenuTextures[i]->createImg(gMediaFactory->getTxtSDL_Texture(
                MenuItem[i].c_str(), 
                gFont64, 
                COLOR_BLACK
            ));
        }
    }
}

void LMainMenu::handleEvents(SDL_Event* e) {
    this->handleKeyEvents(e);
    this->handleMouseEvents(e);
}

void LMainMenu::handleKeyEvents(SDL_Event* e) {
	if(e->type == SDL_QUIT) {
		this->Notify();
		gStateMachine->pop();
	} 
    else if(e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_ESCAPE:
				this->Notify();
				gStateMachine->pop();
				break;
            case SDLK_1:
				gMusicPlayer->stop();
                playPVP();
                this->flushEvents();
                break;
            case SDLK_2:
				gMusicPlayer->stop();
                playerVersusComputer();
                this->flushEvents();
                break;
            case SDLK_3:
                SDL_Delay(20);
                this->flushEvents();
				gStateMachine->push(new LSettingsState);
                break;
            }
	}
}

void LMainMenu::handleMouseEvents(SDL_Event* e) {
    for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		if(mMenuTextures[i]->getButton()->handleInside(e)) {
			this->isNowSelected(i);
			if(mMenuTextures[i]->getButton()->handleClick(e)) {
				switch (i) {
				case PLAY:
					gMusicPlayer->stop();
					this->flushEvents();
					playPVP();
                	this->flushEvents();
					break;
				case PLAY_AI:
					gMusicPlayer->stop();
              		this->flushEvents();
					playerVersusComputer();
                	this->flushEvents();
					break;
				case SETTINGS:
					SDL_Delay(20);
					this->flushEvents();
					gStateMachine->push(new LSettingsState);
					this->flushEvents();
					break;
				case DEVMODE:
					SDL_Delay(200);
					this->flushEvents();
					break;
				}
			}
		}
	}
}

void LMainMenu::setButtons() {
	for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		mMenuTextures[i]->setButton();
	}
}

// Pull hanging events from the queue and flush them
// This is useful to avoid using an event from a previous state
void LMainMenu::flushEvents() {
	SDL_PumpEvents();
	SDL_FlushEvents(SDL_MOUSEMOTION, SDL_MOUSEBUTTONUP);
}