#include "LMain.h"

LMain::LMain() {
    mButtonsAreSet = false;
}

void LMain::free() {
    for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
        mMenuTextures[i].free();
    }
    Mix_FreeChunk(mMenuClick);
	mMenuClick = NULL;
	 
	Mix_FreeMusic(mMenuMusic);
	mMenuMusic = NULL;
}

bool LMain::init() {
	bool success = true; 
	if(!(mMenuTextures[PLAY].loadFromRenderedText(gFont64, MENU_PLAY_STR.c_str() , COLOR_BLACK))) {
		success = false;
	}
	else if(!(mMenuTextures[PLAY_AI].loadFromRenderedText(gFont64, MENU_PLAY_AI_STR.c_str() , COLOR_BLACK))) {
		success = false;
	}
	else if(!(mMenuTextures[SETTINGS].loadFromRenderedText(gFont64, MENU_SETTINGS_STR.c_str() , COLOR_BLACK))) {
		success = false;
	}
	else if(!(mMenuTextures[DEVMODE].loadFromRenderedText(gFont64, MENU_DEVMODE_STR.c_str() , COLOR_BLACK))) {
		success = false;
	}
	else if(!(gBackgroundTexture.loadFromFile(SPRITE_BACKGROUND))) {
		success = false;
	}
    
    this->setTexturePositions();
    this->setButtons();

    mMenuMusic = loadMusic(MUSIC_MENU);

	return success;
}

void LMain::setTexturePositions() {
    // std::cout << "Setting texture positions" << std::endl;
    mMenuTextures[PLAY_AI].setX((SCREEN_WIDTH - mMenuTextures[PLAY_AI].getWidth()) / 2);
    mMenuTextures[PLAY_AI].setY((SCREEN_HEIGHT / 2) - (mMenuTextures[PLAY_AI].getHeight())); 
    mMenuTextures[SETTINGS].setX((SCREEN_WIDTH - mMenuTextures[SETTINGS].getWidth()) / 2);
    mMenuTextures[SETTINGS].setY(mMenuTextures[PLAY_AI].y() + mMenuTextures[PLAY_AI].getHeight());
    mMenuTextures[PLAY].setX((SCREEN_WIDTH - mMenuTextures[PLAY].getWidth()) / 2);
    mMenuTextures[PLAY].setY(mMenuTextures[PLAY_AI].y() - mMenuTextures[PLAY].getHeight());
    mMenuTextures[DEVMODE].setX((SCREEN_WIDTH - mMenuTextures[DEVMODE].getWidth()) / 2);
    mMenuTextures[DEVMODE].setY(mMenuTextures[SETTINGS].y() + mMenuTextures[SETTINGS].getHeight());
}

void LMain::highlightSelected(int position) {
	switch(position) {
		case PLAY:
			mMenuTextures[PLAY].loadFromRenderedText(gFont64, MENU_PLAY_STR.c_str() , COLOR_RED);
			mMenuTextures[PLAY_AI].loadFromRenderedText(gFont64, MENU_PLAY_AI_STR.c_str() , COLOR_BLACK);
			mMenuTextures[SETTINGS].loadFromRenderedText(gFont64, MENU_SETTINGS_STR.c_str() , COLOR_BLACK);
			mMenuTextures[DEVMODE].loadFromRenderedText(gFont64, MENU_DEVMODE_STR.c_str() , COLOR_BLACK);
			break;
		case PLAY_AI:
			mMenuTextures[PLAY].loadFromRenderedText(gFont64, MENU_PLAY_STR.c_str() , COLOR_BLACK);
			mMenuTextures[PLAY_AI].loadFromRenderedText(gFont64, MENU_PLAY_AI_STR.c_str() , COLOR_RED);
			mMenuTextures[SETTINGS].loadFromRenderedText(gFont64, MENU_SETTINGS_STR.c_str() , COLOR_BLACK);
			mMenuTextures[DEVMODE].loadFromRenderedText(gFont64, MENU_DEVMODE_STR.c_str() , COLOR_BLACK);
			break;
		case SETTINGS:
			mMenuTextures[PLAY].loadFromRenderedText(gFont64, MENU_PLAY_STR.c_str() , COLOR_BLACK);
			mMenuTextures[PLAY_AI].loadFromRenderedText(gFont64, MENU_PLAY_AI_STR.c_str() , COLOR_BLACK);
			mMenuTextures[SETTINGS].loadFromRenderedText(gFont64, MENU_SETTINGS_STR.c_str() , COLOR_RED);
			mMenuTextures[DEVMODE].loadFromRenderedText(gFont64, MENU_DEVMODE_STR.c_str() , COLOR_BLACK);
			break;
		case DEVMODE:
			mMenuTextures[PLAY].loadFromRenderedText(gFont64, MENU_PLAY_STR.c_str() , COLOR_BLACK);
			mMenuTextures[PLAY_AI].loadFromRenderedText(gFont64, MENU_PLAY_AI_STR.c_str() , COLOR_BLACK);
			mMenuTextures[SETTINGS].loadFromRenderedText(gFont64, MENU_SETTINGS_STR.c_str() , COLOR_BLACK);
			mMenuTextures[DEVMODE].loadFromRenderedText(gFont64, MENU_DEVMODE_STR.c_str() , COLOR_RED);
			break;
		default: 
			mMenuTextures[PLAY].loadFromRenderedText(gFont64, MENU_PLAY_STR.c_str() , COLOR_BLACK);
			mMenuTextures[PLAY_AI].loadFromRenderedText(gFont64, MENU_PLAY_AI_STR.c_str() , COLOR_BLACK);
			mMenuTextures[SETTINGS].loadFromRenderedText(gFont64, MENU_SETTINGS_STR.c_str() , COLOR_BLACK);
			mMenuTextures[DEVMODE].loadFromRenderedText(gFont64, MENU_DEVMODE_STR.c_str() , COLOR_BLACK);
			break;
	}
    // Re-loading the textures with the new color will remove the position of the textures, so we need to reset them
    this->setTexturePositions();
}

bool LMain::handleEvents(SDL_Event* e) {
    if(handleKeyEvents(e)) {
        return true;
    }
    handleMouseEvents(e);
    return false;
}

bool LMain::handleKeyEvents(SDL_Event* e) {
	if(e->type == SDL_QUIT) {
		return true;
	} 
    else if(e->type == SDL_KEYDOWN) {
        switch(e->key.keysym.sym) {
            case SDLK_ESCAPE:
                return true;
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
    return false;
}

void LMain::handleMouseEvents(SDL_Event* e) {
    for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		if(mMenuButtons[i].handleInside(e)) {
			highlightSelected(i);
			if(mMenuButtons[i].handleClick(e)) {
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


void LMain::render() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	
	//render BG 1st
	gBackgroundTexture.render();

	mMenuTextures[PLAY_AI].render(mMenuTextures[PLAY_AI].x(), mMenuTextures[PLAY_AI].y());
	mMenuTextures[SETTINGS].render(mMenuTextures[SETTINGS].x(), mMenuTextures[SETTINGS].y());
	mMenuTextures[PLAY].render(mMenuTextures[PLAY].x(), mMenuTextures[PLAY].y());
	mMenuTextures[DEVMODE].render(mMenuTextures[DEVMODE].x(), mMenuTextures[DEVMODE].y());

	SDL_RenderPresent(gRenderer);
}

void LMain::setButtons() {
	for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		mMenuButtons[i].setWidthAndHeight(mMenuTextures[i].getWidth(), mMenuTextures[i].getHeight());
		mMenuButtons[i].setPosition(mMenuTextures[i].x(), mMenuTextures[i].y());
	}
    // std::cout << "Buttons are now set" << std::endl;
    mButtonsAreSet = true;
}

void LMain::unsetButtons() {
	for(int i(0); i < TOTAL_MENU_ITEMS; i++) {
		mMenuButtons[i].setWidthAndHeight(1,1);
		mMenuButtons[i].setPosition(0,0);
	}
    // std::cout << "Buttons are now unset" << std::endl;
    mButtonsAreSet = false;
}

void LMain::flushEvents() {
	SDL_PumpEvents();
	SDL_FlushEvents(SDL_MOUSEMOTION, SDL_MOUSEBUTTONUP);
}

void LMain::playMusic() {
	if(Mix_PlayingMusic() == 0) {
		// loop infinitely menu music
		Mix_PlayMusic(mMenuMusic, -1);
		// set volume 
		Mix_Volume(-1, gMusicVolume);
		Mix_VolumeMusic(gMusicVolume);
	}
}

void LMain::stopMusic() {
	if(Mix_PlayingMusic()) {
		Mix_FadeOutMusic(300);
	}
}