#include "LRook.h"

void LRook::blackMove(const int map[SPL][SPL], int x, int y, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
	const int maxVertMove = 7;
	
	//7 possible vertical moves
	//4 vertical directions 
	
	int posYU[maxVertMove] = {0};
	int posXU[maxVertMove] = {0};
	
	for(int i(0); i < maxVertMove; i++) {
		posYU[i] = y - i - 1;
		posXU[i] = x;
	}
	
	int posYR[maxVertMove] = {0};
	int posXR[maxVertMove] = {0};
	
	for(int i(0); i < maxVertMove; i++) {
		posYR[i] = y;
		posXR[i] = x + i + 1;
	}
	
	int posYD[maxVertMove] = {0};
	int posXD[maxVertMove] = {0};
	
	for(int i(0); i < maxVertMove; i++) {
		posYD[i] = y + i + 1;
		posXD[i] = x;
	}
	
	int posYL[maxVertMove] = {0};
	int posXL[maxVertMove] = {0};
	
	for(int i(0); i < maxVertMove; i++) {
		posYL[i] = y;
		posXL[i] = x - i - 1;
	}
	
	//True when an obstacle is met
	//1 for each diagonale
	bool stop1 = false;
	bool stop2 = false;
	bool stop3 = false;
	bool stop4 = false;
	
	for(int i(0); i < maxVertMove; i++) {
		//if move is in bound
		if((posYU[i] >= 0) && (posYU[i] < SPL)) {
			if((posXU[i] >= 0) && (posXU[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop1)) {
					//if move is black piece -> obstacle
					if(map[posYU[i]][posXU[i]] < WBISHOP) {
						stop1 = true;
					}
					//if move is white piece -> obstacle
					else if((map[posYU[i]][posXU[i]] >= WBISHOP) && (map[posYU[i]][posXU[i]] < EMPTY)) {
						tilePosX->push_back(posXU[i]);
						tilePosY->push_back(posYU[i]);
						stop1 = true;
					}
					//if move is empty
					else if(map[posYU[i]][posXU[i]] == EMPTY) {
						tilePosX->push_back(posXU[i]);
						tilePosY->push_back(posYU[i]);
					}
				}
			}
		}
		if((posYR[i] >= 0) && (posYR[i] < SPL)) {
			if((posXR[i] >= 0) && (posXR[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop2)) {
					//if move is black -> obstacle
					if(map[posYR[i]][posXR[i]] < WBISHOP) {
						stop2 = true;
					}
					//if move is white piece -> obstacle
					else if((map[posYR[i]][posXR[i]] >= WBISHOP) && (map[posYR[i]][posXR[i]] < EMPTY)) {
						tilePosX->push_back(posXR[i]);
						tilePosY->push_back(posYR[i]);
						stop2 = true;
					}
					//if move is empty
					else if(map[posYR[i]][posXR[i]] == EMPTY) {
						tilePosX->push_back(posXR[i]);
						tilePosY->push_back(posYR[i]);
					}
				}
			}
		}
		if((posYD[i] >= 0) && (posYD[i] < SPL)) {
			if((posXD[i] >= 0) && (posXD[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop3)) {
					//if move is black -> obstacle
					if(map[posYD[i]][posXD[i]] < WBISHOP) {
						stop3 = true;
					}
					//if move is white piece -> obstacle
					else if((map[posYD[i]][posXD[i]] >= WBISHOP) && (map[posYD[i]][posXD[i]] < EMPTY)) {
						tilePosX->push_back(posXD[i]);
						tilePosY->push_back(posYD[i]);
						stop3 = true;
					}
					//if move is empty
					else if(map[posYD[i]][posXD[i]] == EMPTY) {
						tilePosX->push_back(posXD[i]);
						tilePosY->push_back(posYD[i]);
					}
				}
			}
		}
		if((posYL[i] >= 0) && (posYL[i] < SPL)) {
			if((posXL[i] >= 0) && (posXL[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop4)) {
					//if move is black -> obstacle
					if(map[posYL[i]][posXL[i]] < WBISHOP) {
						stop4 = true;
					}
					//if move is white piece -> obstacle
					else if((map[posYL[i]][posXL[i]] >= WBISHOP) && (map[posYL[i]][posXL[i]] < EMPTY)) {
						tilePosX->push_back(posXL[i]);
						tilePosY->push_back(posYL[i]);
						stop4 = true;
					}
					//if move is empty
					else if(map[posYL[i]][posXL[i]] == EMPTY) {
						tilePosX->push_back(posXL[i]);
						tilePosY->push_back(posYL[i]);
					}
				}
			}
		}
	}
}

void LRook::whiteMove(const int map[SPL][SPL], int x, int y, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
	const int maxVertMove = 7;
	
	//7 possible vertical moves
	//4 vertical directions 
	
	int posYU[maxVertMove] = {0};
	int posXU[maxVertMove] = {0};
	
	for(int i(0); i < maxVertMove; i++) {
		posYU[i] = y - i - 1;
		posXU[i] = x;
	}
	
	int posYR[maxVertMove] = {0};
	int posXR[maxVertMove] = {0};
	
	for(int i(0); i < maxVertMove; i++) {
		posYR[i] = y;
		posXR[i] = x + i + 1;
	}
	
	int posYD[maxVertMove] = {0};
	int posXD[maxVertMove] = {0};
	
	for(int i(0); i < maxVertMove; i++) {
		posYD[i] = y + i + 1;
		posXD[i] = x;
	}
	
	int posYL[maxVertMove] = {0};
	int posXL[maxVertMove] = {0};
	
	for(int i(0); i < maxVertMove; i++) {
		posYL[i] = y;
		posXL[i] = x - i - 1;
	}
	
	//True when an obstacle is met
	//1 for each diagonale
	bool stop1 = false;
	bool stop2 = false;
	bool stop3 = false;
	bool stop4 = false;
	
	for(int i(0); i < maxVertMove; i++) {
		//if move is in bound
		if((posYU[i] >= 0) && (posYU[i] < SPL)) {
			if((posXU[i] >= 0) && (posXU[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop1)) {
					//if move is white piece or bking -> obstacle
					if((map[posYU[i]][posXU[i]] >= WBISHOP) && (map[posYU[i]][posXU[i]] < EMPTY)) {
						stop1 = true;
					}
					//if move is black piece -> obstacle
					else if(map[posYU[i]][posXU[i]] < WBISHOP) {
						tilePosX->push_back(posXU[i]);
						tilePosY->push_back(posYU[i]);
						stop1 = true;
					}
					//if move is empty
					else if(map[posYU[i]][posXU[i]] == EMPTY) {
						tilePosX->push_back(posXU[i]);
						tilePosY->push_back(posYU[i]);
					}
				}
			}
		}
		if((posYR[i] >= 0) && (posYR[i] < SPL)) {
			if((posXR[i] >= 0) && (posXR[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop2)) {
					//if move is black -> obstacle
					if((map[posYR[i]][posXR[i]] >= WBISHOP) && (map[posYR[i]][posXR[i]] < EMPTY)) {
						stop2 = true;
					}
					//if move is black piece -> obstacle
					else if(map[posYR[i]][posXR[i]] < WBISHOP) {
						tilePosX->push_back(posXR[i]);
						tilePosY->push_back(posYR[i]);
						stop2 = true;
					}
					//if move is empty
					else if(map[posYR[i]][posXR[i]] == EMPTY) {
						tilePosX->push_back(posXR[i]);
						tilePosY->push_back(posYR[i]);
					}
				}
			}
		}
		if((posYD[i] >= 0) && (posYD[i] < SPL)) {
			if((posXD[i] >= 0) && (posXD[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop3)) {
					//if move is white -> obstacle
					if((map[posYD[i]][posXD[i]] >= WBISHOP) && (map[posYD[i]][posXD[i]] < EMPTY)) {
						stop3 = true;
					}
					//if move is black piece -> obstacle
					else if(map[posYD[i]][posXD[i]] < WBISHOP) {
						tilePosX->push_back(posXD[i]);
						tilePosY->push_back(posYD[i]);
						stop3 = true;
					}
					//if move is empty
					else if(map[posYD[i]][posXD[i]] == EMPTY) {
						tilePosX->push_back(posXD[i]);
						tilePosY->push_back(posYD[i]);
					}
				}
			}
		}
		if((posYL[i] >= 0) && (posYL[i] < SPL)) {
			if((posXL[i] >= 0) && (posXL[i] < SPL)) {
				//if move has not met an obstacle yet
				if(!(stop4)) {
					//if move is black -> obstacle
					if((map[posYL[i]][posXL[i]] >= WBISHOP) && (map[posYL[i]][posXL[i]] < EMPTY)) {
						stop4 = true;
					}
					//if move is white piece -> obstacle
					else if(map[posYL[i]][posXL[i]] < WBISHOP) {
						tilePosX->push_back(posXL[i]);
						tilePosY->push_back(posYL[i]);
						stop4 = true;
					}
					//if move is empty
					else if(map[posYL[i]][posXL[i]] == EMPTY) {
						tilePosX->push_back(posXL[i]);
						tilePosY->push_back(posYL[i]);
					}
				}
			}
		}
	}
}