#include "LMediaFactory.h"

LMediaFactory* LMediaFactory::mInstance = nullptr; // Define the static member variable

LMediaFactory* LMediaFactory::Instance() {
    if(mInstance == 0) {
        mInstance = new LMediaFactory();
    }
    return mInstance;
}

LMediaFactory::LMediaFactory() {
    try {
        mLogger = spdlog::basic_logger_mt("LMediaFactory", "logs/CApp.log");
    }
    catch(const spdlog::spdlog_ex& ex) {
        std::cerr << "Log init failed: " << ex.what() << std::endl;
    }
}

SDL_Texture* LMediaFactory::getTxtSDL_Texture(const std::string& text, TTF_Font* font, SDL_Color color) {
    TextKey key = {text, font, color};
    if(mTxtCache.find(key) == mTxtCache.end()) {
        SDL_Texture* newTexture = loadText(text, font, color);
        if(newTexture == NULL) return nullptr;
        mTxtCache[key] = std::unique_ptr<SDL_Texture, LMediaFactory::SDLTextureDeleter>(newTexture);
    }
    return mTxtCache[key].get();
}

LTexture* LMediaFactory::getImg(const std::string& path) {
    if(mImgCache.find(path) == mImgCache.end()) {
        SDL_Texture* newTexture = loadImg(path);
        if(newTexture == NULL) return nullptr;
        mImgCache[path] = std::unique_ptr<SDL_Texture, LMediaFactory::SDLTextureDeleter>(newTexture);
    }
    return this->makeLTexture(mImgCache[path].get());
}

LTexture* LMediaFactory::getImgUnique(const std::string& path) {
    SDL_Texture* newTexture = this->loadImg(path);
    if(newTexture == NULL) return nullptr;
    return this->makeLTexture(newTexture);
}

SDL_Texture* LMediaFactory::getImgSDL_Texture(const std::string& path) {
    if(mImgCache.find(path) == mImgCache.end()) {
        SDL_Texture* newTexture = loadImg(path);
        if(newTexture == NULL) return nullptr;
        mImgCache[path] = std::unique_ptr<SDL_Texture, LMediaFactory::SDLTextureDeleter>(newTexture);
    }
    return mImgCache[path].get();
}

LTextureClickable* LMediaFactory::getImgClickable(const std::string& path) {
    if(mImgCache.find(path) == mImgCache.end()) {
        SDL_Texture* newTexture = loadImg(path);
        if(newTexture == NULL) return nullptr;
        mImgCache[path] = std::unique_ptr<SDL_Texture, LMediaFactory::SDLTextureDeleter>(newTexture);
    }
    return this->makeLTextureClickable(mImgCache[path].get());
}

TTF_Font* LMediaFactory::getFont(const std::string& path, int size) {
    FontKey key = {path, size};
    if(mFontCache.find(key) == mFontCache.end()) {
        TTF_Font* newFont = TTF_OpenFont(path.c_str(), size);
        if(newFont == NULL) return nullptr;
        mFontCache[key] = std::unique_ptr<TTF_Font, LMediaFactory::TTFDeleter>(newFont);
    }
    return mFontCache[key].get();
}

LTexture* LMediaFactory::getTxt(const std::string& text, TTF_Font* font, SDL_Color color) {
    TextKey key = {text, font, color};
    if(mTxtCache.find(key) == mTxtCache.end()) {
        SDL_Texture* newTexture = loadText(text, font, color);
        if(newTexture == NULL) return nullptr;
        mTxtCache[key] = std::unique_ptr<SDL_Texture, LMediaFactory::SDLTextureDeleter>(newTexture);
    
    }
    return this->makeLTexture(mTxtCache[key].get());
}

LTextureClickable* LMediaFactory::getTxtClickable(const std::string& text, TTF_Font* font, SDL_Color color) {
    TextKey key = {text, font, color};
    if(mTxtCache.find(key) == mTxtCache.end()) {
        SDL_Texture* newTexture = loadText(text, font, color);
        if(newTexture == NULL) return nullptr;
        mTxtCache[key] = std::unique_ptr<SDL_Texture, LMediaFactory::SDLTextureDeleter>(newTexture);
    
    }
    return this->makeLTextureClickable(mTxtCache[key].get());
}

Mix_Chunk* LMediaFactory::getChunk(const char* path) {
    if(mChunkCache.find(path) == mChunkCache.end()) {
        Mix_Chunk* newChunk = Mix_LoadWAV(path);
        if(newChunk == NULL) return nullptr;
        mChunkCache[path] = std::unique_ptr<Mix_Chunk, LMediaFactory::MixChunkDeleter>(newChunk);
    }
    return mChunkCache[path].get();
}

Mix_Music* LMediaFactory::getMusic(const char* path) {
    if(mMusicCache.find(path) == mMusicCache.end()) {
        Mix_Music* newMusic = Mix_LoadMUS(path);
        if(newMusic == NULL) return nullptr;
        mMusicCache[path] = std::unique_ptr<Mix_Music, LMediaFactory::MixMusicDeleter>(newMusic);
    }
    return mMusicCache[path].get();
}


SDL_Texture* LMediaFactory::loadImg(const std::string& path) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL) {
        mLogger->error("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return nullptr;
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture == NULL) {
            mLogger->error("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
            return nullptr;
        }
        SDL_FreeSurface(loadedSurface);
        return newTexture;
    }
}

LTexture* LMediaFactory::makeLTexture(SDL_Texture* texture) {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    return new LTexture(0, 0, w, h, texture);
}

LTextureClickable* LMediaFactory::makeLTextureClickable(SDL_Texture* texture) {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    return new LTextureClickable(0, 0, w, h, texture);
}

SDL_Texture* LMediaFactory::loadText(const std::string path, TTF_Font* font, SDL_Color color) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, path.c_str(), color);
    if(textSurface == NULL) {
        mLogger->error("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return nullptr;
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if(newTexture == NULL) {
            mLogger->error("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            return nullptr;
        }
        SDL_FreeSurface(textSurface);
        return newTexture;
    }
}