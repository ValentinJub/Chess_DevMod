#include "graphics/LPromotion.h"
#include "factories/LMediaFactory.h"
#include "states/LStateMachine.h"

extern SDL_Renderer* gRenderer;
extern LMediaFactory* gMediaFactory;
extern LStateMachine* gStateMachine;

const std::array<int, 4> WhitePiece = {WBISHOP, WKNIGHT, WQUEEN, WROOK};
const std::array<int, 4> BlackPiece = {BBISHOP, BKNIGHT, BQUEEN, BROOK};

LPromotion::LPromotion(bool isWhite, int column, const char* pieceType, LObserver* observer) : mIsWhite(isWhite), mBoardObs(observer) {
    this->Attach(observer);
    int x = column * TOTAL_SQUARES + (4 * TOTAL_SQUARES) < SCREEN_WIDTH ? column * TOTAL_SQUARES : SCREEN_WIDTH - (4 * TOTAL_SQUARES);
    x = column > 0 && column < 7 ? x - (TOTAL_SQUARES / 2) : x; // adjust x position to be centered to the piece we promote
    if(isWhite) {
        mRect = {x, 0, TOTAL_SQUARES * 4, TOTAL_SQUARES}; 
    } else {
        mRect = {x, SCREEN_HEIGHT - TOTAL_SQUARES, TOTAL_SQUARES * 4, TOTAL_SQUARES};
    }
    for(int i(0); i < 4; i++) {
        mTexture[i] = gMediaFactory->getImgClickable(pieceType);
        if(isWhite) {
            mTexture[i]->setClip(new SDL_Rect{WhitePiece[i] * TOTAL_SQUARES, 0, TOTAL_SQUARES, TOTAL_SQUARES});
        } else {
            mTexture[i]->setClip(new SDL_Rect{BlackPiece[i] * TOTAL_SQUARES, 0, TOTAL_SQUARES, TOTAL_SQUARES});
        }
        mTexture[i]->setPos(mRect.x + (i * TOTAL_SQUARES), mRect.y);
        mTexture[i]->setButton(new LButton(mRect.x + (i * TOTAL_SQUARES), mRect.y, TOTAL_SQUARES, TOTAL_SQUARES));
    }
}

void LPromotion::enter(LObserver* observer) {
    // do nothing;
}

void LPromotion::exit() {
    this->free();
}

void LPromotion::free() {
    this->Detach(mBoardObs);
    for(auto texture : mTexture) {
        texture->free();
    }
}

void LPromotion::render() {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x7F);
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(gRenderer, &mRect);
    for(auto texture : mTexture) {
        texture->render();
    }
}

void LPromotion::update(Uint64 dt) {
    SDL_Event e;
    while(SDL_PollEvent(&e) > 0) {
        for(int i(0); i < 4; i++) {
            if(mTexture[i]->getButton()->isHovered()) {
                // do something
                if(mTexture[i]->getButton()->isClicked(&e)) {
                    this->Notify(mIsWhite ? WhitePiece[i] : BlackPiece[i]);
                    gStateMachine->pop();
                    return;
                }
            }
        }
    }
}

