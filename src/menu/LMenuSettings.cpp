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
    try {
        mLogger = spdlog::basic_logger_mt("LMenuSettings", "logs/CApp.log");
    }
    catch(const spdlog::spdlog_ex& ex) {
        std::cerr << "Log init failed: " << ex.what() << std::endl;
    }
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
    spdlog::drop("LMenuSettings");
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
    this->outlineSelected(); 
}

void LMenuSettings::render() {
    SDL_RenderClear(gRenderer);
    gBackgroundTexture->render();
    this->renderLeftTexture();
    this->renderClickableTexture();
    this->renderSlider();
    if(mShowButtonOutline) {
        this->drawButtons();
    }
    SDL_RenderPresent(gRenderer);
}

void LMenuSettings::initFont() {
    mFont = gMediaFactory->getFont(FONT_BRANDA, 28);
    if(mFont == NULL) {
        mLogger->error("Failed to load LMenuSettings front! SDL_ttf Error: {}", TTF_GetError());
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
    mClickableMenuTexture[SLM_YES]->setRenderDecorators(mOptionValues.showLegalMoves == 0 ? true : false);
    mClickableMenuTexture[SLM_NO]->setRenderDecorators(mOptionValues.showLegalMoves == 1 ? true : false);
    mClickableMenuTexture[TL_YES]->setRenderDecorators(mOptionValues.useTimer == 0 ? true : false);
    mClickableMenuTexture[TL_NO]->setRenderDecorators(mOptionValues.useTimer == 1 ? true : false);
    mClickableMenuTexture[TL_5]->setRenderDecorators(mOptionValues.timeLimit == 0 ? true : false);
    mClickableMenuTexture[TL_10]->setRenderDecorators(mOptionValues.timeLimit == 1 ? true : false);
    mClickableMenuTexture[TC_BROWN]->setRenderDecorators(mOptionValues.tileColor == 0 ? true : false);
    mClickableMenuTexture[TC_GREY]->setRenderDecorators(mOptionValues.tileColor == 1 ? true : false);
    mClickableMenuTexture[MT_JAZZY]->setRenderDecorators(mOptionValues.musicTheme == 0 ? true : false);
    mClickableMenuTexture[MT_CLASSIC]->setRenderDecorators(mOptionValues.musicTheme == 1 ? true : false);
    mClickableMenuTexture[PT_1]->setRenderDecorators(mOptionValues.pieceTheme == 0 ? true : false);
    mClickableMenuTexture[PT_2]->setRenderDecorators(mOptionValues.pieceTheme == 1 ? true : false);
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
        if(mClickableMenuTexture[i]->isHovered() && mClickableMenuTexture[i]->isClicked(e)) {
            switch(i) {
            case SLM_YES:
                mOptionValues.showLegalMoves = 0;
                break;
            case SLM_NO:
                mOptionValues.showLegalMoves = 1;
                break;
            case TL_YES:
                mOptionValues.useTimer = 0;
                break;
            case TL_NO:
                mOptionValues.useTimer = 1;
                break;
            case TL_5:
                mOptionValues.timeLimit = 0;
                break;
            case TL_10:
                mOptionValues.timeLimit = 1;
                break;
            case TC_BROWN:
                mOptionValues.tileColor = 0;
                break;
            case TC_GREY:
                mOptionValues.tileColor = 1;
                break;
            case MT_JAZZY:
                mOptionValues.musicTheme = 0;
                break;
            case MT_CLASSIC:
                mOptionValues.musicTheme = 1;
                break;
            case PT_1:
                mOptionValues.pieceTheme = 0;
                break;
            case PT_2:
                mOptionValues.pieceTheme = 1;
                break;
            case BACK:
                this->saveSettingsToFile();
                gStateMachine->pop();
                break;
            default:
                break;
            }
        }
    }
}

void LMenuSettings::loadSavedSettings() {
    std::vector<int> values = Util::readSettingsFromFile(FILE_SETTINGS);
    if(values.size() < 7) {
        mLogger->error("Settings file is corrupted! {} values found", values.size());
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
    mLogger->info("Settings loaded from file");
}

void LMenuSettings::saveSettingsToFile() {
    int values[LEFT_MENU] = {
        mOptionValues.showLegalMoves,
        mOptionValues.useTimer,
        mOptionValues.timeLimit,
        mOptionValues.tileColor,
        mOptionValues.musicTheme,
        mOptionValues.volume,
        mOptionValues.pieceTheme
    };
    Util::saveSettingsToFile(FILE_SETTINGS, values);
    mLogger->info("Settings saved to file");
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
        mOptionMenuTexture[i]->render();
    }
}

void LMenuSettings::renderClickableTexture() {
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i++) {
        mClickableMenuTexture[i]->render();
    }
}

void LMenuSettings::setOptionTexturePosition() {
     int x(0),
         y(0);
    const int   padding(5),
                bigPadding(10),
                titleHeight(mOptionMenuTexture[0]->h());
    for(int i(0); i < LEFT_MENU; i++) {
        // settings title is handled differently, we display it at the top
        if(i == 0) {
            x = (SCREEN_WIDTH - mOptionMenuTexture[0]->w()) / 2;
            y = (padding);
        }
        // render the piece theme header in the middle of the page
        else if(i == 7) {
            x = (SCREEN_WIDTH -  mOptionMenuTexture[i]->w()) / 2;
            y = y +  mOptionMenuTexture[i]->h() * 2;
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
                y = (bigPadding + titleHeight + (mOptionMenuTexture[i]->h() * (i - 1)) + (bigPadding * (i - 1)));
            }
        }
        mOptionMenuTexture[i]->setPos(x, y);
        mLogger->debug("Set position x: {} y: {} for option texture {}", x, y, MENU_LEFT_TEXT[i]);
    }
}

// set the position of the right hand side textures
void LMenuSettings::setClickableTexturePosition() {
    const int   bigPadding(10),
                titleHeight(mOptionMenuTexture[0]->h()),
                topY(bigPadding + titleHeight),
                textureHeight(mClickableMenuTexture[0]->h());
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
                piece1Width = mClickableMenuTexture[PT_1]->w(),
                piece1Height = mClickableMenuTexture[PT_1]->h(),
                piece2Width = mClickableMenuTexture[PT_2]->w(),
                piece2Height = mClickableMenuTexture[PT_2]->h();
    
    for(int i(0); i < TOTAL_CLICKABLE_ITEMS; i+=2) {
        int y(i + 1),
            leftTabHeight(mClickableMenuTexture[i]->h()),
            leftTabWidth(mClickableMenuTexture[i]->w()),
            rightTabHeight(0),
            rightTabWidth(0);
        if(y < TOTAL_CLICKABLE_ITEMS) {
            rightTabHeight = mClickableMenuTexture[y]->h();
            rightTabWidth = mClickableMenuTexture[y]->w();
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
        if(y < TOTAL_CLICKABLE_ITEMS) {
            mClickableMenuTexture[y]->setPos(rightX, rightY);
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
            mClickableMenuTexture[i]->w(),
            mClickableMenuTexture[i]->h()
        ));
        mLogger->debug("Set button for clickable texture {}", i);
    }
}