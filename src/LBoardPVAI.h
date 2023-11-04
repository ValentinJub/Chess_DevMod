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

#include "headers.h"
#include "funct_headers.h"
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
    bool initMap();
    void free();
    bool loadPiecesTextures();
    bool loadTileTextures();
    bool loadPauseTexture();
    void setPiecesClip();
    void setTileRectClip();
    void renderTile();
    void renderPieces();
    void setButtons();
    void handleEvents(SDL_Event* e);
    bool pollVictory() const;
    void playVictorySound() const;
    void startWhiteTimer();
    void pauseWhiteTimer();
    void unpauseWhiteTimer();
    void stopWhiteTimer();
    void renderTimer();
    void renderScore();
    void renderDeadPieces();
    void playMusic();
    bool pollTimeOut();
    void renderPause();
    bool isPaused() const;
    void pause();
    bool isBlackThinking() const;
    void stopBlackThinking();
    void playMoveSound();
    //checks if W|B has only a king remaining, if yes mGameOver true
    void checkRemainingPieces();
    void drawButtons();
    
    
protected:

private:
    void renderBeforeBlackPlays();
    void fillDeadPieceTab(const int fallenPiece);
    void blackMove();
    void fillBlackVectors(int tradeValue, const int desX, const int desY, const int srcX, const int srcY);
    bool checkIfWhiteCanTake(int map[SPL][SPL], const int piece, const int piecePosX, const int piecePosY, int* trade);
    bool checkIfBlackCanTake(int map[SPL][SPL], const int piece, const int piecePosX, const int piecePosY, int* trade);
    int pieceValue(int const pieceType) const;
    void eveluateBlackMove(int map[SPL][SPL], const int piece, const int desX, const int desY, const int srcX, const int srcY);
    void clearBlackVectors();
    void blackTurn();
    bool pollCheck(const int map[SPL][SPL]);
    bool pollDiscoverAttack(const int map[SPL][SPL], const int piece, const int desX, const int desY, const int srcX, const int srcY);
    void showLegalMove(const int map[SPL][SPL], int const pieceType, int const pieceXPos, const int pieceYPos, bool preCheck = false);
    void readSettingsFromFile();
    void initGameSettings();
    bool checkMate();
    void changeTurn();
    void move(int destX, int destY, int srcX, int srcY, int piece);
    bool castling(int piece, int destinationPosX, int destinationPosY, int sourcePosX, int sourcePosY);
    void movePiece(SDL_Event* e);
    void setCastlingBools(int x, int y, int piece);
    bool checkPromotion(int x, int y, int piece);
    
    void setEnPassantVar(const int piece, const int srcX, const int srcY , const int desX, const int desY);
    void enPassant(int destinationPosX);
    
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
    LTexture *mPieceTexture;
    LTexture *mHighlightedPieceTexture;
    LTexture *mTileTexture;
    LTexture *mHighlightedTileTexture;
    LTexture *mWhiteTimerTexture;
    LTexture *mWhiteScoreTexture;
    LTexture *mBlackScoreTexture;
    LTexture *mPauseBackgroundTexture;
    LTexture *mPauseTextTexture;
    //Texture clipping 
    SDL_Rect mPieceClip[TOTAL_PIECES - 1];
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