#ifndef HEADER_LTEXTURE
#define HEADER_LTEXTURE

#include "headers.h"


class LTexture {
    public:
    LTexture(int x = 0, int y = 0);
    ~LTexture();
    #if defined(SDL_TTF_MAJOR_VERSION)
    ////Creates image from font string
    bool loadFromRenderedText(TTF_Font* font, std::string textureText, SDL_Color textColor);
    #endif 
    bool loadFromRenderedTextTabLeft(std::string menuStr[], TTF_Font* font, int tabSize, SDL_Color textColor);
    bool loadFromRenderedTextTabRight(std::string menuStr[], TTF_Font* font, int tabSize, SDL_Color textColor);
    //Render at given pos, clip, rotate flip
    void render( int x = 0, int y = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void renderFromTabLeftSide(int tabSize, SDL_Rect* clip = NULL);
    void renderFromTabRightSide(int tabSize, SDL_Rect* clip = NULL);
    //Load IMG from file
    bool loadFromFile(std::string path);
    //Destroy texture 
    void free();
    void freeLeftTab();
    void freeRightTab();
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
    int getLeftWidthTab(int i);
    int getLeftHeightTab(int i);
    int getRightWidthTab(int i);
    int getRightHeightTab(int i);

    private:
    SDL_Texture* mTexture;
    
    SDL_Texture* mLeftMenuTexture[LEFT_MENU]; //7
    SDL_Texture* mRightMenuTexture[TOTAL_CLICKABLE_ITEMS - 2]; //11
    std::string mMenuStr[PLACEHOLDER];
    int mLeftTabWidth[LEFT_MENU];
    int mLeftTabHeight[LEFT_MENU];
    int mRightTabWidth[TOTAL_CLICKABLE_ITEMS - 2]; //11
    int mRightTabHeight[TOTAL_CLICKABLE_ITEMS - 2];
    
    int mHeight;
    int mWidth;
    int mX;
    int mY;
};

#endif