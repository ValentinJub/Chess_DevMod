#ifndef LTEXTURECLICKABLE_H
#define LTEXTURECLICKABLE_H

#include "LTexture.h"
#include "LButton.h"

class LTextureClickable : public LTexture {
    public:
        LTextureClickable(int x = 0, int y = 0, int w = 0, int h = 0, SDL_Texture* texture = NULL, LButton* button = NULL);
        ~LTextureClickable();
        void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void setButton(LButton* button);
        void setButton();
        LButton* getButton() const;
        void renderDecorators();
        void setRenderDecorators(bool render);
    private:
        LButton* mButton;
        bool mRenderDecorators = false;
};

#endif
