/*
LButton.cpp
by Valentin
--------------
Methods for LButtons class

*/

#include "LButton.h"

LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;
    mWidth = 0;
    mHeight = 0;

}

void LButton::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::setWidthAndHeight(int width, int height) {
    mWidth = width;
    mHeight = height;
}

bool LButton::handleInside( SDL_Event* e ) {
    bool success = false;
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        bool inside = true;

        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > mPosition.x + mWidth )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > mPosition.y + mHeight )
        {
            inside = false;
        }
        if(inside) success = true;
    }
        
   return success; 
}

bool LButton::handleClick(SDL_Event* e) {
    bool success = false;
        switch( e->type )
        {
            case SDL_MOUSEBUTTONUP:
            success = true;
            break;
        }
    return success;
}

int LButton::getX() const {
    return mPosition.x;
}

int LButton::getY() const {
    return mPosition.y;
}

int LButton::getH() const {
    return mHeight;
}

int LButton::getW() const {
    return mWidth;
}

void LButton::render() {
    /*Show current button sprite
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
    */
}
