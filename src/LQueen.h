/*
LQueen.h
by Valentin
--------------
Prototypes and const for LQueen piece object

*/
#ifndef HEADER_LQUEEN
#define HEADER_LQUEEN

#include "headers.h"
#include "LPiece.h"
#include "vector"
#include "LBishop.h"
#include "LRook.h"


class LQueen : public LPiece {
public:
	//Fill vectors to provide possible moves to LBoard
	void blackMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);
	void whiteMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);
protected:
private:
	LRook mRook;
	LBishop mBishop;
	
};

#endif 