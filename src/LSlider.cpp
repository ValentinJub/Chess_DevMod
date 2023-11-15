/*
LSlider.cpp
by Valentin
--------------
Methods for LSlider class

*/

#include "LSlider.h"

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;

LSlider::LSlider(int width, int height, int posX, int posY) {
    setSliderWidthAndHeight(width, height);
    setSliderPosition(posX, posY);
    mDotTexture = new LTexture();
    loadDotTexture();
    mDot = new LDot();
    setDotPosition();
    setMouseFollow(false);  
    mVolumeTexture = new LTexture();
}

bool LSlider::loadDotTexture() {
    bool success = true;
    if(!mDotTexture->loadFromFile("sprites/dot.png")) {
        std::cerr << "Failed to load dot texture!" << std::endl;
        success = false;
    }
    return success;
}

LSlider::~LSlider() {
    delete mDot;
    mDot = NULL;
    mVolumeTexture->free();
    mVolumeTexture = NULL;
    mDotTexture->free();
    mDotTexture = NULL;
}

int LSlider::getVolume() const {
    if(mSliderWidth < 100) {
        float res = ((mDot->getDotPosition().x - mSliderPosition.x) / (mSliderWidth * 1.0)) * 100.0;
        return res;
    }
    else if(mSliderWidth == 100) return mDot->getDotPosition().x - mSliderPosition.x;
    else if(mSliderWidth > 100) {
        float coeff = (mSliderWidth / 100.0);
        return (mDot->getDotPosition().x - mSliderPosition.x) / coeff;
    }
}

// void LSlider::renderVolume() {
//     std::stringstream volumeText;
//     volumeText << "Volume: " << getVolume() << "%";
//     if(!mVolumeTexture->loadFromRenderedText(gFont, volumeText.str().c_str(), {0, 0, 0, 255})) {
//         std::cerr << "Failed to render volume text texture!" << std::endl;
//     }
//     mVolumeTexture->render(mSliderPosition.x, mSliderPosition.y - (mVolumeTexture->getHeight()));
// }

void LSlider::setSliderPosition( int x, int y ) {
    mSliderPosition.x = x;
    mSliderPosition.y = y;
}

void LSlider::setSliderWidthAndHeight(int width, int height) {
    mSliderWidth = width;
    mSliderHeight = height;
}

void LSlider::setDotPosition() {   
    int posX = mSliderPosition.x + (mSliderWidth/2);
    int posY = mSliderPosition.y + (mSliderHeight/2) - (LDot::DOT_HEIGHT/2);
    mDot->setDotPosition(posX, posY);
}

void LSlider::setMouseFollow(bool follow) {
    mMouseFollow = follow;
}

bool LSlider::getMouseFollow() const {
    return mMouseFollow;
}

// handle inside the slider, returns true if mouse is in/on the dot

bool LSlider::handleEvents( SDL_Event* e, SDL_Point mouse ) {

    bool inside = true;
    SDL_Point dotPosition = mDot->getDotPosition();

    if(e->type == SDL_MOUSEBUTTONDOWN) {
        if((e->button.button == SDL_BUTTON_LEFT)) {
            //Mouse is to the left of the button, it's outside
            if( mouse.x < dotPosition.x )
            {
                inside = false;
            }
            
            //Mouse is to the right of the button, it's outside
            else if( mouse.x > dotPosition.x + LDot::DOT_WIDTH )
            {
                inside = false;
            }
            //Mouse above the button
            else if( mouse.y < dotPosition.y )
            {
                inside = false;
            }
            //Mouse below the button
            else if( mouse.y > dotPosition.y + LDot::DOT_HEIGHT )
            {
                inside = false;
            }
            if(inside) {
                setMouseFollow(true);
                mDot->setMoving(true);
                return inside;
            }
            else {
                setMouseFollow(false);
                return inside;
            }  
        }
    }
    if( getMouseFollow() && e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT ) {
        setMouseFollow(false);
        mDot->setMoving(false);
        return false;
    }
    return inside;
}

void LSlider::handleMotion(SDL_Point mouse) {
    mDot->setDotPosition(mouse.x - LDot::DOT_WIDTH/2, mDot->getDotPosition().y);

    // left boudary
    if(mDot->getDotPosition().x < mSliderPosition.x) {
        mDot->setDotPosition(mSliderPosition.x, mDot->getDotPosition().y);
    }
    // right boundary
    else if(mDot->getDotPosition().x > mSliderPosition.x + mSliderWidth) {
        mDot->setDotPosition(mSliderPosition.x + mSliderWidth, mDot->getDotPosition().y);
    }
}



int LSlider::getX() const {
    return mSliderPosition.x;
}

int LSlider::getY() const {
    return mSliderPosition.y;
}

int LSlider::getH() const {
    return mSliderHeight;
}

int LSlider::getW() const {
    return mSliderWidth;
}

void LSlider::renderSlider() {
    // the rectangle that represents the slider
    SDL_Rect fillRect = { mSliderPosition.x, mSliderPosition.y, mSliderWidth, mSliderHeight };
    // is filled with white
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );     
    // and rendered   
    SDL_RenderFillRect( gRenderer, &fillRect );
    // the horizontal line in the middle of the slider is black
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    // draw a horizontal line in the middle of the slider, thickness 2
    int i(-1);
    while(i < 2) {
        SDL_RenderDrawLine( gRenderer, mSliderPosition.x, mSliderPosition.y + i + mSliderHeight/2, mSliderPosition.x + mSliderWidth, mSliderPosition.y + i + mSliderHeight/2 );
        i++;
    }
    // render the dot
    mDot->render(mDotTexture);
}
