#ifndef HEADER_LTEXTURE
#define HEADER_LTEXTURE

#include "utils/util.h"
#include "com/LDecorator.h"
#include "graphics/LVisualComponent.h"
#include <vector>
#include <memory>


class LTexture : public LVisualComponent {
    public:
    LTexture(int x = 0, int y = 0, int w = 0, int h = 0, SDL_Texture* texture = NULL, SDL_Rect* rect = NULL);
    ~LTexture();
    void createImg(SDL_Texture* texture);
    void render() override;
    void renderAt(int x, int y, SDL_Rect* clip = NULL);
    void free() override;
    void setClip(SDL_Rect* clip);
    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);
    void setRenderDecorators(bool render);

    void addDecorator(LDecorator* decorator);
    void removeDecorator(LDecorator* decorator);
    void renderDecorators();
    std::vector<std::unique_ptr<LDecorator>>& getDecorators();

    void setPos(int x, int y);
    void setX(int x);
    void setY(int y);

    int x(); 
    int y(); 
    int w();
    int h();
    SDL_Texture* getTexture();
    std::vector<std::unique_ptr<LDecorator>> mDecorators;
    
    protected:
    SDL_Rect* mClip;
    int mWidth;
    int mHeight;
    SDL_Texture* mTexture;


    private:
    bool mRenderDecorators = false;
    int mX;
    int mY;
    int mVelX;
    int mVelY;
};

#endif