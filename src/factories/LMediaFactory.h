#ifndef LMEDIAFACTORY_H
#define LMEDIAFACTORY_H

#include "LFactory.h"
#include "TextKey.h"
#include "SDL2/SDL.h"

extern SDL_Renderer* gRenderer;

class LMediaFactory : public LFactory {
public:
    LMediaFactory() {}
    LTexture* getImg(const std::string& path) override;
    LTexture* getTxt(const std::string& text, TTF_Font* font, SDL_Color color) override;
    Mix_Chunk* getChunk(const char* path) override;
    Mix_Music* getMusic(const char* path) override;
    void free() override {
        mImgCache.clear();
        mTxtCache.clear();
        mChunkCache.clear();
        mMusicCache.clear();
    }
    ~LMediaFactory() {}
private:

    struct SDLTextureDeleter {
        void operator()(SDL_Texture* txt) const {
            if(txt != nullptr) {
                SDL_DestroyTexture(txt);
            }
        }
    };

    std::unordered_map<TextKey, std::unique_ptr<SDL_Texture, SDLTextureDeleter>, TextKeyHash> mTxtCache;
    std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> mImgCache;

    struct MixChunkDeleter {
        void operator()(Mix_Chunk* chunk) const {
            Mix_FreeChunk(chunk);
        }
    };

    struct MixMusicDeleter {
        void operator()(Mix_Music* music) const {
            Mix_FreeMusic(music);
        }
    };

    std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, MixChunkDeleter>> mChunkCache;
    std::unordered_map<std::string, std::unique_ptr<Mix_Music, MixMusicDeleter>> mMusicCache;

    SDL_Texture* loadImg(const std::string& path);
    SDL_Texture* loadText(const std::string path, TTF_Font* font, SDL_Color color);
    LTexture* makeLTexture(SDL_Texture* texture);
};

#endif // LMEDIAFACTORY_H