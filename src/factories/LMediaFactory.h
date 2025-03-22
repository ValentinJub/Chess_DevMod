#ifndef LMEDIAFACTORY_H
#define LMEDIAFACTORY_H

#include "LFactory.h"
#include "TextKey.h"
#include "FontKey.h"
#include "SDL2/SDL.h"
#include "com/logger.h"

extern SDL_Renderer* gRenderer;

class LMediaFactory : public LFactory {
public:
    static LMediaFactory* Instance();
    LTexture* getImg(const std::string& path) override;
    LTexture* getImgUnique(const std::string& path);
    SDL_Texture* getImgSDL_Texture(const std::string& path);
    LTextureClickable* getImgClickable(const std::string& path);
    TTF_Font* getFont(const std::string& path, int size);
    SDL_Texture* getTxtSDL_Texture(const std::string& path, TTF_Font* font, SDL_Color color);
    LTexture* getTxt(const std::string& text, TTF_Font* font, SDL_Color color) override;
    LTextureClickable* getTxtClickable(const std::string& text, TTF_Font* font, SDL_Color color);
    Mix_Chunk* getChunk(const char* path) override;
    Mix_Music* getMusic(const char* path) override;
    void free() override {
        mImgCache.clear();
        mTxtCache.clear();
        mChunkCache.clear();
        mMusicCache.clear();
        mFontCache.clear();
    }
    ~LMediaFactory() {}
protected:
    LMediaFactory();
private:
    static LMediaFactory* mInstance;
    std::shared_ptr<spdlog::logger> mLogger;
    struct SDLTextureDeleter {
        void operator()(SDL_Texture* txt) const {
            if(txt != nullptr) {
                SDL_DestroyTexture(txt);
            }
        }
    };

    std::unordered_map<TextKey, std::unique_ptr<SDL_Texture, SDLTextureDeleter>, TextKeyHash> mTxtCache;
    std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> mImgCache;

    struct TTFDeleter {
        void operator()(TTF_Font* font) const {
            if(font != nullptr) {
                TTF_CloseFont(font);
            }
        }
    };

    std::unordered_map<FontKey, std::unique_ptr<TTF_Font, TTFDeleter>, FontKeyHash> mFontCache;

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
    LTextureClickable* makeLTextureClickable(SDL_Texture* texture);
};

#endif // LMEDIAFACTORY_H