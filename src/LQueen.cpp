#include "LQueen.h"

void LQueen::blackMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
	mBishop.blackMove(map, posX, posY, tilePosX, tilePosY);
	mRook.blackMove(map, posX, posY, tilePosX, tilePosY);
}
void LQueen::whiteMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY) {
	mBishop.whiteMove(map, posX, posY, tilePosX, tilePosY);
	mRook.whiteMove(map, posX, posY, tilePosX, tilePosY);

}