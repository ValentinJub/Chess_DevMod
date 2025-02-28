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
#include "factories/LMediaFactory.h"
#include "iostream"
#include "sstream"
#include "fstream"
#include "graphics/LTextureClickable.h"
#include "com/LButton.h"
#include "graphics/LSlider.h"
#include <array>
#include "com/logger.h"

const std::array<std::string, LEFT_MENU> MENU_LEFT_TEXT = {
    "Settings",
    "Show Legal Moves",
    "Use Timer",
    "Time Limit",
    "Tile Color",
    "Music Theme",
    "Volume",
    "Piece Theme"
};

const std::array<std::string, TOTAL_SETTINGS_RIGHT_MENU_TEXT_ITEMS> MENU_RIGHT_TEXT = {
    "Yes",
    "No",
    "Yes",
    "No",
    "5m",
    "10m",
    "Brown",
    "Grey",
    "Jazzy",
    "Classic",
    "Back.."
};



class LMenuSettings : public LSubject, public LVisualComponent {
public:
    LMenuSettings(LObserver* observer);
    ~LMenuSettings();
    void update();
    void render() override;
    void free() override;

private:
    void initFont();

    void loadTextures();
    void loadLeftSideTextures();
    void loadRightSideTextures();

    void setButtons();
    
    void loadSavedSettings();    
    
    void renderLeftTexture();
    void renderClickableTexture();
    void renderSlider();
    
   
    void handleEvents(SDL_Event* e, SDL_Point mouse); 
    void drawButtons();
    void outlineSelected() const;
    bool getMouseFollow() const;
    void handleSliderMotion(SDL_Point mouse);
   
    void setClickableTexturePosition();
    void setOptionTexturePosition();
    void saveSettingsToFile(); 
    void setBorders();

    bool mShowButtonOutline = false;
    LObserver* mAppObserver;
    LSlider* mSlider = NULL;
    LTextureClickable* mClickableMenuTexture[TOTAL_CLICKABLE_ITEMS]; //13
    LTexture* mOptionMenuTexture[LEFT_MENU]; //7
    TTF_Font* mFont;
    OptionValues mOptionValues;
    std::shared_ptr<spdlog::logger> mLogger;
};

#endif
