#include "sound/LMusicPlayer.h"

LMusicPlayer* LMusicPlayer::mInstance = 0;
extern LMediaFactory* gMediaFactory;

LMusicPlayer::LMusicPlayer() {
    mVolume = MIX_MAX_VOLUME / 2;
}

LMusicPlayer::~LMusicPlayer() {
    this->free();
}

LMusicPlayer* LMusicPlayer::Instance() {
    if(mInstance == 0) {
        mInstance = new LMusicPlayer();
    }
    return mInstance;
}

void LMusicPlayer::free() {
    this->pause();
}

void LMusicPlayer::play(const char* music) {
    Mix_Music* m = gMediaFactory->getMusic(music);
    if(m != NULL) {
        Mix_PlayMusic(m, -1);
        Mix_VolumeMusic(mVolume);
    }
}

void LMusicPlayer::playFadeIn(const char* music, int ms) {
    Mix_Music* m = gMediaFactory->getMusic(music);
    if(m != NULL) {
        Mix_FadeInMusic(m, -1, ms);
        Mix_VolumeMusic(mVolume);
    }
}

bool LMusicPlayer::isPlaying() const {
    return Mix_PlayingMusic();
}

void LMusicPlayer::pause() {
    if(Mix_PlayingMusic()) {
        Mix_HaltMusic();
    } else {
        Mix_ResumeMusic();
    }
}

void LMusicPlayer::setVolume(uint8_t volume) {
    mVolume = volume;
    Mix_VolumeMusic(mVolume);
}

uint8_t LMusicPlayer::getVolume() const {
    return mVolume;
}