/*
LQueen.h
by Valentin
--------------
Prototypes and const for LQueen piece object

*/
#ifndef HEADER_LQUEEN
#define HEADER_LQUEEN

#include "game/pieces/util.h"
#include "LPiece.h"
#include "vector"
#include "game/pieces/LBishop.h"
#include "game/pieces/LRook.h"


class LQueen : public LPiece {
public:
	std::vector<SDL_Point> moves(std::array<std::array<int, SPL>, SPL> map, SDL_Point queenPos);
	//Fill vectors to provide possible moves to LBoard
	void blackMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);
	void whiteMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);
protected:
private:
	LRook mRook;
	LBishop mBishop;
};

#endif 