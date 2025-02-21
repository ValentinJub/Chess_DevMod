#include "game/pieces/LKnight.h"

const SDL_Point Move[8] = {
	{1, 2},
	{2, 1},
	{2, -1},
	{1, -2},
	{-1, -2},
	{-2, -1},
	{-2, 1},
	{-1, 2}
};

void LKnight::blackMove(const int map[SPL][SPL], SDL_Point knightPos, std::vector<SDL_Point> legalPos) {
	for(int i(0); i < 8; i++) {
		SDL_Point nextPos = {knightPos.x + Move[i].x, knightPos.y + Move[i].y};
		if(UtilPiece::isInBounds(nextPos)) {
			if(map[nextPos.y][nextPos.x] >= WBISHOP) {
				legalPos.push_back(nextPos);
			}
		}
	}
}

void LKnight::whiteMove(const int map[SPL][SPL], SDL_Point knightPos, std::vector<SDL_Point> legalPos) {
    for(int i = 0; i < 8; i++) {
		SDL_Point nextPos = {knightPos.x + Move[i].x, knightPos.y + Move[i].y};
		if(UtilPiece::isInBounds(nextPos)) {
			if((map[nextPos.y][nextPos.x] < WBISHOP) || (map[nextPos.y][nextPos.x] == EMPTY)) {
				legalPos.push_back(nextPos);
			}
        }
    }
}

std::vector<SDL_Point> LKnight::moves(std::array<std::array<int, SPL>, SPL> map, SDL_Point knightPos) {
	std::vector<SDL_Point> legalPos;
    for(int i = 0; i < 8; i++) {
		SDL_Point nextPos = {knightPos.x + Move[i].x, knightPos.y + Move[i].y};
		if(UtilPiece::isInBounds(nextPos)) {
			if(UtilPiece::isEnemyOrEmpty(map[knightPos.y][knightPos.x], map[nextPos.y][nextPos.x])) {
				legalPos.push_back(nextPos);
			}
		}
	}
	return legalPos;
}