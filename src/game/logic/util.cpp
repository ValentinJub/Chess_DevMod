#include "util.h"

bool isInBounds(SDL_Point point) {
	return (point.x >= 0 && point.x < SPL && point.y >= 0 && point.y < SPL);
}

bool isWhitePiece(int piece) {
	return (piece >= WBISHOP && piece < EMPTY);
}

bool isBlackPiece(int piece) {
	return (piece < WBISHOP);
}

bool isEnemyOrEmpty(int curr, int next) {
	return (isWhitePiece(curr) && isBlackPiece(next)) || (isBlackPiece(curr) && isWhitePiece(next)) || (next == EMPTY);
}

bool isEnemy(int curr, int next) {
	return (isWhitePiece(curr) && isBlackPiece(next)) || (isBlackPiece(curr) && isWhitePiece(next));
}