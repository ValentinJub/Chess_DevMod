// #include "game/pieces/LPawn.h"

// // const std::array<SDL_Point, 2> WhitePawnTakeMoves = {SDL_Point{-1, -1}, SDL_Point{1, -1}};
// // const std::array<SDL_Point, 2> BlackPawnTakeMoves = {SDL_Point{-1, 1}, SDL_Point{1, 1}};

// // Returns the list of moves the pawn can make
// // The self check bool is only set to true in the context of checking if the king is in check
// // we need to know if we're listing the moves in the context of a check in order to not include the forward 
// // moves which aren't capturing moves and wouldn't result in a check.
// std::vector<SDL_Point> LPawn::moves(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck) {
// 	if(UtilPiece::isWhite(map[pawnPos.y][pawnPos.x])) {
// 		return this->whiteMove(map, pawnPos, selfCheck);
// 	}
// 	else return this->blackMove(map, pawnPos, selfCheck);
// }

// std::vector<SDL_Point> LPawn::whiteMove(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck) {
// 	std::vector<SDL_Point> legalPos;
// 	// Attacking moves
// 	for(int i(0); i < 2; i++) {
// 		SDL_Point nextPos = {pawnPos.x + WhitePawnTakeMoves[i].x, pawnPos.y + WhitePawnTakeMoves[i].y};
// 		if(UtilPiece::isInBounds(nextPos) && UtilPiece::isEnemy(map[pawnPos.y][pawnPos.x], map[nextPos.y][nextPos.x])) {
// 			legalPos.push_back(nextPos);
// 		}
// 	}
// 	// Forward moves, only if selfCheck is false
// 	if(!(selfCheck)) {
// 		if(map[pawnPos.y - 1][pawnPos.x] == EMPTY) {
// 			legalPos.push_back(SDL_Point{pawnPos.x, pawnPos.y - 1});
// 			if(pawnPos.y == 6 && map[pawnPos.y - 2][pawnPos.x] == EMPTY) {
// 				legalPos.push_back(SDL_Point{pawnPos.x, pawnPos.y - 2});
// 			}
// 		}
// 	}
// 	// En passant moves need to be added here later.
// 	return legalPos;
// }

// std::vector<SDL_Point> LPawn::blackMove(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck) {
// 	std::vector<SDL_Point> legalPos;
// 	// Attacking moves
// 	for(int i(0); i < 2; i++) {
// 		SDL_Point nextPos = {pawnPos.x + BlackPawnTakeMoves[i].x, pawnPos.y + BlackPawnTakeMoves[i].y};
// 		if(UtilPiece::isInBounds(nextPos) && UtilPiece::isEnemy(map[pawnPos.y][pawnPos.x], map[nextPos.y][nextPos.x])) {
// 			legalPos.push_back(nextPos);
// 		}
// 	}
// 	// Forward moves, only if selfCheck is false
// 	if(!(selfCheck)) {
// 		if(map[pawnPos.y + 1][pawnPos.x] == EMPTY) {
// 			legalPos.push_back(SDL_Point{pawnPos.x, pawnPos.y + 1});
// 			if(pawnPos.y == 1 && map[pawnPos.y + 2][pawnPos.x] == EMPTY) {
// 				legalPos.push_back(SDL_Point{pawnPos.x, pawnPos.y + 2});
// 			}
// 		}
// 	}
// 	// En passant moves need to be added here later.
// 	return legalPos;
// }

// void LPawn::blackMoveOld(const int map[SPL][SPL], int piecePosX, int piecePosY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, int lastMovedPiece, int lastMovedPieceXSrc, int lastMovedPieceYSrc, int lastMovedPieceXDes, int lastMovedPieceYDes, bool *mEnPassant, bool preCheck) {
// 	//Possible forward move
// 	int forward = piecePosY + 1;
// 	//starting pos y1
// 	//If precheck is true it meens the forward squares arent included in the count
// 	//as they can't take another piece 
// 	if(!(preCheck)) {
// 		// move 2 squares if on starting position
// 		if((piecePosY == 1) && (map[forward][piecePosX] == EMPTY)) {
// 			tilePosX->push_back(piecePosX);
// 			tilePosY->push_back(forward);
// 			if(map[forward + 1][piecePosX] == EMPTY) {
// 				tilePosX->push_back(piecePosX);
// 				tilePosY->push_back(forward + 1);
// 			}
// 		}
// 		if(map[forward][piecePosX] == EMPTY) {
// 			tilePosX->push_back(piecePosX);
// 			tilePosY->push_back(forward);
// 		}
// 	}
	
// 	if(lastMovedPiece == WPAWN) {
// 		if((piecePosY == 4) && (lastMovedPieceYDes == piecePosY) && (lastMovedPieceYSrc - 2 == lastMovedPieceYDes)) {
// 			if(lastMovedPieceXDes - 1 == piecePosX) {
// 				tilePosX->push_back(piecePosX + 1);
// 				tilePosY->push_back(forward);
// 				*mEnPassant = true;
// 			}
// 			else if(lastMovedPieceXDes + 1 == piecePosX) {
// 				tilePosX->push_back(piecePosX - 1);
// 				tilePosY->push_back(forward);
// 				*mEnPassant = true;
// 			}
// 		}
// 	}
	
// 	//if cell down left is within bounds
// 	if((piecePosX - 1 >= 0) && (piecePosX - 1 < SPL)) {
// 		//if cell down left is enemy or empty
// 		if((map[forward][piecePosX - 1] >= WBISHOP) && (map[forward][piecePosX - 1] < EMPTY)) {
// 			tilePosX->push_back(piecePosX - 1);
// 			tilePosY->push_back(forward);
// 		}
// 	}
// 	//if cell down right is within bounds
// 	if((piecePosX + 1 >= 0) && (piecePosX + 1 < SPL)) {
// 	//if cell down right is enemy or empty
// 		if((map[forward][piecePosX + 1] >= WBISHOP) && (map[forward][piecePosX + 1] < EMPTY)) {
// 			tilePosX->push_back(piecePosX + 1);
// 			tilePosY->push_back(forward);
// 		}
// 	}
// }

// void LPawn::whiteMoveOld(const int map[SPL][SPL], int piecePosX, int piecePosY, std::vector<int> *tilePosX, std::vector<int> *tilePosY,  int lastMovedPiece, int lastMovedPieceXSrc, int lastMovedPieceYSrc, int lastMovedPieceXDes, int lastMovedPieceYDes, bool *mEnPassant, bool preCheck) {
// 	//Possible forward move
// 	int forward = piecePosY - 1;
// 	//starting pos y1
// 	//if not in a PollPreCheck fill the empty pos
// 	if(!(preCheck)) {
// 		if((piecePosY == 6) && (map[forward][piecePosX] == EMPTY)) {
// 			tilePosX->push_back(piecePosX);
// 			tilePosY->push_back(forward);
// 			if(map[forward - 1][piecePosX] == EMPTY) {
// 				tilePosX->push_back(piecePosX);
// 				tilePosY->push_back(forward - 1);
// 			}
// 		}
// 		//if forward is empty highlight cell 
// 		if(map[forward][piecePosX] == EMPTY) {
// 			tilePosX->push_back(piecePosX);
// 			tilePosY->push_back(forward);
// 		}
// 	}
// 	if(lastMovedPiece == BPAWN) {
// 		if((piecePosY == 3) && (lastMovedPieceYDes == piecePosY) && (lastMovedPieceYSrc + 2 == lastMovedPieceYDes)) {
// 			if(lastMovedPieceXDes - 1 == piecePosX) {
// 				tilePosX->push_back(piecePosX + 1);
// 				tilePosY->push_back(forward);
// 				*mEnPassant = true;
// 			}
// 			else if(lastMovedPieceXDes + 1 == piecePosX) {
// 				tilePosX->push_back(piecePosX - 1);
// 				tilePosY->push_back(forward);
// 				*mEnPassant = true;
// 			}
// 		}
// 	}
// 	//if pawn can take
// 	if((piecePosX - 1 >= 0) && (piecePosX - 1 < SPL)) {
// 		if(map[forward][piecePosX - 1] < WBISHOP) {
// 			tilePosX->push_back(piecePosX - 1);
// 			tilePosY->push_back(forward);
// 		}
// 	}
// 	if((piecePosX + 1 >= 0) && (piecePosX + 1 < SPL)) {
// 		if(map[forward][piecePosX + 1] < WBISHOP) {
// 			tilePosX->push_back(piecePosX + 1);
// 			tilePosY->push_back(forward);
// 		}
// 	}
// }
