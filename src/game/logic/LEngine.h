#ifndef LEngine_H
#define LEngine_H

#include "com/headers.h"
#include "com/constantes.h"
#include "game/logic/util.h"

bool operator==(const SDL_Point& a, const SDL_Point& b);

class LEngine {
    public:
        LEngine() {};
        ~LEngine() {};

        bool isMoveSelfCheck(const std::array<std::array<int, SPL>, SPL>& board, SDL_Point src, SDL_Point dst, bool isWhite);
        bool isKingInCheck(const std::array<std::array<int, SPL>, SPL>& board, bool isWhite);
        bool isCheckMate(const std::array<std::array<int, SPL>, SPL>& board, bool isWhite);
        bool isPieceAttacked(const std::array<std::array<int, SPL>, SPL>& board, SDL_Point attackerPos, SDL_Point defenderPos, bool selfCheck = false);
        std::vector<SDL_Point> getLegalMoves(std::array<std::array<int, SPL>, SPL> board, SDL_Point piecePos);
        void setKingHasMoved(bool isWhite, bool hasMoved);
        void setRookHasMoved(bool isWhite, int rook, bool hasMoved);
        bool getKingHasMoved(bool isWhite);
        bool getRookHasMoved(bool isWhite, int rook);

        void setEnPassant(int piece, SDL_Point src, SDL_Point dest);
        int pieceValue(int const pieceType) const;
        private:

        SDL_Point findKing(const std::array<std::array<int, SPL>, SPL>& board, bool isWhite);
        std::vector<SDL_Point> findKnights(const std::array<std::array<int, SPL>, SPL>& board, bool isWhite);

        // Piece moves
        std::vector<SDL_Point> bishopMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point bishopPos);
        std::vector<SDL_Point> rookMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point rookPos);
        std::vector<SDL_Point> knightMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point knightPos);
        std::vector<SDL_Point> queenMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point queenPos);
        std::vector<SDL_Point> kingMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point kingPos, bool kingMoved, bool rook1Moved, bool rook2Moved);
        std::vector<SDL_Point> pawnMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck = false);
        std::vector<SDL_Point> wPawnMove(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck = false);
        std::vector<SDL_Point> bPawnMove(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck = false);

        // To castle we keep track of kings and rooks movements
        bool mWKingHasMoved = false;
        bool mBKingHasMoved = false;
        bool mWRook1HasMoved = false;
        bool mWRook2HasMoved = false;
        bool mBRook1HasMoved = false;
        bool mBRook2HasMoved = false;

        // For en passant
        int       mLastMovedPiece;
        SDL_Point mLastMovedPieceSrcPos;
        SDL_Point mLastMovedPieceDestPos;
};

#endif