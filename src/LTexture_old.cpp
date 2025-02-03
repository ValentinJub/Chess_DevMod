#include "LTexture_old.h"

using std::string;

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont64;

LTextureOLD::LTextureOLD(int x, int y, int w, int h) :
        mX(x),
        mY(y),
        mWidth(w),
        mHeight(h)
    {
        mTexture = NULL;
    }

LTextureOLD::~LTextureOLD() {
    free();
}

void LTextureOLD::createImg(SDL_Texture* texture) {
    mTexture = texture;
    // SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTextureOLD::loadFromRenderedText(TTF_Font* font, string textureText, SDL_Color textColor, bool isTitle)
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

bool LTextureOLD::loadFromFile(std::string path, bool colorKey, Uint8 red, Uint8 green, Uint8 blue) {
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

void LTextureOLD::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
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

void LTextureOLD::free() {
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

void LTextureOLD::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTextureOLD::setBlendMode(SDL_BlendMode blending) {
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}
        
void LTextureOLD::setAlpha(Uint8 alpha) {
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

int LTextureOLD::getWidth() {
    return mWidth;
}

int LTextureOLD::getHeight() {
    return mHeight;
}

void LTextureOLD::moveUp() {
    mY -=5;
}

void LTextureOLD::moveDown() {
    mY +=5;
}

void LTextureOLD::moveLeft() {
    mX -=5;
}

void LTextureOLD::moveRight() {
    mX +=5;
}

int LTextureOLD::x() {
    return mX;
}

int LTextureOLD::y() {
    return mY;
}

void LTextureOLD::setPos(int x, int y) {
    this->setX(x);
    this->setY(y);
}

void LTextureOLD::setX(int x) {
    // std::cout << "Setting x to " << x << std::endl;
    mX = x;
}

void LTextureOLD::setY(int y) {
    // std::cout << "Setting y to " << y << std::endl;
    mY = y;
}