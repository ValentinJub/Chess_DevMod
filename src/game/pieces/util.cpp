#include "util.h"

bool UtilPiece::isInBounds(SDL_Point point) {
	return (point.x >= 0 && point.x < SPL && point.y >= 0 && point.y < SPL);
}

bool UtilPiece::isWhite(int piece) {
	return (piece >= WBISHOP && piece < EMPTY);
}

bool UtilPiece::isBlack(int piece) {
	return (piece < WBISHOP);
}

bool UtilPiece::isEnemyOrEmpty(int curr, int next) {
	return (isWhite(curr) && isBlack(next)) || (isBlack(curr) && isWhite(next)) || (next == EMPTY);
}

bool UtilPiece::isEnemy(int curr, int next) {
	return (isWhite(curr) && isBlack(next)) || (isBlack(curr) && isWhite(next));
}