#ifndef LMUSICPLAYER_H
#define LMUSICPLAYER_H

#include "com/headers.h"
#include "factories/LMediaFactory.h"

class LMusicPlayer {
    public:
        static LMusicPlayer* Instance();
        ~LMusicPlayer();
        void free();
        void play(const char* music);
        void playFadeIn(const char* music, int ms);
        void pause();
        void setVolume(uint8_t volume);
        uint8_t getVolume() const;
        bool isPlaying() const;
    protected:
        LMusicPlayer();
    private:
        static LMusicPlayer* mInstance;
        uint8_t mVolume;
};

#endif

