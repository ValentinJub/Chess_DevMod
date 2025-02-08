#ifndef HEADER_LTEXTURE
#define HEADER_LTEXTURE

#include "utils/util.h"
#include "com/LDecorator.h"
#include <vector>
#include <memory>


class LTexture {
    public:
    LTexture(int x = 0, int y = 0, int w = 0, int h = 0, SDL_Texture* texture = NULL);
    ~LTexture();
    void createImg(SDL_Texture* texture);
    void render( int x = 0, int y = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void renderAuto();
    void free();
    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);

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

    private:
    SDL_Texture* mTexture;
    
    int mX;
    int mY;
    int mWidth;
    int mHeight;
    int mVelX;
    int mVelY;
};

#endif