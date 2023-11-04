/*
LKnight.h
by Valentin
--------------
Prototypes and const for LKing piece object

*/
#ifndef HEADER_LKNIGHT
#define HEADER_LKNIGHT

#include "headers.h"
#include "LPiece.h"
#include "vector"

class LKnight : public LPiece {
public:
	//Fill vectors to provide possible moves to LBoard
	void blackMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);
	void whiteMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);
protected:
private:
	
};

#endif 