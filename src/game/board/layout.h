#ifndef LAYOUT_H
#define LAYOUT_H

#include <array>
#include "com/constantes.h"

extern std::array<std::array<int, 8>, 8> emptyBoard;
extern std::array<std::array<int, 8>, 8> normalBoard;
extern std::array<std::array<int, 8>, 8> promotionBoard;
extern std::array<std::array<int, 8>, 8> castlingBoard;

#endif