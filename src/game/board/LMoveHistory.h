#ifndef LMOVEHISTORY_H
#define LMOVEHISTORY_H

#include "graphics/LVisualComponent.h"
#include "com/headers.h"
#include <vector>
#include <string>

class LMoveHistory : public LVisualComponent {
    private:
        SDL_Rect* mRect; // The rectangle that will be rendered
        SDL_Color mColor; // The color of the rectangle
        std::vector<std::string> mMoves; // The moves that will be displayed
    protected:
    public:
        LMoveHistory(SDL_Rect* rect, SDL_Color color);
        ~LMoveHistory();
        void render();
        void free();
        void addMove(std::string move);
};

#endif