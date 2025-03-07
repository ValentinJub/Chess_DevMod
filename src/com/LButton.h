/*
LButtons.h
by Valentin
--------------
Prototypes and const for LButtons class

*/
#ifndef LBUTTON_H
#define LBUTTON_H

#include "utils/util.h"


//The mouse button
class LButton
{
    public:
        LButton(int x = 0, int y = 0, int w = 0, int h = 0);
        //Sets top left position
        void setPos( int x, int y );
        void setSize(int width, int height);

        //Handles mouse event
        bool isHovered();
        bool isClicked( SDL_Event* e );
        
        int getX() const;
        int getY() const;
        int getW() const;
        int getH() const;

    private:
        //Top left position
        SDL_Point mPosition;
        int mWidth,
            mHeight;

};

#endif
