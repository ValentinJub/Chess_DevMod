#ifndef LBORDERDECORATOR_H
#define LBORDERDECORATOR_H

#include "utils/util.h"
#include "com/LDecorator.h"

class LBorderDecorator : public LDecorator {
    public:
        LBorderDecorator(SDL_Point a, SDL_Point b, SDL_Point c, SDL_Point d, int w, int h);
        ~LBorderDecorator() {}
        void render();
        void free();
    private:
        SDL_Point   mA,
                    mB,
                    mC,
                    mD;
        int mWidth,
            mHeight;
};

#endif // LDECORATOR_H