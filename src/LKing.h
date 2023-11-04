/*
LKing.h
by Valentin
--------------
Prototypes and const for LKing piece object

*/
#ifndef HEADER_LKING
#define HEADER_LKING

#include "headers.h"
#include "LPiece.h"
#include "vector"
#include "check.h"

class LKing : public LPiece {
public:
	//Fill vectors to provide possible moves to LBoard
	void blackMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool kingMoved, bool rook1Moved, bool rook2Moved, int checkStatus, bool *canCastle);
	void whiteMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool kingMoved, bool rook1Moved, bool rook2Moved, int checkStatus, bool *canCastle);
protected:
private:
	
};

#endif 