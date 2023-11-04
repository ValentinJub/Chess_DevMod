/*
LButtons.h
by Valentin
--------------
Prototypes and const for LButtons class

*/
#ifndef LBUTTON_H
#define LBUTTON_H

#include "headers.h"


//The mouse button
class LButton
{
    public:
        LButton();
        //Sets top left position
        void setPosition( int x, int y );
        void setWidthAndHeight(int width, int height);

        //Handles mouse event
        bool handleInside ( SDL_Event* e );
        bool handleClick( SDL_Event* e );
    
        //Shows button sprite
        void render();
        
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
