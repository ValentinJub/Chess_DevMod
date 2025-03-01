#ifndef LCHUNKPLAYER_H
#define LCHUNKPLAYER_H

#include "com/headers.h"
#include "factories/LMediaFactory.h"

class LChunkPlayer {
public:
    static LChunkPlayer* Instance();
    ~LChunkPlayer();
    void free();
    int play(const char* chunk, int loop = 0);
    void stop();
    bool isPlaying() const;
    void setVolume(uint8_t volume);
    uint8_t getVolume() const;

protected:
    LChunkPlayer();

private:
    static LChunkPlayer* mInstance; 
    uint8_t mVolume;
};

#endif // LCHUNKPLAYER_H


