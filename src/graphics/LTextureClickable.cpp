#include "graphics/LTextureClickable.h"

extern SDL_Renderer* gRenderer;

LTextureClickable::LTextureClickable(int x, int y, int w, int h, SDL_Texture* texture, LButton* button) : LTexture(x, y, w, h, texture), mButton(button) {};

LTextureClickable::~LTextureClickable() {
    delete mButton;
    mButton = NULL;
}

bool LTextureClickable::isHovered() {
    return mButton->isHovered();
}

bool LTextureClickable::isClicked(SDL_Event* e) {
    return mButton->isClicked(e);
}

void LTextureClickable::setButton(LButton* button) {
    mButton = button;
}

void LTextureClickable::setButton() {
    mButton = new LButton(this->x(), this->y(), this->w(), this->h());
}

LButton* LTextureClickable::getButton() const {
    return mButton;
}