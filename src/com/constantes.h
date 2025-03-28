/*
constantes.h
Sun Mar 20 2022
by Valentin
-----------
main const
*/

#ifndef HEADER_CONSTANTES
#define HEADER_CONSTANTES

#include "SDL2/SDL.h"
#include <array>

// Resource paths

extern const char* FONT_BRANDA;
extern const char* MUSIC_CLASSIC;
extern const char* MUSIC_JAZZY;
extern const char* MUSIC_MENU;

extern const char* CHUNK_CHIME;
extern const char* CHUNK_CHECK;
extern const char* CHUNK_CHECKMATE;
extern const char* CHUNK_ERROR;
extern const char* CHUNK_MOVE;
extern const char* CHUNK_CASTLE;
extern const char* CHUNK_CAPTURE;
extern const char* CHUNK_VICTORY;
extern const char* CHUNK_DEFEAT;
extern const char* CHUNK_TITLE;

extern const char* FILE_SETTINGS;
extern const char* FILE_SETTINGS_LEFT;
extern const char* FILE_SETTINGS_RIGHT;
extern const char* FILE_MAP;
extern const char* FILE_MAP_CASTLE;

extern const char* SPRITE_BACKGROUND;
extern const char* SPRITE_BACKGROUND_FULLBLACK;
extern const char* SPRITE_PIECE_SHEET;
extern const char* SPRITE_PIECE_SHEET_32;
extern const char* SPRITE_RETRO_PIECE_SHEET;
extern const char* SPRITE_BOARD;
extern const char* SPRITE_DOT;
extern const char* SPRITE_QUEEN_NORMAL;
extern const char* SPRITE_QUEEN_RETRO;

constexpr SDL_Color COLOR_BLACK = {0,0,0,0xFF};
constexpr SDL_Color COLOR_WHITE = {0xFF,0xFF,0xFF,0xFF};
constexpr SDL_Color COLOR_RED = {0xFF,0,0,0xFF};

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 800;
constexpr int TILE_SIZE = 84;
constexpr int TILE_OFFSET = 10;
constexpr int NBR_OF_MUSIC = 7;

constexpr int LEFT_MENU = 8;
constexpr int TOTAL_SETTINGS_MENU_TEXTURES = 17;
constexpr int TOTAL_SQUARES = 64;
constexpr int VOFFSET = 64;
constexpr int HOFFSET = VOFFSET * 2;
constexpr int SPL = 8;
constexpr int INITIAL_PIECES_TOTAL = 32;
constexpr int PLACEHOLDER = 1;

// https://www.chessprogramming.org/Simplified_Evaluation_Function
enum PieceValues {
    BISHOP_VALUE = 330,
    KNIGHT_VALUE = 320,
    PAWN_VALUE = 100,
    KING_VALUE = 20000,
    QUEEN_VALUE = 900,
    ROOK_VALUE = 500
};

constexpr std::array<SDL_Point, SPL> KingMoves = {
	SDL_Point{0, -1}, //U
	SDL_Point{1, -1}, //UR
	SDL_Point{1, 0}, //R
	SDL_Point{1, 1}, //DR
	SDL_Point{0, 1}, //D
	SDL_Point{-1, 1}, //DL
	SDL_Point{-1, 0}, //L
	SDL_Point{-1, -1} //UL
};

constexpr std::array<SDL_Point, 4> BishopMoves = {
	SDL_Point{1, -1},
	SDL_Point{1, 1},
	SDL_Point{-1, 1},
	SDL_Point{-1, -1}
};

constexpr std::array<SDL_Point, SPL> KnightMoves = {
	SDL_Point{1, -2},
    SDL_Point{2, -1},
    SDL_Point{2, 1},
    SDL_Point{1, 2},
    SDL_Point{-1, 2},
    SDL_Point{-2, 1},
    SDL_Point{-2, -1},
    SDL_Point{-1, -2}
};

constexpr std::array<SDL_Point, 4> RookMoves = {
	SDL_Point{0, -1},
	SDL_Point{1, 0},
	SDL_Point{0, 1},
	SDL_Point{-1, 0}
};

constexpr std::array<SDL_Point, 2> WhitePawnTakeMoves = {SDL_Point{-1, -1}, SDL_Point{1, -1}};
constexpr std::array<SDL_Point, 2> BlackPawnTakeMoves = {SDL_Point{-1, 1}, SDL_Point{1, 1}};

enum CastlingResult {
    CASTLE_WR1,
    CASTLE_WR2,
    CASTLE_BR1,
    CASTLE_BR2,
    NO_CASTLING,
    NO_MOVE
};

struct OptionValues {
    int showLegalMoves;
    int useTimer;
    int timeLimit;
    int tileColor;
    int musicTheme;
    int volume;
    int pieceTheme;
};

constexpr int TOTAL_SETTINGS_RIGHT_MENU_TEXT_ITEMS = 11;

enum SettingsMenuButtons {
    SLM_YES,
    SLM_NO,
    TL_YES,
    TL_NO,
    TL_5,
    TL_10,
    TC_BROWN,
    TC_GREY,
    MT_JAZZY,
    MT_CLASSIC,
    PT_1,
    PT_2,
    BACK,
    TOTAL_CLICKABLE_ITEMS
};

// This is the order in which the pieces are stored in the texture sheet
// The order is important for the clipping
enum Pieces {
    BBISHOP, 
    BKING,
    BKNIGHT,
    BPAWN,
    BQUEEN,
    BROOK,
    WBISHOP,
    WKING,
    WKNIGHT,
    WPAWN,
    WQUEEN,
    WROOK,
    EMPTY,
    TOTAL_PIECES // = 13 (including one empty)
};

enum TitleItems {
    TITLE,
    AUTHOR,
    TOTAL_TITLE_ITEMS // = 2
};

enum MenuItems {
    PLAY,
    PLAY_AI,
    SETTINGS,
    DEVMODE,
    TOTAL_MENU_ITEMS // = 4
};

enum Tile {
    LIGHT1,
    DARK1,
    LIGHT2,
    DARK2,
    TOTAL_TILE_COLOR
};

enum KingPos {
    U,
    UR,
    R,
    DR,
    D,
    DL,
    L,
    UL,
    TOTAL_POS // = 8
};

enum CheckStatus {
    BCHECK,
    WCHECK,
    NO_CHECK
};

#endif 
