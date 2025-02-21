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

extern const char* SPRITE_BACKGROUND;
extern const char* SPRITE_BACKGROUND_FULLBLACK;
extern const char* SPRITE_PIECE_SHEET;
extern const char* SPRITE_PIECE_SHEET_32;
extern const char* SPRITE_RETRO_PIECE_SHEET;
extern const char* SPRITE_BOARD;
extern const char* SPRITE_DOT;
extern const char* SPRITE_QUEEN_NORMAL;
extern const char* SPRITE_QUEEN_RETRO;

const SDL_Color COLOR_BLACK = {0,0,0,0xFF};
const SDL_Color COLOR_WHITE = {0xFF,0xFF,0xFF,0xFF};
const SDL_Color COLOR_RED = {0xFF,0,0,0xFF};

const int SCREEN_WIDTH = 64*10;
const int SCREEN_HEIGHT = 64*10;
const int NBR_OF_MUSIC = 7;

const int LEFT_MENU = 8;
const int TOTAL_SETTINGS_MENU_TEXTURES = 17;
const int TOTAL_SQUARES = 64;
const int OFFSET = 64;
const int SPL = 8;
const int INITIAL_PIECES_TOTAL = 32;
const int PLACEHOLDER = 1;

const int BISHOP_VALUE = 3;
const int KNIGHT_VALUE = 3;
const int PAWN_VALUE = 1;
const int KING_VALUE = 20;
const int QUEEN_VALUE = 10;
const int ROOK_VALUE = 5;

const SDL_Point KingMoves[SPL] = {
	SDL_Point{0, -1}, //U
	SDL_Point{1, -1}, //UR
	SDL_Point{1, 0}, //R
	SDL_Point{1, 1}, //DR
	SDL_Point{0, 1}, //D
	SDL_Point{-1, 1}, //DL
	SDL_Point{-1, 0}, //L
	SDL_Point{-1, -1} //UL
};

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

const int TOTAL_SETTINGS_RIGHT_MENU_TEXT_ITEMS = 11;

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
