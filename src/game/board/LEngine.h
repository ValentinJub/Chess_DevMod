#ifndef LEngine_H
#define LEngine_H

#include "com/headers.h"
#include "com/constantes.h"
#include "game/pieces/util.h"
#include "game/pieces/LKing.h"
#include "game/pieces/LBishop.h"
#include "game/pieces/LRook.h"
#include "game/pieces/LKnight.h"
#include "game/pieces/LPawn.h"
#include "game/pieces/LQueen.h"

class LEngine {
    public:
        LEngine() {};
        ~LEngine() {};

        bool isMoveSelfCheck(const std::array<std::array<int, SPL>, SPL>& board, SDL_Point src, SDL_Point dst, bool isWhite);
        bool isKingInCheck(const std::array<std::array<int, SPL>, SPL>& board, bool isWhite);
        bool isCheckMate(const std::array<std::array<int, SPL>, SPL>& board, bool isWhite);
        std::vector<SDL_Point> getLegalMoves(std::array<std::array<int, SPL>, SPL> board, SDL_Point piecePos);
        private:

        bool isPieceAttacked(const std::array<std::array<int, SPL>, SPL>& board, SDL_Point attackerPos, SDL_Point defenderPos, bool selfCheck = false);
        SDL_Point findKing(const std::array<std::array<int, SPL>, SPL>& board, bool isWhite);

        // To access the moves function of each piece
        LKing mKing;
        LBishop mBishop;
        LRook mRook;
        LKnight mKnight;
        LPawn mPawn;
        LQueen mQueen;
};

#endif