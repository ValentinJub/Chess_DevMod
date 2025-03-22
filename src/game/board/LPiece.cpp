#include "game/board/LPiece.h"

extern SDL_Renderer* gRenderer;

LPiece::LPiece(int x, int y, int w, int h, SDL_Texture* texture, SDL_Rect* clip, LButton* button, SDL_Point pos, int type) : LTextureClickable(x, y, w, h, texture, button), mPos(pos), mType(type), mIsHighlighted(false) {
    this->setClip(clip);
}

LPiece::~LPiece() {
    this->free();
}

void LPiece::setPos(int x, int y) {
    mPos.x = x;
    mPos.y = y;
}

void LPiece::setType(int type) {
    mType = type;
}

void LPiece::setHighlighted(bool isHighlighted) {
    mIsHighlighted = isHighlighted;
}

SDL_Point LPiece::pos() const {
    return mPos;
}

int LPiece::type() const {
    return mType;
}

bool LPiece::isHighlighted() const {
    return mIsHighlighted;
}

void LPiece::render() {
    this->renderAt(this->x(), this->y(), mClip);
}

void LPiece::free() {
    LTextureClickable::free();
}