#include "game/board/LMoveHistory.h"
#include "factories/LMediaFactory.h"

extern SDL_Renderer* gRenderer;
extern LMediaFactory* gMediaFactory;
extern TTF_Font* gFont16;

LMoveHistory::LMoveHistory(SDL_Rect* rect, SDL_Color color) : mRect(rect), mColor(color) {

}

LMoveHistory::~LMoveHistory() {
    this->free();
}

void LMoveHistory::render() {
    // Render the rectangle
    SDL_SetRenderDrawColor(gRenderer, mColor.r, mColor.g, mColor.b, mColor.a);
    SDL_RenderFillRect(gRenderer, mRect);
    // Render the moves
    int y = mRect->y;
    for (std::string move : mMoves) {
        LTexture* txt = gMediaFactory->getTxt(move, gFont16, COLOR_WHITE);
        txt->renderAt(mRect->x + 10, y);
        y += txt->h();
    }
}

void LMoveHistory::free() {
    delete mRect;
}

void LMoveHistory::addMove(std::string move) {
    mMoves.push_back(move);
}

