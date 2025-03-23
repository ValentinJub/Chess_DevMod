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

std::string pieceToString(int piece) {
	switch(piece) {
		case WBISHOP:
		case BBISHOP:
			return "B";
		case WKING:
		case BKING:
			return "K";
		case WKNIGHT:
		case BKNIGHT:
			return "N";
		// For pawn moves, a letter indicating pawn is not used
		case WPAWN:
		case BPAWN:
			return "";
		case WQUEEN:
		case BQUEEN:
			return "Q";
		case WROOK:
		case BROOK:
			return "R";
		default:
			return "";
	}
}

std::string squareToString(SDL_Point square) {
	std::string str = "";
	str += 'a' + square.x;
	str += std::to_string(8 - square.y);
	return str;
}

