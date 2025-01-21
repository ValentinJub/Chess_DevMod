#include "LTexture.h"

using std::string;

extern SDL_Renderer* gRenderer;
extern SDL_Surface* gScreenSurface; 
extern SDL_Surface* gCurrentSurface; 
extern SDL_Window* gWindow;
extern TTF_Font* gFont64;

LTexture::LTexture(int x, int y) :
        mX(x),
        mY(y)
    {
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }

LTexture::~LTexture() {
    free();
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(TTF_Font* font, string textureText, SDL_Color textColor, bool isTitle)
{
    //Get rid of preexisting texture
    free();

    if(isTitle) {
        font = gFont64;
    }
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, textureText.c_str(), textColor);
    if(textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if(mTexture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    
    //Return success
    return mTexture != NULL;
}
#endif 

bool LTexture::loadFromFile(std::string path, bool colorKey, Uint8 red, Uint8 green, Uint8 blue) {
    //Get rid of pre-existing texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        //Color key image
        if(colorKey) {
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, red, green, blue));
        }
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    // set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight };

    // set clip rendering dimensions
    if(clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // render to screen
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void LTexture::free() {
    // free texture if it exists
    if(mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
        mX = 0;
        mY = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}
        
void LTexture::setAlpha(Uint8 alpha) {
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}

void LTexture::moveUp() {
    mY -=5;
}

void LTexture::moveDown() {
    mY +=5;
}

void LTexture::moveLeft() {
    mX -=5;
}

void LTexture::moveRight() {
    mX +=5;
}

int LTexture::x() {
    return mX;
}

int LTexture::y() {
    return mY;
}

void LTexture::setX(int x) {
    // std::cout << "Setting x to " << x << std::endl;
    mX = x;
}

void LTexture::setY(int y) {
    // std::cout << "Setting y to " << y << std::endl;
    mY = y;
}