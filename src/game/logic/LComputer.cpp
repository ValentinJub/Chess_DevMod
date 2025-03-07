#include "game/logic/LComputer.h"
#include "game/logic/LEngine.h"
#include "game/logic/pieceSquareTables.h"
#include <algorithm>

LComputer::LComputer(LEngine* engine) : mEngine(engine) {}

LComputer::~LComputer() {
    mEngine = NULL;
}

ChessMove LComputer::playTest(std::array<std::array<int, SPL>, SPL> board, bool isWhite, int depth) {
    // The current score of the board
    int score = this->evaluate(board, isWhite);

    // The list of legal moves
    std::vector<ChessMove> moves = this->getMoves(board, isWhite);
    
    // The best move, we default to a random move in case we can't find one that leads to a better score
    ChessMove bestMove = moves[rand() % moves.size()];
    
    for(auto move : moves) {
        int pieceDst = board[move.dst.y][move.dst.x];
        int pieceSrc = board[move.src.y][move.src.x];


        // Simulate the move
        std::array<std::array<int, SPL>, SPL> newBoard = board;
        newBoard[move.dst.y][move.dst.x] = move.piece;
        newBoard[move.src.y][move.src.x] = EMPTY;
        
        // Get the score of the new board
        int newScore = this->evaluate(newBoard, isWhite);

        // If the new score is better than the current score, we update the move
        if(newScore > score) {
            score = newScore;
            bestMove = move;
        }
    }
    return bestMove;
}

ChessMove LComputer::play(std::array<std::array<int, SPL>, SPL> board, bool isWhite, bool maximazing) {
    ChessMove move = this->minimax(this->buildTree(board, isWhite, 0, 3), maximazing);
    return move;
}

// Play a random move within the list of legal moves
ChessMove LComputer::playRandom(std::array<std::array<int, SPL>, SPL> board, bool isWhite) {
    std::vector<ChessMove> moves = this->getMoves(board, isWhite);
    return moves[rand() % moves.size()];
}

// Play a move by evaluating the score of the board after the move
ChessMove LComputer::playHalfPly(std::array<std::array<int, SPL>, SPL> board, bool isWhite) {
    std::vector<ChessMove> moves = this->getMoves(board, isWhite);
    int high = -100000000;
    std::vector<ChessMove> bestMoves;
    for(auto& move : moves) {
        std::array<std::array<int, SPL>, SPL> newBoard = board;
        newBoard[move.dst.y][move.dst.x] = move.piece;
        newBoard[move.src.y][move.src.x] = EMPTY;
        int score = this->evaluate(newBoard, isWhite);
        if(score >= high) {
            high = score;
            bestMoves.push_back(move);
        }
    }
    return bestMoves[rand() % bestMoves.size()];
}

ChessMove LComputer::minimax(Node* node, bool max) {
    this->minimaxTree(node, max);
    // Look for the child with the highest score
    int high = -100000000;
    Node* theMove = NULL;
    for(auto& child : node->children) {
        if(child->score >= high) {
            high = child->score;
            theMove = child;
        }
    }
    return theMove->move;
}

bool LComputer::minimaxTree(Node* node, bool max) {
    if(node->children.empty()) {
        return max;
    }
    bool state = max;
    // Go to the bottom antipenultimate level of the tree
    for(auto& child : node->children) {
        state = minimaxTree(child, max);
    }
    // If max is true, we want to maximize the score and thus will pick the highest score
    // in the children of the current node and apply it to the current node
    // we then return the inverse of max and carry on
    if(state) {
        int maxScore = -1000000;
        for(auto& child : node->children) {
            if(child->score > maxScore) {
                maxScore = child->score;
            }
        }
        node->score = maxScore;
        return !state;
    }
    else {
        int minScore = 1000000;
        for(auto& child : node->children) {
            if(child->score < minScore) {
                minScore = child->score;
            }
        }
        node->score = minScore;
        return !state;   
    }
}

Node* LComputer::buildTree(std::array<std::array<int, SPL>, SPL> board, bool isWhite, int currentDepth, int maxDepth) {
    if(currentDepth == maxDepth) {
        return new Node(evaluate(board, isWhite));
    }
    Node* node = new Node(evaluate(board, isWhite));
    if(node->score == 1000000 || node->score == -1000000) {
        return node;
    }
    std::vector<ChessMove> moves = getMoves(board, isWhite);
    for(auto& move : moves) {
        std::array<std::array<int, SPL>, SPL> newBoard = board;
        newBoard[move.dst.y][move.dst.x] = move.piece;
        newBoard[move.src.y][move.src.x] = EMPTY;
        Node* child = buildTree(newBoard, !isWhite, currentDepth + 1, maxDepth);
        child->move = move;
        node->children.push_back(child);
    }
    return node;
}

std::vector<ChessMove> LComputer::getMoves(std::array<std::array<int, SPL>, SPL> board, bool isWhite) {
    std::vector<ChessMove> moves;
    for(int y(0); y < SPL; y++) {
        for(int x(0); x < SPL; x++) {
            int piece = board[y][x];
            if(piece == EMPTY) {
                continue;
            }
            else if((isWhite && isWhitePiece(piece)) || (!isWhite && !isWhitePiece(piece))) {
                std::vector<SDL_Point> legalMoves = mEngine->getLegalMoves(board, SDL_Point{x, y});
                for(auto& move : legalMoves) {
                    // Only push the move if it doesn't put the king in check
                    if(!mEngine->isMoveSelfCheck(board, SDL_Point{x, y}, move, isWhite)) {
                        moves.push_back(ChessMove{SDL_Point{x, y}, move, piece});
                    }
                }
            }
        }
    }
    return moves;
}

int LComputer::evaluate(std::array<std::array<int, SPL>, SPL> board, bool isWhite) {
    int score = 0;
    // If we do checkmate on our opponent, it's the best score
    if(mEngine->isCheckMate(board, !isWhite)) {
        return 1000000;
    }
    // If we do checkmate on ourselves, it's the worst score
    if(mEngine->isCheckMate(board, isWhite)) {
        return -1000000;
    }
    // If we are in check, it's a bad score
    if(mEngine->isKingInCheck(board, isWhite)) {
        score += -200;
    }
    // If we put our opponent in check, it's a good score
    if(mEngine->isKingInCheck(board, !isWhite)) {
        score += 200;
    }
    score += this->scorePieces(board, isWhite);
    return score;
}

int LComputer::scorePieces(std::array<std::array<int, SPL>, SPL> board, bool isWhite) {
    int score = 0;
    for(int y(0); y < SPL; y++) {
        for(int x(0); x < SPL; x++) {
            int piece = board[y][x];
            SDL_Point pos = {x, y};
            if(piece == EMPTY) {
                continue;
            }
            if(isWhite && isWhitePiece(piece)) {
                score += mEngine->pieceValue(piece);
            } else if(!isWhite && !isWhitePiece(piece)) {
                score += mEngine->pieceValue(piece);
            }
            score += this->scorePosition(pos, piece, isWhite);
        }
    }
    return score;
}

int LComputer::scorePosition(SDL_Point pos, int piece, bool isWhite) {
    if(isWhite) {
        switch(piece) {
        case WPAWN:
            return pst::wPawnTable[pos.y][pos.x];
        case WKNIGHT:
            return pst::wKnightTable[pos.y][pos.x];
        case WBISHOP:
            return pst::wBishopTable[pos.y][pos.x];
        case WROOK:
            return pst::wRookTable[pos.y][pos.x];
        case WQUEEN:
            return pst::wQueenTable[pos.y][pos.x];
        case WKING:
            return pst::wKingTableMiddle[pos.y][pos.x];
        default:
            return 0;
        }
    } else {
        switch(piece) {
        case BPAWN:
            return pst::bPawnTable[pos.y][pos.x];
        case BKNIGHT:
            return pst::bKnightTable[pos.y][pos.x];
        case BBISHOP:
            return pst::bBishopTable[pos.y][pos.x];
        case BROOK:
            return pst::bRookTable[pos.y][pos.x];
        case BQUEEN:
            return pst::bQueenTable[pos.y][pos.x];
        case BKING:
            return pst::bKingTableMiddle[pos.y][pos.x];
        default:
            return 0;
        }
    }
}