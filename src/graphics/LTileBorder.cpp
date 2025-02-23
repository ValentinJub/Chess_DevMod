#include "graphics/LTileBorder.h"

extern SDL_Renderer* gRenderer;

LTileBorder::LTileBorder(SDL_Point a, SDL_Point b, int w, int h, int thickness) 
    : mA(a), mB(b), mW(w), mH(h), mThickness(thickness) {
}

void LTileBorder::free() {
    // nothing to free
}

LTileBorder::~LTileBorder() {
    this->free();
}

void LTileBorder::render() {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
    int length = (mW < mH) ? mW / 4 : mH / 4;
    for(int i(0); i < mThickness; i++) {
        // draw the lines from point TL horizontally
        SDL_RenderDrawLine(
            gRenderer, 
            mA.x + i, 
            mA.y + i, 
            mA.x + mW - i, 
            mA.y + i
        ); 
        // draw the lines from point TL vertically
        SDL_RenderDrawLine(
            gRenderer, 
            mA.x + i, 
            mA.y + i, 
            mA.x + i, 
            mA.y + mH - i
        );
        // draw the lines from point BR horizontally
        SDL_RenderDrawLine(
            gRenderer, 
            mB.x - i, 
            mB.y - i, 
            mB.x - mW + i, 
            mB.y - i
        );
        // draw the lines from point BR vertically
        SDL_RenderDrawLine(
            gRenderer, 
            mB.x - i, 
            mB.y - i, 
            mB.x - i,
            mB.y - mH + i
        );
    }
}