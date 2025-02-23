#ifndef LTILEBORDER_H
#define LTILEBORDER_H

#include "utils/util.h"
#include "com/LDecorator.h"

/*

LTileBorder is a class that creates a border around the tile

*/

class LTileBorder : public LDecorator {
    public:
        LTileBorder(SDL_Point a, SDL_Point b, int w, int h, int thickness = 2);
        ~LTileBorder();
        void render() override;
        void free() override;
    private:
        SDL_Point   mA,
                    mB;
        int mW,
            mH,
            mThickness;
};

#endif // LDECORATOR_H