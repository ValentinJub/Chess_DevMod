/*
LMenu.cpp
by Valentin
--------------
Methods for LMenu class

*/

#include "LMenuSettings.h"

extern SDL_Renderer* gRenderer;
extern LTexture gBackgroundTexture;

LMenu::LMenu(){
    mFont = NULL;
    mMenuTextTextures = new LTexture;
    mPieceTheme1Texture = new LTexture;
    mPieceTheme2Texture = new LTexture;
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        mButtons[i] = new LButton;
    }

    // if the slider is not set to 100 then there will be some issues with
    // the volume calculation after having set the volume once.
    mSlider = new LSlider(100, 25, 320, 271);

    //open font in 28px
    initFont();

    //fill text menu into str[] from 2 txt files
    initMenuStrings();
    
    loadSettingsFromFile();
    
    loadPieceThemeTextures();
    
    loadTextureFromTextLeft();
    loadTextureFromTextRight();
    
    mSound = loadChunk("SoundEffects/START.wav");
    
    //texture have to be created before calling these 2
    setButtonWH();
    setButtonPosition();
    
    mRun = true;
    
}

LMenu::~LMenu(){}
void LMenu::free() {
    mMenuTextTextures->freeLeftTab();
    mMenuTextTextures->freeRightTab();
    mPieceTheme1Texture->free();
    mPieceTheme2Texture->free();
    mSound = NULL;
    mFont = NULL;
}

void LMenu::loadPieceThemeTextures() {
    mPieceTheme1Texture->loadFromFile("Sprites/64/queen0.png");
    mPieceTheme2Texture->loadFromFile("Sprites/64/queen1.png");
}

void LMenu::renderPieceTheme() {
    int const   hPadding = 12,
                startPosX = 243,
                maxPosX = startPosX + 153,
                startPosY = 359 + hPadding,
                piece1Width = mPieceTheme1Texture->getWidth(),
                piece2Width = mPieceTheme2Texture->getWidth();

    mPieceTheme1Texture->render(startPosX, startPosY);
    mPieceTheme2Texture->render(maxPosX - piece2Width, startPosY);
}

void LMenu::initMenuStrings() {
    std::ifstream settingsLeft("resources/setl.config");
    if(settingsLeft.fail()) {
        printf("Unable to load settings file!\n");
    }
    else {
        for(int i(0); i < LEFT_MENU; i++) {
            std::string line;
            std::getline(settingsLeft, line);
            std::cout << line << std::endl;
            mMenuLeftStr[i] = line;
        }
        settingsLeft.close();
    }
    std::ifstream settingsRight("resources/setr.config");
    if(settingsRight.fail()) {
        printf("Unable to load settings file!\n");
    }
    else {
        // fill the mMenuRightStr[] and subtracting 2 for the 2 piece textures
        for(int i(0); i < TOTAL_SETTINGS_RIGHT_MENU_TEXT_ITEMS; i++) {
            std::string line;
            std::getline(settingsRight, line);
            std::cout << line << std::endl;
            mMenuRightStr[i] = line;
        }
        settingsRight.close();
    }
}

void LMenu::initCurrentItemList() {
    mSettingsTable[SLM_YES] = 0;
    mSettingsTable[SLM_NO] = 1;
    mSettingsTable[TL_YES] = 0;
    mSettingsTable[TL_NO] = 1;
    mSettingsTable[TL_5] = 0;
    mSettingsTable[TL_10] = 0;
    mSettingsTable[TC_BROWN] = 0;
    mSettingsTable[TC_GREY] = 1;
    mSettingsTable[MT_JAZZY] = 0;
    mSettingsTable[MT_CLASSIC] = 1;
    mSettingsTable[PT_1] = 1;
    mSettingsTable[PT_2] = 0;
}

/*
bool LMenu::initController() {
    bool success = true;
    mGameController->init();
    if(mGameController->gc() == NULL) {
        printf("failed to init controller! Error %s\n", SDL_GetError());
        success = false;
    }
    return success;

}

*/

void LMenu::initFont() {
    mFont = TTF_OpenFont("resources/branda.ttf", 28);
    if(mFont == NULL) {
        printf("Failed to load resources/valentin font! SDL_ttf Error: %s\n", TTF_GetError());
    }
}

bool LMenu::getRun() const {
    return mRun;
}

void LMenu::underlineSelected() const {
    SDL_SetRenderDrawColor(gRenderer, 0, 0xFF, 0, 0xFF);
    // - 1 because we don't want to underline back
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS - 1; i++) {
        if(mSettingsTable[i] == 1) {
            for(int y(-2); y < 0; y++) {
                //y is the offset to render a thicker line
                SDL_RenderDrawLine(gRenderer, mButtons[i]->getX(), (mButtons[i]->getY() + mButtons[i]->getH() + y), (mButtons[i]->getX() + mButtons[i]->getW()), (mButtons[i]->getY() + mButtons[i]->getH() + y));
            }
        }
    }
}

void LMenu::crossOut() const {
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0xFF);
    // - 1 because we don't want to cross out the back button
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS - 1; i++) {
        if(mSettingsTable[i] == 0) {
            for(int y(-1); y < 1; y++) {
                //y is the offset to render a thicker line
                SDL_RenderDrawLine(gRenderer, mButtons[i]->getX(), ((mButtons[i]->getY() + mButtons[i]->getH() / 2) + y), (mButtons[i]->getX() + mButtons[i]->getW()), ((mButtons[i]->getY() + mButtons[i]->getH() / 2) + y));
            }
        }
    }
}

void LMenu::renderSlider() {
    mSlider->renderSlider();
}

void LMenu::handleSliderMotion(SDL_Point mouse) {
    mSlider->handleMotion(mouse);
}

bool LMenu::getMouseFollow() const {
    return mSlider->getMouseFollow();
}

void LMenu::handleEvent(SDL_Event* e, SDL_Point mouse) {
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

bool LMenu::loadSettingsFromFile() {
    std::ifstream settings;
    settings.open("resources/settings.config", std::ios::in);
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

void LMenu::saveSettingsToFile() {
    std::ofstream settings;
        settings.open("resources/settings.config", std::ios::trunc);
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
/*
SDL_GameController* LMenu::getGameController() {
    return mGameController->gc();
}

LGameController* LMenu::getLGameController() {
    return mGameController;
}

void LMenu::generateText(LGameController* controller){
    mMenuTextTextures->fillScript(controller);
}
*/

void LMenu::loadTextureFromTextLeft() {
    SDL_Color colorBlack = {0,0,0,0xFF};
    mMenuTextTextures->loadFromRenderedTextTabLeft(mMenuLeftStr, mFont, LEFT_MENU, colorBlack);
}
void LMenu::loadTextureFromTextRight() {
    SDL_Color colorBlack = {0,0,0,0xFF};
    mMenuTextTextures->loadFromRenderedTextTabRight(mMenuRightStr, mFont, TOTAL_SETTINGS_RIGHT_MENU_TEXT_ITEMS, colorBlack);
}

void LMenu::renderLeftTexture() {
    mMenuTextTextures->renderFromTabLeftSide(LEFT_MENU);
}

void LMenu::renderRightTexture() {
    mMenuTextTextures->renderFromTabRightSide(TOTAL_SETTINGS_RIGHT_MENU_TEXT_ITEMS);
}

//function used to check the buttons box are set properly
void LMenu::drawButtons() {
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

void LMenu::setButtonPosition() {
    const int   padding(5),
                bigPadding(10),
                center(SCREEN_WIDTH / 2),
                titleHeight(mMenuTextTextures->getLeftHeightTab(0)),
                averageItemHeight(mMenuTextTextures->getRightHeightTab(0)),
                startPosY(titleHeight + bigPadding);

    const int   pieceStartPosX = 243,
                hPadding = 12,
                pieceMaxPosX = pieceStartPosX + 153,
                pieceStartPosY = 359 + hPadding,
                piece1Width = mPieceTheme1Texture->getWidth(),
                piece2Width = mPieceTheme2Texture->getWidth();

    // displays the height of each texture, just for debugging purposes

    for(int y(0); y < 11; ++y) {
        std::cout << "Right texture number: " << y << ", height: " << mMenuTextTextures->getRightHeightTab(y) << std::endl;
    }
    for(int y(0); y < LEFT_MENU; ++y) {
        std::cout << "Left texture number: " << y << ", height: " << mMenuTextTextures->getLeftHeightTab(y) << std::endl;
    }

    // Debug end


    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {

        int previousTextureWidth(0);
        if(i != 0) {
            previousTextureWidth = mMenuTextTextures->getRightWidthTab(i - 1);
        }
        switch(i) {
            case SLM_YES:
                mButtons[i]->setPosition(center, startPosY);
                break;
            case SLM_NO:
                mButtons[i]->setPosition(center + (bigPadding + previousTextureWidth), startPosY);
                break;
            case TL_YES:
                mButtons[i]->setPosition(center, startPosY + (averageItemHeight * 1) + (bigPadding * 1));
                break;
            case TL_NO:
                mButtons[i]->setPosition(center + (bigPadding + previousTextureWidth), startPosY + (averageItemHeight * 1) + (bigPadding * 1));
                break;
            case TL_5:
                mButtons[i]->setPosition(center, startPosY + (averageItemHeight * 2) + (bigPadding * 2));
                break;
            case TL_10:
                mButtons[i]->setPosition(center + (bigPadding + previousTextureWidth), startPosY + (averageItemHeight * 2) + (bigPadding * 2));
                break;
            case TC_BROWN:
                mButtons[i]->setPosition(center, startPosY + (averageItemHeight * 3) + (bigPadding * 3));
                break;
            case TC_GREY:
                mButtons[i]->setPosition(center + (bigPadding + previousTextureWidth), startPosY + (averageItemHeight * 3) + (bigPadding * 3));
                break;
            case MT_JAZZY:
                mButtons[i]->setPosition(center, startPosY + (averageItemHeight * 4) + (bigPadding * 4));
                break;
            case MT_CLASSIC:
                mButtons[i]->setPosition(center + (bigPadding + previousTextureWidth), startPosY + (averageItemHeight * 4) + (bigPadding * 4));
                break;
            case PT_1:
                mButtons[i]->setPosition(pieceStartPosX, pieceStartPosY);
                break;
            case PT_2:
                mButtons[i]->setPosition(pieceMaxPosX - piece2Width, pieceStartPosY);
                break;
            case BACK:
                mButtons[i]->setPosition(padding, SCREEN_HEIGHT - mMenuTextTextures->getRightHeightTab(i - 2));
                break;
            default:
                break;
        }

    }
}

void LMenu::setButtonWH() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        if(i == PT_1) {
            mButtons[i]->setWidthAndHeight(mPieceTheme1Texture->getWidth(), (mPieceTheme1Texture->getHeight()));
        }
        else if(i == PT_2) {
            mButtons[i]->setWidthAndHeight(mPieceTheme2Texture->getWidth(), (mPieceTheme2Texture->getHeight()));
        }
        else if(i == BACK) {
            mButtons[i]->setWidthAndHeight(mMenuTextTextures->getRightWidthTab(i - 2), (mMenuTextTextures->getRightHeightTab(i - 2)));
        }
        else {
            mButtons[i]->setWidthAndHeight(mMenuTextTextures->getRightWidthTab(i), (mMenuTextTextures->getRightHeightTab(i)));
        }
    }
}