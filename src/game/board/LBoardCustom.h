/*
LBoardCustom.h
Sun Mar 20 2022
by Valentin
-----------

LBoardCustom is the object containing:

- An interactive board where the player can place pieces

*/

#ifndef HEADER_LBOARDCUST
#define HEADER_LBOARDCUST

#include "utils/util.h"
#include "fstream"
#include "iostream"
#include "vector"
#include "graphics/LTexture.h"
#include "com/LButton.h"
#include "factories/LMediaFactory.h"
#include "com/LSubject.h"
#include "states/LStateMachine.h"
#include "states/LTransition.h"
#include "game/logic/LEngine.h"

class LBoardCustom : public LSubject{

public:
    LBoardCustom(LObserver* observer);
    ~LBoardCustom();
    void update();
    void render();
    void free();

private:
    LObserver* mAppObserver;
    LEngine mEngine;
    std::array<std::array<int, SPL>, SPL> mBoard;

    //textures
    LTexture *mPieceTexture;
    LTexture *mTileTexture;

    //Texture clipping 
    SDL_Rect mPieceClip[TOTAL_PIECES - 1];
    SDL_Rect mTileRectClip[TOTAL_TILE_COLOR];
    
    //Buttons for pieces
    std::vector <LButton*> mPieceButtons;
    
    //Selected piece
    SDL_Point mSelectedPiecePos;

    void setBoardButtons();
    void renderTile();
    void renderBoardPieces();
    void drawButtons();
    void handleEvents(SDL_Event* e);
    int mSelectedPiece = EMPTY;
    bool mAPieceIsSelected = false;
};

#endif