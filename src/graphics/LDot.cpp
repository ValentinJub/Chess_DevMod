#include "graphics/LDot.h"

LDot::LDot()
{
    //Initialize the dot on the slider position
    mPosX = 0;
    mPosY = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //Initialize particles
    // for( int i = 0; i < TOTAL_PARTICLES; ++i )
    // {
    //     particles[ i ] = new LParticle( mPosX, mPosY );
    // }

    setMoving(false);
}

LDot::~LDot() {
    //Delete particles
    // for( int i = 0; i < TOTAL_PARTICLES; ++i )
    // {
    //     delete particles[ i ];
    // }
}

void LDot::setDotPosition( int x, int y ) {
    mPosX = x;
    mPosY = y;
}

SDL_Point LDot::getDotPosition() const {
    SDL_Point point = {mPosX, mPosY};
    return point;
}

void LDot::handleEvent( SDL_Event& e )
{
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

void LDot::move()
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
    }
    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void LDot::render(LTexture* texture) {
    texture->setPos(mPosX, mPosY);
    texture->render();
}

void LDot::setMoving(bool moving) {
    isMoving = moving;
}

bool LDot::getMoving() const {
    return isMoving;
}

