#include "LMainMenuState.h"

LMainMenuState::LMainMenuState() {
    mButtonsAreSet = false;
	if(!this->init()) {
		printf("Failed to initialise LMainMenuState");
	}
}

void LMainMenuState::enter(LObserver* observer) {
	mAppObserver = observer;
	this->Attach(observer);
	this->init();
}

void LMainMenuState::exit() {
	this->Detach(mAppObserver);
	this->stopMusic();
	this->unsetButtons();
	this->free();
}

void LMainMenuState::update() {
	this->playMusic();
	SDL_Event e;
	while(SDL_PollEvent(&e) > 0) {
		this->handleEvents(&e);
	}
}

void LMainMenuState::render() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	
	//render BG 1st
	gBackgroundTexture->render();

	mMenuTextures[PLAY_AI]->render(mMenuTextures[PLAY_AI]->x(), mMenuTextures[PLAY_AI]->y());
	mMenuTextures[SETTINGS]->render(mMenuTextures[SETTINGS]->x(), mMenuTextures[SETTINGS]->y());
	mMenuTextures[PLAY]->render(mMenuTextures[PLAY]->x(), mMenuTextures[PLAY]->y());
	mMenuTextures[DEVMODE]->render(mMenuTextures[DEVMODE]->x(), mMenuTextures[DEVMODE]->y());

	SDL_RenderPresent(gRenderer);
}

void LMainMenuState::free() {
    for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
        mMenuTextures[i]->free();
    }
    Mix_FreeChunk(mMenuClick);
	mMenuClick = NULL;
	 
	Mix_FreeMusic(mMenuMusic);
	mMenuMusic = NULL;
}

bool LMainMenuState::init() {
	mMenuTextures[PLAY] = gMediaFactory->getTxt(MENU_PLAY_STR, gFont64, COLOR_BLACK);
	mMenuTextures[PLAY_AI] = gMediaFactory->getTxt(MENU_PLAY_AI_STR, gFont64, COLOR_BLACK);
	mMenuTextures[SETTINGS] = gMediaFactory->getTxt(MENU_SETTINGS_STR, gFont64, COLOR_BLACK);
	mMenuTextures[DEVMODE] = gMediaFactory->getTxt(MENU_DEVMODE_STR, gFont64, COLOR_BLACK);
    mMenuMusic = Util::loadMusic(MUSIC_MENU);
    this->setTexturePositions();
    this->setButtons();

	return mMenuTextures[PLAY] != NULL && mMenuTextures[PLAY_AI] != NULL && mMenuTextures[SETTINGS] != NULL && mMenuTextures[DEVMODE] != NULL && mMenuMusic != NULL;	
}

void LMainMenuState::setTexturePositions() {
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

void LMainMenuState::highlightSelected(int position) {
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


void LMainMenuState::handleEvents(SDL_Event* e) {
    this->handleKeyEvents(e);
    this->handleMouseEvents(e);
}

void LMainMenuState::handleKeyEvents(SDL_Event* e) {
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
                stopMusic();
                playPVP();
                break;
            case SDLK_2:
                stopMusic();
                playerVersusComputer();
                break;
            case SDLK_3:
                SDL_Delay(200);
                this->flushEvents();
                settings();
                break;
            }
	}
}

void LMainMenuState::handleMouseEvents(SDL_Event* e) {
    for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		if(mMenuButtons[i]->handleInside(e)) {
			highlightSelected(i);
			if(mMenuButtons[i]->handleClick(e)) {
				if(i == PLAY) {
					stopMusic();
					playPVP();
					flushEvents();
                    break;
				}
				else if(i == PLAY_AI) {
					stopMusic();
					playerVersusComputer();
					flushEvents();
                    break;
				}
				else if(i == SETTINGS) {
					SDL_Delay(200);
					flushEvents();
					settings();
                    break;
				}
				else if(i == DEVMODE) {
					SDL_Delay(200);
					//flush events to prevent clicks clicking the 
					flushEvents();
                    break;
				}
			}
		}
	}
}

void LMainMenuState::setButtons() {
	for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		mMenuButtons[i] = new LButton(
			mMenuTextures[i]->x(), 
			mMenuTextures[i]->y(), 
			mMenuTextures[i]->getWidth(), 
			mMenuTextures[i]->getHeight()
		);
	}
    mButtonsAreSet = true;
}

void LMainMenuState::unsetButtons() {
	for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		mMenuButtons[i]->setWidthAndHeight(1,1);
		mMenuButtons[i]->setPosition(0,0);
	}
    mButtonsAreSet = false;
}

void LMainMenuState::flushEvents() {
	SDL_PumpEvents();
	SDL_FlushEvents(SDL_MOUSEMOTION, SDL_MOUSEBUTTONUP);
}

void LMainMenuState::playMusic() {
	if(Mix_PlayingMusic() == 0) {
		Mix_PlayMusic(mMenuMusic, -1);
		Mix_VolumeMusic(gMusicVolume);
	}
}

void LMainMenuState::stopMusic() {
	if(Mix_PlayingMusic()) {
		Mix_HaltMusic();
	}
}