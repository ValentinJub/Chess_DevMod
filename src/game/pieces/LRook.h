/*
LRook.h
by Valentin
--------------
Prototypes and const for LRook piece object

*/
#ifndef HEADER_LROOK
#define HEADER_LROOK

#include "game/pieces/util.h"
#include "LPiece.h"
#include "vector"

class LRook : public LPiece {
public:
	//Fill vectors to provide possible moves to LBoard
	void blackMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);
	void whiteMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);

	std::vector<SDL_Point> moves(std::array<std::array<int, SPL>, SPL> map, SDL_Point rookPos);
protected:
private:
	
};

#endif 