/*
LBoard.cpp
Sun Mar 20 2022
by Valentin
-----------
Methods for class LBoard
*/

#include "game/board/LBoard.h"
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

LBoard::LBoard(LObserver* observer, PlayMode mode) : mPlayMode(mode), mAppObserver(observer) {
	gStateMachine->push(new LTransition(FADE_IN, NULL));
	this->Attach(observer);
	this->initSettings();
	this->initTileTextures();
	this->initPiecesTextures();
	this->initPauseTexture();
	this->setTileRectClip();
	this->setPiecesClip();

	if(mPlayMode == PVAI) {
		mComputer = new LComputer(new LEngine);
	} else {
		mComputer = NULL;
	}

	mBoard = normalBoard;

	for(int i(0); i < INITIAL_PIECES_TOTAL; i++) {
		mPieceButtons.push_back(new LButton);
	}
	
	for(int i(0); i < EMPTY; i++) {
		mDeadWhitePiece[i] = 0;
		mDeadBlackPiece[i] = 0;
	}
	
	//Set to 9 so no pieces 
	//Are highlighted at start
	mSelectedPiecePos = {9, 9};

	mClock = mSettings.useTimer == 0 ? new LClock(mTimeLimit) : NULL;
	if(mClock != NULL) {
		mClock->start();
	}
}

void LBoard::poll(LSubject* sub, int value) {
	if(value != -1) {
		mBoard[mPromotedPiecePos.y][mPromotedPiecePos.x] = value;
		this->postMove(mPromotedPiecePos);
	}
}

void LBoard::update() {
	if(mPlayMode == PVAI && !mWhiteTurn && !mIsPaused) {
		SDL_Delay(500);
		this->computerMove();
		return;
	}
	if(mGameOver) {
		if(Mix_PlayingMusic()) {
			Mix_FadeOutMusic(3000);
		}
		const int chanCM = gChunkPlayer->play(CHUNK_CHECKMATE);
		while(Mix_Playing(chanCM) > 0) {
			SDL_Delay(16);
		}
		const int chanVictory = gChunkPlayer->play(CHUNK_VICTORY);
		while(Mix_Playing(chanVictory) > 0) {
			SDL_Delay(16);
		}
		gStateMachine->push(new LTransition(FADE_OUT, new LMainMenuState));
		return;
	}
	else if(mWhiteTimerRanOut || mBlackTimerRanOut) {
		if(Mix_PlayingMusic()) {
			Mix_FadeOutMusic(500);
		}
		this->initOutOfTimeTexture();
		this->renderOutOfTimeScreen();
		const int chanDef = gChunkPlayer->play(CHUNK_DEFEAT);
		while(Mix_Playing(chanDef) > 0) {
			SDL_Delay(16);
		}
		gStateMachine->push(new LTransition(FADE_OUT, new LMainMenuState));
		return;
	}

	if(Mix_PlayingMusic() == 0) {
		this->playMusic();
	}
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
				this->pause();
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
		if(!(mIsPaused)) {
			this->handleEvents(&e);
		}
	}
	//set buttons according to piece pos
	this->setButtons();
}

void LBoard::render() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	this->renderTile();
	this->renderPieces();
	if(mSettings.useTimer == 0) {
		this->renderTimer();
	}
	this->renderScore();
	this->renderDeadPieces();
	if(mIsPaused) {
		this->renderPause();
	}
}

void LBoard::renderTileCoordinates() {

}

void LBoard::playMusic() {
	if(Mix_PlayingMusic() == 0 && !mIsPaused) {
		int i = rand() % 7;
		std::string track;
		if(mSettings.musicTheme == 1) {
			track = MUSIC_CLASSIC + std::to_string(i) + ".mp3";
		}
		else {
			track = MUSIC_JAZZY + std::to_string(i) + ".mp3";
		}
		gMusicPlayer->setVolume(mSettings.volume);
		gMusicPlayer->playFadeIn(track.c_str(), 5000);
	}
}

void LBoard::pause() {
	if(mIsPaused && mSettings.useTimer == 0) {
		mClock->unpause();
	} else if(!mIsPaused && mSettings.useTimer == 0) {
		mClock->pause();
	}
	mIsPaused = !mIsPaused;
	gMusicPlayer->pause();
}

LBoard::~LBoard() {
	free();
}

void LBoard::free() {
	gMusicPlayer->pause();
	this->Detach(mAppObserver);
	mPieceTexture->free();
	mPieceTexture = NULL;
	
	mHighlightedPieceTexture->free();
	mHighlightedPieceTexture = NULL;
	
	mTileTexture->free();
	mTileTexture = NULL;
	
	if(mWhiteTimerTexture != NULL) {
		mWhiteTimerTexture->free();
		mWhiteTimerTexture = NULL;
	}
	if(mBlackTimerTexture != NULL) {
		mBlackTimerTexture->free();
		mBlackTimerTexture = NULL;
	}
	
	mWhiteScoreTexture->free();
	mWhiteScoreTexture = NULL;
	mBlackScoreTexture->free();
	mBlackScoreTexture = NULL;
	
	mPieceButtons.clear();
	mLegalMoveTile.clear();
}

void LBoard::initSettings() {
	std::vector<int> values = Util::readSettingsFromFile(FILE_SETTINGS);	
	if(values.size() < 7) {
		spdlog::error("Settings file is corrupted! {} values found", values.size());
		return;
	}
	mSettings = {
		values[0],
		values[1],
		values[2],
		values[3],
		values[4],
		values[5],
		values[6]
	};

	mTimeLimit = mSettings.timeLimit == 0 ? 3 : 600;
}

bool LBoard::initPiecesTextures() {
	const char* spriteSheet = mSettings.pieceTheme == 0 ? SPRITE_PIECE_SHEET : SPRITE_RETRO_PIECE_SHEET;
	mPieceTexture = gMediaFactory->getImg(spriteSheet);
	mHighlightedPieceTexture = gMediaFactory->getImgUnique(spriteSheet);
	mHighlightedPieceTexture->setColor(255,0,0);
	mMiniPieceTexture = gMediaFactory->getImg(SPRITE_PIECE_SHEET_32);
	return mPieceTexture != NULL && mHighlightedPieceTexture != NULL && mMiniPieceTexture != NULL; 
}

bool LBoard::initTileTextures() {
	mTileTexture = gMediaFactory->getImg(SPRITE_BOARD);
	return mTileTexture != NULL;
}

bool LBoard::initPauseTexture() {
	mPauseBackgroundTexture = gMediaFactory->getImg(SPRITE_BACKGROUND_FULLBLACK);
	mPauseBackgroundTexture->setAlpha(127);
	mPauseBackgroundTexture->setBlendMode(SDL_BLENDMODE_BLEND);
	mPauseTextTexture = gMediaFactory->getTxt("Pause", gFont64, COLOR_WHITE);
	return mPauseBackgroundTexture != NULL && mPauseTextTexture != NULL;
}

bool LBoard::initOutOfTimeTexture() {
	mOutOfTimeTexture = gMediaFactory->getTxt("Out of time!", gFont64, COLOR_WHITE);
	return mOutOfTimeTexture != NULL;
}

void LBoard::setPiecesClip() {
	for(int i(0); i < TOTAL_PIECES - 1; i++) {
		mPieceClip[i].x = TOTAL_SQUARES*i;
		mPieceClip[i].y = 0;
		mPieceClip[i].w = TOTAL_SQUARES;
		mPieceClip[i].h = TOTAL_SQUARES;
		
		mMiniPieceClip[i].x = (TOTAL_SQUARES / 2) * i;
		mMiniPieceClip[i].y = 0;
		mMiniPieceClip[i].w = (TOTAL_SQUARES / 2);
		mMiniPieceClip[i].h = (TOTAL_SQUARES / 2);	
	}
}

void LBoard::setTileRectClip() {
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
}

void LBoard::renderTile() {
	bool light = true,
	dark = true;
	int xPos(0);
	int yPos(0);
	//color settings
	for(int y(0); y < SPL; y++) {
		for(int x(0); x < SPL; x++) {
			xPos = OFFSET + (TOTAL_SQUARES * x);
			yPos = OFFSET + (TOTAL_SQUARES * y);
			if(y % 2 == 0) {
				if(mSettings.tileColor == 0) {
					if(!light) mTileTexture->renderAt(xPos, yPos, &mTileRectClip[DARK1]);
					else mTileTexture->renderAt(xPos, yPos, &mTileRectClip[LIGHT1]);
				}
				else if(mSettings.tileColor == 1) {
					if(!light) mTileTexture->renderAt(xPos, yPos, &mTileRectClip[DARK2]);
					else mTileTexture->renderAt(xPos, yPos, &mTileRectClip[LIGHT2]);
				}
				if(light) light = false;
				else light = true;
			}
			else {
				if(mSettings.tileColor == 0) {
					if(!dark) mTileTexture->renderAt(xPos, yPos, &mTileRectClip[LIGHT1]);
					else mTileTexture->renderAt(xPos, yPos, &mTileRectClip[DARK1]);
				}
				else if(mSettings.tileColor == 1) {
					if(!dark) mTileTexture->renderAt(xPos, yPos, &mTileRectClip[LIGHT2]);
					else mTileTexture->renderAt(xPos, yPos, &mTileRectClip[DARK2]);
				}
				if(dark) dark = false;
				else dark = true;
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
	//only highlight tile if a piece is selected
	if(mAPieceIsSelected && mSettings.showLegalMoves == 0) {
		std::vector<SDL_Point> legalMoves = mEngine.getLegalMoves(mBoard, mSelectedPiecePos);
		for(int z(0); z < legalMoves.size(); z++) {
			yPos = OFFSET + (legalMoves[z].y * TOTAL_SQUARES);
			xPos = OFFSET + (legalMoves[z].x * TOTAL_SQUARES);
			mTileTexture->addDecorator(new LTileBorder(
				SDL_Point{xPos, yPos},
				SDL_Point{xPos + TOTAL_SQUARES, yPos + TOTAL_SQUARES},
				TOTAL_SQUARES,
				TOTAL_SQUARES,
				3
			));
			mTileTexture->setRenderDecorators(true);
			mTileTexture->renderAt(xPos, yPos, &mTileRectClip[LIGHT1]);
			mTileTexture->removeDecorator(mTileTexture->getDecorators()[0].get());
		}
	}
	if(mRightClickedTile.size() > 0) {
		int size = mRightClickedTile.size();
		for(int z(0); z < size; z++) {
			yPos = OFFSET + (mRightClickedTile[z].y * TOTAL_SQUARES);
			xPos = OFFSET + (mRightClickedTile[z].x * TOTAL_SQUARES);
			mTileTexture->renderAt(xPos, yPos, &mTileRectClip[LIGHT2]);
		}
	}
}
	
void LBoard::renderPieces() {
	for(int y(0); y < SPL; y++) {
		for(int x(0); x < SPL; x++) {
			if((mBoard[y][x] >= 0) && (mBoard[y][x] < TOTAL_PIECES - 1)) {
				if( (mAPieceIsSelected) && (mSelectedPiecePos.x == x) && (mSelectedPiecePos.y == y)) {
					mHighlightedPieceTexture->renderAt(
						OFFSET + (TOTAL_SQUARES * x), 
						OFFSET + (TOTAL_SQUARES * y), 
						&mPieceClip[mBoard[y][x]]
					);
				}
				else mPieceTexture->renderAt(
					OFFSET + (TOTAL_SQUARES * x), 
					OFFSET + (TOTAL_SQUARES * y), 
					&mPieceClip[mBoard[y][x]]
				);
			}
		}
	}
}

//function used to check the buttons box are set properly
void LBoard::drawButtons() {
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

void LBoard::setButtons() {
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

void LBoard::renderPause() {
	mPauseBackgroundTexture->render();
	mPauseTextTexture->renderAt((SCREEN_WIDTH - mPauseTextTexture->w()) / 2, (SCREEN_HEIGHT - mPauseTextTexture->h()) / 2); 
}

void LBoard::renderOutOfTimeScreen() {
	mPauseBackgroundTexture->render();
	mOutOfTimeTexture->renderAt((SCREEN_WIDTH - mOutOfTimeTexture->w()) / 2, (SCREEN_HEIGHT - mOutOfTimeTexture->h()) / 2);
	SDL_RenderPresent(gRenderer);
}

void LBoard::handleEvents(SDL_Event* e) {
	bool outside = true; 
	int x, y;
	SDL_GetMouseState( &x, &y );
	x = (x / TOTAL_SQUARES) - 1;
	y = (y / TOTAL_SQUARES) - 1; 
	//check for each buttons
	int size = mPieceButtons.size();
	for(int i(0); i < size; i++) {
		//if mouse is inside a button
		if(mPieceButtons[i]->isHovered()) {
			//if THE LEFT mouse button clicked 
			if(mPieceButtons[i]->isClicked(e)) {
				//if a piece is not selected 
				//if a piece is already selected, count as clicking outside 
				if(!(mAPieceIsSelected)) {
					mSelectedPiece = mBoard[y][x];
					//if white's turn - select white
					if((mWhiteTurn) && (mSelectedPiece >= WBISHOP) && (mSelectedPiece < EMPTY)) {
						outside = false;
						mSelectedPiecePos = {x,y};
						mAPieceIsSelected = true;
						mRightClickedTile.clear();
					}
					//if black's turn - select black
					else if(!(mWhiteTurn) && (mSelectedPiece >= BBISHOP) && (mSelectedPiece < WBISHOP)) {
						outside = false;
						mSelectedPiecePos = {x,y};
						mAPieceIsSelected = true;
						mRightClickedTile.clear();
					}
				}
			}
		}
	}
	//if a piece is selected - can move
	//to pos set in shwlegalMove()
	if(mAPieceIsSelected) move(e);
	if(outside) {
		if((e->type == SDL_MOUSEBUTTONUP) && (e->button.button == SDL_BUTTON_LEFT)) {
			mAPieceIsSelected = false;
			mSelectedPiecePos = {9, 9};
			mLegalMoveTile.clear();
			mRightClickedTile.clear();
		}
	}
	if((e->type == SDL_MOUSEBUTTONUP) && (e->button.button == SDL_BUTTON_RIGHT)) {
		bool dont = false;
		int size = mRightClickedTile.size();
		for(int z(0); z < size; z++) {
			if((mRightClickedTile[z].x == x) && (mRightClickedTile[z].y == y)) {
				mRightClickedTile.erase(mRightClickedTile.begin() + z);
				dont = true;
			}
		}
		if(!(dont)) {
			mRightClickedTile.push_back({x,y});
		}
	}
}

void LBoard::move(SDL_Event* e) {
	int destinationPosX, destinationPosY;
	SDL_GetMouseState( &destinationPosX, &destinationPosY );
	SDL_Point destPos = {(destinationPosX / TOTAL_SQUARES) - 1, (destinationPosY / TOTAL_SQUARES) - 1};
	if((e->type == SDL_MOUSEBUTTONUP) && (e->button.button == SDL_BUTTON_LEFT)) {
		std::vector<SDL_Point> legalPos = mEngine.getLegalMoves(mBoard, mSelectedPiecePos);
		int size = legalPos.size();
		for(int i(0); i < size; i++) {
			if((destPos.x == legalPos[i].x) && (destPos.y == legalPos[i].y)) {
				//only move if it does not check your own king 
				if(!mEngine.isMoveSelfCheck(mBoard, mSelectedPiecePos, destPos, mWhiteTurn)) { 
					doMove(destPos, mSelectedPiecePos, mSelectedPiece);
					break;
				} else gChunkPlayer->play(CHUNK_ERROR);
			}
		}
	}
}

void LBoard::doMove(SDL_Point dest, SDL_Point src, int piece) {
	bool captured = false;
	//if the square destination is not empty, substract a button
	if(mBoard[dest.y][dest.x] != EMPTY) {
		mPieceButtons.resize(mPieceButtons.size() - 1);
		fillDeadPieceTab(mBoard[dest.y][dest.x]);
		captured = true;
	}
	
	bool castled = false;
	// Handle castling move
	if((piece == WKING) && (src.x == 4) && (src.y == 7) && (dest.x == 6) && (dest.y == 7)) {
		mBoard[7][5] = WROOK;
		mBoard[7][7] = EMPTY;
		castled = true;
	}
	else if((piece == WKING) && (src.x == 4) && (src.y == 7) && (dest.x == 2) && (dest.y == 7)) {
		mBoard[7][3] = WROOK;
		mBoard[7][0] = EMPTY;
		castled = true;
	}
	else if((piece == BKING) && (src.x == 4) && (src.y == 0) && (dest.x == 6) && (dest.y == 0)) {
		mBoard[0][5] = BROOK;
		mBoard[0][7] = EMPTY;
		castled = true;
	}
	else if((piece == BKING) && (src.x == 4) && (src.y == 0) && (dest.x == 2) && (dest.y == 0)) {
		mBoard[0][3] = BROOK;
		mBoard[0][0] = EMPTY;
		castled = true;
	}

	// Handle en passant move
	if((piece == WPAWN) && (src.x != dest.x) && (mBoard[dest.y][dest.x] == EMPTY)) {
		mBoard[dest.y + 1][dest.x] = EMPTY;
		mPieceButtons.resize(mPieceButtons.size() - 1);
		this->fillDeadPieceTab(BPAWN);
		captured = true;
	}
	else if((piece == BPAWN) && (src.x != dest.x) && (mBoard[dest.y][dest.x] == EMPTY)) {
		mBoard[dest.y - 1][dest.x] = EMPTY;
		mPieceButtons.resize(mPieceButtons.size() - 1);
		this->fillDeadPieceTab(WPAWN);
		captured = true;	
	}

	this->playMoveSound(captured, castled);
	
	//move selected piece to destination
	mBoard[dest.y][dest.x] = piece;
	//empty source cell 
	mBoard[src.y][src.x] = EMPTY;
	
	//turn castling bools to true if king or rook moves
	this->setCastlingBools(src, piece); 
	
	//turn Pawn into Queen if possible
	bool promoting = this->checkPromotion(dest);
	
	if(promoting) {
		return;
	}

	this->postMove(dest);
}

void LBoard::postMove(SDL_Point dest) {
	// See if the move puts the king in check
	if(mEngine.isKingInCheck(mBoard, !mWhiteTurn)) {
		// Is it checkmate?
		if(mEngine.isCheckMate(mBoard, mWhiteTurn)) {
			mGameOver = true;
			return;
		} else {
			gChunkPlayer->play(CHUNK_CHECK);
		}
	}
	//change turn
	this->changeTurn();
	//en passant
	mEngine.setEnPassant(mSelectedPiece, mSelectedPiecePos, dest);	
}

void LBoard::setCastlingBools(SDL_Point pos, int piece) {
	switch(piece) {
		case WKING:
			mEngine.setKingHasMoved(mWhiteTurn, true);
			break;
		case BKING:
			mEngine.setKingHasMoved(mWhiteTurn, true);
			break;
		case WROOK:
			if(pos.x == 0 && pos.y == 7) {
				mEngine.setRookHasMoved(mWhiteTurn, 1, true);
			}
			else if(pos.x == 7 && pos.y == 7) {
				mEngine.setRookHasMoved(mWhiteTurn, 2, true);
			}
			break;
		case BROOK:
			if(pos.x == 7 && pos.y == 0) {
				mEngine.setRookHasMoved(mWhiteTurn, 2, true);
			}
			else if(pos.x == 0 && pos.y == 0) {
				mEngine.setRookHasMoved(mWhiteTurn, 1, true);
			}
			break;
		default:
			break;
	}
}

bool LBoard::checkPromotion(SDL_Point pos) {
	if((mSelectedPiece == WPAWN && pos.y == 0) || (mSelectedPiece == BPAWN && pos.y == 7)) {
		gStateMachine->push(new LPromotion(mWhiteTurn, pos.x, mSettings.pieceTheme == 0 ? SPRITE_PIECE_SHEET : SPRITE_RETRO_PIECE_SHEET, this));
		mPromotedPiecePos = pos;
		return true;
	}
	return false;
}

void LBoard::playMoveSound(bool captured, bool castled) const {
	if(captured) {
		gChunkPlayer->play(CHUNK_CAPTURE);
	}
	else if(castled) {
		gChunkPlayer->play(CHUNK_CASTLE);
	}
	else {
		gChunkPlayer->play(CHUNK_MOVE);
	}
}

void LBoard::playVictorySound() const {
	
}

void LBoard::renderTimer() {
	// white timer total time left in seconds
	int wtime = mClock->white();
	// if the timer ran out we set it to 0 to avoid displaying negative time
	if(wtime < 0) {
		wtime = 0;
		mWhiteTimerRanOut = true;
	}

	// minutes left for white 
	int wminutes = wtime / 60;
	// seconds left for white 
	int ws = wtime % 60;
	
	//In memory text stream
	std::stringstream whiteTimeText;
	if(ws > 9) {
		whiteTimeText << std::to_string(wminutes) + ":" + std::to_string(ws);
	}
	else whiteTimeText << std::to_string(wminutes) + ":" + "0" + std::to_string(ws);
	
	mWhiteTimerTexture = gMediaFactory->getTxt(whiteTimeText.str().c_str(), gFont64, COLOR_BLACK);
	mWhiteTimerTexture->renderAt(0,SCREEN_HEIGHT - 64);
	
	// black timer total time left in seconds
	int btime = mClock->black();
	// if the timer ran out we set it to 0 to avoid displaying negative time
	if(btime < 0) {
		btime = 0;
		mBlackTimerRanOut = true;
	}

	// minutes left for black
	int bminutes = btime / 60;
	// seconds left for black
	int bs = btime % 60;
	
	//In memory text stream
	std::stringstream blackTimeText;
	if(bs > 9) blackTimeText << std::to_string(bminutes) + ":" + std::to_string(bs);
	else blackTimeText << std::to_string(bminutes) + ":" + "0" + std::to_string(bs);
	mBlackTimerTexture = gMediaFactory->getTxt(blackTimeText.str().c_str(), gFont64, COLOR_BLACK);
	mBlackTimerTexture->renderAt(0,0);
}

void LBoard::renderScore() {
	//sum up each score
	int whiteScore(0);
	int blackScore(0);
	const int MAX = 60;
	for(int y(0); y < SPL; y++) {
		for(int x(0); x < SPL; x++) {
			//black piece
			if((mBoard[y][x] < WBISHOP)) {
				int piece = mBoard[y][x];
				blackScore += mEngine.pieceValue(piece);
			}
			//white piece
			else if((mBoard[y][x] >= WBISHOP) && (mBoard[y][x] < EMPTY)) {
				int piece = mBoard[y][x];
				whiteScore += mEngine.pieceValue(piece);
			}
		}
	}
	
	int a(blackScore);
	blackScore = MAX - whiteScore;
	whiteScore = MAX - a;
	
	std::stringstream whiteScoreStr;
	std::stringstream blackScoreStr;

	whiteScoreStr << std::to_string(whiteScore);
	blackScoreStr << std::to_string(blackScore);
	

	mWhiteScoreTexture = gMediaFactory->getTxt(whiteScoreStr.str().c_str(), gFont64, COLOR_BLACK);
	mBlackScoreTexture = gMediaFactory->getTxt(blackScoreStr.str().c_str(), gFont64, COLOR_BLACK);
	
	if(mSettings.useTimer == 1) {
		mWhiteScoreTexture->renderAt(0, SCREEN_HEIGHT - 64);
		mBlackScoreTexture->renderAt(0, 0);
	}
	else {
		mWhiteScoreTexture->renderAt(OFFSET * 3, SCREEN_HEIGHT - 64);
		mBlackScoreTexture->renderAt(OFFSET * 3, 0);
	}
}

void LBoard::renderDeadPieces() {
	int whiteOffset = 0;
	int blackOffset = 0;
	int whitePosX = OFFSET * 4;
	int whitePosY = (OFFSET * 9);
	int blackPosX = OFFSET * 4;
	int blackPosY = 0;
	for(int i(0); i < EMPTY; i++) {
		//black side
		if(mDeadWhitePiece[i] != 0) {
			for(int y(0); y < mDeadWhitePiece[i]; y++) {
				//make a second line once 8 elements
				if(blackOffset >= 32 * 8) {
					blackOffset = 0;
					blackPosY = 32;
				}
				mMiniPieceTexture->renderAt(blackPosX + blackOffset, blackPosY, &mMiniPieceClip[i]);
				blackOffset += 32;
			}
		}
		//white side
		if(mDeadBlackPiece[i] != 0) {
			for(int y(0); y < mDeadBlackPiece[i]; y++) {
				//make a second line once 8 elements
				if(whiteOffset >= 32 * 8) {
					whiteOffset = 0;
					whitePosY = (OFFSET * 9) + 32;
				}
				mMiniPieceTexture->renderAt(whitePosX + whiteOffset, whitePosY, &mMiniPieceClip[i]);
				whiteOffset += 32;
			}
		}
	}
}

void LBoard::changeTurn() {
	mWhiteTurn = !mWhiteTurn;
	if(mSettings.useTimer == 0) {
		mClock->next();
	}
}

void LBoard::fillDeadPieceTab(const int fallenPiece) {
	if(mWhiteTurn) {
		mDeadBlackPiece[fallenPiece]++;
	}
	else {
		mDeadWhitePiece[fallenPiece]++;
	}
}

void LBoard::computerMove() {
	ChessMove move = mComputer->playTest(mBoard, mWhiteTurn, 1);
	this->doMove(move.dst, move.src, move.piece);
}