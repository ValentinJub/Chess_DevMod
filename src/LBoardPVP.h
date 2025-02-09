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
#include "LButton.h"
#include "pieces/LKing.h"
#include "pieces/LRook.h"
#include "pieces/LBishop.h"
#include "pieces/LKnight.h"
#include "pieces/LQueen.h"
#include "pieces/LPawn.h"
#include "com/LTimer.h"
#include "sound/LMusicPlayer.h"
#include "sound/LChunkPlayer.h"
#include "factories/LMediaFactory.h"

class LBoardPVP {

public:
    LBoardPVP();
    ~LBoardPVP();
    bool initMap();
    void free();
    bool loadPiecesTextures();
    bool loadTileTextures();
    bool loadPauseTexture();
    bool loadOutOfTimeTexture();
    void setPiecesClip();
    void setTileRectClip();
    void renderTile();
    void renderPieces();
    void renderOutOfTimeScreen();
    void showLegalMove();
    void setButtons();
    void handleEvents(SDL_Event* e);
    bool pollCheck();
    bool pollDiscoverAttack(int piece, int x, int y);
    bool pollVictory() const;
    void playVictorySound() const;
    void startWhiteTimer();
    void startBlackTimer();
    void pauseWhiteTimer();
    void pauseBlackTimer();
    void unpauseWhiteTimer();
    void unpauseBlackTimer();
    void stopWhiteTimer();
    void stopBlackTimer();
    void renderTimer();
    void renderScore();
    void renderDeadPieces();
    void playMusic();
    bool pollTimeOut();
    void renderPause();
    bool isPaused() const;
    void pause();
    void drawButtons();
    
    
protected:

private:
    void fillDeadPieceTab(const int fallenPiece);
    int pieceValue(int const pieceType) const;
    void calculateRemainingTime();
    void readSettingsFromFile();
    void initGameSettings();
    void playMoveSound();
    bool pollCheckMate(int mapCopy[SPL][SPL]);
    bool checkMate();
    void enPassant(int destinationPosX);
    void changeTurn();
    void move(int destX, int destY, int srcX, int srcY, int piece);
    bool castling(int piece, int destinationPosX, int destinationPosY);
    void movePiece(SDL_Event* e);
    void setCastlingBools(int x, int y, int piece);
    bool checkPromotion(int x, int y);
    
    //contains the current piece layout
    int mMap[SPL][SPL];
    LTimer mWhiteTimer;
    LTimer mBlackTimer;
    int mTimeLimit;
    bool mWhiteTimerRanOut,
         mBlackTimerRanOut;
    bool mIsPaused;
    int mDeadWhitePiece[EMPTY];
    int mDeadBlackPiece[EMPTY];
    //textures
    LTexture *mPieceTexture;
    LTexture *mMiniPieceTexture;
    LTexture *mHighlightedPieceTexture;
    LTexture *mTileTexture;
    LTexture *mHighlightedTileTexture;
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
    //Legal moves coordinates
    std::vector <int> mHighlightedTileXPos,
                      mHighlightedTileYPos,
                      mRightClickedTileXPos,
                      mRightClickedTileYPos;
    int mSelectedPieceType,
        mSelectedPieceXPos,
        mSelectedPieceYPos,
        mTileColor,
        mPieceTheme,
        mCheckStatus,
        mLastMovedPiece,
        mLastMovedPieceXSrc,
        mLastMovedPieceYSrc,
        mLastMovedPieceXDes,
        mLastMovedPieceYDes;
    bool mAPieceIsSelected;
    bool mWhiteTurn,
         mEnPassantTurn,
         mPreviewMove,
         mTookAPiece,
         mIsCastling,
         mGameOver,
         mWKingHasMoved,
         mBKingHasMoved,
         mWRook1HasMoved,
         mWRook2HasMoved,
         mBRook1HasMoved,
         mBRook2HasMoved;
    LKing mKing;
    LRook mRook;
    LBishop mBishop;
    LKnight mKnight;
    LQueen mQueen;
    LPawn mPawn;
    
    int mSettingsTable[TOTAL_CLICKABLE_ITEMS - 1]; //minus back
};

#endif