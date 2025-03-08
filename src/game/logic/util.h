/*
headers.h
Sun Mar 20 2022
by Valentin
-----------
headers for all files - do not put L*.h!!!!
*/

#ifndef HEADER_UTILPIECE
#define HEADER_UTILPIECE

#include "com/headers.h"
#include "com/logger.h"
#include "com/constantes.h"
#include <fstream>

bool isInBounds(SDL_Point point);
bool isWhitePiece(int piece);
bool isBlackPiece(int piece);
bool isEnemyOrEmpty(int curr, int next);
bool isEnemy(int curr, int next);


#endif