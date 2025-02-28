#include "LMainMenuState.h"

LMainMenuState::LMainMenuState() {};

void LMainMenuState::enter(LObserver* observer) {
	mMainMenu = new LMainMenu(observer);
}

void LMainMenuState::exit() {
	mMainMenu->free();
}

void LMainMenuState::update(Uint64 dt) {
	mMainMenu->update(dt);
}

void LMainMenuState::render() {
	mMainMenu->render();
}