#include "LBishop.h"

//Fill vectors to provide possible moves to LBoard
void LBishop::blackMove(const int map[SPL][SPL], int x, int y, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
	
	const int maxDiagMove = 7;
	
	//7 possible diagonal moves
	//4 diagonal directions
	//UR Up Right UL Up Left
	//DR Down Right DL Down Left
	
	int posYUR[maxDiagMove] = {0};
	int posXUR[maxDiagMove] = {0};
	
	for(int i(0); i < maxDiagMove; i++) {
		posYUR[i] = y - i - 1;
		posXUR[i] = x + i + 1;
	}
	
	int posYDR[maxDiagMove] = {0};
	int posXDR[maxDiagMove] = {0};
	
	for(int i(0); i < maxDiagMove; i++) {
		posYDR[i] = y + i + 1;
		posXDR[i] = x + i + 1;
	}
	
	int posYUL[maxDiagMove] = {0};
	int posXUL[maxDiagMove] = {0};
	
	for(int i(0); i < maxDiagMove; i++) {
		posYUL[i] = y - i - 1;
		posXUL[i] = x - i - 1;
	}
	
	int posYDL[maxDiagMove] = {0};
	int posXDL[maxDiagMove] = {0};
	
	for(int i(0); i < maxDiagMove; i++) {
		posYDL[i] = y + i + 1;
		posXDL[i] = x - i - 1;
	}
	
	//True when an obstacle is met
	//1 for each diagonale
	bool stop1 = false;
	bool stop2 = false;
	bool stop3 = false;
	bool stop4 = false;
	
	for(int i(0); i < maxDiagMove; i++) {
		//if move is in bound
		if((posYUR[i] >= 0) && (posYUR[i] < SPL)) {
			if((posXUR[i] >= 0) && (posXUR[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop1)) {
					//if move is empty
					if(map[posYUR[i]][posXUR[i]] == EMPTY) {
						tilePosX->push_back(posXUR[i]);
						tilePosY->push_back(posYUR[i]);
					}
					//if move is white piece -> obstacle
					else if((map[posYUR[i]][posXUR[i]] >= WBISHOP) && (map[posYUR[i]][posXUR[i]] < EMPTY)) {
						tilePosX->push_back(posXUR[i]);
						tilePosY->push_back(posYUR[i]);
						stop1 = true;
					}
					//if move is black piece -> obstacle
					else if(map[posYUR[i]][posXUR[i]] < WBISHOP) {
						stop1 = true;
					}
				}
			}
		}
		if((posYDR[i] >= 0) && (posYDR[i] < SPL)) {
			if((posXDR[i] >= 0) && (posXDR[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop2)) {
					//if possible move is empty
					if(map[posYDR[i]][posXDR[i]] == EMPTY) {
						tilePosX->push_back(posXDR[i]);
						tilePosY->push_back(posYDR[i]);
					}
					//if possible move is white piece -> obstacle
					else if((map[posYDR[i]][posXDR[i]] >= WBISHOP) && (map[posYDR[i]][posXDR[i]] < EMPTY)) {
						tilePosX->push_back(posXDR[i]);
						tilePosY->push_back(posYDR[i]);
						stop2 = true;
					}
					//if possible move is black -> obstacle
					else if(map[posYDR[i]][posXDR[i]] < WBISHOP) {
						stop2 = true;
					}
				}
			}
		}
		if((posYDL[i] >= 0) && (posYDL[i] < SPL)) {
			if((posXDL[i] >= 0) && (posXDL[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop3)) {
					//if move is empty
					if(map[posYDL[i]][posXDL[i]] == EMPTY) {
						tilePosX->push_back(posXDL[i]);
						tilePosY->push_back(posYDL[i]);
					}
					//if move is white piece -> obstacle
					else if((map[posYDL[i]][posXDL[i]] >= WBISHOP) && (map[posYDL[i]][posXDL[i]] < EMPTY)) {
						tilePosX->push_back(posXDL[i]);
						tilePosY->push_back(posYDL[i]);
						stop3 = true;
					}
					//if move is black -> obstacle
					else if(map[posYDL[i]][posXDL[i]] < WBISHOP) {
						stop3 = true;
					}
				}
			}
		}
		if((posYUL[i] >= 0) && (posYUL[i] < SPL)) {
			if((posXUL[i] >= 0) && (posXUL[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop4)) {
					//if move is empty
					if(map[posYUL[i]][posXUL[i]] == EMPTY) {
						tilePosX->push_back(posXUL[i]);
						tilePosY->push_back(posYUL[i]);
					}
					//if move is white piece -> obstacle
					else if((map[posYUL[i]][posXUL[i]] >= WBISHOP) && (map[posYUL[i]][posXUL[i]] < EMPTY)) {
						tilePosX->push_back(posXUL[i]);
						tilePosY->push_back(posYUL[i]);
						stop4 = true;
					}
					//if move is black -> obstacle
					else if(map[posYUL[i]][posXUL[i]] < WBISHOP) {
						stop4 = true;
					}
				}
			}
		}
	}
}
void LBishop::whiteMove(const int map[SPL][SPL], int x, int y, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {

	const int maxDiagMove = 7;
	
	//7 possible diagonal moves
	//4 diagonal directions
	//UR Up Right UL Up Left
	//DR Down Right DL Down Left
	
	int posYUR[maxDiagMove] = {0};
	int posXUR[maxDiagMove] = {0};
	
	for(int i(0); i < maxDiagMove; i++) {
		posYUR[i] = y - i - 1;
		posXUR[i] = x + i + 1;
	}
	
	int posYDR[maxDiagMove] = {0};
	int posXDR[maxDiagMove] = {0};
	
	for(int i(0); i < maxDiagMove; i++) {
		posYDR[i] = y + i + 1;
		posXDR[i] = x + i + 1;
	}
	
	int posYUL[maxDiagMove] = {0};
	int posXUL[maxDiagMove] = {0};
	
	for(int i(0); i < maxDiagMove; i++) {
		posYUL[i] = y - i - 1;
		posXUL[i] = x - i - 1;
	}
	
	int posYDL[maxDiagMove] = {0};
	int posXDL[maxDiagMove] = {0};
	
	for(int i(0); i < maxDiagMove; i++) {
		posYDL[i] = y + i + 1;
		posXDL[i] = x - i - 1;
	}
	
	//True when an obstacle is met
	//1 for each diagonale
	bool stop1 = false;
	bool stop2 = false;
	bool stop3 = false;
	bool stop4 = false;
	
	for(int i(0); i < maxDiagMove; i++) {
		//if move is in bound
		if((posYUR[i] >= 0) && (posYUR[i] < SPL)) {
			if((posXUR[i] >= 0) && (posXUR[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop1)) {
					//if move is empty
					if(map[posYUR[i]][posXUR[i]] == EMPTY) {
						tilePosX->push_back(posXUR[i]);
						tilePosY->push_back(posYUR[i]);
					}
					//if move is black piece -> can take but obstacle
					else if(map[posYUR[i]][posXUR[i]] < WBISHOP) {
						tilePosX->push_back(posXUR[i]);
						tilePosY->push_back(posYUR[i]);
						stop1 = true;
					}
					//if move is white piece -> obstacle
					else if((map[posYUR[i]][posXUR[i]] >= WBISHOP) && (map[posYUR[i]][posXUR[i]] < EMPTY)) {
						stop1 = true;
					}
				}
			}
		}
		if((posYDR[i] >= 0) && (posYDR[i] < SPL)) {
			if((posXDR[i] >= 0) && (posXDR[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop2)) {
					//if move is empty
					if(map[posYDR[i]][posXDR[i]] == EMPTY) {
						tilePosX->push_back(posXDR[i]);
						tilePosY->push_back(posYDR[i]);
					}
					//if move is black piece -> can take but -> obstacle
					else if(map[posYDR[i]][posXDR[i]] < WBISHOP) {
						tilePosX->push_back(posXDR[i]);
						tilePosY->push_back(posYDR[i]);
						stop2 = true;
					}
					//if move is white -> obstacle
					else if((map[posYDR[i]][posXDR[i]] >= WBISHOP) && (map[posYDR[i]][posXDR[i]] < EMPTY)) {
						stop2 = true;
					}
				}
			}
		}
		if((posYDL[i] >= 0) && (posYDL[i] < SPL)) {
			if((posXDL[i] >= 0) && (posXDL[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop3)) {
					//if move is empty
					if(map[posYDL[i]][posXDL[i]] == EMPTY) {
						tilePosX->push_back(posXDL[i]);
						tilePosY->push_back(posYDL[i]);
					}
					//if move is black piece -> can take obstacle
					else if(map[posYDL[i]][posXDL[i]] < WBISHOP) {
						tilePosX->push_back(posXDL[i]);
						tilePosY->push_back(posYDL[i]);
						stop3 = true;
					}
					//if move is white -> obstacle
					else if((map[posYDL[i]][posXDL[i]] >= WBISHOP) && (map[posYDL[i]][posXDL[i]] < EMPTY)) {
						stop3 = true;
					}
				}
			}
		}
		if((posYUL[i] >= 0) && (posYUL[i] < SPL)) {
			if((posXUL[i] >= 0) && (posXUL[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop4)) {
					//if move is empty
					if(map[posYUL[i]][posXUL[i]] == EMPTY) {
						tilePosX->push_back(posXUL[i]);
						tilePosY->push_back(posYUL[i]);
					}
					//if move is black piece can take -> obstacle
					else if(map[posYUL[i]][posXUL[i]] < WBISHOP) {
						tilePosX->push_back(posXUL[i]);
						tilePosY->push_back(posYUL[i]);
						stop4 = true;
					}
					//if move is white -> obstacle
					else if((map[posYUL[i]][posXUL[i]] >= WBISHOP) && (map[posYUL[i]][posXUL[i]] < EMPTY)) {
						stop4 = true;
					}
				}
			}
		}
	}
}