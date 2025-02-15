#include "graphics/LDot.h"
#include "factories/LMediaFactory.h"

extern LMediaFactory* gMediaFactory;

LDot::LDot(int x, int y) : mX(x), mY(y) {
    mVelX = 0;
    mVelY = 0;
    mIsMoving = false;
    mTexture = gMediaFactory->getImg(SPRITE_DOT);
    mClip = new SDL_Rect{29, 28, DOT_WIDTH, DOT_HEIGHT};
    mTexture->setClip(mClip);
}

LDot::~LDot() {
    this->free();
}

void LDot::setPos(int x, int y) {
    mX = x;
    mY = y;
}

SDL_Point LDot::getDotPosition() const {
    return SDL_Point{mX, mY};
}

void LDot::handleEvent(SDL_Event& e) {
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            // case SDLK_UP: mVelY -= DOT_VEL; break;
            // case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            // case SDLK_UP: mVelY += DOT_VEL; break;
            // case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void LDot::move() {
    //Move the dot left or right
    mX += mVelX;

    //If the dot went too far to the left or right
    if( ( mX < 0 ) || ( mX + DOT_WIDTH > SCREEN_WIDTH ) )
    {
        //Move back
        mX -= mVelX;
    }
    //Move the dot up or down
    mY += mVelY;

    //If the dot went too far up or down
    if( ( mY < 0 ) || ( mY + DOT_HEIGHT > SCREEN_HEIGHT ) )
    {
        //Move back
        mY -= mVelY;
    }
}

void LDot::free() {}

void LDot::render() {
    mTexture->setPos(mX, mY);
    mTexture->render();
}

void LDot::setMoving(bool moving) {
    mIsMoving = moving;
}

bool LDot::isMoving() const {
    return mIsMoving;
}

