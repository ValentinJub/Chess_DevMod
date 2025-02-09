#ifndef LMAINMENU_H
#define LMAINMENU_H

#include "com/headers.h"
#include "com/constantes.h"
#include "LButton.h"
#include "graphics/LTextureClickable.h"
#include "com/LSubject.h"
#include "states/LStateMachine.h"
#include "states/LSettingsState.h"
#include "factories/LMediaFactory.h"
#include "sound/LMusicPlayer.h"
#include "playPVAI.h"
#include "playPVP.h"
#include "com/logger.h"

class LMainMenu : public LSubject, public LVisualComponent {
public:
    LMainMenu(LObserver* observer);
    void update();
    void free() override;
    void render() override;
private:
    bool init();
    void setButtons();
    void setTexturePositions();

    // Handler operations
    void handleEvents(SDL_Event* e);
    void handleKeyEvents(SDL_Event* e);
    void handleMouseEvents(SDL_Event* e);
    void isNowSelected(int position);

    // Misc operations
    void flushEvents();

    SDL_Rect mMiniPieceClip[TOTAL_PIECES - 1]; // 32x32 clipping of the spritesheet
    LTexture* mMiniPieceTexture;
    LTextureClickable* mMenuTextures[TOTAL_MENU_ITEMS];
    LButton* mMenuButtons[TOTAL_MENU_ITEMS];
    LObserver* mAppObserver;
    std::shared_ptr<spdlog::logger> mLogger;
};

#endif

