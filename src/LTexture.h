#ifndef HEADER_LTEXTURE
#define HEADER_LTEXTURE

#include "headers.h"


class LTexture {
    public:
    LTexture(int x = 0, int y = 0);
    ~LTexture();
    #if defined(SDL_TTF_MAJOR_VERSION)
    ////Creates image from font string
    bool loadFromRenderedText(TTF_Font* font, std::string textureText, SDL_Color textColor, bool isTitle = false);
    #endif 
    //Render at given pos, clip, rotate flip
    void render( int x = 0, int y = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    //Load IMG from file
    bool loadFromFile(std::string path, bool colorKey = false, Uint8 red = 0x00, Uint8 green = 0xFF, Uint8 blue = 0xFF);
    //Destroy texture 
    void free();
    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    //Set blending
    void setBlendMode(SDL_BlendMode blending);
    //Set Alpha 
    void setAlpha(Uint8 alpha);

    //Move texture up
    void moveUp(); 
    void moveDown(); 
    void moveLeft(); 
    void moveRight(); 
    void setX(int x);
    void setY(int y);
    int x(); 
    int y(); 

    //Gets image dimensions
    int getWidth();
    int getHeight();

    private:
    SDL_Texture* mTexture;
    
    int mHeight;
    int mWidth;
    int mX;
    int mY;
};

#endif