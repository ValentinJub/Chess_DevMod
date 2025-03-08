#include "states/game/LBoardState.h"

extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;

LBoardState::LBoardState(PlayMode mode) : mPlayMode(mode) {}

LBoardState::~LBoardState() {
	mBoard->free();
	mBoard = NULL;
}

void LBoardState::enter(LObserver* observer) {
	mBoard = new LBoard(observer, mPlayMode);
}

void LBoardState::exit() {
	mBoard->free();
}

void LBoardState::update(Uint64 dt) {
	mBoard->update();
}

void LBoardState::render() {
	mBoard->render();
}