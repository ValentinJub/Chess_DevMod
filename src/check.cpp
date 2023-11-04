#include "check.h"

using std::vector;

bool isKingChecked(const int mapCopy[SPL][SPL], const int piece, const int desX, const int desY, const int srcX, const int srcY, bool whiteTurn) {
	//default false, no check
	bool check = false;
	
	//make a local copy of the map to preview the move
	//if the move checks its own color - pollDiscoverAttack 
	//returns true, error sound is played + no move
	int map[SPL][SPL] = {0};
	for(int a(0); a < SPL; a++) {
		for(int b(0); b < SPL; b++) {
			map[a][b] = mapCopy[a][b];
		}
	}
	
	//move selected piece to destination in the local map
	map[desY][desX] = piece;
	//empty source cell 
	map[srcY][srcX] = EMPTY;
	
	//check all white pieces on the map if black's turn
	//for each white piece, check its possible moves 
	//if one of these possible move pos contain 
	//a black King - check 
	if(!(whiteTurn)) {
		for(int y(0); y < SPL; y++) {
			for(int x(0); x < SPL; x++) {
				if((map[y][x] >= WBISHOP) && (map[y][x] < EMPTY)) {
					//Select piece from map otherwise selected piece will be the one 
					//that just moved
					int pieceType = map[y][x];
					int selectedPieceYPos = y;
					int selectedPieceXPos = x;
					vector<int> tileYPos;
					vector<int> tileXPos;
					//fill possible moves
					showMove(map, pieceType, selectedPieceXPos, selectedPieceYPos, &tileXPos, &tileYPos, true);
					//check if on one if these moves there is a BKING, in which case it checks 
					//its own color and returns true
					for(int i(0); i < tileYPos.size(); i++) {
						if(map[tileYPos[i]][tileXPos[i]] == BKING) {
							check = true;
							return check;
						}
					}
				}
			}
		}
	}
	//White turn check all Black piece possible moves
	else {
		for(int y(0); y < SPL; y++) {
			for(int x(0); x < SPL; x++) {
				if(map[y][x] < WBISHOP) {
					//Select piece from map otherwise selected piece will be the one 
					//that just moved
					int pieceType = map[y][x];
					int selectedPieceYPos = y;
					int selectedPieceXPos = x;
					//fill possible moves
					vector<int> tileYPos;
					vector<int> tileXPos;
					showMove(map, pieceType, selectedPieceXPos, selectedPieceYPos, &tileXPos, &tileYPos, true);
					//check if on one if these moves there is a WKING, in which case it checks 
					//its own color and returns true
					for(int i(0); i < tileYPos.size(); i++) {
						if(map[tileYPos[i]][tileXPos[i]] == WKING) {
							check = true;
							return check;
						}
					}
				}
			}
		}
	}
	return check; //default false
}

void whiteBishopMove(const int map[SPL][SPL], int x, int y, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
	
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

void whiteKnightMove(const int map[SPL][SPL], int x, int y, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
	
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

void whitePawnMove(const int map[SPL][SPL], int piecePosX, int piecePosY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool preCheck) {
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

void whiteRookMove(const int map[SPL][SPL], int x, int y, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
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

void whiteQueenMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
	whiteBishopMove(map, posX, posY, tilePosX, tilePosY);
	whiteRookMove(map, posX, posY, tilePosX, tilePosY);
	
}

void showMove(const int map[SPL][SPL], int const pieceType, int const pieceXPos, const int pieceYPos, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool preCheck) {
	//clear to make sure we don't add to 
	//existing vector content
	switch(pieceType) {
		case WPAWN:
			if(preCheck) {
				whitePawnMove(map, pieceXPos, pieceYPos, tilePosX, tilePosY, preCheck);
			}
			else {
				whitePawnMove(map, pieceXPos, pieceYPos, tilePosX, tilePosY); 
			}
			break;
		case WKNIGHT:
			whiteKnightMove(map, pieceXPos, pieceYPos, tilePosX, tilePosY);
			break;
		case WBISHOP:
			whiteBishopMove(map, pieceXPos, pieceYPos, tilePosX, tilePosY);
			break;
		case WROOK:
			whiteRookMove(map, pieceXPos, pieceYPos, tilePosX, tilePosY);
			break;
		case WQUEEN:
			whiteQueenMove(map, pieceXPos, pieceYPos, tilePosX, tilePosY);
			break;
		default:
			break;
	}
}

void lookForForbiddenSquare(const int map[SPL][SPL], std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool whiteTurn) {
	for(int y(0); y < SPL; y++) {
		for(int x(0); x < SPL; x++) {
			//white turn look for black king
			if(whiteTurn) {
				if(map[y][x] == BKING) {
					tilePosY->push_back(y - 1);
					tilePosX->push_back(x);
					
					tilePosY->push_back(y - 1);
					tilePosX->push_back(x + 1); 
					
					tilePosY->push_back(y);
					tilePosX->push_back(x + 1); 
					
					tilePosY->push_back(y + 1);
					tilePosX->push_back(x + 1); 
					
					tilePosY->push_back(y + 1);
					tilePosX->push_back(x); 
					
					tilePosY->push_back(y + 1);
					tilePosX->push_back(x - 1); 
					
					tilePosY->push_back(y);
					tilePosX->push_back(x - 1); 
					
					tilePosY->push_back(y - 1);
					tilePosX->push_back(x - 1); 
					
					y = 99;
					x = 99;
				}
			}
			//black turn look for white king
			else {
				if(map[y][x] == WKING) {
					tilePosY->push_back(y - 1);
					tilePosX->push_back(x);
					
					tilePosY->push_back(y - 1);
					tilePosX->push_back(x + 1); 
					
					tilePosY->push_back(y);
					tilePosX->push_back(x + 1); 
					
					tilePosY->push_back(y + 1);
					tilePosX->push_back(x + 1); 

					tilePosY->push_back(y + 1);
					tilePosX->push_back(x); 
					
					tilePosY->push_back(y + 1);
					tilePosX->push_back(x - 1); 
					
					tilePosY->push_back(y);
					tilePosX->push_back(x - 1); 
					
					tilePosY->push_back(y - 1);
					tilePosX->push_back(x - 1); 
					
					y = 99;
					x = 99;
				}
			}
		}
	}
}