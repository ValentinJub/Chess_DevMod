#include "LTextureClickable.h"

extern SDL_Renderer* gRenderer;

LTextureClickable::LTextureClickable(int x, int y, int w, int h, SDL_Texture* texture, LButton* button) : LTexture(x, y, w, h, texture), mButton(button) {};

LTextureClickable::~LTextureClickable() {
    delete mButton;
    mButton = NULL;
}

void LTextureClickable::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    // The quad to render the texture to
    SDL_Rect renderQuad = {x, y, this->getWidth(), this->getHeight()};

    // set clip rendering dimensions
    if(clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // render to screen
    SDL_RenderCopyEx(gRenderer, this->getTexture(), clip, &renderQuad, angle, center, flip);
    // render optional decorators
    this->renderDecorators();
}

void LTextureClickable::setButton(LButton* button) {
    mButton = button;
}

LButton* LTextureClickable::getButton() const {
    return mButton;
}

void LTextureClickable::setRenderDecorators(bool render) {
    mRenderDecorators = render;
}

void LTextureClickable::renderDecorators() {
    if(mRenderDecorators) {
        for(const auto& decorator : mDecorators) {
            decorator->render();
        }
    }
}