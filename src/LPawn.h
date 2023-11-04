/*
LPawn.h
by Valentin
--------------
Prototypes and const for LPawn piece object

*/
#ifndef HEADER_LPAWN
#define HEADER_LPAWN

#include "headers.h"
#include "LPiece.h"
#include "vector"

class LPawn : public LPiece {
public:
	//Fill vectors to provide possible moves to LBoard
	void blackMoveOld(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, int mLastMovedPiece, int mLastMovedPieceXSrc, int mLastMovedPieceYSrc, int mLastMovedPieceXDes, int mLastMovedPieceYDes, bool *mEnPassant, bool preCheck = false);
	void blackMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool preCheck = false);
	void whiteMoveOld(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY,int mLastMovedPiece, int mLastMovedPieceXSrc, int mLastMovedPieceYSrc, int mLastMovedPieceXDes, int mLastMovedPieceYDes, bool *mEnPassant, bool preCheck = false);
	void whiteMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool preCheck = false);
protected:
private:
	
};

#endif 