#ifndef LCHESS_LCOMPUTER_H
#define LCHESS_LCOMPUTER_H

#include "com/headers.h"
#include "com/constantes.h"

class LEngine;

struct ChessMove {
    SDL_Point src;
    SDL_Point dst;
    int piece;
    int capturedPiece;
    int score;
};

class LComputer {
    public:
        LComputer(LEngine* engine);
        ~LComputer();
        ChessMove play(std::array<std::array<int, SPL>, SPL> board, bool isWhite);
    private:
        LEngine* mEngine;
};

#endif