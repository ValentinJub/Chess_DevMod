#ifndef LPROMOTION_H
#define LPROMOTION_H

#include "graphics/LTextureClickable.h"
#include "com/LSubject.h"
#include "states/LState.h"

class LPromotion : public LVisualComponent, public LSubject, public LState {
public:
    LPromotion(bool isWhite, int column, const char* pieceType, LObserver* observer);
    void enter(LObserver* observer);
    void exit();
    void update(Uint64 dt);
    void render();
    void free();
private:
    bool mIsWhite;
    // The piece spritesheet texture
    std::array<LTextureClickable*, 4> mTexture;
    // The pieces clip
    SDL_Rect mClip[4];
    // The rectangle for the promotion
    SDL_Rect mRect;
    LObserver* mBoardObs;
};


#endif