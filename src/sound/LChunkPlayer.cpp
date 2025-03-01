#include "sound/LChunkPlayer.h"

LChunkPlayer* LChunkPlayer::mInstance = nullptr;
extern LMediaFactory* gMediaFactory;

LChunkPlayer::LChunkPlayer() : mVolume(MIX_MAX_VOLUME / 2) {}

LChunkPlayer::~LChunkPlayer() {
    this->free();
}

LChunkPlayer* LChunkPlayer::Instance() {
    if(mInstance == 0) {
        mInstance = new LChunkPlayer();
    }
    return mInstance;
}

void LChunkPlayer::free() {
    Mix_HaltChannel(-1);
}

bool LChunkPlayer::isPlaying() const {
    return Mix_Playing(-1);
}

int LChunkPlayer::play(const char* chunk, int loop) {
    Mix_Chunk* sound = gMediaFactory->getChunk(chunk);
    if(sound != NULL) {
        int chan = Mix_PlayChannel(-1, sound, loop);
        Mix_Volume(chan, mVolume);
        return chan;
    }
    return -1;
}

void LChunkPlayer::stop() {
    Mix_HaltChannel(-1);
}

void LChunkPlayer::setVolume(uint8_t volume) {
    mVolume = volume;
    Mix_VolumeMusic(mVolume);
}

uint8_t LChunkPlayer::getVolume() const {
    return mVolume;
}