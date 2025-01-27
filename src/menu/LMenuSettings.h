/*
LMenuSettings.h
by Valentin
--------------
Prototypes and const for LMenuclass

*/
#ifndef LMENUSETTINGS_H
#define LMENUSETTINGS_H

#include "utils/util.h"
#include "com/constantes.h"
#include "com/LObserver.h"
#include "com/LSubject.h"
#include "states/LStateMachine.h"
#include "iostream"
#include "sstream"
#include "fstream"
#include "LTexture.h"
#include "LButton.h"
#include "LSlider.h"

class LMenuSettings : public LSubject {
public:
    LMenuSettings(LObserver* observer);
    ~LMenuSettings();
    void update();
    void render();
    void free();
    bool initController();
    void initFont();
    void initCurrentItemList();
    void loadTextureFromTextLeft();
    void loadClickableTexture();
    bool loadSettingsFromFile();    
    void renderLeftTexture();
    void renderClickableTexture();
    void renderPieceTheme();
    void renderSlider();
    void setButtonPosition();
    void setButtonWH();
    void handleEvents(SDL_Event* e, SDL_Point mouse); 
    void drawButtons();
    bool getRun() const;
    void outlineSelected() const;
    // deprecated functions
    // void underlineSelected() const;
    // void crossOut() const;
    bool getMouseFollow() const;
    void handleSliderMotion(SDL_Point mouse);
    
private:
    LObserver* mAppObserver;
    void setClickableTexturePosition();
    void setOptionTexturePosition();
    void loadPieceThemeTextures();
    void saveSettingsToFile(); 
    bool mRun;

    // holds the clickable textures position
    SDL_Rect mClickableTexturePositions[TOTAL_CLICKABLE_ITEMS];

    // holds the option menu textures position
    SDL_Rect mOptionTexturePositions[LEFT_MENU];

    LSlider* mSlider = NULL;
    //Create text menu and put it into mMenuStr[]
    void initMenuStrings();
    //menu textures
    LTexture* mClickableMenuTexture[TOTAL_CLICKABLE_ITEMS]; //13
    LTexture* mOptionMenuTexture[LEFT_MENU]; //7
    //menu buttons
    LButton* mButtons[TOTAL_CLICKABLE_ITEMS];
    //font used to render texture
    TTF_Font* mFont;
    //no need including back as it's not a setting
    int mSettingsTable[TOTAL_CLICKABLE_ITEMS - 1]; //minus back
    std::string mMenuLeftStr[LEFT_MENU];
    std::string mMenuRightStr[TOTAL_SETTINGS_RIGHT_MENU_TEXT_ITEMS];
};

#endif
