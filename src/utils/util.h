/*
headers.h
Sun Mar 20 2022
by Valentin
-----------
headers for all files - do not put L*.h!!!!
*/

#ifndef HEADER_UTIL
#define HEADER_UTIL

#include "../com/headers.h"

namespace Util {
    bool showTitleScreen(int argc, char *argv[]);
    Mix_Music* loadMusic(const char *src);
    Mix_Chunk* loadChunk(const char *src);
}
#endif