#ifndef CHECK_H
#define CHECK_H

#include "headers.h"
#include "vector"

void showMove(const int map[SPL][SPL], int const pieceType, int const pieceXPos, const int pieceYPos, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool preCheck = false);

bool isKingChecked(const int map[SPL][SPL], const int piece, const int desX, const int desY, const int srcX, const int srcY, bool whiteTurn);

void whiteBishopMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);
void whiteKnightMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);
void whitePawnMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool preCheck = false);
void whiteRookMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);
void whiteQueenMove(const int map[SPL][SPL], int posX, int posY, std::vector<int> *tilePosX, std::vector<int> *tilePosY);

void lookForForbiddenSquare(const int map[SPL][SPL], std::vector<int> *tilePosX, std::vector<int> *tilePosY, bool whiteTurn);

#endif