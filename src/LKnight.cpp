#include "LKnight.h"

void LKnight::blackMove(const int map[SPL][SPL], int x, int y, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {

	const int maxMove = 8;
	//8 Possible move for the Knight
	int posX[maxMove] = {0};
	int posY[maxMove] = {0};
	
	posY[0] = y + 2;
	posX[0] = x + 1;
	
	posY[1] = y + 1;
	posX[1] = x + 2;
	
	posY[2] = y - 1;
	posX[2] = x + 2;
	
	posY[3] = y - 2;
	posX[3] = x + 1;
	
	posY[4] = y - 2;
	posX[4] = x - 1;
	
	posY[5] = y - 1;
	posX[5] = x - 2;
	
	posY[6] = y + 1;
	posX[6] = x - 2;
	
	posY[7] = y + 2;
	posX[7] = x - 1;
	
	//check if possible move is in bound 
	for(int i(0); i < maxMove; i++) {
		if((posY[i] >= 0) && (posY[i] < maxMove)) {
			if((posX[i] >= 0) && (posX[i] < maxMove)) {
				//if move is in bound
				if(map[posY[i]][posX[i]] >= WBISHOP) {
					//if move is white piece or empty
					tilePosX->push_back(posX[i]);
					tilePosY->push_back(posY[i]);
				}
			}
		}
	}
}

void LKnight::whiteMove(const int map[SPL][SPL], int x, int y, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
	
	const int maxMove = 8;
	//8 Possible move for the Knight
	int posX[maxMove] = {0};
	int posY[maxMove] = {0};
	
	posY[0] = y + 2;
	posX[0] = x + 1;
	
	posY[1] = y + 1;
	posX[1] = x + 2;
	
	posY[2] = y - 1;
	posX[2] = x + 2;
	
	posY[3] = y - 2;
	posX[3] = x + 1;
	
	posY[4] = y - 2;
	posX[4] = x - 1;
	
	posY[5] = y - 1;
	posX[5] = x - 2;
	
	posY[6] = y + 1;
	posX[6] = x - 2;
	
	posY[7] = y + 2;
	posX[7] = x - 1;
	
	//check if possible move is in bound 
	for(int i(0); i < maxMove; i++) {
		if((posY[i] >= 0) && (posY[i] < maxMove)) {
			if((posX[i] >= 0) && (posX[i] < maxMove)) {
				//if move is in bound
				if((map[posY[i]][posX[i]] < WBISHOP) || (map[posY[i]][posX[i]] == EMPTY)) {
					//if move is black piece or empty
					tilePosX->push_back(posX[i]);
					tilePosY->push_back(posY[i]);
				}
			}
		}
	}
}
