#include "com/LBorderDecorator.h"

extern SDL_Renderer* gRenderer;

LBorderDecorator::LBorderDecorator(SDL_Point a, SDL_Point b, SDL_Point c, SDL_Point d, int w, int h) : mA(a), mB(b), mC(c), mD(d), mWidth(w), mHeight(h) {}

void LBorderDecorator::free() {
    // nothing to free
}

void LBorderDecorator::render() {
    // we render the border in black
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    int offset = 2;
    int length = (mWidth < mHeight) ? mWidth / 4 : mHeight / 4;
    for(int j(0); j < offset; j++) {
        // draw the corners to smooth the angle
        if(j > 0) {
            SDL_RenderDrawPoint(gRenderer, mA.x - j, mA.y - j);
            SDL_RenderDrawPoint(gRenderer, mB.x + j, mB.y - j);
            SDL_RenderDrawPoint(gRenderer, mC.x - j, mC.y + j);
            SDL_RenderDrawPoint(gRenderer, mD.x + j, mD.y + j);
        }

        // draw the lines from point a horizontally
        SDL_RenderDrawLine(gRenderer, mA.x, mA.y - j, mA.x + length, mA.y - j); 
        // draw the lines from point a vertically
        SDL_RenderDrawLine(gRenderer, mA.x - j, mA.y, mA.x - j, mA.y + length);

        // draw the lines from point b horizontally
        SDL_RenderDrawLine(gRenderer, mB.x, mB.y - j, mB.x - length, mB.y - j);
        // draw the lines from point b vertically
        SDL_RenderDrawLine(gRenderer, mB.x + j, mB.y, mB.x + j, mB.y + length);

        // draw the lines from point c horizontally
        SDL_RenderDrawLine(gRenderer, mC.x, mC.y + j, mC.x + length, mC.y + j);
        // draw the lines from point c vertically
        SDL_RenderDrawLine(gRenderer, mC.x - j, mC.y, mC.x - j, mC.y - length);

        // draw the lines from point d horizontally
        SDL_RenderDrawLine(gRenderer, mD.x, mD.y + j, mD.x - length, mD.y + j);
        // draw the lines from point d vertically
        SDL_RenderDrawLine(gRenderer, mD.x + j, mD.y, mD.x + j, mD.y - length);

    }
}