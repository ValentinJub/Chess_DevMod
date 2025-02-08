#include "LTexture.h"

using std::string;

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont64;

LTexture::LTexture(int x, int y, int w, int h, SDL_Texture* texture) :
        mX(x),
        mY(y),
        mWidth(w),
        mHeight(h),
        mTexture(texture)
    {
        mVelX = 0;
        mVelY = 0;
    }

LTexture::~LTexture() {
    free();
}

void LTexture::createImg(SDL_Texture* texture) {
    mTexture = texture;
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    // set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight };

    // set clip rendering dimensions
    if(clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // render to screen
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
    this->renderDecorators();
}

// Render the texture at its current position
void LTexture::renderAuto() {
    this->render(mX, mY);
}

std::vector<std::unique_ptr<LDecorator>>& LTexture::getDecorators() {
    return mDecorators;
}

void LTexture::free() {
    // The responsibility of freeing the texture pointer is left to the factory
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}
        
void LTexture::setAlpha(Uint8 alpha) {
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::addDecorator(LDecorator* decorator) {
    mDecorators.push_back(std::unique_ptr<LDecorator>(decorator));
}

void LTexture::removeDecorator(LDecorator* decorator) {
    mDecorators.erase(std::remove_if(mDecorators.begin(), mDecorators.end(),
        [decorator](const std::unique_ptr<LDecorator>& ptr) { return ptr.get() == decorator; }), mDecorators.end());
}

void LTexture::renderDecorators() {
    for(auto& decorator : mDecorators) {
        decorator->render();
    }
}

SDL_Texture* LTexture::getTexture() {
    return mTexture;
}

int LTexture::w() {
    return mWidth;
}

int LTexture::h() {
    return mHeight;
}

int LTexture::x() {
    return mX;
}

int LTexture::y() {
    return mY;
}

void LTexture::setPos(int x, int y) {
    this->setX(x);
    this->setY(y);
}

void LTexture::setX(int x) {
    mX = x;
}

void LTexture::setY(int y) {
    mY = y;
}