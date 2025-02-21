/*
LPawn.h
by Valentin
--------------
Prototypes and const for LPawn piece object

*/
#ifndef HEADER_LPAWN
#define HEADER_LPAWN

#include "game/pieces/util.h"
#include "LPiece.h"
#include "vector"

class LPawn : public LPiece {
public:
	// Ideal function:
	std::vector<SDL_Point> moves(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck = false);
	//Fill vectors to provide possible moves to LBoard
	void blackMoveOld(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, int mLastMovedPiece, int mLastMovedPieceXSrc, int mLastMovedPieceYSrc, int mLastMovedPieceXDes, int mLastMovedPieceYDes, bool *mEnPassant, bool preCheck = false);
	void whiteMoveOld(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY,int mLastMovedPiece, int mLastMovedPieceXSrc, int mLastMovedPieceYSrc, int mLastMovedPieceXDes, int mLastMovedPieceYDes, bool *mEnPassant, bool preCheck = false);
protected:
private:
	std::vector<SDL_Point> whiteMove(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck = false);
	std::vector<SDL_Point> blackMove(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck = false);
};

#endif 