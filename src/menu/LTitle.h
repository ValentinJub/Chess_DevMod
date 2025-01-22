#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include "../utils/util.h"
#include "../com/constantes.h"
#include "../LTexture.h"
#include "LMenu.h"

extern TTF_Font* gFont64; 
extern TTF_Font* gFont32;
extern SDL_Renderer* gRenderer;
extern LTexture gBackgroundTexture;


const std::string TITLE_STR = "Chess";
const std::string TITLE_AUTHOR_STR = "by Valentin with Love";


class LTitle : public LMenu {

public:
    LTitle();
    void free();
    bool init();
    void render();
private:
    bool loadTitle();
	Mix_Chunk* mStartupSound;
    LTexture mTitleTexture[TOTAL_TITLE_ITEMS];
};

#endif
