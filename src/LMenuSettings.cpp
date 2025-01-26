/*
LSettings.cpp
by Valentin
--------------
Methods for LSettings class

*/

#include "LMenuSettings.h"

extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;

LSettings::LSettings(){
    mFont = NULL;
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        mButtons[i] = new LButton;
    }
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        mClickableMenuTexture[i] = new LTexture;
    }
    for(int i(0); i < LEFT_MENU; i++) {
        mOptionMenuTexture[i] = new LTexture;
    }

    // if the slider is not set to 100 then there will be some issues with
    // the volume calculation after having set the volume once.
    mSlider = new LSlider(100, 25, 320, 271);

    //open font in 28px
    initFont();

    //fill text menu into str[] from 2 txt files
    initMenuStrings();
    
    // load the saved settings from the config file
    loadSettingsFromFile();

    // load the left hand side menu textures, allows to get the width and height of each texture
    loadTextureFromTextLeft();
    // load the clickable menu textures, allows to get the width and height of each texture
    loadClickableTexture();

    setOptionTexturePosition();
    setClickableTexturePosition();
    
    
    //texture have to be created before calling these 2
    setButtonWH();
    setButtonPosition();
    
    mRun = true;
    
}

LSettings::~LSettings(){}
void LSettings::free() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        delete mButtons[i];
        mClickableMenuTexture[i]->free();
    }
    for(int i(0); i < LEFT_MENU; i++) {
        mOptionMenuTexture[i]->free();
    }
    mFont = NULL;
}

void LSettings::initMenuStrings() {
    std::ifstream settingsLeft(FILE_SETTINGS_LEFT);
    if(settingsLeft.fail()) {
        printf("Unable to load settings file!\n");
    }
    else {
        for(int i(0); i < LEFT_MENU; i++) {
            std::string line;
            std::getline(settingsLeft, line);
            // std::cout << line << std::endl;
            mMenuLeftStr[i] = line;
        }
        settingsLeft.close();
    }
    std::ifstream settingsRight(FILE_SETTINGS_RIGHT);
    if(settingsRight.fail()) {
        printf("Unable to load settings file!\n");
    }
    else {
        // fill the mMenuRightStr[] and subtracting 2 for the 2 piece textures
        for(int i(0); i < TOTAL_SETTINGS_RIGHT_MENU_TEXT_ITEMS; i++) {
            std::string line;
            std::getline(settingsRight, line);
            // std::cout << line << std::endl;
            mMenuRightStr[i] = line;
        }
        settingsRight.close();
    }
}

// void LSettings::initCurrentItemList() {
//     mSettingsTable[SLM_YES] = 0;
//     mSettingsTable[SLM_NO] = 1;
//     mSettingsTable[TL_YES] = 0;
//     mSettingsTable[TL_NO] = 1;
//     mSettingsTable[TL_5] = 0;
//     mSettingsTable[TL_10] = 0;
//     mSettingsTable[TC_BROWN] = 0;
//     mSettingsTable[TC_GREY] = 1;
//     mSettingsTable[MT_JAZZY] = 0;
//     mSettingsTable[MT_CLASSIC] = 1;
//     mSettingsTable[PT_1] = 1;
//     mSettingsTable[PT_2] = 0;
// }

void LSettings::initFont() {
    mFont = TTF_OpenFont(FONT_BRANDA, 28);
    if(mFont == NULL) {
        printf("Failed to load resources/valentin font! SDL_ttf Error: %s\n", TTF_GetError());
    }
}

bool LSettings::getRun() const {
    return mRun;
}


// renders the border of the selected options in the menu
void LSettings::outlineSelected() const {

    // the padding is the space between the border and the texture
    const int padding = 2;

    /* 
        I want to render each corner of the rectangle separately
        There will be 4 points (a, b, c, d) from which the lines will be drawn where:
        - a is posX , posY 
        - b is posX + width , posY
        - c is posX , posY + height
        - d is posX + width , posY + height
        The lines will be drawn:
        - from a to a + width /2 and from a to a + height / 2
        - from b to b - width /2 and from b to b + height / 2 
        - from c to c + width /2 and from c to c - height / 2
        - from d to d - width /2 and from d to d - height / 2
    */

   // we render the border in black
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

    // - 1 because we don't want to outline back nor do we want a segfault, mSettingsTable[12] doesn't exist
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS - 1; i++) {

        // if the option is selected
        if(mSettingsTable[i] == 1) {
            
            // point a is the top left corner of the rectangle
            SDL_Point a = {mButtons[i]->getX() - padding, mButtons[i]->getY() - padding};
            
            // point b is the top right corner of the rectangle
            SDL_Point b = {mButtons[i]->getX() + mButtons[i]->getW() + padding, mButtons[i]->getY() - padding};

            // point c is the bottom left corner of the rectangle
            SDL_Point c = {mButtons[i]->getX() - padding, mButtons[i]->getY() + mButtons[i]->getH() + padding};

            // point d is the bottom right corner of the rectangle
            SDL_Point d = {mButtons[i]->getX() + mButtons[i]->getW() + padding, mButtons[i]->getY() + mButtons[i]->getH() + padding};

            // the offset is the how many times we render the line to make it thicker
            const int offset = 2;

            // the width of the texture:
            int width = mButtons[i]->getW();
            // the height of the texture:
            int height = mButtons[i]->getH();

            // the length of the lines that will be drawn is 1/4 of the width or height, whichever is smaller
            int length = (width < height) ? width / 4 : height / 4;

            for(int j(0); j < offset; j++) {

                // if j > 0 then we increase length by 1 to make the border thicker
                // and we decrease the x,y value by 1 to make the border thicker
                // if(j > 0) {
                //     length++;
                //     a.x--;
                //     a.y--;
                //     b.x++;
                //     b.y--;
                //     c.x--;
                //     c.y++;
                //     d.x++;
                //     d.y++;
                // }
                

                // draw the corners to smooth the angle
                if(j > 0) {
                    SDL_RenderDrawPoint(gRenderer, a.x - j, a.y - j);
                    SDL_RenderDrawPoint(gRenderer, b.x + j, b.y - j);
                    SDL_RenderDrawPoint(gRenderer, c.x - j, c.y + j);
                    SDL_RenderDrawPoint(gRenderer, d.x + j, d.y + j);
                }

                // draw the lines from point a horizontally
                SDL_RenderDrawLine(gRenderer, a.x, a.y - j, a.x + length, a.y - j); 
                // draw the lines from point a vertically
                SDL_RenderDrawLine(gRenderer, a.x - j, a.y, a.x - j, a.y + length);

                // draw the lines from point b horizontally
                SDL_RenderDrawLine(gRenderer, b.x, b.y - j, b.x - length, b.y - j);
                // draw the lines from point b vertically
                SDL_RenderDrawLine(gRenderer, b.x + j, b.y, b.x + j, b.y + length);

                // draw the lines from point c horizontally
                SDL_RenderDrawLine(gRenderer, c.x, c.y + j, c.x + length, c.y + j);
                // draw the lines from point c vertically
                SDL_RenderDrawLine(gRenderer, c.x - j, c.y, c.x - j, c.y - length);

                // draw the lines from point d horizontally
                SDL_RenderDrawLine(gRenderer, d.x, d.y + j, d.x - length, d.y + j);
                // draw the lines from point d vertically
                SDL_RenderDrawLine(gRenderer, d.x + j, d.y, d.x + j, d.y - length);

            }
        }
    }
}

void LSettings::renderSlider() {
    mSlider->renderSlider();
}

void LSettings::handleSliderMotion(SDL_Point mouse) {
    mSlider->handleMotion(mouse);
}

bool LSettings::getMouseFollow() const {
    return mSlider->getMouseFollow();
}

void LSettings::handleEvent(SDL_Event* e, SDL_Point mouse) {
    if(e->type == SDL_QUIT) {
        mRun = false;
    }
    // handles slider events to control the volume
    mSlider->handleEvents(e, mouse);

    // handles button events for the settings menu 
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        if(mButtons[i]->handleInside(e)) {
            switch(i) {
                case SLM_YES:
                    if(mButtons[i]->handleClick(e)) {
                        //Switch on yes and switch off no
                        mSettingsTable[i] = 1;
                        mSettingsTable[i + 1] = 0;
                    }
                    break;
                case SLM_NO:
                    if(mButtons[i]->handleClick(e)) {
                        //Switch on no and switch off yes
                        mSettingsTable[i] = 1;
                        mSettingsTable[i - 1] = 0;
                    }
                    break;
                case TL_YES:
                    if(mButtons[i]->handleClick(e)) {
                        //Switch on yes and switch off no
                        mSettingsTable[i] = 1;
                        mSettingsTable[i + 1] = 0;
                        //Automatically switch on 5"
                        if(mSettingsTable[i] == 1) {
                            mSettingsTable[TL_10] = 0;
                            mSettingsTable[TL_5] = 1;
                        }
                    }
                    break;
                case TL_NO:
                    if(mButtons[i]->handleClick(e)) {
                        mSettingsTable[i] = 1;
                        mSettingsTable[i - 1] = 0;
                        //Automatically switch off both
                        if(mSettingsTable[i] == 1) {
                            mSettingsTable[TL_10] = 0;
                            mSettingsTable[TL_5] = 0;
                        }
                    }
                    break;
                case TL_5:
                    if(mButtons[i]->handleClick(e)) {
                        //Only allow switching if time limit is on
                        if(mSettingsTable[TL_YES] == 1) {
                            mSettingsTable[i] = 1;
                            mSettingsTable[i + 1] = 0;
                        }
                    }
                    break;
                case TL_10:
                    if(mButtons[i]->handleClick(e)) {
                        //Only allow switching if time limit is on
                        if(mSettingsTable[TL_YES] == 1) {
                            mSettingsTable[i] = 1;
                            mSettingsTable[i - 1] = 0;
                        }
                    }
                    break;
                case TC_BROWN:
                    if(mButtons[i]->handleClick(e)) {
                        mSettingsTable[i] = 1;
                        mSettingsTable[i + 1] = 0;
                    }
                    break;
                case TC_GREY:
                    if(mButtons[i]->handleClick(e)) {
                        mSettingsTable[i] = 1;
                        mSettingsTable[i - 1] = 0;
                    }
                    break;
                case MT_JAZZY:
                    if(mButtons[i]->handleClick(e)) {
                        mSettingsTable[i] = 1;
                        mSettingsTable[i + 1] = 0;
                    }
                    break;
                case MT_CLASSIC:
                    if(mButtons[i]->handleClick(e)) {
                        mSettingsTable[i] = 1;
                        mSettingsTable[i - 1] = 0;
                    }
                    break;
                case PT_1:
                    if(mButtons[i]->handleClick(e)) {
                        mSettingsTable[i] = 1;
                        mSettingsTable[i + 1] = 0;
                    }
                    break;
                case PT_2:
                    if(mButtons[i]->handleClick(e)) {
                        mSettingsTable[i] = 1;
                        mSettingsTable[i - 1] = 0;
                    }
                    break;
                case BACK:
                    if(mButtons[i]->handleClick(e)) {
                        mRun = false;
                        saveSettingsToFile();
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

bool LSettings::loadSettingsFromFile() {
    std::ifstream settings;
    settings.open(FILE_SETTINGS, std::ios::in);
    if(settings.is_open()) {
        // - 1 because we don't want to load back
        for(int i(0); i < TOTAL_CLICKABLE_ITEMS - 1; i++) {
            std::string line;
            std::getline(settings, line);
            std::stringstream ss(line);
            int a;
            ss >> a;
            mSettingsTable[i] = a;
        }
        settings.close();
        return true;
    }
    else {
        std::cerr << "Unable to load settings file!\n";
        return false;
    }
}

void LSettings::saveSettingsToFile() {
    std::ofstream settings;
        settings.open(FILE_SETTINGS, std::ios::trunc);
    if(settings.is_open()) {
        // - 1 because we don't want to save back
        for(int i(0); i < TOTAL_CLICKABLE_ITEMS - 1; i++) {
            int a = mSettingsTable[i];
            std::stringstream ss;
            ss << a;
            std::string str = ss.str();
            settings << str + "\n";
        }
        settings.close();
    }
    else {
        printf("Unable to load settings file!\n");
    }
}

void LSettings::loadTextureFromTextLeft() {
    SDL_Color colorBlack = {0,0,0,0xFF};
    // mMenuTextTextures->loadFromRenderedTextTabLeft(mMenuLeftStr, mFont, LEFT_MENU, colorBlack);
    
    for(int i(0); i < LEFT_MENU; i++) {
        if(i == 0) {
            mOptionMenuTexture[i]->loadFromRenderedText(mFont, mMenuLeftStr[i], colorBlack, true);
        } 
        else { 
            mOptionMenuTexture[i]->loadFromRenderedText(mFont, mMenuLeftStr[i], colorBlack);
        }
    }

}
void LSettings::loadClickableTexture() {
    SDL_Color colorBlack = {0,0,0,0xFF};
    // mMenuTextTextures->loadFromRenderedTextTabRight(mMenuRightStr, mFont, TOTAL_SETTINGS_RIGHT_MENU_TEXT_ITEMS, colorBlack);

    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        if(i == PT_1 || i == PT_2) {
            if(i == PT_1) {
                mClickableMenuTexture[i]->loadFromFile(SPRITE_QUEEN_NORMAL);
            }
            else {
                mClickableMenuTexture[i]->loadFromFile(SPRITE_QUEEN_RETRO);
            }
        }
        else if(i  == BACK) {
            mClickableMenuTexture[i]->loadFromRenderedText(mFont, mMenuRightStr[i - 2], colorBlack);
        }
        else {
            mClickableMenuTexture[i]->loadFromRenderedText(mFont, mMenuRightStr[i], colorBlack);
        }
    }
}

void LSettings::renderLeftTexture() {
    // mMenuTextTextures->renderFromTabLeftSide(LEFT_MENU);

    for(int i(0); i < LEFT_MENU; i++) {
        mOptionMenuTexture[i]->render(mOptionTexturePositions[i].x, mOptionTexturePositions[i].y);
    }
}

void LSettings::renderClickableTexture() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        mClickableMenuTexture[i]->render(mClickableTexturePositions[i].x, mClickableTexturePositions[i].y);
    }
}

void LSettings::setOptionTexturePosition() {
     int x(0),
        y(0);
    const int   padding(5),
                bigPadding(10),
                titleHeight(mOptionMenuTexture[0]->getHeight());
    for(int i(0); i < LEFT_MENU; i++) {
        // settings title is handled differently, we display it at the top
        if(i == 0) {
            x = (SCREEN_WIDTH - mOptionMenuTexture[0]->getWidth()) / 2;
            y = (padding);
        }
        // render the piece theme header in the middle of the page
        else if(i == 7) {
            x = (SCREEN_WIDTH -  mOptionMenuTexture[i]->getWidth()) / 2;
            y = y +  mOptionMenuTexture[i]->getHeight() * 2;
        }
        // render the other option headers in a list style format 
        else {
            // we add the title height to the initial space to avoid overlapping
            if(i == 1) {
                x = padding;
                y = (bigPadding + titleHeight);
            }
            else {
                x = padding;
                y = (bigPadding + titleHeight + (mOptionMenuTexture[i]->getHeight() * (i - 1)) + (bigPadding * (i - 1)));
            }
        }
        mOptionTexturePositions[i] = {x, y, mOptionMenuTexture[i]->getWidth(), mOptionMenuTexture[i]->getHeight()};
    }
}

// set the position of the right hand side textures
void LSettings::setClickableTexturePosition() {
    const int   bigPadding(10),
                titleHeight(mOptionMenuTexture[0]->getHeight()),
                topY(bigPadding + titleHeight),
                textureHeight(mClickableMenuTexture[0]->getHeight());
    int leftX(0),
        leftY(0),
        rightX(0),
        rightY(0),
        inc(0);

    // the positionS FOR THE piece texture
    const int   pieceStartPosX = 243,
                hPadding = 12,
                pieceMaxPosX = pieceStartPosX + 153,
                pieceStartPosY = 359 + hPadding,
                piece1Width = mClickableMenuTexture[PT_1]->getWidth(),
                piece1Height = mClickableMenuTexture[PT_1]->getHeight(),
                piece2Width = mClickableMenuTexture[PT_2]->getWidth(),
                piece2Height = mClickableMenuTexture[PT_2]->getHeight();
    
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i+=2) {
        int y(i + 1),
            leftTabHeight(mClickableMenuTexture[i]->getHeight()),
            leftTabWidth(mClickableMenuTexture[i]->getWidth()),
            rightTabHeight(0),
            rightTabWidth(0);
        if(y < TOTAL_CLICKABLE_ITEMS) {
            rightTabHeight = mClickableMenuTexture[y]->getHeight();
            rightTabWidth = mClickableMenuTexture[y]->getWidth();
        }
        
        leftX = SCREEN_WIDTH / 2;
        rightX = ((SCREEN_WIDTH / 2) + (leftTabWidth + bigPadding));

        leftY = topY + (textureHeight * inc) + bigPadding * inc;
        rightY = topY + (textureHeight * inc) + bigPadding * inc;

        if(i == 0) {
            leftY = topY;
            rightY = topY;
        }
        if(i == PT_1) {
            leftX = pieceStartPosX;
            leftY = pieceStartPosY;
            leftTabWidth = piece1Width;
            leftTabHeight = piece1Height;
        }
        if( y == PT_2) {
            rightX = pieceMaxPosX - piece2Width;
            rightY = pieceStartPosY;
            rightTabWidth = piece2Width;
            rightTabHeight = piece2Height;
        }
        if(i == BACK) {
            leftX = bigPadding;
            leftY = SCREEN_HEIGHT - leftTabHeight;
        }

        inc++;
        
        mClickableTexturePositions[i] = {leftX, leftY, leftTabWidth, leftTabHeight};
        if(y < TOTAL_CLICKABLE_ITEMS) {
            mClickableTexturePositions[y] = {rightX, rightY, rightTabWidth, rightTabHeight};
        }
    }
}



//function used to check the buttons box are set properly
void LSettings::drawButtons() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        SDL_Rect r;
        r.x = mButtons[i]->getX();
        r.y = mButtons[i]->getY();
        r.w = mButtons[i]->getW();
        r.h = mButtons[i]->getH();
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0xFF);
        SDL_RenderDrawRect(gRenderer, &r);
    }
}

void LSettings::setButtonPosition() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {  
        mButtons[i]->setPosition(mClickableTexturePositions[i].x, mClickableTexturePositions[i].y);
    }
}

void LSettings::setButtonWH() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        mButtons[i]->setWidthAndHeight(mClickableTexturePositions[i].w, mClickableTexturePositions[i].h);
    }
}