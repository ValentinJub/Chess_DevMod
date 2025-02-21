/*
LBishop.h
by Valentin
--------------
Prototypes and const for LBishop piece object

*/
#ifndef HEADER_LBISHOP
#define HEADER_LBISHOP

#include "game/pieces/util.h"
#include "LPiece.h"
#include "vector"

class LBishop : public LPiece {
public:
	//Fill vectors to provide possible moves to LBoard
	void blackMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);
	void whiteMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);

	std::vector<SDL_Point> moves(std::array<std::array<int, SPL>, SPL> map, SDL_Point bishopPos);
protected:
private:
	
};

#endif 