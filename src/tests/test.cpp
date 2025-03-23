#include <gtest/gtest.h>
#include "game/logic/LEngine.h"
#include "game/board/layout.h"
#include "com/constantes.h"

struct Move {
    SDL_Point src;
    SDL_Point dest;
};

struct MoveCase {
    Move move;
    std::string expected;
};

std::vector<MoveCase> PawnNonCapture = {
    // White pawns
    {{{0, 6}, {0, 4}}, "a4"},
    {{{1, 6}, {1, 4}}, "b4"},
    {{{2, 6}, {2, 4}}, "c4"},
    {{{3, 6}, {3, 4}}, "d4"},
    {{{4, 6}, {4, 4}}, "e4"},
    {{{5, 6}, {5, 4}}, "f4"},
    {{{6, 6}, {6, 4}}, "g4"},
    {{{7, 6}, {7, 4}}, "h4"},
    // Black pawns
    {{{0, 1}, {0, 3}}, "a5"},
    {{{1, 1}, {1, 3}}, "b5"},
    {{{2, 1}, {2, 3}}, "c5"},
    {{{3, 1}, {3, 3}}, "d5"},
    {{{4, 1}, {4, 3}}, "e5"},
    {{{5, 1}, {5, 3}}, "f5"},
    {{{6, 1}, {6, 3}}, "g5"},
    {{{7, 1}, {7, 3}}, "h5"}
};

// The fixture for testing class LEngine.
class LEngineTest : public testing::Test {
    protected:
    LEngineTest() {
        // You can do set-up work for each test here.
    }
    
    ~LEngineTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }
    
    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }
    
    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
    
    // Class members declared here can be used by all tests in the test suite
    // for LEngine.
};

TEST_F(LEngineTest, MoveToString) {
    std::array<std::array<int, SPL>, SPL> nboard = normalBoard;
    LEngine engine;
    std::cout << "Testing moveToString" << std::endl;
    for (auto moveCase : PawnNonCapture) {
        std::string result = engine.moveToString(nboard, moveCase.move.src, moveCase.move.dest);
        EXPECT_EQ(result, moveCase.expected);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}