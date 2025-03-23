#!/bin/bash

# Compile flags
CXX=g++
CFLAGS="-Wall -std=c++14 -ferror-limit=100"
SRC="src/tests/test.cpp src/game/board/layout.cpp src/game/logic/LEngine.cpp src/game/logic/util.cpp"
TARGET="build/test"
LDFLAGS="-lSDL2main -lSDL2 -lgtest -lgtest_main -pthread"
CPPFLAGS="-I/Users/valentin/Dev/Chess_DevMod/src"

# Compile command
$CXX $CFLAGS $CPPFLAGS $SRC -o $TARGET $LDFLAGS

# Run the test
./build/test
