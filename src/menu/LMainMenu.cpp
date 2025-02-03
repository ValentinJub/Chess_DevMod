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

LMainMenu::LMainMenu(LObserver* observer) {
    mAppObserver = observer;
    this->Attach(observer);
    if(!this->init()) {
        printf("Failed to initialise LMainMenuState");
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
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	gBackgroundTexture->render();

	mMenuTextures[PLAY_AI]->render(mMenuTextures[PLAY_AI]->x(), mMenuTextures[PLAY_AI]->y());
	mMenuTextures[SETTINGS]->render(mMenuTextures[SETTINGS]->x(), mMenuTextures[SETTINGS]->y());
	mMenuTextures[PLAY]->render(mMenuTextures[PLAY]->x(), mMenuTextures[PLAY]->y());
	mMenuTextures[DEVMODE]->render(mMenuTextures[DEVMODE]->x(), mMenuTextures[DEVMODE]->y());

	SDL_RenderPresent(gRenderer);
}

void LMainMenu::free() {
    this->Detach(mAppObserver);
    for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
        mMenuTextures[i]->free();
    }
}

bool LMainMenu::init() {
	mMenuTextures[PLAY] = gMediaFactory->getTxt(MENU_PLAY_STR, gFont64, COLOR_BLACK);
	mMenuTextures[PLAY_AI] = gMediaFactory->getTxt(MENU_PLAY_AI_STR, gFont64, COLOR_BLACK);
	mMenuTextures[SETTINGS] = gMediaFactory->getTxt(MENU_SETTINGS_STR, gFont64, COLOR_BLACK);
	mMenuTextures[DEVMODE] = gMediaFactory->getTxt(MENU_DEVMODE_STR, gFont64, COLOR_BLACK);
    this->setTexturePositions();
    this->setButtons();
	return mMenuTextures[PLAY] != NULL && mMenuTextures[PLAY_AI] != NULL && mMenuTextures[SETTINGS] != NULL && mMenuTextures[DEVMODE] != NULL;
}

void LMainMenu::setTexturePositions() {
	mMenuTextures[PLAY_AI]->setPos(
		(SCREEN_WIDTH - mMenuTextures[PLAY_AI]->getWidth()) / 2, 
		(SCREEN_HEIGHT / 2) - (mMenuTextures[PLAY_AI]->getHeight())
	); 
	mMenuTextures[SETTINGS]->setPos(
		(SCREEN_WIDTH - mMenuTextures[SETTINGS]->getWidth()) / 2, 
		mMenuTextures[PLAY_AI]->y() + mMenuTextures[PLAY_AI]->getHeight()
	);
	mMenuTextures[PLAY]->setPos(
		(SCREEN_WIDTH - mMenuTextures[PLAY]->getWidth()) / 2,
		mMenuTextures[PLAY_AI]->y() - mMenuTextures[PLAY]->getHeight()
	);
	mMenuTextures[DEVMODE]->setPos(
		(SCREEN_WIDTH - mMenuTextures[DEVMODE]->getWidth()) / 2,
		mMenuTextures[SETTINGS]->y() + mMenuTextures[SETTINGS]->getHeight()
	);
}

void LMainMenu::highlightSelected(int position) {
	switch(position) {
		case PLAY:
			mMenuTextures[PLAY] = gMediaFactory->getTxt(MENU_PLAY_STR, gFont64, COLOR_RED);
			mMenuTextures[PLAY_AI] = gMediaFactory->getTxt(MENU_PLAY_AI_STR, gFont64, COLOR_BLACK);
			mMenuTextures[SETTINGS] = gMediaFactory->getTxt(MENU_SETTINGS_STR, gFont64, COLOR_BLACK);
			mMenuTextures[DEVMODE] = gMediaFactory->getTxt(MENU_DEVMODE_STR, gFont64, COLOR_BLACK);
			break;
		case PLAY_AI:
			mMenuTextures[PLAY] = gMediaFactory->getTxt(MENU_PLAY_STR, gFont64, COLOR_BLACK);
			mMenuTextures[PLAY_AI] = gMediaFactory->getTxt(MENU_PLAY_AI_STR, gFont64, COLOR_RED);
			mMenuTextures[SETTINGS] = gMediaFactory->getTxt(MENU_SETTINGS_STR, gFont64, COLOR_BLACK);
			mMenuTextures[DEVMODE] = gMediaFactory->getTxt(MENU_DEVMODE_STR, gFont64, COLOR_BLACK);
			break;
		case SETTINGS:
			mMenuTextures[PLAY] = gMediaFactory->getTxt(MENU_PLAY_STR, gFont64, COLOR_BLACK);
			mMenuTextures[PLAY_AI] = gMediaFactory->getTxt(MENU_PLAY_AI_STR, gFont64, COLOR_BLACK);
			mMenuTextures[SETTINGS] = gMediaFactory->getTxt(MENU_SETTINGS_STR, gFont64, COLOR_RED);
			mMenuTextures[DEVMODE] = gMediaFactory->getTxt(MENU_DEVMODE_STR, gFont64, COLOR_BLACK);
			break;
		case DEVMODE:
			mMenuTextures[PLAY] = gMediaFactory->getTxt(MENU_PLAY_STR, gFont64, COLOR_BLACK);
			mMenuTextures[PLAY_AI] = gMediaFactory->getTxt(MENU_PLAY_AI_STR, gFont64, COLOR_BLACK);
			mMenuTextures[SETTINGS] = gMediaFactory->getTxt(MENU_SETTINGS_STR, gFont64, COLOR_BLACK);
			mMenuTextures[DEVMODE] = gMediaFactory->getTxt(MENU_DEVMODE_STR, gFont64, COLOR_RED);
			break;
		default: 
			mMenuTextures[PLAY] = gMediaFactory->getTxt(MENU_PLAY_STR, gFont64, COLOR_BLACK);
			mMenuTextures[PLAY_AI] = gMediaFactory->getTxt(MENU_PLAY_AI_STR, gFont64, COLOR_BLACK);
			mMenuTextures[SETTINGS] = gMediaFactory->getTxt(MENU_SETTINGS_STR, gFont64, COLOR_BLACK);
			mMenuTextures[DEVMODE] = gMediaFactory->getTxt(MENU_DEVMODE_STR, gFont64, COLOR_BLACK);
			break;
	}
    // Re-loading the textures with the new color will remove the position of the textures, so we need to reset them
    this->setTexturePositions();
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
                break;
            case SDLK_2:
				gMusicPlayer->stop();
                playerVersusComputer();
                break;
            case SDLK_3:
                SDL_Delay(200);
                this->flushEvents();
				gStateMachine->push(new LSettingsState);
                break;
            }
	}
}

void LMainMenu::handleMouseEvents(SDL_Event* e) {
    for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		if(mMenuButtons[i]->handleInside(e)) {
			highlightSelected(i);
			if(mMenuButtons[i]->handleClick(e)) {
				switch (i) {
				case PLAY:
					gMusicPlayer->stop();
					flushEvents();
					playPVP();
					break;
				case PLAY_AI:
					gMusicPlayer->stop();
					flushEvents();
					playerVersusComputer();
					break;
				case SETTINGS:
					SDL_Delay(200);
					flushEvents();
					gStateMachine->push(new LSettingsState);
					break;
				case DEVMODE:
					SDL_Delay(200);
					flushEvents();
					break;
				}
			}
		}
	}
}

void LMainMenu::setButtons() {
	for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		mMenuButtons[i] = new LButton(
			mMenuTextures[i]->x(), 
			mMenuTextures[i]->y(), 
			mMenuTextures[i]->getWidth(), 
			mMenuTextures[i]->getHeight()
		);
	}
}

void LMainMenu::flushEvents() {
	SDL_PumpEvents();
	SDL_FlushEvents(SDL_MOUSEMOTION, SDL_MOUSEBUTTONUP);
}