#include "utils/util.h"
#include "com/constantes.h"
#include "LTexture.h"

#ifndef LDOT_H
#define LDOT_H

//The dot that will move around on the screen
class LDot
{
    public:
        //The dimensions of the dot
        static const int DOT_WIDTH = 10;
        static const int DOT_HEIGHT = 10;

        //Maximum axis velocity of the dot
        static const int DOT_VEL = 5;

        //Initializes the variables
        LDot();
        //Deallocates particles
        ~LDot();

        void setDotPosition( int x, int y );

        void setMoving(bool moving);
        bool getMoving() const;

        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );

        //Moves the dot
        void move();

        SDL_Point getDotPosition() const;

        //Shows the dot on the screen
        void render(LTexture* texture);

    private:

        bool isMoving;

        //The X and Y offsets of the dot
        int mPosX, mPosY;

        //The velocity of the dot
        int mVelX, mVelY;
};

#endif