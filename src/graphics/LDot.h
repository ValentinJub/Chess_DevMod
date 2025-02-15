#include "utils/util.h"
#include "com/constantes.h"
#include "graphics/LTexture.h"

#ifndef LDOT_H
#define LDOT_H

//The dot that will move around on the screen
class LDot : public LVisualComponent {
    public:
        static const int DOT_WIDTH = 8;
        static const int DOT_HEIGHT = 8;
        //Maximum axis velocity of the dot
        static const int DOT_VEL = 5;
        LDot(int x = 0, int y = 0);
        ~LDot();
        void free() override;
        void setPos( int x, int y );
        void setMoving(bool moving);
        bool isMoving() const;
        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );
        void move();
        SDL_Point getDotPosition() const;
        void render() override;

    private:
        LTexture* mTexture;
        SDL_Rect* mClip;
        bool mIsMoving;
        int mX, mY;
        int mVelX, mVelY;
};

#endif