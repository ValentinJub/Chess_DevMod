/*
LBoard.h
Sun Mar 20 2022
by Valentin
-----------
LBoard is the object containing:
- the pieces position on the mBoard[][]
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
#include "com/LButton.h"
#include "com/LTimer.h"
#include "sound/LMusicPlayer.h"
#include "sound/LChunkPlayer.h"
#include "factories/LMediaFactory.h"
#include "com/LObserver.h"
#include "com/LSubject.h"
#include "states/LStateMachine.h"
#include "states/LTransition.h"
#include "game/logic/LEngine.h"

class LClock;
class LComputer;

enum PlayMode {
    PVP = 0,
    PVAI = 1
};

class LBoard : public LSubject, public LObserver {

public:
    LBoard(LObserver* observer, PlayMode mode);
    ~LBoard();
    void update();
    void render();
    void free();
    void poll(LSubject* sub, int value) override;

private:
    PlayMode mPlayMode;
    LObserver* mAppObserver;
    OptionValues mSettings;
    LEngine mEngine;
    LComputer* mComputer;
    LClock* mClock;
    std::array<std::array<int, SPL>, SPL> mBoard;

    //textures
    LTexture *mPieceTexture;
    LTexture *mMiniPieceTexture;
    LTexture *mHighlightedPieceTexture;
    LTexture *mTileTexture;
    LTexture *mWhiteTimerTexture = NULL;
    LTexture *mBlackTimerTexture = NULL;
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
    SDL_Point mPromotedPiecePos;

    bool initPiecesTextures();
    bool initTileTextures();
    bool initPauseTexture();
    bool initOutOfTimeTexture();
    void initSettings();

    void setPiecesClip();
    void setTileRectClip();
    void setButtons();
    void setCastlingBools(SDL_Point src, int piece);

    void renderTile();
    void renderPieces();
    void renderTileCoordinates();
    void renderOutOfTimeScreen();
    void renderTimer();
    void renderScore();
    void renderDeadPieces();
    void renderPause();
    void drawButtons();

    void playVictorySound() const;
    void playMusic();
    void playMoveSound(bool captured, bool castled) const;

    void handleEvents(SDL_Event* e);
    void changeTurn();

    void doMove(SDL_Point dest, SDL_Point src, int piece);
    void move(SDL_Event* e);
    void postMove(SDL_Point dest);
    void computerMove();

    void pause();
    void fillDeadPieceTab(const int fallenPiece);
    bool checkPromotion(SDL_Point dest);

    int mTimeLimit;
    int mDeadWhitePiece[EMPTY];
    int mDeadBlackPiece[EMPTY];

    int mSelectedPiece = EMPTY;

    bool mWhiteTurn = true,
         mWhiteTimerRanOut = false,
         mBlackTimerRanOut = false,
         mIsPaused = false,
         mAPieceIsSelected = false,
         mGameOver = false;
};

#endif