/*
LBoardPVP.cpp
Sun Mar 20 2022
by Valentin
-----------
Methods for class LBoardPVP
*/

#include "game/board/LBoardPVP.h"

using std::vector;
extern TTF_Font* gFont64;
extern SDL_Renderer* gRenderer;
extern uint8_t gMusicVolume;
extern LMediaFactory* gMediaFactory;
extern LChunkPlayer* gChunkPlayer;
extern LMusicPlayer* gMusicPlayer;
extern LStateMachine* gStateMachine;
extern LTexture* gBackgroundTexture;

LBoardPVP::LBoardPVP(LObserver* observer) {
	mEngine = new LEngine();
	this->Attach(observer);
	this->initSettings();
	mTimeLimit = mSettings.timeLimit == 0 ? 300 : 600;
	this->initTileTextures();
	this->initPiecesTextures();
	this->initPauseTexture();
	this->initBoard();
	this->setTileRectClip();
	this->setPiecesClip();

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

	//start white and black timer and pause the black as white plays first
	if(mSettings.useTimer == 0) {
		mWhiteTimer = new LTimer();
		mBlackTimer = new LTimer();
		this->startWhiteTimer();
		this->startBlackTimer();
		this->pauseBlackTimer();
	}
}

void LBoardPVP::update() {
	if(Mix_PlayingMusic() == 0) {
		this->playMusic();
	}
	SDL_Event e;
	while(SDL_PollEvent(&e) > 0) {
		if(e.type == SDL_QUIT) {
			this->Notify();
			gStateMachine->pop();
			return;
		}
		else if(e.type == SDL_KEYDOWN) {
			if(e.key.keysym.sym == SDLK_ESCAPE) {
				gStateMachine->pop();
				return;
			}
			else if(e.key.keysym.sym == SDLK_SPACE) {
				this->pause();
			}
		}
		if(!(mIsPaused)) {
			this->handleEvents(&e);
		}
	}
}

void LBoardPVP::render() {
	SDL_RenderClear(gRenderer);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	gBackgroundTexture->render();

	//display tiles
	this->renderTile();
	//display Pieces 
	this->renderPieces();
	//display timer and checks for time remaining
	this->renderTimer();
	//display score
	this->renderScore();
	//display dead pieces
	this->renderDeadPieces();
	
	//set buttons according to piece pos
	this->setButtons();
	
	if(this->isPaused()) {
		this->renderPause();
	}
	
	SDL_RenderPresent(gRenderer);
	// SDL_Delay(16);

	//check for victory
	if(this->isGameOver()) {
		//Pay victory sound and quit
		this->playVictorySound();
		gStateMachine->pop();
		return;
	}
	//check for defeat by time out
	if(this->isOutOfTime()) {
		SDL_Delay(1750);
		gStateMachine->pop();
		return;
	}
}

void LBoardPVP::playMusic() {
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

bool LBoardPVP::isPaused() const {
	return mIsPaused;
}

void LBoardPVP::pause() {
	if(mIsPaused) {
		gMusicPlayer->pause();
		mIsPaused = false;
		if(mSettings.useTimer == 0) {
			if(mWhiteTurn) unpauseWhiteTimer();
			else unpauseBlackTimer();
		}
	}
	else {
		mIsPaused = true;
		gMusicPlayer->pause();
		if(mSettings.useTimer == 0) {
			if(mWhiteTurn) pauseWhiteTimer();
			else pauseBlackTimer();
		}
	}
}
		
LBoardPVP::~LBoardPVP() {
	free();
}

void LBoardPVP::free() {
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
	if(mWhiteTimer != NULL) {
		delete mWhiteTimer;
		mWhiteTimer = NULL;
	}
	if(mBlackTimer != NULL) {
		delete mBlackTimer;
		mBlackTimer = NULL;
	}
	
	mWhiteScoreTexture->free();
	mWhiteScoreTexture = NULL;
	mBlackScoreTexture->free();
	mBlackScoreTexture = NULL;
	
	mPieceButtons.clear();
	mLegalMoveTile.clear();
}

void LBoardPVP::startWhiteTimer() {
	mWhiteTimer->start();
}

void LBoardPVP::initSettings() {
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
}

bool LBoardPVP::initBoard() {
	std::ifstream map( FILE_MAP );
	bool success = true;
	if(map.fail()) {
		spdlog::error( "Unable to load map file!\n" );
		success = false;
	}
	else {
		for(int y(0); y < SPL; y++) {
			for(int x(0); x < SPL; x++) {
				//Determines what kind of tile will be made
				int pieceType = -1;
				
				//Read piece from map file
				map >> pieceType;
				
				//If the was a problem in reading the map
				if(map.fail()) {
					//Stop loading map
					spdlog::error("Error loading map: Unexpected end of file!\n");
					success = false;
					break;
				}
				//If the number is a valid piece number
				if((pieceType >= 0) && (pieceType < TOTAL_PIECES)) {
					mBoard[y][x] = pieceType;
				}
				//If we don't recognize the tile type
				else {
					//Stop loading map
					spdlog::error( "Error loading map: Invalid piece type at line %d element %d!\n", y, x);
					success = false;
					break;
				}
			}
		}
	}
	return success;
}

bool LBoardPVP::initPiecesTextures() {
	const char* spriteSheet = mSettings.pieceTheme == 0 ? SPRITE_PIECE_SHEET : SPRITE_RETRO_PIECE_SHEET;
	mPieceTexture = gMediaFactory->getImg(spriteSheet);
	mHighlightedPieceTexture = gMediaFactory->getImgUnique(spriteSheet);
	mHighlightedPieceTexture->setColor(255,0,0);
	mMiniPieceTexture = gMediaFactory->getImg(SPRITE_PIECE_SHEET_32);
	return mPieceTexture != NULL && mHighlightedPieceTexture != NULL && mMiniPieceTexture != NULL; 
}

bool LBoardPVP::initTileTextures() {
	mTileTexture = gMediaFactory->getImg(SPRITE_BOARD);
	return mTileTexture != NULL;
}

bool LBoardPVP::initPauseTexture() {
	mPauseBackgroundTexture = gMediaFactory->getImg(SPRITE_BACKGROUND_FULLBLACK);
	mPauseBackgroundTexture->setAlpha(127);
	mPauseBackgroundTexture->setBlendMode(SDL_BLENDMODE_BLEND);
	mPauseTextTexture = gMediaFactory->getTxt("Pause", gFont64, COLOR_WHITE);
	return mPauseBackgroundTexture != NULL && mPauseTextTexture != NULL;
}

bool LBoardPVP::initOutOfTimeTexture() {
	mOutOfTimeTexture = gMediaFactory->getTxt("Out of time!", gFont64, COLOR_WHITE);
	return mOutOfTimeTexture != NULL;
}

void LBoardPVP::setPiecesClip() {
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

void LBoardPVP::setTileRectClip() {
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

void LBoardPVP::renderTile() {
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
		}
	}
	//only highlight tile if a piece is selected
	if(mAPieceIsSelected && mSettings.showLegalMoves == 0) {
		std::vector<SDL_Point> legalMoves = mEngine->getLegalMoves(mBoard, mSelectedPiecePos);
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
	
void LBoardPVP::renderPieces() {
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
void LBoardPVP::drawButtons() {
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

void LBoardPVP::setButtons() {
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

void LBoardPVP::renderPause() {
	mPauseBackgroundTexture->render();
	mPauseTextTexture->renderAt((SCREEN_WIDTH - mPauseTextTexture->w()) / 2, (SCREEN_HEIGHT - mPauseTextTexture->h()) / 2); 
}

void LBoardPVP::renderOutOfTimeScreen() {
	mPauseBackgroundTexture->render();
	mOutOfTimeTexture->renderAt((SCREEN_WIDTH - mOutOfTimeTexture->w()) / 2, (SCREEN_HEIGHT - mOutOfTimeTexture->h()) / 2);
	SDL_RenderPresent(gRenderer);
}

void LBoardPVP::handleEvents(SDL_Event* e) {
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

void LBoardPVP::move(SDL_Event* e) {
	int destinationPosX, destinationPosY;
	SDL_GetMouseState( &destinationPosX, &destinationPosY );
	SDL_Point destPos = {(destinationPosX / TOTAL_SQUARES) - 1, (destinationPosY / TOTAL_SQUARES) - 1};
	if((e->type == SDL_MOUSEBUTTONUP) && (e->button.button == SDL_BUTTON_LEFT)) {
		std::vector<SDL_Point> legalPos = mEngine->getLegalMoves(mBoard, mSelectedPiecePos);
		int size = legalPos.size();
		for(int i(0); i < size; i++) {
			if((destPos.x == legalPos[i].x) && (destPos.y == legalPos[i].y)) {
				SDL_Point srcPos = mSelectedPiecePos;
				//only move if it does not check your own king 
				if(!mEngine->isMoveSelfCheck(mBoard, mSelectedPiecePos, destPos, mWhiteTurn)) { 
					doMove(destPos, mSelectedPiecePos, mSelectedPiece);
					break;
				} else gChunkPlayer->play(CHUNK_ERROR);
			}
		}
	}
}

void LBoardPVP::doMove(SDL_Point dest, SDL_Point src, int piece) {
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
		fillDeadPieceTab(BPAWN);
		captured = true;
	}
	else if((piece == BPAWN) && (src.x != dest.x) && (mBoard[dest.y][dest.x] == EMPTY)) {
		mBoard[dest.y - 1][dest.x] = EMPTY;
		mPieceButtons.resize(mPieceButtons.size() - 1);
		fillDeadPieceTab(WPAWN);
		captured = true;	
	}

	this->playMoveSound(captured, castled);
	
	//move selected piece to destination
	mBoard[dest.y][dest.x] = piece;
	//empty source cell 
	mBoard[src.y][src.x] = EMPTY;
	
	//turn castling bools to true if king or rook moves
	setCastlingBools(src, piece); 
	
	//turn Pawn into Queen if possible
	checkPromotion(dest);
	
	// See if the move puts the king in check
	if(mEngine->isKingInCheck(mBoard, !mWhiteTurn)) {
		// Is it checkmate?
		if(mEngine->isCheckMate(mBoard, mWhiteTurn)) {
			mGameOver = true;
		} else {
			gChunkPlayer->play(CHUNK_CHECK);
		}
	}
	
	//change turn
	changeTurn();
	//en passant
	mEngine->setEnPassant(piece, src, dest);
}

void LBoardPVP::setCastlingBools(SDL_Point pos, int piece) {
	switch(piece) {
		case WKING:
			mEngine->setKingHasMoved(mWhiteTurn, true);
			break;
		case BKING:
			mEngine->setKingHasMoved(mWhiteTurn, true);
			break;
		case WROOK:
			if(pos.x == 0 && pos.y == 7) {
				mEngine->setRookHasMoved(mWhiteTurn, 1, true);
			}
			else if(pos.x == 7 && pos.y == 7) {
				mEngine->setRookHasMoved(mWhiteTurn, 2, true);
			}
			break;
		case BROOK:
			if(pos.x == 7 && pos.y == 0) {
				mEngine->setRookHasMoved(mWhiteTurn, 2, true);
			}
			else if(pos.x == 0 && pos.y == 0) {
				mEngine->setRookHasMoved(mWhiteTurn, 1, true);
			}
			break;
		default:
			break;
	}
}

bool LBoardPVP::checkPromotion(SDL_Point pos) {
	bool promotion = false;
	if((mSelectedPiece == WPAWN) && (pos.y == 0)) {
		mBoard[pos.y][pos.x] = WQUEEN;
		promotion = true;
	}
	if((mSelectedPiece == BPAWN) && (pos.y == 7)) {
		mBoard[pos.y][pos.x] = BQUEEN;
		promotion = true;
	}
	return promotion;
}

void LBoardPVP::playMoveSound(bool captured, bool castled) const {
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

void LBoardPVP::playVictorySound() const {
	if(Mix_PlayingMusic()) {
		Mix_FadeOutMusic(3000);
	}
	gChunkPlayer->play(CHUNK_CHECKMATE);
	while(Mix_Playing(-1) > 0) {
		SDL_Delay(16);
	}
	gChunkPlayer->play(CHUNK_VICTORY);
	while(Mix_Playing(-1) > 0) {
		SDL_Delay(16);
	}
}

void LBoardPVP::renderTimer() {
	if(mSettings.useTimer == 0) {
		// white timer total time left in seconds
		int wtime = mTimeLimit - (mWhiteTimer->getTicks() / 1000);

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
		int btime = mTimeLimit - (mBlackTimer->getTicks() / 1000);

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
}

void LBoardPVP::renderScore() {
	//sum up each score
	int whiteScore(0);
	int blackScore(0);
	const int MAX = 60;
	for(int y(0); y < SPL; y++) {
		for(int x(0); x < SPL; x++) {
			//black piece
			if((mBoard[y][x] < WBISHOP)) {
				int piece = mBoard[y][x];
				blackScore += pieceValue(piece);
			}
			//white piece
			else if((mBoard[y][x] >= WBISHOP) && (mBoard[y][x] < EMPTY)) {
				int piece = mBoard[y][x];
				whiteScore += pieceValue(piece);
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

void LBoardPVP::renderDeadPieces() {
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

void LBoardPVP::changeTurn() {
	if(mWhiteTurn) {
		mWhiteTurn = false;
		pauseWhiteTimer();
		unpauseBlackTimer();
	}
	else {
		mWhiteTurn = true;
		unpauseWhiteTimer();
		pauseBlackTimer();
	}
}

bool LBoardPVP::isGameOver() const {
	return mGameOver;
}

bool LBoardPVP::isOutOfTime() {
	if(mWhiteTimerRanOut || mBlackTimerRanOut) {
		if(Mix_PlayingMusic()) {
			Mix_FadeOutMusic(500);
		}
		this->initOutOfTimeTexture();
		this->renderOutOfTimeScreen();
		gChunkPlayer->play(CHUNK_DEFEAT);
		while(Mix_Playing(-1) > 0) {
			SDL_Delay(16);
		}
		return 1;
	}
	return 0;
}

int LBoardPVP::pieceValue(int const pieceType) const {
	switch(pieceType) {
		case BBISHOP: 
			return BISHOP_VALUE;
		case BKING:
			return KING_VALUE;
		case BKNIGHT:
			return KNIGHT_VALUE;
		case BPAWN:
			return PAWN_VALUE;
		case BQUEEN:
			return QUEEN_VALUE;
		case BROOK:
			return ROOK_VALUE;
		case WBISHOP:
			return BISHOP_VALUE;
		case WKING:
			return KING_VALUE;
		case WKNIGHT:
			return KNIGHT_VALUE;
		case WPAWN:
			return PAWN_VALUE;
		case WQUEEN:
			return QUEEN_VALUE;
		case WROOK:
			return ROOK_VALUE;
		default:
			return 0;
	}
}

void LBoardPVP::fillDeadPieceTab(const int fallenPiece) {
	if(mWhiteTurn) {
		mDeadBlackPiece[fallenPiece]++;
	}
	else {
		mDeadWhitePiece[fallenPiece]++;
	}
}

void LBoardPVP::startBlackTimer() {
	mBlackTimer->start();
}

void LBoardPVP::pauseWhiteTimer() {
	mWhiteTimer->pause();
}

void LBoardPVP::pauseBlackTimer() {
	mBlackTimer->pause();
}

void LBoardPVP::unpauseWhiteTimer() {
	mWhiteTimer->unpause();
}

void LBoardPVP::unpauseBlackTimer() {
	mBlackTimer->unpause();
}

void LBoardPVP::stopWhiteTimer() {
	mWhiteTimer->stop();
}

void LBoardPVP::stopBlackTimer() {
	mBlackTimer->stop();
}
