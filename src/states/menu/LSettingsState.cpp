/*
settings.cpp
Sun Mar 20 2022
by Valentin
-----------
Settings menu allowing to select various options such as:

- Showing legal moves 
- Play with a timer
- Tile/Piece color/design
- Choice of different music? (playing in loop?) or a set of music playing
- etc....
*/

#include "states/menu/LSettingsState.h"

extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;

void LSettingsState::enter(LObserver* observer) {
	mSettings = new LMenuSettings(observer);
};

void LSettingsState::exit() {
	mSettings->free();
	mSettings = NULL;
};

void LSettingsState::update(Uint64 dt) {
	mSettings->update();
};

void LSettingsState::render() {
	mSettings->render();
};