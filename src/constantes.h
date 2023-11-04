/*
constantes.h
Sun Mar 20 2022
by Valentin
-----------
main const
*/

#ifndef HEADER_CONSTANTES
#define HEADER_CONSTANTES

const int SCREEN_WIDTH = 64*10;
const int SCREEN_HEIGHT = 64*10;
const int NBR_OF_MUSIC = 7;

const int LEFT_MENU = 7;
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

enum CastlingResult {
    CASTLE_WR1,
    CASTLE_WR2,
    CASTLE_BR1,
    CASTLE_BR2,
    NO_CASTLING,
    NO_MOVE
};

enum Settings {
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
    TOTAL_MENU_ITEMS // = 3
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
