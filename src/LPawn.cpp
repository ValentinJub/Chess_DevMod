#include "LPawn.h"

void LPawn::blackMoveOld(const int map[SPL][SPL], int piecePosX, int piecePosY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, int lastMovedPiece, int lastMovedPieceXSrc, int lastMovedPieceYSrc, int lastMovedPieceXDes, int lastMovedPieceYDes, bool *mEnPassant, bool preCheck) {
	//Possible forward move
	int forward = piecePosY + 1;
	//starting pos y1
	//If precheck is true it meens the forward squares arent included in the count
	//as they can't take another piece 
	if(!(preCheck)) {
		if((piecePosY == 1) && (map[forward][piecePosX] == EMPTY)) {
			tilePosX->push_back(piecePosX);
			tilePosY->push_back(forward);
			if(map[forward + 1][piecePosX] == EMPTY) {
				tilePosX->push_back(piecePosX);
				tilePosY->push_back(forward + 1);
			}
		}
		if(map[forward][piecePosX] == EMPTY) {
			tilePosX->push_back(piecePosX);
			tilePosY->push_back(forward);
		}
	}
	
	if(lastMovedPiece == WPAWN) {
		if((piecePosY == 4) && (lastMovedPieceYDes == piecePosY) && (lastMovedPieceYSrc - 2 == lastMovedPieceYDes)) {
			if(lastMovedPieceXDes - 1 == piecePosX) {
				tilePosX->push_back(piecePosX + 1);
				tilePosY->push_back(forward);
				*mEnPassant = true;
			}
			else if(lastMovedPieceXDes + 1 == piecePosX) {
				tilePosX->push_back(piecePosX - 1);
				tilePosY->push_back(forward);
				*mEnPassant = true;
			}
		}
	}
	
	//if cell down left is within bounds
	if((piecePosX - 1 >= 0) && (piecePosX - 1 < SPL)) {
		//if cell down left is enemy or empty
		if((map[forward][piecePosX - 1] >= WBISHOP) && (map[forward][piecePosX - 1] < EMPTY)) {
			tilePosX->push_back(piecePosX - 1);
			tilePosY->push_back(forward);
		}
	}
	//if cell down right is within bounds
	if((piecePosX + 1 >= 0) && (piecePosX + 1 < SPL)) {
	//if cell down right is enemy or empty
		if((map[forward][piecePosX + 1] >= WBISHOP) && (map[forward][piecePosX + 1] < EMPTY)) {
			tilePosX->push_back(piecePosX + 1);
			tilePosY->push_back(forward);
		}
	}
}

void LPawn::whiteMoveOld(const int map[SPL][SPL], int piecePosX, int piecePosY, std::vector<int> *tilePosX, std::vector<int> *tilePosY,  int lastMovedPiece, int lastMovedPieceXSrc, int lastMovedPieceYSrc, int lastMovedPieceXDes, int lastMovedPieceYDes, bool *mEnPassant, bool preCheck) {
	//Possible forward move
	int forward = piecePosY - 1;
	//starting pos y1
	//if not in a PollPreCheck fill the empty pos
	if(!(preCheck)) {
		if((piecePosY == 6) && (map[forward][piecePosX] == EMPTY)) {
			tilePosX->push_back(piecePosX);
			tilePosY->push_back(forward);
			if(map[forward - 1][piecePosX] == EMPTY) {
				tilePosX->push_back(piecePosX);
				tilePosY->push_back(forward - 1);
			}
		}
		//if forward is empty highlight cell 
		if(map[forward][piecePosX] == EMPTY) {
			tilePosX->push_back(piecePosX);
			tilePosY->push_back(forward);
		}
	}
	if(lastMovedPiece == BPAWN) {
		if((piecePosY == 3) && (lastMovedPieceYDes == piecePosY) && (lastMovedPieceYSrc + 2 == lastMovedPieceYDes)) {
			if(lastMovedPieceXDes - 1 == piecePosX) {
				tilePosX->push_back(piecePosX + 1);
				tilePosY->push_back(forward);
				*mEnPassant = true;
			}
			else if(lastMovedPieceXDes + 1 == piecePosX) {
				tilePosX->push_back(piecePosX - 1);
				tilePosY->push_back(forward);
				*mEnPassant = true;
			}
		}
	}
	//if pawn can take
	if((piecePosX - 1 >= 0) && (piecePosX - 1 < SPL)) {
		if(map[forward][piecePosX - 1] < WBISHOP) {
			tilePosX->push_back(piecePosX - 1);
			tilePosY->push_back(forward);
		}
	}
	if((piecePosX + 1 >= 0) && (piecePosX + 1 < SPL)) {
		if(map[forward][piecePosX + 1] < WBISHOP) {
			tilePosX->push_back(piecePosX + 1);
			tilePosY->push_back(forward);
		}
	}
}
void LPawn::blackMove(const int map[SPL][SPL], int piecePosX, int piecePosY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool preCheck) {
	//Possible forward move
	int forward = piecePosY + 1;
	//starting pos y1
	//If precheck is true it meens the forward squares arent included in the count
	//as they can't take another piece 
	if(!(preCheck)) {
		if((piecePosY == 1) && (map[forward][piecePosX] == EMPTY)) {
			tilePosX->push_back(piecePosX);
			tilePosY->push_back(forward);
			if(map[forward + 1][piecePosX] == EMPTY) {
				tilePosX->push_back(piecePosX);
				tilePosY->push_back(forward + 1);
			}
		}
		if(map[forward][piecePosX] == EMPTY) {
			tilePosX->push_back(piecePosX);
			tilePosY->push_back(forward);
		}
	}
	//if cell down left is enemy and within bounds	
	if((piecePosX - 1 >= 0) && (piecePosX - 1 < 8)) {
		if((map[forward][piecePosX - 1] >= WBISHOP) && (map[forward][piecePosX - 1] < EMPTY)) {
			tilePosX->push_back(piecePosX - 1);
			tilePosY->push_back(forward);
		}
	}
	//if cell down right is enemy and within bounds
	if((piecePosX + 1 >= 0) && (piecePosX + 1 < 8)) {
		if((map[forward][piecePosX + 1] >= WBISHOP) && (map[forward][piecePosX + 1] < EMPTY)) {
			tilePosX->push_back(piecePosX + 1);
			tilePosY->push_back(forward);
		}
	}
}

void LPawn::whiteMove(const int map[SPL][SPL], int piecePosX, int piecePosY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool preCheck) {
	//Possible forward move
	int forward = piecePosY - 1;
	//starting pos y1
	//if not in a PollPreCheck fill the empty pos
	if(!(preCheck)) {
		if((piecePosY == 6) && (map[forward][piecePosX] == EMPTY)) {
			tilePosX->push_back(piecePosX);
			tilePosY->push_back(forward);
			if(map[forward - 1][piecePosX] == EMPTY) {
				tilePosX->push_back(piecePosX);
				tilePosY->push_back(forward - 1);
			}
		}
		//if forward is empty highlight cell 
		if(map[forward][piecePosX] == EMPTY) {
			tilePosX->push_back(piecePosX);
			tilePosY->push_back(forward);
		}
	}
	//if pawn can take
	if((piecePosX - 1 >= 0) && (piecePosX - 1 < 8)) {
		if(map[forward][piecePosX - 1] < WBISHOP) {
			tilePosX->push_back(piecePosX - 1);
			tilePosY->push_back(forward);
		}
	}
	if((piecePosX + 1 >= 0) && (piecePosX + 1 < 8)) {
		if(map[forward][piecePosX + 1] < WBISHOP) {
			tilePosX->push_back(piecePosX + 1);
			tilePosY->push_back(forward);
		}
	}
}