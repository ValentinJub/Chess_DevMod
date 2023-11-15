/*
LBoardPVP.cpp
Sun Mar 20 2022
by Valentin
-----------
Methods for class LBoardPVP
*/

#include "LBoardPVP.h"

using std::vector;
extern TTF_Font* gFont64;
extern SDL_Renderer* gRenderer;

LBoardPVP::LBoardPVP() {
	//grab settings from file
	readSettingsFromFile();
	//init some game variable according to settings (mPreview, mTileColor)
	initGameSettings();
	//Init textures
	mPieceTexture = new LTexture;
	mMiniPieceTexture = new LTexture;
	mHighlightedPieceTexture = new LTexture;
	mTileTexture = new LTexture;
	mHighlightedTileTexture = new LTexture;
	mWhiteTimerTexture = new LTexture;
	mBlackTimerTexture = new LTexture;
	mWhiteScoreTexture = new LTexture;
	mBlackScoreTexture = new LTexture;
	mPauseBackgroundTexture = new LTexture;
	mPauseTextTexture = new LTexture;
	mOutOfTimeTexture = new LTexture;
	
	for(int i(0); i < INITIAL_PIECES_TOTAL; i++) {
		mPieceButtons.push_back(new LButton);
	}
	
	for(int i(0); i < EMPTY; i++) {
		mDeadWhitePiece[i] = 0;
		mDeadBlackPiece[i] = 0;
	}
	
	//Set to 9 so no pieces 
	//Are highlighted at start
	mSelectedPieceXPos = 9;
	mSelectedPieceYPos = 9;
	//No piece is selected at start
	mAPieceIsSelected = false;
	
	//White start
	mWhiteTurn = true;
	
	mIsPaused = false;
	
	//Check status
	mCheckStatus = NO_CHECK;
	
	//bools used to play sound depending on the context
	mTookAPiece = false;
	mIsCastling = false;
	mEnPassantTurn = false;
	mGameOver = false;
	
	//Bools below used to determine if we can use Castling();  
	mWKingHasMoved = false;
	mBKingHasMoved = false;
	mWRook1HasMoved = false;
	mWRook2HasMoved = false;
	mBRook1HasMoved = false;
	mBRook2HasMoved = false;
	
	//load sound effect
	mBingo = loadChunk("SoundEffects/chime.wav");
	mCheck = loadChunk("SoundEffects/checkUS.wav");
	mCheckMate = loadChunk("SoundEffects/checkMateUS.wav");
	mError = loadChunk("SoundEffects/error.wav");
	mPawnForward = loadChunk("SoundEffects/pawnForward.wav");
	mCastling = loadChunk("SoundEffects/0-0-0.wav");
	mPieceFall = loadChunk("SoundEffects/drop3.wav");
	mVictory = loadChunk("SoundEffects/victoryOrchestra.wav");
	mDefeat = loadChunk("SoundEffects/gameOver.wav");
	
	//start white and black timer and pause the black as white plays first
	if(mSettingsTable[TL_YES]) {
		startWhiteTimer();
		startBlackTimer();
		pauseBlackTimer();
	}
	
	mWhiteTimerRanOut = false;
	mBlackTimerRanOut = false;
	
}

void LBoardPVP::initGameSettings() {
	//Set show legal move (highlight tile with selected piece's possible moves)
	if(mSettingsTable[SLM_YES]) mPreviewMove = true;
	else mPreviewMove = false;
	//Set tile color
	if(mSettingsTable[TC_GREY]) mTileColor = 1;
	else mTileColor = 0;
	//Set timer duration
	if(mSettingsTable[TL_YES]) {
		if(mSettingsTable[TL_5]) {
			mTimeLimit = 3; 
		}
		else {
			mTimeLimit = 600; 
		}
	}
	//Set music
	if(mSettingsTable[MT_CLASSIC]) {
		for(int i(0); i < NBR_OF_MUSIC; i++) {
			std::string str = "Music/Classic/class" + std::to_string(i) + ".mp3";
			mMusic[i] = loadMusic(str.c_str());
		}
	}
	//Jazzy
	else {
		for(int i(0); i < NBR_OF_MUSIC; i++) {
			std::string str = "Music/Jazz/jazz" + std::to_string(i) + ".mp3";
			mMusic[i] = loadMusic(str.c_str());
		}
	}
	//Piece Theme 
	if(mSettingsTable[PT_1]) {
		mPieceTheme = 0;
	}
	else mPieceTheme = 1;
}

void LBoardPVP::playMusic() {
	//if no music is playing
	if(Mix_PlayingMusic() == 0) {
		int i = rand() % 7;
		//Play the music
		Mix_FadeInMusic(mMusic[i], 0, 10000);
		Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	}
}

bool LBoardPVP::isPaused() const {
	return mIsPaused;
}

void LBoardPVP::pause() {
	if(mIsPaused) {
		Mix_ResumeMusic();
		mIsPaused = false;
		if(mSettingsTable[TL_YES]) {
			if(mWhiteTurn) unpauseWhiteTimer();
			else unpauseBlackTimer();
		}
	}
	else {
		mIsPaused = true;
		Mix_PauseMusic();
		if(mSettingsTable[TL_YES]) {
			if(mWhiteTurn) pauseWhiteTimer();
			else pauseBlackTimer();
		}
	}
}
		
LBoardPVP::~LBoardPVP() {
	free();
}

void LBoardPVP::free() {
	mPieceTexture->free();
	mPieceTexture = NULL;
	
	mHighlightedPieceTexture->free();
	mHighlightedPieceTexture = NULL;
	
	mTileTexture->free();
	mTileTexture = NULL;
	
	mWhiteTimerTexture->free();
	mWhiteTimerTexture = NULL;
	mBlackTimerTexture->free();
	mBlackTimerTexture = NULL;
	
	mWhiteScoreTexture->free();
	mWhiteScoreTexture = NULL;
	mBlackScoreTexture->free();
	mBlackScoreTexture = NULL;
	
	mHighlightedTileTexture->free();
	mHighlightedTileTexture = NULL;
	
	mPieceButtons.clear();
	mHighlightedTileXPos.clear();
	mHighlightedTileYPos.clear();
	
	Mix_FreeChunk(mError);
	mError = NULL;
	Mix_FreeChunk(mBingo);
	mBingo = NULL;
	Mix_FreeChunk(mCheck);
	mCheck = NULL;
	Mix_FreeChunk(mCheckMate);
	mCheckMate = NULL;
	Mix_FreeChunk(mCastling);
	mCastling = NULL;
	Mix_FreeChunk(mPawnForward);
	mPawnForward = NULL;
	Mix_FreeChunk(mPieceFall);
	mPieceFall = NULL;
	Mix_FreeChunk(mVictory);
	mVictory = NULL;
	Mix_FreeChunk(mDefeat);
	mDefeat = NULL;
	
	for(int i(0); i < NBR_OF_MUSIC; i++) {
		Mix_FreeMusic(mMusic[i]);
		mMusic[i] = NULL;
	}
}

void LBoardPVP::startWhiteTimer() {
	mWhiteTimer.start();
}

void LBoardPVP::startBlackTimer() {
	mBlackTimer.start();
}

void LBoardPVP::pauseWhiteTimer() {
	mWhiteTimer.pause();
}

void LBoardPVP::pauseBlackTimer() {
	mBlackTimer.pause();
}

void LBoardPVP::unpauseWhiteTimer() {
	mWhiteTimer.unpause();
}

void LBoardPVP::unpauseBlackTimer() {
	mBlackTimer.unpause();
}

void LBoardPVP::stopWhiteTimer() {
	mWhiteTimer.stop();
}

void LBoardPVP::stopBlackTimer() {
	mBlackTimer.stop();
}

void LBoardPVP::readSettingsFromFile() {
	std::ifstream settings;
    settings.open("resources/settings.config", std::ios::in);
    if(settings.is_open()) {
		// minus 1 because we don't want to read the back button
        for(int i(0); i < TOTAL_CLICKABLE_ITEMS - 1; i++) {
            std::string line;
            std::getline(settings, line);
            std::stringstream ss(line);
            int a;
            ss >> a;
            mSettingsTable[i] = a;
        }
        settings.close();
    }
    else {
        std::cerr << "Unable to load settings file!\n";
    }
}

bool LBoardPVP::loadPiecesTextures() {
	bool success = true;
	if(mPieceTheme == 0) {
		//regular piece
		if(!(mPieceTexture->loadFromFile("Sprites/64/pieceSpritesheet.png"))) success = false;
		else if(!(mHighlightedPieceTexture->loadFromFile("Sprites/64/pieceSpritesheet.png"))) success = false;
		else mHighlightedPieceTexture->setColor(255,0,0);
		//score piece
		if(!(mMiniPieceTexture->loadFromFile("Sprites/32/spritesheet32.png"))) success = false;
	} else if(mPieceTheme == 1) {
		if(!(mPieceTexture->loadFromFile("Sprites/64/retroPieceSpriteSheet.png"))) success = false;
		else if(!(mHighlightedPieceTexture->loadFromFile("Sprites/64/retroPieceSpritesheet.png"))) success = false;
		else mHighlightedPieceTexture->setColor(255,0,0);
		if(!(mMiniPieceTexture->loadFromFile("Sprites/32/spritesheet32.png"))) success = false;
	}
	return success; 
}

bool LBoardPVP::loadTileTextures() {
	bool success = true;
	if(!(mTileTexture->loadFromFile("Sprites/64/board.png"))) success = false;
	else if(!(mHighlightedTileTexture->loadFromFile("Sprites/64/board.png"))) success = false;
	else mHighlightedTileTexture->setColor(255,0,0);
	return success;
}

bool LBoardPVP::loadPauseTexture() {
	SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
	bool success = true;
	if(!(mPauseBackgroundTexture->loadFromFile("Sprites/blackScreen.png"))) success = false;
	else {
		mPauseBackgroundTexture->setAlpha(127);
		mPauseBackgroundTexture->setBlendMode(SDL_BLENDMODE_BLEND);
	}
	if(!(mPauseTextTexture->loadFromRenderedText(gFont64, "Pause", white))) success = false;
	return success;
}

// for now this function is called when either white or black run out of time
// because it relies on the mWhiteTimerRanOut and mBlackTimerRanOut bools
// alternatively we could load two textures but considering we are not going 
// to use one it would be a waste of memory
bool LBoardPVP::loadOutOfTimeTexture() {
	SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
	if(mWhiteTimerRanOut) {
		if(!(mOutOfTimeTexture->loadFromRenderedText(gFont64, "White ran out of time!", white))) return false;
	}
	else if(mBlackTimerRanOut) {
		if(!(mOutOfTimeTexture->loadFromRenderedText(gFont64, "Black ran out of time!", white))) return false;
	}
	return true;
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
				if(mTileColor == 0) {
					if(!light) mTileTexture->render(xPos, yPos, &mTileRectClip[DARK1]);
					else mTileTexture->render(xPos, yPos, &mTileRectClip[LIGHT1]);
				}
				else if(mTileColor == 1) {
					if(!light) mTileTexture->render(xPos, yPos, &mTileRectClip[DARK2]);
					else mTileTexture->render(xPos, yPos, &mTileRectClip[LIGHT2]);
				}
				if(light) light = false;
				else light = true;
			}
			else {
				if(mTileColor == 0) {
					if(!dark) mTileTexture->render(xPos, yPos, &mTileRectClip[LIGHT1]);
					else mTileTexture->render(xPos, yPos, &mTileRectClip[DARK1]);
				}
				else if(mTileColor == 1) {
					if(!dark) mTileTexture->render(xPos, yPos, &mTileRectClip[LIGHT2]);
					else mTileTexture->render(xPos, yPos, &mTileRectClip[DARK2]);
				}
				if(dark) dark = false;
				else dark = true;
			}
		}
	}
	//only highlight tile if a piece is selected
	if(mAPieceIsSelected) {
		//show legal move bool settings
		if(mPreviewMove) {
			int size = mHighlightedTileYPos.size();
			for(int z(0); z < size; z++) {
				yPos = OFFSET + (mHighlightedTileYPos[z] * TOTAL_SQUARES);
				xPos = OFFSET + (mHighlightedTileXPos[z] * TOTAL_SQUARES);
				mHighlightedTileTexture->render(xPos, yPos, &mTileRectClip[LIGHT1]);
			}
		}
	}
	if(mRightClickedTileXPos.size() > 0) {
		int size = mRightClickedTileXPos.size();
		for(int z(0); z < size; z++) {
			yPos = OFFSET + (mRightClickedTileYPos[z] * TOTAL_SQUARES);
			xPos = OFFSET + (mRightClickedTileXPos[z] * TOTAL_SQUARES);
			mHighlightedTileTexture->render(xPos, yPos, &mTileRectClip[LIGHT2]);
		}
	}
}
	
void LBoardPVP::renderPieces() {
	for(int y(0); y < SPL; y++) {
		for(int x(0); x < SPL; x++) {
			if((mMap[y][x] >= 0) && (mMap[y][x] < TOTAL_PIECES - 1)) {
				if( (mAPieceIsSelected) && (mSelectedPieceXPos == x) && (mSelectedPieceYPos == y)) {
					mHighlightedPieceTexture->render(OFFSET + (TOTAL_SQUARES * x), OFFSET + (TOTAL_SQUARES * y), &mPieceClip[mMap[y][x]]);
				}
				else mPieceTexture->render(OFFSET + (TOTAL_SQUARES * x), OFFSET + (TOTAL_SQUARES * y), &mPieceClip[mMap[y][x]]);
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
			if((mMap[y][x] >= 0) && (mMap[y][x] < TOTAL_PIECES - 1)) {
				mPieceButtons[i]->setPosition((x * TOTAL_SQUARES) + OFFSET, (y * TOTAL_SQUARES) + OFFSET);
				mPieceButtons[i]->setWidthAndHeight(TOTAL_SQUARES,TOTAL_SQUARES);
				i++;
			}
		}
	}
}

void LBoardPVP::renderPause() {
	mPauseBackgroundTexture->render();
	mPauseTextTexture->render((SCREEN_WIDTH - mPauseTextTexture->getWidth()) / 2, (SCREEN_HEIGHT - mPauseTextTexture->getHeight()) / 2); 
}

void LBoardPVP::renderOutOfTimeScreen() {
	mPauseBackgroundTexture->render();
	mOutOfTimeTexture->render((SCREEN_WIDTH - mOutOfTimeTexture->getWidth()) / 2, (SCREEN_HEIGHT - mOutOfTimeTexture->getHeight()) / 2);
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
		if(mPieceButtons[i]->handleInside(e)) {
			//if THE LEFT mouse button clicked 
			if(mPieceButtons[i]->handleClick(e)) {
				//if a piece is not selected 
				//if a piece is already selected, count as clicking outside 
				if(!(mAPieceIsSelected)) {
					mSelectedPieceType = mMap[y][x];
					//if white's turn - select white
					if((mWhiteTurn) && (mSelectedPieceType >= WBISHOP) && (mSelectedPieceType < EMPTY)) {
						outside = false;
						mSelectedPieceXPos = x;
						mSelectedPieceYPos = y;
						mAPieceIsSelected = true;
						mRightClickedTileXPos.clear();
						mRightClickedTileYPos.clear();
					}
					//if black's turn - select black
					else if(!(mWhiteTurn) && (mSelectedPieceType >= BBISHOP) && (mSelectedPieceType < WBISHOP)) {
						outside = false;
						mSelectedPieceXPos = x;
						mSelectedPieceYPos = y;
						mAPieceIsSelected = true;
						mRightClickedTileXPos.clear();
						mRightClickedTileYPos.clear();
					}
				}
			}
		}
	}
	//if a piece is selected - can move
	//to pos set in shwlegalMove()
	if(mAPieceIsSelected) movePiece(e);
	if(outside) {
		if((e->type == SDL_MOUSEBUTTONUP) && (e->button.button == SDL_BUTTON_LEFT)) {
			mAPieceIsSelected = false;
			mSelectedPieceXPos = 9;
			mSelectedPieceYPos = 9;
			mHighlightedTileXPos.clear();
			mHighlightedTileYPos.clear();
			mRightClickedTileXPos.clear();
			mRightClickedTileYPos.clear();
		}
	}
	if((e->type == SDL_MOUSEBUTTONUP) && (e->button.button == SDL_BUTTON_RIGHT)) {
		bool dont = false;
		int size = mRightClickedTileXPos.size();
		for(int z(0); z < size; z++) {
			if((mRightClickedTileXPos[z] == x) && (mRightClickedTileYPos[z] == y)) {
				// mRightClickedTileXPos[z] = 9;
				// mRightClickedTileYPos[z] = 9;
				mRightClickedTileXPos.erase(mRightClickedTileXPos.begin() + z);
				mRightClickedTileYPos.erase(mRightClickedTileYPos.begin() + z);
				dont = true;
			}
		}
		if(!(dont)) {
			mRightClickedTileXPos.push_back(x);
			mRightClickedTileYPos.push_back(y);
		}
	} 
}

//fill mHighlightedTileXPos/YPos with
//possible moves according to selected piece
void LBoardPVP::showLegalMove() {
	//clear to make sure we don't add to 
	//existing vector content
	bool f = false;
	mHighlightedTileXPos.clear();
	mHighlightedTileYPos.clear();
	if(mAPieceIsSelected) {
		switch(mSelectedPieceType) {
			case BPAWN:
				mPawn.blackMoveOld(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mLastMovedPiece, mLastMovedPieceXSrc, mLastMovedPieceYSrc, mLastMovedPieceXDes, mLastMovedPieceYDes, &mEnPassantTurn);
				break;
			case WPAWN:
				mPawn.whiteMoveOld(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mLastMovedPiece, mLastMovedPieceXSrc, mLastMovedPieceYSrc, mLastMovedPieceXDes, mLastMovedPieceYDes, &mEnPassantTurn);
				break;
			case BKNIGHT:
				mKnight.blackMove(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
				break;
			case WKNIGHT:
				mKnight.whiteMove(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
				break;
			case BBISHOP:
				mBishop.blackMove(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
				break;
			case WBISHOP:
				mBishop.whiteMove(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
				break;
			case BROOK:
				mRook.blackMove(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
				break;
			case WROOK:
				mRook.whiteMove(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
				break;
			case BQUEEN:
				mQueen.blackMove(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
				break;
			case WQUEEN:
				mQueen.whiteMove(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
				break;
			case WKING:
				mKing.whiteMove(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mWKingHasMoved, mWRook1HasMoved, mWRook2HasMoved, mCheckStatus, &f);
				break;
			case BKING:
				mKing.blackMove(mMap, mSelectedPieceXPos, mSelectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mBKingHasMoved, mBRook1HasMoved, mBRook2HasMoved, mCheckStatus, &f);
				break;
			default:
				break;
		}
	}
}

void LBoardPVP::movePiece(SDL_Event* e) {
	int destinationPosX, destinationPosY;
	SDL_GetMouseState( &destinationPosX, &destinationPosY );
	destinationPosX = (destinationPosX / TOTAL_SQUARES) - 1;
	destinationPosY = (destinationPosY / TOTAL_SQUARES) - 1;
	if((e->type == SDL_MOUSEBUTTONUP) && (e->button.button == SDL_BUTTON_LEFT)) {
		//if click is in possible square2 posX&Y[i]
		int size = mHighlightedTileYPos.size();
		for(int i(0); i < size; i++) {
			if((destinationPosX == mHighlightedTileXPos[i]) && (destinationPosY == mHighlightedTileYPos[i])) {
				//make a copy of the pos and piece as it 
				//will be messed with in the PrePollCheck()
				int piece = mSelectedPieceType;
				int sourcePosX = mSelectedPieceXPos;
				int sourcePosY = mSelectedPieceYPos;
				//only move if it does not check your own king 
				if(!(pollDiscoverAttack(piece, destinationPosX, destinationPosY))) { 
					//if castling returns false cant move
					if(castling(piece, destinationPosX, destinationPosY)) {
							move(destinationPosX, destinationPosY, sourcePosX, sourcePosY, piece);
							i = 99;
					}
					else {
						//Play unable to move sound 
						Mix_PlayChannel(-1, mError, 0);
					}
				}
				else {
					//Play unable to move sound 
					Mix_PlayChannel(-1, mError, 0);
				}
			}
		}
	}
}

void LBoardPVP::enPassant(int destinationPosX) { 
	if(destinationPosX == mLastMovedPieceXDes) {
		mMap[mLastMovedPieceYDes][mLastMovedPieceXDes] = EMPTY;
		mPieceButtons.resize(mPieceButtons.size() - 1);
		if(mWhiteTurn) {
			int fallenPiece = BPAWN;
			fillDeadPieceTab(fallenPiece);
		}
		else {
			int fallenPiece = WPAWN;
			fillDeadPieceTab(fallenPiece);
		}
		mTookAPiece = true;
	}
}

void LBoardPVP::move(int destinationPosX, int destinationPosY, int sourcePosX, int sourcePosY, int piece) {
	if(mEnPassantTurn) {
		enPassant(destinationPosX);
	}
	
	//if the square destination is not empty, substract a button
	if(mMap[destinationPosY][destinationPosX] != EMPTY) {
		mPieceButtons.resize(mPieceButtons.size() - 1);
		mTookAPiece = true;
		int fallenPiece = mMap[destinationPosY][destinationPosX];
		fillDeadPieceTab(fallenPiece);
	}
	
	
	
	//move selected piece to destination
	mMap[destinationPosY][destinationPosX] = piece;
	//empty source cell 
	mMap[sourcePosY][sourcePosX] = EMPTY;
	
	//manage playing sound according to played move
	playMoveSound();
	
	//turn castling bools to true if king or rook moves
	setCastlingBools(sourcePosX, sourcePosY, piece); 
	
	//turn Pawn into Queen if possible
	checkPromotion(destinationPosX, destinationPosY);
	
	//Poll for check 
	//if there's check
	if(pollCheck()) {
		//poll for checkmate
		//if theres checkmate
		if(checkMate()) {
			mGameOver = true;
		}
		//check
		else {
			Mix_PlayChannel(-1, mCheck, 0 );
		}
	}
	
	//change turn
	changeTurn();
	
	if(mEnPassantTurn) mEnPassantTurn = false;
	
	//en passant
	mLastMovedPiece = piece;
	mLastMovedPieceXSrc = sourcePosX;
	mLastMovedPieceYSrc = sourcePosY;
	mLastMovedPieceXDes = destinationPosX;
	mLastMovedPieceYDes = destinationPosY; 
	
}

bool LBoardPVP::castling(int piece, int destinationPosX, int destinationPosY) {
	int castling = NO_CASTLING;
	if(piece == WKING) {
		if(!(mWKingHasMoved)) {
			//WR1
			if((destinationPosY == 7) && (destinationPosX == 2)) {
				if(!(pollDiscoverAttack(piece, destinationPosX + 1, destinationPosY))) {
					castling = CASTLE_WR1;
				} else castling = NO_MOVE;
			}
			//WR2
			else if((destinationPosY == 7) && (destinationPosX == 6)) {
				if(!(pollDiscoverAttack(piece, destinationPosX - 1, destinationPosY))) {
					castling = CASTLE_WR2;
				} else castling = NO_MOVE;
			}
		}
	}
	
	else if(piece == BKING) {
		if(!(mBKingHasMoved)) {
			//BR1
			if((destinationPosY == 0) && (destinationPosX == 2)) {
				if(!(pollDiscoverAttack(piece, destinationPosX + 1, destinationPosY))) {
					castling = CASTLE_BR1;
				} else castling = NO_MOVE;
			}
			//BR2
			else if((destinationPosY == 0) && (destinationPosX == 6)) {
				if(!(pollDiscoverAttack(piece, destinationPosX - 1, destinationPosY))) {
					castling = CASTLE_BR2;
				} else castling = NO_MOVE;
			}
		}
	}
	switch(castling) {
		case CASTLE_WR1:
			mMap[7][3] = WROOK;
			mMap[7][0] = EMPTY;
			mIsCastling = true;
			return true;
		case CASTLE_WR2:
			mMap[7][5] = WROOK;
			mMap[7][7] = EMPTY;
			mIsCastling = true;
			return true;
		case CASTLE_BR1:
			mMap[0][3] = BROOK;
			mMap[0][0] = EMPTY;
			mIsCastling = true;
			return true;
		case CASTLE_BR2:
			mMap[0][5] = BROOK;
			mMap[0][7] = EMPTY;
			mIsCastling = true;
			return true;
		case NO_CASTLING:
			return true;
		case NO_MOVE:
			return false;
	}
}

void LBoardPVP::setCastlingBools(int piecePosX, int piecePosY, int piece) {
	if(piece == WKING) mWKingHasMoved = true;
	else if(piece == BKING) mBKingHasMoved = true;
	else if((piece == WROOK) && (piecePosX == 0) && (piecePosY == 7)) {
		mWRook1HasMoved = true;
	} 
	else if((piece == WROOK) && (piecePosX == 7) && (piecePosY == 7)) {
		mWRook2HasMoved = true;
	} 
	else if((piece == BROOK) && (piecePosX == 7) && (piecePosY == 0)) {
		mBRook2HasMoved = true;
	}
	else if((piece == BROOK) && (piecePosX == 0) && (piecePosY == 0)) {
		mBRook1HasMoved = true;
	}
}

bool LBoardPVP::checkPromotion(int x, int y) {
	bool promotion = false;
	if((mSelectedPieceType == WPAWN) && (y == 0)) {
		mMap[y][x] = WQUEEN;
		promotion = true;
	}
	if((mSelectedPieceType == BPAWN) && (y == 7)) {
		mMap[y][x] = BQUEEN;
		promotion = true;
	}
	return promotion;
}
		
bool LBoardPVP::initMap() {
	std::ifstream map( "resources/map.map" );
	bool success = true;
	if(map.fail()) {
		printf( "Unable to load map file!\n" );
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
					printf("Error loading map: Unexpected end of file!\n");
					success = false;
					break;
				}
				//If the number is a valid piece number
				if((pieceType >= 0) && (pieceType < TOTAL_PIECES)) {
					mMap[y][x] = pieceType;
				}
				//If we don't recognize the tile type
				else {
					//Stop loading map
					printf( "Error loading map: Invalid piece type at line %d element %d!\n", y, x);
					success = false;
					break;
				}
			}
		}
	}
	return success;
}

void LBoardPVP::playVictorySound() const {
	if(Mix_PlayingMusic()) {
		Mix_FadeOutMusic(3000);
	}
	Mix_PlayChannel(-1, mCheckMate, 0 );
	while(Mix_Playing(-1) > 0) {
		SDL_Delay(16);
	}
	Mix_PlayChannel(-1, mVictory, 0 );
	while(Mix_Playing(-1) > 0) {
		SDL_Delay(16);
	}
}

void LBoardPVP::playMoveSound() {
	if((!(mTookAPiece)) && (!(mIsCastling))) {
		//Play move sound 
		Mix_PlayChannel(-1, mPawnForward, 0);
	}
	else if(mTookAPiece) {
		//Play unable to move sound 
		Mix_PlayChannel(-1, mPieceFall, 0);
		mTookAPiece = false;
	}
	else if(mIsCastling) {
		//Play unable to move sound 
		Mix_PlayChannel(-1, mCastling, 0);
		mIsCastling = false;
	}
}

bool LBoardPVP::pollCheck() { 
	bool check = false;
	
	//check all black pieces on the map if black's turn
	//for each b piece, check its possible moves 
	//if one of these possible move pos contain 
	//a white King - check 
	if(!(mWhiteTurn)) {
		for(int y(0); y < SPL; y++) {
			for(int x(0); x < SPL; x++) {
				if(mMap[y][x] < WBISHOP) {
					//Select piece from map otherwise selected piece will be the one 
					//that just moved
					mSelectedPieceType = mMap[y][x];
					mSelectedPieceYPos = y;
					mSelectedPieceXPos = x;
					//fill possible moves
					showLegalMove();
					int size = mHighlightedTileYPos.size();
					for(int i(0); i < size; i++) {
						if(mMap[mHighlightedTileYPos[i]][mHighlightedTileXPos[i]] == WKING) {
							mCheckStatus = WCHECK;
							check = true;
							return check;
						}
					}
				}
			}
		}
	}
	//check white
	else {
		for(int y(0); y < SPL; y++) {
			for(int x(0); x < SPL; x++) {
				if((mMap[y][x] >= WBISHOP) && (mMap[y][x] < EMPTY)) {
					//Select piece from map otherwise selected piece will be the one 
					//that just moved
					mSelectedPieceType = mMap[y][x];
					mSelectedPieceYPos = y;
					mSelectedPieceXPos = x;
					//fill possible moves
					showLegalMove();
					int size = mHighlightedTileYPos.size();
					for(int i(0); i < size; i++) {
						if(mMap[mHighlightedTileYPos[i]][mHighlightedTileXPos[i]] == BKING) {
							mCheckStatus = BCHECK;
							check = true;
							return check;
						}
					}
				}
			}
		}
	}
	if(!(check)) mCheckStatus = NO_CHECK;
	return check;
}

bool LBoardPVP::pollDiscoverAttack(int piece, int posX, int posY) { 
	
	bool check = false;

	//make a local copy of the map to preview the move
	//if the move checks its own color - cancel the move 
	//in the movePiece() function
	int map[SPL][SPL] = {0};
	for(int a(0); a < SPL; a++) {
		for(int b(0); b < SPL; b++) {
			map[a][b] = mMap[a][b];
		}
	}
	
	//move selected piece to destination in the local map
	map[posY][posX] = piece;
	//empty source cell 
	map[mSelectedPieceYPos][mSelectedPieceXPos] = EMPTY;
	
	//check all white pieces on the map if black's turn
	//for each white piece, check its possible moves 
	//if one of these possible move pos contain 
	//a black King - check 
	if(!(mWhiteTurn)) {
		for(int y(0); y < SPL; y++) {
			for(int x(0); x < SPL; x++) {
				if((map[y][x] >= WBISHOP) && (map[y][x] < EMPTY)) {
					//Select piece from map otherwise selected piece will be the one 
					//that just moved
					int pieceType = map[y][x];
					int selectedPieceYPos = y;
					int selectedPieceXPos = x;
					mHighlightedTileXPos.clear();
					mHighlightedTileYPos.clear();
					//fill possible moves
					bool f = false;
					switch(pieceType) {
						case WPAWN:
							mPawn.whiteMoveOld(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mLastMovedPiece, mLastMovedPieceXSrc, mLastMovedPieceYSrc, mLastMovedPieceXDes, mLastMovedPieceYDes, &mEnPassantTurn, true);
							break;
						case WKNIGHT:
							mKnight.whiteMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WBISHOP:
							mBishop.whiteMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WROOK:
							mRook.whiteMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WQUEEN:
							mQueen.whiteMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WKING:
							mKing.whiteMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mWKingHasMoved, mWRook1HasMoved, mWRook2HasMoved, mCheckStatus, &f);
							break;
					}
					int size = mHighlightedTileYPos.size();
					for(int i(0); i < size; i++) {
						if(map[mHighlightedTileYPos[i]][mHighlightedTileXPos[i]] == BKING) {
							check = true;
							return check;
						}
					}
				}
			}
		}
	}
	//check white
	else {
		for(int y(0); y < SPL; y++) {
			for(int x(0); x < SPL; x++) {
				if(map[y][x] < WBISHOP) {
					//Select piece from map otherwise selected piece will be the one 
					//that just moved
					int pieceType = map[y][x];
					int selectedPieceYPos = y;
					int selectedPieceXPos = x;
					mHighlightedTileXPos.clear();
					mHighlightedTileYPos.clear();
					bool f = false;
					//fill possible moves
					switch(pieceType) {
						case BPAWN:
							mPawn.blackMoveOld(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mLastMovedPiece, mLastMovedPieceXSrc, mLastMovedPieceYSrc, mLastMovedPieceXDes, mLastMovedPieceYDes, &mEnPassantTurn, true);
							break;
						case BKNIGHT:
							mKnight.blackMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BBISHOP:
							mBishop.blackMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BROOK:
							mRook.blackMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BQUEEN:
							mQueen.blackMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BKING:
							mKing.blackMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mBKingHasMoved, mBRook1HasMoved, mBRook2HasMoved, mCheckStatus, &f);
							break;
					}
					int size = mHighlightedTileYPos.size();
					for(int i(0); i < size; i++) {
						if(map[mHighlightedTileYPos[i]][mHighlightedTileXPos[i]] == WKING) {
							check = true;
							return check;
						}
					}
				}
			}
		}
	}
	return check; //default false
}

void LBoardPVP::renderTimer() {
	//only render time if TL_YES == 1 in mSettingsTable
	if(mSettingsTable[TL_YES] == 1) {
		SDL_Color black = {0,0,0,0xFF};
		
		// white timer total time left in seconds
		int wtime = mTimeLimit - (mWhiteTimer.getTicks() / 1000);

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
		
		mWhiteTimerTexture->loadFromRenderedText(gFont64, whiteTimeText.str().c_str(), black);
		mWhiteTimerTexture->render(0,SCREEN_HEIGHT - 64);
		
		// black timer total time left in seconds
		int btime = mTimeLimit - (mBlackTimer.getTicks() / 1000);

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
		mBlackTimerTexture->loadFromRenderedText(gFont64, blackTimeText.str().c_str(), black);
		mBlackTimerTexture->render(0,0);
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
			if((mMap[y][x] < WBISHOP)) {
				int piece = mMap[y][x];
				blackScore += pieceValue(piece);
			}
			//white piece
			else if((mMap[y][x] >= WBISHOP) && (mMap[y][x] < EMPTY)) {
				int piece = mMap[y][x];
				whiteScore += pieceValue(piece);
			}
		}
	}
	
	int a(blackScore);
	blackScore = MAX - whiteScore;
	whiteScore = MAX - a;
	
	SDL_Color black = {0,0,0,0xFF};
	std::stringstream whiteScoreStr;
	std::stringstream blackScoreStr;

	whiteScoreStr << std::to_string(whiteScore);
	blackScoreStr << std::to_string(blackScore);
	
	mWhiteScoreTexture->loadFromRenderedText(gFont64, whiteScoreStr.str().c_str(), black);
	mBlackScoreTexture->loadFromRenderedText(gFont64, blackScoreStr.str().c_str(), black);
	
	if(mSettingsTable[TL_NO]) {
		mWhiteScoreTexture->render(0, SCREEN_HEIGHT - 64);
		mBlackScoreTexture->render(0, 0);
	}
	else {
		mWhiteScoreTexture->render(OFFSET * 3, SCREEN_HEIGHT - 64);
		mBlackScoreTexture->render(OFFSET * 3, 0);
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
				mMiniPieceTexture->render(blackPosX + blackOffset, blackPosY, &mMiniPieceClip[i]);
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
				mMiniPieceTexture->render(whitePosX + whiteOffset, whitePosY, &mMiniPieceClip[i]);
				whiteOffset += 32;
			}
		}
	}
}

void LBoardPVP::calculateRemainingTime() {
	//Handle white timer
	if(mWhiteTurn) {
		//number of elapsed seconds
				
	}
	//Handle black timer
	else {
		
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

bool LBoardPVP::checkMate() {
	//win condition
	bool checkMate = true;
	
	//local copy of the mMap to not mess with anything
	int map[SPL][SPL] = {0};
	for(int a(0); a < SPL; a++) {
		for(int b(0); b < SPL; b++) {
			map[a][b] = mMap[a][b];
		}
	}
	
	int selectedPieceType = 0;
	int selectedPieceXPos = 0;
	int	selectedPieceYPos = 0;
	
	//White turn
	if(mWhiteTurn) {
		for(int y(0); y < SPL; y++) {
			for(int x(0); x < SPL; x++) {
				//check each black piece
				if(map[y][x] < WBISHOP) {
					selectedPieceType = map[y][x];
					selectedPieceYPos = y;
					selectedPieceXPos = x;
					mHighlightedTileXPos.clear();
					mHighlightedTileYPos.clear();
					bool f = false;
					//fill possible moves
					switch(selectedPieceType) {
						case BPAWN:
							mPawn.blackMoveOld(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mLastMovedPiece, mLastMovedPieceXSrc, mLastMovedPieceYSrc, mLastMovedPieceXDes, mLastMovedPieceYDes, &mEnPassantTurn);
							break;
						case BKNIGHT:
							mKnight.blackMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BBISHOP:
							mBishop.blackMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BROOK:
							mRook.blackMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BQUEEN:
							mQueen.blackMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BKING:
							mKing.blackMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mBKingHasMoved, mBRook1HasMoved, mBRook2HasMoved, mCheckStatus, &f);
							break;
					}
					//local copy of hilghlighted tile pos as it
					//is going to be messed with when checking if 
					//the possible move checks its own king
					std::vector<int> destPosX;
					std::vector<int> destPosY;
					//only fill if there's something to fill
					if(mHighlightedTileXPos.size() > 0) {
						int size = mHighlightedTileXPos.size();
						for(int i(0); i < size; i++) {
							destPosX.push_back(mHighlightedTileXPos[i]);
							destPosY.push_back(mHighlightedTileYPos[i]);
						}
					}
					int size = destPosX.size();
					//for each possible move
					for(int i(0); i < size; i++) {
						//copy of local copy of the map to preview the moves
						int mapCopy[SPL][SPL] = {0};
						for(int a(0); a < SPL; a++) {
							for(int b(0); b < SPL; b++) {
								mapCopy[a][b] = map[a][b];
							}
						}
						//move selected piece to destination in the local mapCopy
						mapCopy[destPosY[i]][destPosX[i]] = selectedPieceType;
						//empty source cell 
						mapCopy[selectedPieceYPos][selectedPieceXPos] = EMPTY;
						
						//if pollCheckMate returns false then
						//the move in the copy canceled the check
						if(!(pollCheckMate(mapCopy))) {
							checkMate = false;
							return checkMate;
						}
					}
				}
			}
		}
	}
	//Black turn
	else {
		for(int y(0); y < SPL; y++) {
			for(int x(0); x < SPL; x++) {
				//check each white piece
				if((map[y][x] >= WBISHOP) && (map[y][x] < EMPTY)) {
					selectedPieceType = map[y][x];
					selectedPieceYPos = y;
					selectedPieceXPos = x;
					mHighlightedTileXPos.clear();
					mHighlightedTileYPos.clear();
					bool f = false;
					//fill possible moves
					switch(selectedPieceType) {
						case WPAWN:
							mPawn.whiteMoveOld(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mLastMovedPiece, mLastMovedPieceXSrc, mLastMovedPieceYSrc, mLastMovedPieceXDes, mLastMovedPieceYDes, &mEnPassantTurn);
							break;
						case WKNIGHT:
							mKnight.whiteMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WBISHOP:
							mBishop.whiteMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WROOK:
							mRook.whiteMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WQUEEN:
							mQueen.whiteMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WKING:
							mKing.whiteMove(map, selectedPieceXPos, selectedPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mBKingHasMoved, mBRook1HasMoved, mBRook2HasMoved, mCheckStatus, &f);
							break;
					}
					//local copy of hilghlighted tile pos as it
					//is going to be messed with when checking if 
					//the possible move checks its own king
					std::vector<int> destPosX;
					std::vector<int> destPosY;
					//only fill if there's something to fill
					if(mHighlightedTileXPos.size() > 0) {
						int size = mHighlightedTileXPos.size();
						for(int i(0); i < size; i++) {
							destPosX.push_back(mHighlightedTileXPos[i]);
							destPosY.push_back(mHighlightedTileYPos[i]);
						}
					}
					int size = destPosX.size();
					//for each possible move
					for(int i(0); i < size; i++) {
						//copy of local copy of the map to preview the moves
						int mapCopy[SPL][SPL] = {0};
						for(int a(0); a < SPL; a++) {
							for(int b(0); b < SPL; b++) {
								mapCopy[a][b] = map[a][b];
							}
						}
						//move selected piece to destination in the local mapCopy
						mapCopy[destPosY[i]][destPosX[i]] = selectedPieceType;
						//empty source cell 
						mapCopy[selectedPieceYPos][selectedPieceXPos] = EMPTY;
						
						//if pollCheckMate returns false then
						//the move in the copy canceled the check
						if(!(pollCheckMate(mapCopy))) {
							checkMate = false;
							return checkMate;
						}
					}
				}
			}
		}
	}	
	//default true 
	return checkMate; 
}

bool LBoardPVP::pollCheckMate(int mapCopy[SPL][SPL]) {
	bool checkMate = false;
	if(mWhiteTurn) {
		for(int y(0); y < SPL; y++) {
			for(int x(0); x < SPL; x++) {
				if((mapCopy[y][x] >= WBISHOP) && (mapCopy[y][x] < EMPTY)) {
					//Select piece from map otherwise selected piece will be the one 
					//that just moved
					int checkPieceType = mapCopy[y][x];
					int checkPieceYPos = y;
					int checkPieceXPos = x;
					//fill possible moves
					mHighlightedTileXPos.clear();
					mHighlightedTileYPos.clear();
					bool f = false;
					switch(checkPieceType) {
						case WPAWN:
							mPawn.whiteMoveOld(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mLastMovedPiece, mLastMovedPieceXSrc, mLastMovedPieceYSrc, mLastMovedPieceXDes, mLastMovedPieceYDes, &mEnPassantTurn);
							break;
						case WKNIGHT:
							mKnight.whiteMove(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WBISHOP:
							mBishop.whiteMove(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WROOK:
							mRook.whiteMove(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WQUEEN:
							mQueen.whiteMove(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case WKING:
							mKing.whiteMove(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mWKingHasMoved, mWRook1HasMoved, mWRook2HasMoved, mCheckStatus, &f);
							break;
					}
					int size = mHighlightedTileYPos.size();
					for(int i(0); i < size; i++) {
						if(mapCopy[mHighlightedTileYPos[i]][mHighlightedTileXPos[i]] == BKING) {
							//still check if black king is found
							checkMate = true;
							return checkMate;
						}
					}
				}
			}
		}
	}
	else {
		for(int y(0); y < SPL; y++) {
			for(int x(0); x < SPL; x++) {
				if(mapCopy[y][x] < WBISHOP) {
					//Select piece from map otherwise selected piece will be the one 
					//that just moved
					int checkPieceType = mapCopy[y][x];
					int checkPieceYPos = y;
					int checkPieceXPos = x;
					//fill possible moves
					mHighlightedTileXPos.clear();
					mHighlightedTileYPos.clear();
					bool f = false;
					switch(checkPieceType) {
						case BPAWN:
							mPawn.blackMoveOld(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mLastMovedPiece, mLastMovedPieceXSrc, mLastMovedPieceYSrc, mLastMovedPieceXDes, mLastMovedPieceYDes, &mEnPassantTurn);
							break;
						case BKNIGHT:
							mKnight.blackMove(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BBISHOP:
							mBishop.blackMove(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BROOK:
							mRook.blackMove(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BQUEEN:
							mQueen.blackMove(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos);
							break;
						case BKING:
							mKing.blackMove(mapCopy, checkPieceXPos, checkPieceYPos, &mHighlightedTileXPos, &mHighlightedTileYPos, mWKingHasMoved, mWRook1HasMoved, mWRook2HasMoved, mCheckStatus, &f);
							break;
					}
					int size = mHighlightedTileYPos.size();
					for(int i(0); i < size; i++) {
						if(mapCopy[mHighlightedTileYPos[i]][mHighlightedTileXPos[i]] == WKING) {
							//still check if white king is found
							checkMate = true;
							return checkMate;
						}
					}
				}
			}
		}
	}
	return checkMate; // default false
}

bool LBoardPVP::pollVictory() const {
	return mGameOver;
}

bool LBoardPVP::pollTimeOut() {
	if(mWhiteTimerRanOut || mBlackTimerRanOut) {
		if(Mix_PlayingMusic()) {
			Mix_FadeOutMusic(500);
		}
		loadOutOfTimeTexture();
		renderOutOfTimeScreen();
		Mix_PlayChannel(-1, mDefeat, 0);
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