#include "game/pieces/LQueen.h"

std::vector<SDL_Point> LQueen::moves(std::array<std::array<int, SPL>, SPL> map, SDL_Point queenPos) {
	std::vector<SDL_Point> legalPos;
	std::vector<SDL_Point> bishopMoves = mBishop.moves(map, queenPos);
	std::vector<SDL_Point> rookMoves = mRook.moves(map, queenPos);
	legalPos.insert(legalPos.end(), bishopMoves.begin(), bishopMoves.end());
	legalPos.insert(legalPos.end(), rookMoves.begin(), rookMoves.end());
	return legalPos;
}

void LQueen::blackMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
	mBishop.blackMove(map, posX, posY, tilePosX, tilePosY);
	mRook.blackMove(map, posX, posY, tilePosX, tilePosY);
}
void LQueen::whiteMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
	mBishop.whiteMove(map, posX, posY, tilePosX, tilePosY);
	mRook.whiteMove(map, posX, posY, tilePosX, tilePosY);

}