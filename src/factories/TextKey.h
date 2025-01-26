#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>

struct TextKey {
    std::string text;
    TTF_Font* font;
    SDL_Color color;

    // Equality operator
    bool operator==(const TextKey& other) const {
        return text == other.text && font == other.font &&
               color.r == other.color.r &&
               color.g == other.color.g &&
               color.b == other.color.b &&
               color.a == other.color.a;
    }
};

// Hash function for TextKey
struct TextKeyHash {
    std::size_t operator()(const TextKey& key) const {
        std::size_t hash = std::hash<std::string>()(key.text);
        hash ^= std::hash<TTF_Font*>()(key.font) << 1;
        hash ^= std::hash<uint8_t>()(key.color.r) << 2;
        hash ^= std::hash<uint8_t>()(key.color.g) << 3;
        hash ^= std::hash<uint8_t>()(key.color.b) << 4;
        hash ^= std::hash<uint8_t>()(key.color.a) << 5;
        return hash;
    }
};
