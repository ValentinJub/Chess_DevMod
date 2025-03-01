#include "game/board/LComputer.h"

LComputer::LComputer(LEngine* engine) : mEngine(engine) {}

LComputer::~LComputer() {
    mEngine = NULL;
}

ChessMove LComputer::play(std::array<std::array<int, SPL>, SPL> board, bool isWhite) {
    // mock the play for now
    ChessMove move;
    move.src = {0, 1};
    move.dst = {0, 2};
    move.piece = BPAWN;
    move.capturedPiece = EMPTY;
    move.score = 0;
    return move;
}