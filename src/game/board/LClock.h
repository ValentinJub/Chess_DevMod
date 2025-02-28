#ifndef LCLOCK_H
#define LCLOCK_H

#include "com/LTimer.h"
#include "memory"

class LClock {
    public:
        LClock(int timeLimit);
        ~LClock();
        // Starts the clock for white
        void start();
        // Pauses the clock for the current player and starts the clock for the next player
        void next();
        // Pauses the clock for both players
        void pause();
        // Unpauses the clock for both players
        void unpause();
        // Remaining time for white
        int white();
        // Remaining time for black
        int black();
    private:
        std::unique_ptr<LTimer> mWhiteTimer;
        std::unique_ptr<LTimer> mBlackTimer;
        int mTimeLimit;
};



#endif