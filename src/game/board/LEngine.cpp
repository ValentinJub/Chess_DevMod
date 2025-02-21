#include "game/board/LEngine.h"

bool operator==(const SDL_Point& a, const SDL_Point& b) {
    return a.x == b.x && a.y == b.y;
}

std::vector<SDL_Point> LEngine::getLegalMoves(std::array<std::array<int, SPL>, SPL> board, SDL_Point piecePos) {
	const int piece = board[piecePos.y][piecePos.x];
    switch(piece) {
    case BPAWN:
    case WPAWN:
        return mPawn.moves(board, piecePos);
    case WKNIGHT:
    case BKNIGHT:
        return mKnight.moves(board, piecePos);
    case BBISHOP:
    case WBISHOP:
        return mBishop.moves(board, piecePos);
    case BROOK:
    case WROOK:
        return mRook.moves(board, piecePos);
    case BQUEEN:
    case WQUEEN:
        return mQueen.moves(board, piecePos);
    case WKING:
    case BKING:
        return mKing.moves(board, piecePos);
    default:
        return std::vector<SDL_Point>();
    }
}

// Check if the move will put the colored king in check
bool LEngine::isMoveSelfCheck(const std::array<std::array<int, SPL>, SPL>& board, SDL_Point src, SDL_Point dest, bool isWhite) {
    std::array<std::array<int, SPL>, SPL> newBoard = board;
    newBoard[dest.y][dest.x] = newBoard[src.y][src.x];
    newBoard[src.y][src.x] = EMPTY;
    return this->isKingInCheck(newBoard, isWhite);
}

// Check if the king colored king is in check
// This doesn't take into account the knights moves and need implemented
bool LEngine::isKingInCheck(const std::array<std::array<int, SPL>, SPL>& board, bool isWhite) {
    SDL_Point kingPos = findKing(board, isWhite);
    SDL_Point nextPos = {kingPos.x, kingPos.y};
    for(int i(0); i < SPL; i++) {
        nextPos = {kingPos.x + KingMoves[i].x, kingPos.y + KingMoves[i].y};
        while(UtilPiece::isInBounds(nextPos)) {
            if (board[nextPos.y][nextPos.x] != EMPTY) {
                if(UtilPiece::isEnemy(board[kingPos.y][kingPos.x], board[nextPos.y][nextPos.x])) {
                    if (this->isPieceAttacked(board, nextPos, kingPos, true)) {
                        return true;
                    }
                } else {
                    break;
                }
            }
            nextPos = {nextPos.x + KingMoves[i].x, nextPos.y + KingMoves[i].y};
        }
    }
    return false;
    // ADD KNIGHT MOVES
}

bool LEngine::isCheckMate(const std::array<std::array<int, SPL>, SPL> &board, bool isWhite) {
    const int king = isWhite ? BKING : WKING;
    for(int y(0); y < SPL; y++) {
        for(int x(0); x < SPL; x++) {
            const int next = board[y][x];
            // If the piece is the same color as the king 
            if (next != EMPTY && !UtilPiece::isEnemy(king, next)) {
                // Get the legal moves of the piece
                std::vector<SDL_Point> mPieceMoves = this->getLegalMoves(board, SDL_Point{x, y});
                for (auto move : mPieceMoves) {
                    // If the move doesn't put the king in check then the move cancels the current check
                    if (!this->isMoveSelfCheck(board, SDL_Point{x, y}, move, !isWhite)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool LEngine::isPieceAttacked(const std::array<std::array<int, SPL>, SPL>& board, SDL_Point attackerPos, SDL_Point defenderPos, bool selfCheck) {
    std::vector<SDL_Point> mEnemyMoves;
    switch(board[attackerPos.y][attackerPos.x]) {
        case WPAWN:
        case BPAWN:
            mEnemyMoves = mPawn.moves(board, attackerPos, selfCheck);
            if(std::find(mEnemyMoves.begin(), mEnemyMoves.end(), defenderPos) != mEnemyMoves.end()) {
                return true;
            }
            break;
        case WKNIGHT:
        case BKNIGHT:
            mEnemyMoves = mKnight.moves(board, attackerPos);
            if(std::find(mEnemyMoves.begin(), mEnemyMoves.end(), defenderPos) != mEnemyMoves.end()) {
                return true;
            }
            break;
        case WBISHOP:
        case BBISHOP:
            mEnemyMoves = mBishop.moves(board, attackerPos);
            if(std::find(mEnemyMoves.begin(), mEnemyMoves.end(), defenderPos) != mEnemyMoves.end()) {
                return true;
            }
            break;
        case WROOK:
        case BROOK:
            mEnemyMoves = mRook.moves(board, attackerPos);
            if(std::find(mEnemyMoves.begin(), mEnemyMoves.end(), defenderPos) != mEnemyMoves.end()) {
                return true;
            }
            break;
        case WQUEEN:
        case BQUEEN:
            mEnemyMoves = mQueen.moves(board, attackerPos);
            if(std::find(mEnemyMoves.begin(), mEnemyMoves.end(), defenderPos) != mEnemyMoves.end()) {
                return true;
            }
            break;
        case WKING:
        case BKING:
            mEnemyMoves = mKing.moves(board, attackerPos);
            if(std::find(mEnemyMoves.begin(), mEnemyMoves.end(), defenderPos) != mEnemyMoves.end()) {
                return true;
            }
            break;
        default:
            spdlog::error("Piece {} is not a valid piece", board[attackerPos.y][attackerPos.x]);
            break;
    }
    return false;
}


SDL_Point LEngine::findKing(const std::array<std::array<int, SPL>, SPL>& board, bool isWhite) {
	int king = isWhite ? WKING : BKING;
	for(int y(0); y < SPL; y++) {
		for(int x(0); x < SPL; x++) {
			if(board[y][x] == king) {
				return SDL_Point{x, y};
			}
		}
	}
	spdlog::error("King not found on board");
	return SDL_Point{-1, -1};
}