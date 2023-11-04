#include "LKing.h"

void LKing::blackMove(const int map[SPL][SPL], int piecePosX, int piecePosY, std::vector<int> *highlightedTileXPos, std::vector<int> *highlightedTileYPos, bool kingMoved, bool rook1Moved, bool rook2Moved, int checkStatus, bool *canCastle) {
	//BKING pos on map
	int x = piecePosX;
	int y = piecePosY;
	int piece = BKING;
	//possible moves arrays
	int posX[TOTAL_POS] = {0};
	int posY[TOTAL_POS] = {0};
	
	posY[U] = y - 1; 
	posX[U] = x; 
	
	posY[UR] = y - 1; 
	posX[UR] = x + 1; 
	
	posY[R] = y; 
	posX[R] = x + 1; 
	
	posY[DR] = y + 1; 
	posX[DR] = x + 1; 
	
	posY[D] = y + 1; 
	posX[D] = x; 
	
	posY[DL] = y + 1; 
	posX[DL] = x - 1; 
	
	posY[L] = y; 
	posX[L] = x - 1; 
	
	posY[UL] = y - 1; 
	posX[UL] = x - 1; 
	
	//Castling
	if((checkStatus == NO_CHECK) || (checkStatus == WCHECK)) {
		if(!(kingMoved)) {
			if(!(rook1Moved)) {
				if((map[0][1] == EMPTY) && (map[0][2] == EMPTY) && (map[0][3] == EMPTY)) {
					if(!(isKingChecked(map, piece, piecePosX - 1, piecePosY, piecePosX, piecePosY, false))) {
						if(!(isKingChecked(map, piece, piecePosX - 2, piecePosY, piecePosX, piecePosY, false))) {
							highlightedTileXPos->push_back(2);
							highlightedTileYPos->push_back(0);
							*canCastle = true;
						}
					}
				}
			}
			if(!(rook2Moved)) {
				if((map[0][5] == EMPTY) && (map[0][6] == EMPTY)) {
					if(!(isKingChecked(map, piece, piecePosX + 1, piecePosY, piecePosX, piecePosY, false))) {
						if(!(isKingChecked(map, piece, piecePosX + 2, piecePosY, piecePosX, piecePosY, false))) {
							highlightedTileXPos->push_back(6);
							highlightedTileYPos->push_back(0);
							*canCastle = true;
						}
					}
				}
			}
		}
	}
	
	std::vector<int> forbiddenTilePosX;
	std::vector<int> forbiddenTilePosY;
	
	lookForForbiddenSquare(map, &forbiddenTilePosX, &forbiddenTilePosY, false);
	
	for(int i(0); i < TOTAL_POS; i++) {
		//if move is in bound
		if((posY[i] >= 0) && (posY[i] < SPL)) {
			if((posX[i] >= 0) && (posX[i] < SPL)) {
				if((map[posY[i]][posX[i]] == WBISHOP) || (map[posY[i]][posX[i]] > WKING)) {
					bool dont = false;
					for(int p(0); p < TOTAL_POS; p++) {
						if((forbiddenTilePosX[p] == posX[i]) && (forbiddenTilePosY[p] == posY[i])) {
							dont = true; 
						}
						//end of forbiden loop
						if((p == TOTAL_POS - 1) && (!(dont))) {
							highlightedTileXPos->push_back(posX[i]);
							highlightedTileYPos->push_back(posY[i]);
						}
					}
				}
			}
		}
	}
}

void LKing::whiteMove(const int map[SPL][SPL], int piecePosX, int piecePosY, std::vector<int> *highlightedTileXPos, std::vector<int> *highlightedTileYPos, bool kingMoved, bool rook1Moved, bool rook2Moved, int checkStatus, bool *canCastle) {
	//WKING pos on map
	int x = piecePosX;
	int y = piecePosY;
	
	int posX[TOTAL_POS] = {0};
	int posY[TOTAL_POS] = {0};
	
	posY[U] = y - 1; 
	posX[U] = x; 
	
	posY[UR] = y - 1; 
	posX[UR] = x + 1; 
	
	posY[R] = y; 
	posX[R] = x + 1; 
	
	posY[DR] = y + 1; 
	posX[DR] = x + 1; 
	
	posY[D] = y + 1; 
	posX[D] = x; 
	
	posY[DL] = y + 1; 
	posX[DL] = x - 1; 
	
	posY[L] = y; 
	posX[L] = x - 1; 
	
	posY[UL] = y - 1; 
	posX[UL] = x - 1; 
	
	//Castling
	if((checkStatus == NO_CHECK) || (checkStatus == BCHECK))  {
		if(!(kingMoved)) {
			if(!(rook1Moved)) {
				if((map[7][1] == EMPTY) && (map[7][2] == EMPTY) && (map[7][3] == EMPTY)) {
					highlightedTileXPos->push_back(2);
					highlightedTileYPos->push_back(7);
				}
			}
			if(!(rook2Moved)) {
				if((map[7][5] == EMPTY) && (map[7][6] == EMPTY)) {
					highlightedTileXPos->push_back(6);
					highlightedTileYPos->push_back(7);
				}
			}
		}
	}
	
	std::vector<int> forbiddenTilePosX;
	std::vector<int> forbiddenTilePosY;
	
	lookForForbiddenSquare(map, &forbiddenTilePosX, &forbiddenTilePosY, true);
	
	for(int i(0); i < TOTAL_POS; i++) {
		//if move is in bound
		if((posY[i] >= 0) && (posY[i] < SPL)) {
			if((posX[i] >= 0) && (posX[i] < SPL)) {
				if((map[posY[i]][posX[i]] == EMPTY) || (map[posY[i]][posX[i]] == BBISHOP) || ((map[posY[i]][posX[i]] > BKING) && (map[posY[i]][posX[i]] < WBISHOP))) {
					bool dont = false;
					for(int p(0); p < TOTAL_POS; p++) {
						if((forbiddenTilePosX[p] == posX[i]) && (forbiddenTilePosY[p] == posY[i])) {
							dont = true; 
						}
						//end of forbiden loop
						if((p == TOTAL_POS - 1) && (!(dont))) {
							highlightedTileXPos->push_back(posX[i]);
							highlightedTileYPos->push_back(posY[i]);
						}
					}
				}
			}
		}
	}
}
	