/*
LButton.cpp
by Valentin
--------------
Methods for LButtons class

*/

#include "LButton.h"

LButton::LButton(int x, int y, int w, int h) {
    mPosition.x = x;
    mPosition.y = y;
    mWidth = w;
    mHeight = h;
}

void LButton::setPos( int x, int y ) {
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::setSize(int width, int height) {
    mWidth = width;
    mHeight = height;
}

bool LButton::isHovered() {
    int x, y;
    SDL_GetMouseState( &x, &y );
    return x > mPosition.x && x > mPosition.x + mWidth && y > mPosition.y && y < mPosition.y + mHeight;
}

bool LButton::isClicked(SDL_Event* e) {
    bool success = false;
        switch( e->type )
        {
            case SDL_MOUSEBUTTONUP:
            if(e->button.button == SDL_BUTTON_LEFT) {
                success = true;
            }
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
