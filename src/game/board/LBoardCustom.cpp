/*
LBoardCustom.cpp
Sun Mar 20 2022
by Valentin
-----------
Methods for class LBoardCustom
*/

#include "game/board/LBoardCustom.h"
#include "states/menu/LMainMenuState.h"
#include "game/board/LClock.h"
#include "graphics/LPromotion.h"
#include "game/board/layout.h"
#include "game/logic/LComputer.h"

using std::vector;
extern TTF_Font* gFont64;
extern TTF_Font* gFont16;
extern SDL_Renderer* gRenderer;
extern uint8_t gMusicVolume;
extern LMediaFactory* gMediaFactory;
extern LChunkPlayer* gChunkPlayer;
extern LMusicPlayer* gMusicPlayer;
extern LStateMachine* gStateMachine;
extern LTexture* gBackgroundTexture;
constexpr int AI_DELAY = 1000;

LBoardCustom::LBoardCustom(LObserver* observer) : mAppObserver(observer) {
	gStateMachine->push(new LTransition(FADE_IN, NULL));
	this->Attach(observer);
	mTileTexture = gMediaFactory->getImg(SPRITE_BOARD);
	mPieceTexture = gMediaFactory->getImg(SPRITE_PIECE_SHEET);
	
	mTileRectClip[DARK1].x = 0;
	mTileRectClip[DARK1].y = 0;
	mTileRectClip[DARK1].w = TOTAL_SQUARES;
	mTileRectClip[DARK1].h = TOTAL_SQUARES;
	
	mTileRectClip[LIGHT1].x = TOTAL_SQUARES;
	mTileRectClip[LIGHT1].y = 0;
	mTileRectClip[LIGHT1].w = TOTAL_SQUARES;
	mTileRectClip[LIGHT1].h = TOTAL_SQUARES;
	
	mTileRectClip[DARK2].x = 128;
	mTileRectClip[DARK2].y = 0;
	mTileRectClip[DARK2].w = TOTAL_SQUARES;
	mTileRectClip[DARK2].h = TOTAL_SQUARES;
	
	mTileRectClip[LIGHT2].x = 192;
	mTileRectClip[LIGHT2].y = 0;
	mTileRectClip[LIGHT2].w = TOTAL_SQUARES;
	mTileRectClip[LIGHT2].h = TOTAL_SQUARES;
	
	for(int i(0); i < TOTAL_PIECES - 1; i++) {
		mPieceClip[i].x = TOTAL_SQUARES*i;
		mPieceClip[i].y = 0;
		mPieceClip[i].w = TOTAL_SQUARES;
		mPieceClip[i].h = TOTAL_SQUARES;
	}
	mBoard = emptyBoard;
}


void LBoardCustom::update() {
	SDL_Event e;
	while(SDL_PollEvent(&e) > 0) {
		if(e.type == SDL_QUIT) {
			this->Notify(0);
			gStateMachine->pop();
			return;
		}
		else if(e.type == SDL_KEYDOWN) {
			if(e.key.keysym.sym == SDLK_ESCAPE) {
				gStateMachine->push(new LTransition(FADE_OUT, new LMainMenuState));
				return;
			}
			else if(e.key.keysym.sym == SDLK_SPACE) {
				// this->pause();
			} 
			else if(e.key.keysym.sym == SDLK_x) {
				mBoard = promotionBoard;
			} 
			else if(e.key.keysym.sym == SDLK_c) {
				mBoard = castlingBoard;
			}
			else if(e.key.keysym.sym == SDLK_r) {
				mBoard = normalBoard;
			}
		}
		this->handleEvents(&e);
	}
	//set buttons according to piece pos
	this->setBoardButtons();
}

void LBoardCustom::render() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	this->renderTile();
	this->renderBoardPieces();
}

LBoardCustom::~LBoardCustom() {
	free();
}

void LBoardCustom::free() {
	gMusicPlayer->pause();
	mPieceTexture->free();
	mPieceTexture = NULL;
	mTileTexture->free();
	mTileTexture = NULL;
	mPieceButtons.clear();
	this->Detach(mAppObserver);
}

void LBoardCustom::renderTile() {
	bool light = true,
	dark = true;
	int xPos(0);
	int yPos(0);
	for(int y(0); y < SPL; y++) {
		for(int x(0); x < SPL; x++) {
			xPos = OFFSET + (TOTAL_SQUARES * x);
			yPos = OFFSET + (TOTAL_SQUARES * y);
			if(y % 2 == 0) {
				if(!light) mTileTexture->renderAt(xPos, yPos, &mTileRectClip[DARK1]);
				else mTileTexture->renderAt(xPos, yPos, &mTileRectClip[LIGHT1]);
				light = !light;
			}
			else {
				if(!dark) mTileTexture->renderAt(xPos, yPos, &mTileRectClip[LIGHT1]);
				else mTileTexture->renderAt(xPos, yPos, &mTileRectClip[DARK1]);
				dark = !dark;
			}
			if(x == 0) {
				const int coord = 8 - y;
				LTexture* txt = gMediaFactory->getTxt(std::to_string(coord), gFont16, COLOR_RED);
				txt->renderAt(xPos, yPos);
			}
			if(y == 7) {
				const char coord = 'a' + x;
				LTexture* txt = gMediaFactory->getTxt(std::string(1, coord), gFont16, COLOR_RED);
				txt->renderAt(xPos + OFFSET - txt->w() , yPos + OFFSET - txt->h());
			}
		}
	}
}
	
void LBoardCustom::renderBoardPieces() {
	for(int y(0); y < SPL; y++) {
		for(int x(0); x < SPL; x++) {
			if((mBoard[y][x] >= 0) && (mBoard[y][x] < TOTAL_PIECES - 1)) {
				mPieceTexture->renderAt(
					OFFSET + (TOTAL_SQUARES * x), 
					OFFSET + (TOTAL_SQUARES * y), 
					&mPieceClip[mBoard[y][x]]
				);
			}
		}
	}
}

//function used to check the buttons box are set properly
void LBoardCustom::drawButtons() {
	for(int i(0); i < INITIAL_PIECES_TOTAL - 1; i++) {
		SDL_Rect r;
		r.x = mPieceButtons[i]->getX();
		r.y = mPieceButtons[i]->getY();
		r.w = mPieceButtons[i]->getW();
		r.h = mPieceButtons[i]->getH();
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0xFF);
		SDL_RenderDrawRect(gRenderer, &r);
	}
}

void LBoardCustom::setBoardButtons() {
	int i(0);
	for(int y(0); y < SPL; y++) {
		for(int x(0); x < SPL; x++) {
			if((mBoard[y][x] >= 0) && (mBoard[y][x] < TOTAL_PIECES - 1)) {
				mPieceButtons[i]->setPos(
					(x * TOTAL_SQUARES) + OFFSET, 
					(y * TOTAL_SQUARES) + OFFSET
				);
				mPieceButtons[i]->setSize(TOTAL_SQUARES,TOTAL_SQUARES);
				i++;
			}
		}
	}
}

void LBoardCustom::handleEvents(SDL_Event* e) {
	// bool outside = true; 
	// int x, y;
	// SDL_GetMouseState( &x, &y );
	// x = (x / TOTAL_SQUARES) - 1;
	// y = (y / TOTAL_SQUARES) - 1; 
	// //check for each buttons
	// int size = mPieceButtons.size();
	// for(int i(0); i < size; i++) {
	// 	//if mouse is inside a button
	// 	if(mPieceButtons[i]->isHovered()) {
	// 		//if THE LEFT mouse button clicked 
	// 		if(mPieceButtons[i]->isClicked(e)) {
	// 			//if a piece is not selected 
	// 			//if a piece is already selected, count as clicking outside 
	// 			if(!(mAPieceIsSelected)) {
	// 				mSelectedPiece = mBoard[y][x];
	// 				//if white's turn - select white
	// 				if((mWhiteTurn) && (mSelectedPiece >= WBISHOP) && (mSelectedPiece < EMPTY)) {
	// 					outside = false;
	// 					mSelectedPiecePos = {x,y};
	// 					mAPieceIsSelected = true;
	// 					mRightClickedTile.clear();
	// 				}
	// 				//if black's turn - select black
	// 				else if(!(mWhiteTurn) && (mSelectedPiece >= BBISHOP) && (mSelectedPiece < WBISHOP)) {
	// 					outside = false;
	// 					mSelectedPiecePos = {x,y};
	// 					mAPieceIsSelected = true;
	// 					mRightClickedTile.clear();
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	// //if a piece is selected - can move
	// //to pos set in shwlegalMove()
	// if(mAPieceIsSelected) move(e);
	// if(outside) {
	// 	if((e->type == SDL_MOUSEBUTTONUP) && (e->button.button == SDL_BUTTON_LEFT)) {
	// 		mAPieceIsSelected = false;
	// 		mSelectedPiecePos = {9, 9};
	// 		mLegalMoveTile.clear();
	// 		mRightClickedTile.clear();
	// 	}
	// }
	// if((e->type == SDL_MOUSEBUTTONUP) && (e->button.button == SDL_BUTTON_RIGHT)) {
	// 	bool dont = false;
	// 	int size = mRightClickedTile.size();
	// 	for(int z(0); z < size; z++) {
	// 		if((mRightClickedTile[z].x == x) && (mRightClickedTile[z].y == y)) {
	// 			mRightClickedTile.erase(mRightClickedTile.begin() + z);
	// 			dont = true;
	// 		}
	// 	}
	// 	if(!(dont)) {
	// 		mRightClickedTile.push_back({x,y});
	// 	}
	// }
}