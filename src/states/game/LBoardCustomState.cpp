#include "states/game/LBoardCustomState.h"

extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;

LBoardCustomState::LBoardCustomState() {}

LBoardCustomState::~LBoardCustomState() {
	mBoard->free();
	mBoard = NULL;
}

void LBoardCustomState::enter(LObserver* observer) {
	mBoard = new LBoardCustom(observer);
}

void LBoardCustomState::exit() {
	mBoard->free();
}

void LBoardCustomState::update(Uint64 dt) {
	mBoard->update();
}

void LBoardCustomState::render() {
	mBoard->render();
}