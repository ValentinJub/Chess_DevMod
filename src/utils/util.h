/*
headers.h
Sun Mar 20 2022
by Valentin
-----------
headers for all files - do not put L*.h!!!!
*/

#ifndef HEADER_UTIL
#define HEADER_UTIL

#include "com/headers.h"
#include "com/logger.h"

enum LOG_LEVEL {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERR,
    CRITICAL
};

namespace Util {
    bool showTitleScreen(int argc, char *argv[]);
    void initLogger(int argc, char *argv[]);
}

#endif