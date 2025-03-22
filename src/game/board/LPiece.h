#ifndef LPIECE_H
#define LPIECE_H

#include "graphics/LTextureClickable.h"

class LPiece : public LTextureClickable {
    private:
        SDL_Point mPos; // Position on the board
        int mType;
        bool mIsHighlighted;
    public:
        LPiece(int x, int y, int w, int h, SDL_Texture* texture, SDL_Rect* clip, LButton* button, SDL_Point pos, int type);
        ~LPiece();
        void setPos(int x, int y);
        void setType(int type);
        void setHighlighted(bool isHighlighted);
        SDL_Point pos() const;
        int type() const;
        bool isHighlighted() const;
        void render();
        void free();
};

#endif