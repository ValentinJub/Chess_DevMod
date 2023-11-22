/*
LMenu.h
by Valentin
--------------
Prototypes and const for LMenuclass

*/
#ifndef LMENUSETTINGS_H
#define LMENUSETTINGS_H

#include "headers.h"
#include "iostream"
#include "sstream"
#include "fstream"
#include "LTexture.h"
#include "LButton.h"
#include "LSlider.h"
#include "funct_headers.h"

class LMenu {
public:
    LMenu();
    ~LMenu();
    void free();
    bool initController();
    void initFont();
    void initCurrentItemList();
    void loadTextureFromTextLeft();
    void loadTextureFromTextRight();
    bool loadSettingsFromFile();    
    void renderLeftTexture();
    void renderRightTexture();
    void renderPieceTheme();
    void renderSlider();
    void setButtonPosition();
    void setButtonWH();
    void handleEvent(SDL_Event* e, SDL_Point mouse); 
    void drawButtons();
    bool getRun() const;
    void outlineSelected() const;
    void underlineSelected() const;
    void crossOut() const;
    bool getMouseFollow() const;
    void handleSliderMotion(SDL_Point mouse);
    
private:
    void loadPieceThemeTextures();
    void saveSettingsToFile(); 
    bool mRun;
    LSlider* mSlider = NULL;
    Mix_Chunk* mSound;
    //Create text menu and put it into mMenuStr[]
    void initMenuStrings();
    //menu textures
    LTexture* mMenuTextTextures;
    LTexture* mPieceTheme1Texture;
    LTexture* mPieceTheme2Texture;
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
