#ifndef LMAINMENU_H
#define LMAINMENU_H

#include "com/headers.h"
#include "com/constantes.h"
#include "com/LButton.h"
#include "graphics/LTextureClickable.h"
#include "com/LSubject.h"
#include "states/LStateMachine.h"
#include "states/menu/LSettingsState.h"
#include "factories/LMediaFactory.h"
#include "sound/LMusicPlayer.h"
#include "playPVAI.h"
// #include "states/game/LBoardState.h"
#include "com/logger.h"
#include "tweeny/include/tweeny.h"
#include "states/LTransition.h"
using tweeny::easing;

struct FPiece {
    int piece;
    SDL_Point pos;
    tweeny::tween<int> tween;
};

class LMainMenu : public LSubject, public LVisualComponent {
public:
    LMainMenu(LObserver* observer);
    void update(Uint64 dt);
    void free() override;
    void render() override;
private:
    bool init();
    void setTexturePositions();

    // Handler operations
    void handleEvents(SDL_Event* e);
    void handleKeyEvents(SDL_Event* e);
    void handleMouseEvents(SDL_Event* e);
    void isNowSelected(int position);

    LTextureClickable* mMenuTextures[TOTAL_MENU_ITEMS];
    LObserver* mAppObserver;
    std::shared_ptr<spdlog::logger> mLogger;

    // Piece textures and clip
    LTexture* mPieceTexture;
    SDL_Rect mPieceClip[TOTAL_PIECES];

    // The vector of piece positions
    std::vector<FPiece> mPieces;
};

#endif

