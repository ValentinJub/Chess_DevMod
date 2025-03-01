#include "game/board/LEngine.h"

bool operator==(const SDL_Point& a, const SDL_Point& b) {
    return a.x == b.x && a.y == b.y;
}

std::vector<SDL_Point> LEngine::getLegalMoves(std::array<std::array<int, SPL>, SPL> board, SDL_Point piecePos) {
	const int piece = board[piecePos.y][piecePos.x];
    switch(piece) {
    case BPAWN:
    case WPAWN:
        return this->pawnMoves(board, piecePos);
    case WKNIGHT:
    case BKNIGHT:
        return this->knightMoves(board, piecePos);
    case BBISHOP:
    case WBISHOP:
        return this->bishopMoves(board, piecePos);
    case BROOK:
    case WROOK:
        return this->rookMoves(board, piecePos);
    case BQUEEN:
    case WQUEEN:
        return this->queenMoves(board, piecePos);
    case WKING:
    case BKING:
        return this->kingMoves(
            board,
            piecePos,
            this->getKingHasMoved(isWhitePiece(piece)),
            this->getRookHasMoved(isWhitePiece(piece), 1),
            this->getRookHasMoved(isWhitePiece(piece), 2)
        );
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
bool LEngine::isKingInCheck(const std::array<std::array<int, SPL>, SPL>& board, bool isWhite) {
    SDL_Point kingPos = findKing(board, isWhite);
    SDL_Point nextPos = {kingPos.x, kingPos.y};
    for(int i(0); i < SPL; i++) {
        nextPos = {kingPos.x + KingMoves[i].x, kingPos.y + KingMoves[i].y};
        while(isInBounds(nextPos)) {
            if (board[nextPos.y][nextPos.x] != EMPTY) {
                if(isEnemy(board[kingPos.y][kingPos.x], board[nextPos.y][nextPos.x])) {
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
    std::vector<SDL_Point> knightPos = this->findKnights(board, !isWhite);
    for (auto pos : knightPos) {
        if (this->isPieceAttacked(board, pos, kingPos, true)) {
            return true;
        }
    }   
    return false;
}

// Return the colored knight position
std::vector<SDL_Point> LEngine::findKnights(const std::array<std::array<int, SPL>, SPL>& board, bool isWhite) {
    std::vector<SDL_Point> knightPos;
    int oppositeKnight = isWhite ? WKNIGHT : BKNIGHT;
    for(int y(0); y < SPL; y++) {
        for(int x(0); x < SPL; x++) {
            if (board[y][x] == oppositeKnight) {
                knightPos.push_back(SDL_Point{x, y});
            }
        }
    }
    return knightPos;
}

bool LEngine::isCheckMate(const std::array<std::array<int, SPL>, SPL> &board, bool isWhite) {
    const int king = isWhite ? BKING : WKING;
    for(int y(0); y < SPL; y++) {
        for(int x(0); x < SPL; x++) {
            const int next = board[y][x];
            // If the piece is the same color as the king 
            if (next != EMPTY && !isEnemy(king, next)) {
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

void LEngine::setKingHasMoved(bool isWhite, bool hasMoved) {
    if (isWhite) {
        mWKingHasMoved = hasMoved;
    } else {
        mBKingHasMoved = hasMoved;
    }
}

void LEngine::setRookHasMoved(bool isWhite, int rook, bool hasMoved) {
    if (isWhite) {
        if (rook == 1) {
            mWRook1HasMoved = hasMoved;
        } else {
            mWRook2HasMoved = hasMoved;
        }
    } else {
        if (rook == 1) {
            mBRook1HasMoved = hasMoved;
        } else {
            mBRook2HasMoved = hasMoved;
        }
    }
}

bool LEngine::getKingHasMoved(bool isWhite) {
    return isWhite ? mWKingHasMoved : mBKingHasMoved;
}

bool LEngine::getRookHasMoved(bool isWhite, int rook) {
    if (isWhite) {
        return rook == 1 ? mWRook1HasMoved : mWRook2HasMoved;
    } else {
        return rook == 1 ? mBRook1HasMoved : mBRook2HasMoved;
    }
}

bool LEngine::isPieceAttacked(const std::array<std::array<int, SPL>, SPL>& board, SDL_Point attackerPos, SDL_Point defenderPos, bool selfCheck) {
    std::vector<SDL_Point> mEnemyMoves;
    switch(board[attackerPos.y][attackerPos.x]) {
        case WPAWN:
        case BPAWN:
            mEnemyMoves = this->pawnMoves(board, attackerPos, true); // We set true to avoid counting the forward moves as attacking moves
            if(std::find(mEnemyMoves.begin(), mEnemyMoves.end(), defenderPos) != mEnemyMoves.end()) {
                return true;
            }
            break;
        case WKNIGHT:
        case BKNIGHT:
            mEnemyMoves = this->knightMoves(board, attackerPos);
            if(std::find(mEnemyMoves.begin(), mEnemyMoves.end(), defenderPos) != mEnemyMoves.end()) {
                return true;
            }
            break;
        case WBISHOP:
        case BBISHOP:
            mEnemyMoves = this->bishopMoves(board, attackerPos);
            if(std::find(mEnemyMoves.begin(), mEnemyMoves.end(), defenderPos) != mEnemyMoves.end()) {
                return true;
            }
            break;
        case WROOK:
        case BROOK:
            mEnemyMoves = this->rookMoves(board, attackerPos);
            if(std::find(mEnemyMoves.begin(), mEnemyMoves.end(), defenderPos) != mEnemyMoves.end()) {
                return true;
            }
            break;
        case WQUEEN:
        case BQUEEN:
            mEnemyMoves = this->queenMoves(board, attackerPos);
            if(std::find(mEnemyMoves.begin(), mEnemyMoves.end(), defenderPos) != mEnemyMoves.end()) {
                return true;
            }
            break;
        case WKING:
        case BKING:
            mEnemyMoves = this->kingMoves(
                board,
                attackerPos,
                true, // We set true to avoid counting the castling moves as an attacking move
                true,
                true
            );
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

std::vector<SDL_Point> LEngine::bishopMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point bishopPos) {
	std::vector<SDL_Point> legalPos;
	for(int i(0); i < BishopMoves.size(); i++) {
		SDL_Point nextPos = {bishopPos.x + BishopMoves[i].x, bishopPos.y + BishopMoves[i].y};
		while(isInBounds(nextPos)) {
			if(isEnemyOrEmpty(map[bishopPos.y][bishopPos.x], map[nextPos.y][nextPos.x])) {
				legalPos.push_back(nextPos);
				if(map[nextPos.y][nextPos.x] != EMPTY) {
					break;
				}
				nextPos.x += BishopMoves[i].x;
				nextPos.y += BishopMoves[i].y;
			} else {
				break;
			}
		}
	}
	return legalPos;
}

std::vector<SDL_Point> LEngine::knightMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point knightPos) {
	std::vector<SDL_Point> legalPos;
    for(int i = 0; i < KnightMoves.size(); i++) {
		SDL_Point nextPos = {knightPos.x + KnightMoves[i].x, knightPos.y + KnightMoves[i].y};
		if(isInBounds(nextPos)) {
			if(isEnemyOrEmpty(map[knightPos.y][knightPos.x], map[nextPos.y][nextPos.x])) {
				legalPos.push_back(nextPos);
			}
		}
	}
	return legalPos;
}

std::vector<SDL_Point> LEngine::kingMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point kingPos, bool kingMoved, bool rook1Moved, bool rook2Moved) {
	std::vector<SDL_Point> legalPos;
	bool isWhite = isWhitePiece(map[kingPos.y][kingPos.x]);
	// Regular moves
	for(int i(0); i < KingMoves.size(); i++) {
		SDL_Point nxtPos = {kingPos.x + KingMoves[i].x, kingPos.y + KingMoves[i].y};
		if(isInBounds(nxtPos) && isEnemyOrEmpty(map[kingPos.y][kingPos.x], map[nxtPos.y][nxtPos.x])) {
			legalPos.push_back(nxtPos);
		}
	}
	// Castling
	if(!kingMoved) {
		if(!rook1Moved) { 
			if(isWhite) { // A1 rook
				// Ensure the path is clear
				if(map[7][1] == EMPTY && map[7][2] == EMPTY && map[7][3] == EMPTY) {
					// Ensure the king is not in check
					if(!this->isMoveSelfCheck(map, kingPos, {2, 7}, isWhite) && !this->isMoveSelfCheck(map, kingPos, {3, 7}, isWhite)) {
						legalPos.push_back({2, 7});
					}
				}
			}
			else { // A8 rook
				// Ensure the path is clear
				if(map[0][1] == EMPTY && map[0][2] == EMPTY && map[0][3] == EMPTY) {
					// Ensure the king is not in check
					if(!this->isMoveSelfCheck(map, kingPos, {2, 0}, isWhite) && !this->isMoveSelfCheck(map, kingPos, {3, 0}, isWhite)) {
						legalPos.push_back({2, 0});
					}
				}
			}
		}
		if(!rook2Moved) {
			if(isWhite) { // H1 rook
				// Ensure the path is clear
				if(map[7][5] == EMPTY && map[7][6] == EMPTY) {
					// Ensure the king is not in check
					if(!this->isMoveSelfCheck(map, kingPos, {6, 7}, isWhite) && !this->isMoveSelfCheck(map, kingPos, {5, 7}, isWhite)) {
						legalPos.push_back({6, 7});
					}
				}
			}
			else { // H8 rook
				// Ensure the path is clear
				if(map[0][5] == EMPTY && map[0][6] == EMPTY) {
					// Ensure the king is not in check
					if(!this->isMoveSelfCheck(map, kingPos, {6, 0}, isWhite) && !this->isMoveSelfCheck(map, kingPos, {5, 0}, isWhite)) {
						legalPos.push_back({6, 0});
					}
				}
			}
		}
	}
	return legalPos;
}

std::vector<SDL_Point> LEngine::rookMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point rookPos) {
	std::vector<SDL_Point> legalPos;
	for(int i(0); i < RookMoves.size(); i++) {
		SDL_Point nextPos = {rookPos.x + RookMoves[i].x, rookPos.y + RookMoves[i].y};
		while(isInBounds(nextPos)) {
			if(isEnemyOrEmpty(map[rookPos.y][rookPos.x], map[nextPos.y][nextPos.x])) {
				legalPos.push_back(nextPos);
				if(map[nextPos.y][nextPos.x] != EMPTY) {
					break;
				}
				nextPos.x += RookMoves[i].x;
				nextPos.y += RookMoves[i].y;
			} else {
				break;
			}
		}
	}
	return legalPos;
}

std::vector<SDL_Point> LEngine::queenMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point queenPos) {
	std::vector<SDL_Point> legalPos;
	std::vector<SDL_Point> bishopMoves = this->bishopMoves(map, queenPos);
	std::vector<SDL_Point> rookMoves = this->rookMoves(map, queenPos);
	legalPos.insert(legalPos.end(), bishopMoves.begin(), bishopMoves.end());
	legalPos.insert(legalPos.end(), rookMoves.begin(), rookMoves.end());
	return legalPos;
}

std::vector<SDL_Point> LEngine::pawnMoves(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck) {
	if(isWhitePiece(map[pawnPos.y][pawnPos.x])) {
		return this->wPawnMove(map, pawnPos, selfCheck);
	}
	else return this->bPawnMove(map, pawnPos, selfCheck);
}

std::vector<SDL_Point> LEngine::wPawnMove(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck) {
	std::vector<SDL_Point> legalPos;
	// Attacking moves
	for(int i(0); i < 2; i++) {
		SDL_Point nextPos = {pawnPos.x + WhitePawnTakeMoves[i].x, pawnPos.y + WhitePawnTakeMoves[i].y};
		if(isInBounds(nextPos) && isEnemy(map[pawnPos.y][pawnPos.x], map[nextPos.y][nextPos.x])) {
			legalPos.push_back(nextPos);
		}
	}
	// Forward moves, only if selfCheck is false
	if(!(selfCheck)) {
		if(map[pawnPos.y - 1][pawnPos.x] == EMPTY) {
			legalPos.push_back(SDL_Point{pawnPos.x, pawnPos.y - 1});
			if(pawnPos.y == 6 && map[pawnPos.y - 2][pawnPos.x] == EMPTY) {
				legalPos.push_back(SDL_Point{pawnPos.x, pawnPos.y - 2});
			}
		}
	}

    // If the last moved piece was a pawn and it moved two squares forward and landed next to the current pawn
    // then the current pawn can take the last moved piece en passant
    if(mLastMovedPiece == BPAWN && abs(mLastMovedPieceSrcPos.y - mLastMovedPieceDestPos.y) == 2 && mLastMovedPieceDestPos.y == pawnPos.y && abs(mLastMovedPieceDestPos.x - pawnPos.x) == 1) {
        legalPos.push_back(SDL_Point{mLastMovedPieceDestPos.x, mLastMovedPieceDestPos.y - 1});
    }
	return legalPos;
}

std::vector<SDL_Point> LEngine::bPawnMove(std::array<std::array<int, SPL>, SPL> map, SDL_Point pawnPos, bool selfCheck) {
	std::vector<SDL_Point> legalPos;
	// Attacking moves
	for(int i(0); i < 2; i++) {
		SDL_Point nextPos = {pawnPos.x + BlackPawnTakeMoves[i].x, pawnPos.y + BlackPawnTakeMoves[i].y};
		if(isInBounds(nextPos) && isEnemy(map[pawnPos.y][pawnPos.x], map[nextPos.y][nextPos.x])) {
			legalPos.push_back(nextPos);
		}
	}
	// Forward moves, only if selfCheck is false
	if(!(selfCheck)) {
		if(map[pawnPos.y + 1][pawnPos.x] == EMPTY) {
			legalPos.push_back(SDL_Point{pawnPos.x, pawnPos.y + 1});
			if(pawnPos.y == 1 && map[pawnPos.y + 2][pawnPos.x] == EMPTY) {
				legalPos.push_back(SDL_Point{pawnPos.x, pawnPos.y + 2});
			}
		}
	}
    if(mLastMovedPiece == WPAWN && abs(mLastMovedPieceSrcPos.y - mLastMovedPieceDestPos.y) == 2 && mLastMovedPieceDestPos.y == pawnPos.y && abs(mLastMovedPieceDestPos.x - pawnPos.x) == 1) {
        legalPos.push_back(SDL_Point{mLastMovedPieceDestPos.x, mLastMovedPieceDestPos.y + 1});
    }
	// En passant moves need to be added here later.
	return legalPos;
}

void LEngine::setEnPassant(int piece, SDL_Point src, SDL_Point dest) {
    mLastMovedPiece = piece;
    mLastMovedPieceSrcPos = src;
    mLastMovedPieceDestPos = dest;
}

int LEngine::pieceValue(int const pieceType) const {
	switch(pieceType) {
		case BBISHOP: 
			return BISHOP_VALUE;
		case BKING:
			return KING_VALUE;
		case BKNIGHT:
			return KNIGHT_VALUE;
		case BPAWN:
			return PAWN_VALUE;
		case BQUEEN:
			return QUEEN_VALUE;
		case BROOK:
			return ROOK_VALUE;
		case WBISHOP:
			return BISHOP_VALUE;
		case WKING:
			return KING_VALUE;
		case WKNIGHT:
			return KNIGHT_VALUE;
		case WPAWN:
			return PAWN_VALUE;
		case WQUEEN:
			return QUEEN_VALUE;
		case WROOK:
			return ROOK_VALUE;
		default:
			return 0;
	}
}