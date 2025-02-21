/*
LBoardPVP.h
Sun Mar 20 2022
by Valentin
-----------
LBoard is the object containing:
- the pieces position on the mMap[][]
- the pieces object that fill possible movements -> subject to change
- pieces and tiles textures + their highlighted version + their clip
- buttons "attached" to piece textures
- event handling -> select pieces/unselect/move

Board also contains settings such as tile colors, legal move displayed on map...

*/

#ifndef HEADER_LBOARDPVP
#define HEADER_LBOARDPVP

#include "utils/util.h"
#include "fstream"
#include "iostream"
#include "vector"
#include "graphics/LTexture.h"
#include "graphics/LTileBorder.h"
#include "LButton.h"
#include "game/pieces/LKing.h"
#include "game/pieces/LRook.h"
#include "game/pieces/LBishop.h"
#include "game/pieces/LKnight.h"
#include "game/pieces/LQueen.h"
#include "game/pieces/LPawn.h"
#include "com/LTimer.h"
#include "sound/LMusicPlayer.h"
#include "sound/LChunkPlayer.h"
#include "factories/LMediaFactory.h"
#include "com/LObserver.h"
#include "com/LSubject.h"
#include "states/LStateMachine.h"
#include "game/board/LEngine.h"

class LBoardPVP : public LSubject {

public:
    LBoardPVP(LObserver* observer);
    ~LBoardPVP();
    void update();
    void render();
    void free();

private:
    LObserver* mAppObserver;
    OptionValues mSettings;
    LEngine* mEngine;

    bool initMap();
    bool initPiecesTextures();
    bool initTileTextures();
    bool initPauseTexture();
    bool initOutOfTimeTexture();
    void initSettings();

    void setPiecesClip();
    void setTileRectClip();
    void setButtons();

    void renderTile();
    void renderPieces();
    void renderOutOfTimeScreen();
    void renderTimer();
    void renderScore();
    void renderDeadPieces();
    void renderPause();
    void drawButtons();

    void startWhiteTimer();
    void startBlackTimer();
    void pauseWhiteTimer();
    void pauseBlackTimer();
    void unpauseWhiteTimer();
    void unpauseBlackTimer();
    void stopWhiteTimer();
    void stopBlackTimer();

    void playVictorySound() const;
    void playMusic();
    void playMoveSound();

    void handleEvents(SDL_Event* e);
    void changeTurn();

    // To do: decouple the chess logic from the board class
    // and create a new class for the chess logic

    // std::vector<SDL_Point> getLegalMoves();
    // bool pollCheck();
    // bool isSelfCheck(int piece, SDL_Point dest);
    // bool pollCheckMate(std::array<std::array<int, SPL>, SPL> map);
    // bool checkMate();
    bool castling(int piece, SDL_Point dest);

    bool isGameOver() const;
    bool isOutOfTime();
    bool isPaused() const;
    void pause();
    void fillDeadPieceTab(const int fallenPiece);
    int pieceValue(int const pieceType) const;
    void calculateRemainingTime();

    void enPassant(int destinationPosX);
    void move(SDL_Point dest, SDL_Point src, int piece);
    void movePiece(SDL_Event* e);
    void setCastlingBools(SDL_Point src, int piece);
    bool checkPromotion(SDL_Point dest);
    
    //contains the current piece layout
    // int mMap[SPL][SPL];
    // new map
    std::array<std::array<int, SPL>, SPL> mMap;
    LTimer mWhiteTimer;
    LTimer mBlackTimer;
    int mTimeLimit;
    int mDeadWhitePiece[EMPTY];
    int mDeadBlackPiece[EMPTY];
    //textures
    LTexture *mPieceTexture;
    LTexture *mMiniPieceTexture;
    LTexture *mHighlightedPieceTexture;
    LTexture *mTileTexture;
    LTexture *mWhiteTimerTexture;
    LTexture *mBlackTimerTexture;
    LTexture *mWhiteScoreTexture;
    LTexture *mBlackScoreTexture;
    LTexture *mPauseBackgroundTexture;
    LTexture *mPauseTextTexture;
    LTexture *mOutOfTimeTexture;
    //Texture clipping 
    SDL_Rect mPieceClip[TOTAL_PIECES - 1];
    SDL_Rect mMiniPieceClip[TOTAL_PIECES - 1];
    SDL_Rect mTileRectClip[TOTAL_TILE_COLOR];
    //Buttons for pieces
    std::vector <LButton*> mPieceButtons;
    // User highlighted tiles
    std::vector<SDL_Point> mRightClickedTile;
    //Legal moves coordinates
    std::vector<SDL_Point> mLegalMoveTile;
    //Selected piece
    SDL_Point mSelectedPiecePos;
    SDL_Point mLastMovedPieceSrcPos;
    SDL_Point mLastMovedPieceDestPos;
    int mSelectedPieceType,
        mCheckStatus,
        mLastMovedPiece;

    bool mWhiteTurn = true,
         mWhiteTimerRanOut = false,
         mBlackTimerRanOut = false,
         mIsPaused = false,
         mAPieceIsSelected = false,
         mEnPassantTurn = false,
         mTookAPiece = false,
         mIsCastling = false,
         mGameOver = false,
         mWKingHasMoved = false,
         mBKingHasMoved = false,
         mWRook1HasMoved = false,
         mWRook2HasMoved = false,
         mBRook1HasMoved = false,
         mBRook2HasMoved = false;
    LKing mKing;
    LRook mRook;
    LBishop mBishop;
    LKnight mKnight;
    LQueen mQueen;
    LPawn mPawn;
};

#endif