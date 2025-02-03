#ifndef LFACTORY_H
#define LFACTORY_H

#include "LTextureClickable.h"
#include "utils/util.h"
#include <string>
#include <unordered_map>
#include <memory>

class LFactory {
public:
    virtual LTexture* getImg(const std::string& path) = 0;
    virtual LTexture* getTxt(const std::string& text, TTF_Font* font, SDL_Color color) = 0;
    virtual Mix_Chunk* getChunk(const char* path) = 0;
    virtual Mix_Music* getMusic(const char* path) = 0;
    virtual void free() = 0;
};

#endif // LFACTORY_H