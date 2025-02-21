/*
LKing.h
by Valentin
--------------
Prototypes and const for LKing piece object

*/
#ifndef HEADER_LKING
#define HEADER_LKING

#include "game/pieces/util.h"
#include "com/constantes.h"
#include "LPiece.h"
#include "vector"
#include "check.h"

class LKing : public LPiece {
public:
	std::vector<SDL_Point> moves(std::array<std::array<int, SPL>, SPL> map, SDL_Point kingPos);
	//Fill vectors to provide possible moves to LBoard
	void blackMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool kingMoved, bool rook1Moved, bool rook2Moved, int checkStatus, bool *canCastle);
	void whiteMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool kingMoved, bool rook1Moved, bool rook2Moved, int checkStatus, bool *canCastle);
protected:
private:
	
};

#endif 