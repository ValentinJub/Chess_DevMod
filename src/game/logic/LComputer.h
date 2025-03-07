#ifndef LCHESS_LCOMPUTER_H
#define LCHESS_LCOMPUTER_H

#include "com/headers.h"
#include "com/constantes.h"
#include <vector>

struct ChessMove {
    SDL_Point src, dst;
    int piece;
};

struct Node {
    int score;
    ChessMove move;
    std::vector<Node*> children;

    Node(int score) : score(score) {}
    ~Node() {
        for (auto child : children) {
            delete child;
        }
    }
}; 

class Tree {
public:
    Tree(Node* root) : root(root) {};
    ~Tree() {
        delete root;
    };
    Node* root;
};

class LEngine;

class LComputer {
    public:
        LComputer(LEngine* engine);
        ~LComputer();
        ChessMove play(std::array<std::array<int, SPL>, SPL> board, bool isWhite, bool maximizing);
        ChessMove playRandom(std::array<std::array<int, SPL>, SPL> board, bool isWhite);
        ChessMove playHalfPly(std::array<std::array<int, SPL>, SPL> board, bool isWhite);
        ChessMove playFullPlies(std::array<std::array<int, SPL>, SPL> board, bool isWhite);

        ChessMove playTest(std::array<std::array<int, SPL>, SPL> board, bool isWhite, int depth);
    private:
        LEngine* mEngine;
        // https://www.chessprogramming.org/Simplified_Evaluation_Function
        int evaluate(std::array<std::array<int, SPL>, SPL> board, bool isWhite);
        int scorePieces(std::array<std::array<int, SPL>, SPL> board, bool isWhite);
        int scorePosition(SDL_Point pos, int piece, bool isWhite);
        // Get all possible moves for the current player
        std::vector<ChessMove> getMoves(std::array<std::array<int, SPL>, SPL> board, bool isWhite);
        // build the tree of possible moves
        Node* buildTree(std::array<std::array<int, SPL>, SPL> board, bool isWhite, int currentDepth, int maxDepth);
        // https://en.wikipedia.org/wiki/Minimax#Pseudocode
        // Minimax algorithm to find the move to play
        ChessMove minimax(Node* node, bool min);
        // minimax the tree, recurse power
        bool minimaxTree(Node* node, bool min);
};

#endif