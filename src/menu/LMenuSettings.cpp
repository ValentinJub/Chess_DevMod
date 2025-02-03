/*
LMenuSettings.cpp
by Valentin
--------------
Methods for LMenuSettings class

*/

#include "LMenuSettings.h"
#include "com/LBorderDecorator.h"

extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;
extern LStateMachine* gStateMachine;
extern LMediaFactory* gMediaFactory;
extern TTF_Font* gFont64;

LMenuSettings::LMenuSettings(LObserver* observer) : mAppObserver(observer) {
    this->Attach(observer);
    // if the slider is not set to 100 then there will be some issues with
    // the volume calculation after having set the volume once.
    mSlider = new LSlider(100, 25, 320, 271);
    this->initFont();
    this->loadSavedSettings();
    this->loadTextures();
    this->setOptionTexturePosition(); // Sets the position of the option textures
    this->setClickableTexturePosition(); // Sets the position of the clickable textures
    this->setButtons();  // Sets the buttons for the settings menu
    this->setBorders(); // Adds a border decorator to the selectable options
}

LMenuSettings::~LMenuSettings(){}

void LMenuSettings::free() {
    this->Detach(mAppObserver);
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        mClickableMenuTexture[i]->free();
    }
    for(int i(0); i < LEFT_MENU; i++) {
        mOptionMenuTexture[i]->free();
    }
}

void LMenuSettings::update() {
    SDL_Point mouse;
    SDL_GetMouseState( &mouse.x, &mouse.y );
    if(this->getMouseFollow()) {
        this->handleSliderMotion(mouse);
    }
    SDL_Event e;
	while(SDL_PollEvent(&e) > 0) {
		this->handleEvents(&e, mouse);
	}
}

void LMenuSettings::render() {
    SDL_SetRenderDrawColor(gRenderer, 0XFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    gBackgroundTexture->render();
    this->outlineSelected(); 
    this->renderLeftTexture();
    this->renderClickableTexture();
    this->renderSlider();
    if(mShowButtonOutline) {
        this->drawButtons();
    }
    SDL_RenderPresent(gRenderer);
}

void LMenuSettings::initFont() {
    mFont = TTF_OpenFont(FONT_BRANDA, 28);
    if(mFont == NULL) {
        printf("Failed to load resources/valentin font! SDL_ttf Error: %s\n", TTF_GetError());
    }
}

void LMenuSettings::setBorders() {
    const int padding = 2;
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS - 1; i++) {
        mClickableMenuTexture[i]->addDecorator(new LBorderDecorator(
            SDL_Point{mClickableMenuTexture[i]->getButton()->getX() - padding, mClickableMenuTexture[i]->getButton()->getY() - padding},
            SDL_Point{mClickableMenuTexture[i]->getButton()->getX() + mClickableMenuTexture[i]->getButton()->getW() + padding, mClickableMenuTexture[i]->getButton()->getY() - padding},
            SDL_Point{mClickableMenuTexture[i]->getButton()->getX() - padding, mClickableMenuTexture[i]->getButton()->getY() + mClickableMenuTexture[i]->getButton()->getH() + padding},
            SDL_Point{mClickableMenuTexture[i]->getButton()->getX() + mClickableMenuTexture[i]->getButton()->getW() + padding, mClickableMenuTexture[i]->getButton()->getY() + mClickableMenuTexture[i]->getButton()->getH() + padding},
            mClickableMenuTexture[i]->getButton()->getW(),
            mClickableMenuTexture[i]->getButton()->getH()
        ));
    }
}

// renders the border of the selected options in the menu
void LMenuSettings::outlineSelected() const {
    if(mOptionValues.showLegalMoves == 0) {
        mClickableMenuTexture[SLM_YES]->setRenderDecorators(true);
        mClickableMenuTexture[SLM_NO]->setRenderDecorators(false);
    }
    else {
        mClickableMenuTexture[SLM_NO]->setRenderDecorators(true);
        mClickableMenuTexture[SLM_YES]->setRenderDecorators(false);
    }
    if(mOptionValues.useTimer == 0) {
        mClickableMenuTexture[TL_YES]->setRenderDecorators(true);
        mClickableMenuTexture[TL_NO]->setRenderDecorators(false);
    }
    else {
        mClickableMenuTexture[TL_NO]->setRenderDecorators(true);
        mClickableMenuTexture[TL_YES]->setRenderDecorators(false);
    }
    if(mOptionValues.timeLimit == 0) {
        mClickableMenuTexture[TL_5]->setRenderDecorators(true);
        mClickableMenuTexture[TL_10]->setRenderDecorators(false);
    }
    else {
        mClickableMenuTexture[TL_10]->setRenderDecorators(true);
        mClickableMenuTexture[TL_5]->setRenderDecorators(false);
    }
    if(mOptionValues.tileColor == 0) {
        mClickableMenuTexture[TC_BROWN]->setRenderDecorators(true);
        mClickableMenuTexture[TC_GREY]->setRenderDecorators(false);
    }
    else {
        mClickableMenuTexture[TC_GREY]->setRenderDecorators(true);
        mClickableMenuTexture[TC_BROWN]->setRenderDecorators(false);
    }
    if(mOptionValues.musicTheme == 0) {
        mClickableMenuTexture[MT_JAZZY]->setRenderDecorators(true);
        mClickableMenuTexture[MT_CLASSIC]->setRenderDecorators(false);
    }
    else {
        mClickableMenuTexture[MT_CLASSIC]->setRenderDecorators(true);
        mClickableMenuTexture[MT_JAZZY]->setRenderDecorators(false);
    }
    if(mOptionValues.pieceTheme == 0) {
        mClickableMenuTexture[PT_1]->setRenderDecorators(true);
        mClickableMenuTexture[PT_2]->setRenderDecorators(false);
    }
    else {
        mClickableMenuTexture[PT_2]->setRenderDecorators(true);
        mClickableMenuTexture[PT_1]->setRenderDecorators(false);
    }
}

void LMenuSettings::renderSlider() {
    mSlider->renderSlider();
}

void LMenuSettings::handleSliderMotion(SDL_Point mouse) {
    mSlider->handleMotion(mouse);
}

bool LMenuSettings::getMouseFollow() const {
    return mSlider->getMouseFollow();
}

void LMenuSettings::handleEvents(SDL_Event* e, SDL_Point mouse) {
    if(e->type == SDL_QUIT) {
        this->Notify();
    }
    // handles slider events to control the volume
    mSlider->handleEvents(e, mouse);

    if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_x) {
        mShowButtonOutline = !mShowButtonOutline;
    }

    // handles button events for the settings menu 
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        if(mClickableMenuTexture[i]->getButton()->handleInside(e)) {
            switch(i) {
                case SLM_YES:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                        mOptionValues.showLegalMoves = 0;
                    }
                    break;
                case SLM_NO:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                        mOptionValues.showLegalMoves = 1;
                    }
                    break;
                case TL_YES:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                        mOptionValues.useTimer = 0;
                    }
                    break;
                case TL_NO:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                        mOptionValues.useTimer = 1;
                    }
                    break;
                case TL_5:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                        mOptionValues.timeLimit = 0;
                    }
                    break;
                case TL_10:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                        mOptionValues.timeLimit = 1;
                    }
                    break;
                case TC_BROWN:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                        mOptionValues.tileColor = 0;
                    }
                    break;
                case TC_GREY:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                        mOptionValues.tileColor = 1;
                    }
                    break;
                case MT_JAZZY:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                       mOptionValues.musicTheme = 0;
                    }
                    break;
                case MT_CLASSIC:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {                    
                        mOptionValues.musicTheme = 1;
                    }
                    break;
                case PT_1:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                        mOptionValues.pieceTheme = 0;
                    }
                    break;
                case PT_2:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                        mOptionValues.pieceTheme = 1;
                    }
                    break;
                case BACK:
                    if(mClickableMenuTexture[i]->getButton()->handleClick(e)) {
                        saveSettingsToFile();
                        gStateMachine->pop();
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void LMenuSettings::loadSavedSettings() {
    std::ifstream settings;
    settings.open(FILE_SETTINGS, std::ios::in);
    if(!settings.is_open()) {
        printf("Unable to load settings file!\n");
        return;
    }
    std::vector<int> values;
    std::string line;
    while(std::getline(settings, line)) {
        std::stringstream ss(line);
        int a;
        if(ss >> a) {
            values.push_back(a);
        }
    }
    settings.close();
    if(values.size() < 7) {
        std::cerr << "Settings file is corrupted!" << std::endl;
        return;
    }
    mOptionValues = {
        values[0],
        values[1],
        values[2],
        values[3],
        values[4],
        values[5],
        values[6]
    };
}

void LMenuSettings::saveSettingsToFile() {
    std::ofstream settings;
    settings.open(FILE_SETTINGS, std::ios::trunc);
    if(!settings.is_open()) {
        printf("Unable to load settings file!\n");
        return;
    }
    int values[LEFT_MENU] = {
        mOptionValues.showLegalMoves,
        mOptionValues.useTimer,
        mOptionValues.timeLimit,
        mOptionValues.tileColor,
        mOptionValues.musicTheme,
        mOptionValues.volume,
        mOptionValues.pieceTheme
    };
    for(int i(0); i < LEFT_MENU; i++) {
        int a = values[i];
        std::stringstream ss;
        ss << a;
        std::string str = ss.str();
        settings << str + "\n";
    }
    settings.close();
}

void LMenuSettings::loadTextures() {
    this->loadLeftSideTextures();
    this->loadRightSideTextures();
}

void LMenuSettings::loadLeftSideTextures() {
    for(int i(0); i < LEFT_MENU; i++) {
        mOptionMenuTexture[i] = gMediaFactory->getTxt(MENU_LEFT_TEXT[i], i == 0 ? gFont64 : mFont, COLOR_BLACK);
    }
}

void LMenuSettings::loadRightSideTextures() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        if(i == PT_1) {
            mClickableMenuTexture[i] = gMediaFactory->getImgClickable(SPRITE_QUEEN_NORMAL);
        }
        else if(i == PT_2) {
            mClickableMenuTexture[i] = gMediaFactory->getImgClickable(SPRITE_QUEEN_RETRO);
        }
        else {
            mClickableMenuTexture[i] = gMediaFactory->getTxtClickable(MENU_RIGHT_TEXT[i == BACK ? i - 2 : i], mFont, COLOR_BLACK);
        }
    }
}

void LMenuSettings::renderLeftTexture() {
    for(int i(0); i < LEFT_MENU; i++) {
        mOptionMenuTexture[i]->render(mOptionMenuTexture[i]->x(), mOptionMenuTexture[i]->y());
    }
}

void LMenuSettings::renderClickableTexture() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        mClickableMenuTexture[i]->render(mClickableMenuTexture[i]->x(), mClickableMenuTexture[i]->y());
    }
}

void LMenuSettings::setOptionTexturePosition() {
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
        mOptionMenuTexture[i]->setPos(x, y);
    }
}

// set the position of the right hand side textures
void LMenuSettings::setClickableTexturePosition() {
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
        
        mClickableMenuTexture[i]->setPos(leftX, leftY);
        // mClickableTexturePositions[i] = {leftX, leftY, leftTabWidth, leftTabHeight};
        if(y < TOTAL_CLICKABLE_ITEMS) {
            mClickableMenuTexture[y]->setPos(rightX, rightY);
            // mClickableTexturePositions[y] = {rightX, rightY, rightTabWidth, rightTabHeight};
        }
    }
}

//function used to check the buttons box are set properly
void LMenuSettings::drawButtons() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        SDL_Rect r;
        r.x = mClickableMenuTexture[i]->getButton()->getX();
        r.y = mClickableMenuTexture[i]->getButton()->getY();
        r.w = mClickableMenuTexture[i]->getButton()->getW();
        r.h = mClickableMenuTexture[i]->getButton()->getH();
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0, 0, 0xFF);
        SDL_RenderDrawRect(gRenderer, &r);
    }
}

void LMenuSettings::setButtons() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        mClickableMenuTexture[i]->setButton(new LButton(
            mClickableMenuTexture[i]->x(),
            mClickableMenuTexture[i]->y(),
            mClickableMenuTexture[i]->getWidth(),
            mClickableMenuTexture[i]->getHeight()
        ));
    }
}