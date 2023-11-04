#include "LTexture.h"

using std::string;

extern SDL_Renderer* gRenderer;
extern SDL_Surface* gScreenSurface; 
extern SDL_Surface* gCurrentSurface; 
extern SDL_Window* gWindow;

LTexture::LTexture(int x, int y) :
        mX(x),
        mY(y)
    {
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
        for(int i(0); i < LEFT_MENU; i++) {
            mLeftMenuTexture[i] = 0;
        }
        for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
            mRightMenuTexture[i] = 0;
        }
    }

LTexture::~LTexture() {
    free();
    freeLeftTab();
    freeRightTab();
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(TTF_Font* font, string textureText, SDL_Color textColor)
{
    //Get rid of preexisting texture
    free();

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

bool LTexture::loadFromRenderedTextTabLeft(string menuStr[], TTF_Font* font, int size, SDL_Color textColor) {
    //Get rid of preexisting texture
    freeLeftTab();
    bool success = true;
    SDL_Color colorRed = {0xFF, 0, 0, 0xFF};
    SDL_Surface* textSurface = NULL;
    for(int i(0); i < size; i++) {
    //Render text surface
    textSurface = TTF_RenderText_Blended(font, menuStr[i].c_str(), textColor);
    if(textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else {
        //Create texture from surface pixels
        mLeftMenuTexture[i] = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if(mLeftMenuTexture[i] == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Get image dimensions
            mLeftTabWidth[i] = textSurface->w;
            mLeftTabHeight[i] = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
}
    
    //Return success
    return success;
}

bool LTexture::loadFromRenderedTextTabRight(string menuStr[], TTF_Font* font, int size, SDL_Color textColor) {
    //Get rid of preexisting texture
    freeRightTab();
    bool success = true;
    SDL_Color colorRed = {0xFF, 0, 0, 0xFF};
    SDL_Surface* textSurface = NULL;
    for(int i(0); i < size; i++) {
        //Render text surface
        textSurface = TTF_RenderText_Blended(font, menuStr[i].c_str(), textColor);
        if(textSurface == NULL) {
                printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
                success = false;
        }
        if(textSurface !=NULL) {
                //Create texture from surface pixels
                mRightMenuTexture[i] = SDL_CreateTextureFromSurface(gRenderer, textSurface);
                if(mRightMenuTexture[i] == NULL) {
                        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
                        success = false;
                }
                else {
                        //Get image dimensions
                        mRightTabWidth[i] = textSurface->w;
                        mRightTabHeight[i] = textSurface->h;
                }
                //Get rid of old surface
                SDL_FreeSurface(textSurface);
        }
    }
    //Return success
    return success;
}

#endif 

bool LTexture::loadFromFile(std::string path) {
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
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if(clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void LTexture::renderFromTabLeftSide(int tabSize, SDL_Rect* clip) {
    int x(0),
        y(0);
    const int initialVerticalSpace(5);
    const int titlePadding(5);
    const int initialHorizontalSpace(5);
    for(int i(0); i < tabSize; i++) {
        //Settings title is handled differently
        if(i == 0) {
            x = (SCREEN_WIDTH - mLeftTabWidth[i]) / 2;
            y = (initialVerticalSpace + (mLeftTabHeight[0] * i));
        }
        else if(i == 6) {
            x = (SCREEN_WIDTH - mLeftTabWidth[i]) / 2;
            y = ((SCREEN_WIDTH - mLeftTabWidth[i]) / 2) + mLeftTabHeight[i];
        }
        else {
            x = initialHorizontalSpace;
            y = (titlePadding + initialVerticalSpace + (mLeftTabHeight[0] * i));
        }
        SDL_Rect renderQuad = {x, y, mLeftTabWidth[i], mLeftTabHeight[i]};

        //Set clip rendering dimensions
        if(clip != NULL) {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }

        //Render to screen
        SDL_RenderCopy(gRenderer, mLeftMenuTexture[i], clip, &renderQuad);
    }
}

void LTexture::renderFromTabRightSide(int tabSize, SDL_Rect* clip) {
    const int initialVerticalSpace(5);
    const int titlePadding(5);
    const int initialHorizontalSpace(10);
    const int initialSpace(5);
    int leftX(0),
        leftY(0),
        rightX(0),
        rightY(0);
    int ii(1);
    
    for(int i(0); i < tabSize; i+=2) {
        int y(i + 1);
        
        leftX = SCREEN_WIDTH / 2;
        leftY = (initialSpace + titlePadding + (mLeftTabHeight[0] * ii));
        
        //back button
        if(i == 10) {
            leftX = initialHorizontalSpace;
            leftY = SCREEN_HEIGHT - mRightTabHeight[i];
        }
        
        SDL_Rect renderQuad = {leftX, leftY, mRightTabWidth[i], mRightTabHeight[i]};
        
        //Render to screen
        SDL_RenderCopy(gRenderer, mRightMenuTexture[i], clip, &renderQuad);
        
        
        rightX = ((SCREEN_WIDTH / 2) + (mRightTabWidth[i] + initialHorizontalSpace)) ;
        rightY = (initialSpace + titlePadding + (mLeftTabHeight[0] * ii));
        renderQuad = {rightX, rightY, mRightTabWidth[y], mRightTabHeight[y]};
        
        SDL_RenderCopy(gRenderer, mRightMenuTexture[y], clip, &renderQuad);
        
        ii++;
    }
}


void LTexture::free() {
    //Free texture if it exists
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

void LTexture::freeLeftTab() {
    //Free texture if it exists
    for(int i(0); i < LEFT_MENU ; i++) {
        if(mLeftMenuTexture[i] != 0) {
            SDL_DestroyTexture(mLeftMenuTexture[i]);
            mLeftMenuTexture[i] = 0;
            mLeftTabWidth[i] = 0;
            mLeftTabHeight[i] = 0;
        }
    }
}

void LTexture::freeRightTab() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS ; i++) {
        if(mRightMenuTexture[i] != 0) {
            SDL_DestroyTexture(mRightMenuTexture[i]);
            mRightMenuTexture[i] = 0;
            mRightTabWidth[i] = 0;
            mRightTabHeight[i] = 0;
        }
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

int LTexture::getLeftWidthTab(int i) {
    return mLeftTabWidth[i];
}

int LTexture::getLeftHeightTab(int i) {
    return mLeftTabHeight[i];
}

int LTexture::getRightWidthTab(int i) {
    return mRightTabWidth[i];
}

int LTexture::getRightHeightTab(int i) {
    return mRightTabHeight[i];
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
    mX = x;
}

void LTexture::setY(int y) {
    mY = y;
}