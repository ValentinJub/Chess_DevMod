#ifndef LTEXTURECLICKABLE_H
#define LTEXTURECLICKABLE_H

#include "graphics/LTexture.h"
#include "com/LButton.h"

class LTextureClickable : public LTexture {
    public:
        LTextureClickable(int x = 0, int y = 0, int w = 0, int h = 0, SDL_Texture* texture = NULL, LButton* button = NULL);
        ~LTextureClickable();
        bool isHovered();
        bool isClicked(SDL_Event* e);
        void setButton(LButton* button);
        void setButton();
        LButton* getButton() const;
    private:
        LButton* mButton;
};

#endif
