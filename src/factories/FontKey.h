#ifndef FONTKEY_H
#define FONTKEY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>

struct FontKey {
    std::string font;
    int size;

    // Equality operator
    bool operator==(const FontKey& other) const {
        return font == other.font && size == other.size;
    }
};

// Hash function for FontKey
struct FontKeyHash {
    std::size_t operator()(const FontKey& key) const {
        std::size_t hash = std::hash<std::string>()(key.font);
        hash ^= std::hash<std::string>()(key.font) << 1;
        hash ^= std::hash<int>()(key.size) << 2;
        return hash;
    }
};

#endif