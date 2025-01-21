/*
LBoardPVAI.h
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

#ifndef HEADER_LBOARDPVAI
#define HEADER_LBOARDPVAI

#include "utils/util.h"
#include "fstream"
#include "iostream"
#include "vector"
#include "LTexture.h"
#include "LButton.h"
#include "LKing.h"
#include "LRook.h"
#include "LBishop.h"
#include "LKnight.h"
#include "LQueen.h"
#include "LPawn.h"
#include "LTimer.h"

class LBoardPVAI {

public:
LBoardPVAI();
    ~LBoardPVAI();
    void checkRemainingPieces();
    void drawButtons();
    void free();
    void handleEvents(SDL_Event* e);
    bool initMap();
    bool isBlackThinking() const;
    bool isPaused() const;
    bool loadOutOfTimeTexture();
    bool loadPauseTexture();
    bool loadPiecesTextures();
    bool loadTileTextures();
    void pause();
    bool pollTimeOut();
    bool pollVictory() const;
    void playMoveSound();
    void playMusic();
    void playVictorySound() const;
    void renderDeadPieces();
    void renderOutOfTimeScreen();
    void renderPause();
    void renderPieces();
    void renderScore();
    void renderTile();
    void renderTimer();
    void setButtons();
    void setPiecesClip();
    void setTileRectClip();
    void startWhiteTimer();
    void stopBlackThinking();
    void stopWhiteTimer();
    void unpauseWhiteTimer();
    void pauseWhiteTimer();
    
    
protected:

private:
    void blackMove();
    void blackTurn();
    bool castling(int piece, int destinationPosX, int destinationPosY, int sourcePosX, int sourcePosY);
    void changeTurn();
    bool checkIfBlackCanTake(int map[SPL][SPL], const int piece, const int piecePosX, const int piecePosY, int* trade);
    bool checkIfWhiteCanTake(int map[SPL][SPL], const int piece, const int piecePosX, const int piecePosY, int* trade);
    bool checkMate();
    bool checkPromotion(int x, int y, int piece);
    void clearBlackVectors();
    void eveluateBlackMove(int map[SPL][SPL], const int piece, const int desX, const int desY, const int srcX, const int srcY);
    void enPassant(int destinationPosX);
    void fillBlackVectors(int tradeValue, const int desX, const int desY, const int srcX, const int srcY);
    void fillDeadPieceTab(const int fallenPiece);
    void initGameSettings();
    void move(int destX, int destY, int srcX, int srcY, int piece);
    void movePiece(SDL_Event* e);
    bool pollCheck(const int map[SPL][SPL]);
    bool pollDiscoverAttack(const int map[SPL][SPL], const int piece, const int desX, const int desY, const int srcX, const int srcY);
    int pieceValue(int const pieceType) const;
    void readSettingsFromFile();
    void renderBeforeBlackPlays();
    void setCastlingBools(int x, int y, int piece);
    void setEnPassantVar(const int piece, const int srcX, const int srcY , const int desX, const int desY);
    void showLegalMove(const int map[SPL][SPL], int const pieceType, int const pieceXPos, const int pieceYPos, bool preCheck = false);

  int mDeadWhitePiece[EMPTY];
  int mDeadBlackPiece[EMPTY];
  LTexture *mMiniPieceTexture;
  SDL_Rect mMiniPieceClip[TOTAL_PIECES - 1];
  
  std::string mPieceStr[TOTAL_PIECES - 1];
    void initPieceStr();
    int mTurn;
    Mix_Music* mMusic[NBR_OF_MUSIC];
    
    //contains the current piece layout
    int mMap[SPL][SPL];
    LTimer mWhiteTimer;
    int mTimeLimit;
    bool mWhiteTimerRanOut;
    bool mIsPaused;
    //textures
    LTexture *mPieceTexture, 
             *mHighlightedPieceTexture;
    LTexture *mTileTexture;
    LTexture *mHighlightedTileTexture;
    LTexture *mWhiteTimerTexture;
    LTexture *mWhiteScoreTexture;
    LTexture *mBlackScoreTexture;
    LTexture *mPauseBackgroundTexture;
    LTexture *mPauseTextTexture;
    LTexture *mOutOfTimeTexture;
    //Texture clipping 
    SDL_Rect mPieceClip[TOTAL_PIECES - 1];
    SDL_Rect mTileRectClip[TOTAL_TILE_COLOR];
    //Buttons for pieces
    std::vector <LButton*> mPieceButtons;
    //Legal moves coordinates
    std::vector <int> mHighlightedTileXPos,
                      mHighlightedTileYPos;

    std::vector <int> mRightClickedTileXPos,
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
         mBlackIsThinking,
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
         mBRook2HasMoved,
         mBlackCanCastle;
    LKing mKing;
    LRook mRook;
    LBishop mBishop;
    LKnight mKnight;
    LQueen mQueen;
    LPawn mPawn;
    
    Mix_Chunk* mBingo = NULL;
    Mix_Chunk* mDefeat = NULL;
    Mix_Chunk* mVictory = NULL;
    Mix_Chunk* mCheck = NULL;
    Mix_Chunk* mCheckMate = NULL;
    Mix_Chunk* mError = NULL;
    Mix_Chunk* mCastling = NULL;
    Mix_Chunk* mPieceFall = NULL;
    Mix_Chunk* mPawnForward = NULL;
    
    int mSettingsTable[TOTAL_CLICKABLE_ITEMS - 1]; //minus back
    
    //vectors used to store the possible Black moves
    //they are ordered following how good a move is
    //SS = very excellent move
    std::vector<int> mSSSrcMoveX,
                     mSSSrcMoveY,
                     mSSDesMoveX,
                     mSSDesMoveY;
                     
    //S = excellent move
    std::vector<int> mSSrcMoveX,
                     mSSrcMoveY,
                     mSDesMoveX,
                     mSDesMoveY;
                     
    //EP = en passant move
    std::vector<int> mEPSrcMoveX,
                     mEPSrcMoveY,
                     mEPDesMoveX,
                     mEPDesMoveY;
                     
    //A = vey good move
    std::vector<int> mASrcMoveX,
                     mASrcMoveY,
                     mADesMoveX,
                     mADesMoveY;
                     
    //B = okay move
    std::vector<int> mBSrcMoveX,
                     mBSrcMoveY,
                     mBDesMoveX,
                     mBDesMoveY;
                     
    //D = bad
    std::vector<int> mDSrcMoveX,
                     mDSrcMoveY,
                     mDDesMoveX,
                     mDDesMoveY;
                     
};

#endif