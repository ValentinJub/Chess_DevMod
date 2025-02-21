/*
LKnight.h
by Valentin
--------------
Prototypes and const for LKing piece object

*/
#ifndef HEADER_LKNIGHT
#define HEADER_LKNIGHT

#include "game/pieces/util.h"
#include "LPiece.h"
#include "vector"

class LKnight : public LPiece {
public:
	//Fill vectors to provide possible moves to LBoard
	void blackMove(const int map[SPL][SPL], SDL_Point knightPos, std::vector<SDL_Point> legalPos);
	void whiteMove(const int map[SPL][SPL], SDL_Point knightPos, std::vector<SDL_Point> legalPos);

	std::vector<SDL_Point> moves(std::array<std::array<int, SPL>, SPL> map, SDL_Point knightPos);
protected:
private:
	
};

#endif 