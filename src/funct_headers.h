/*
funct_headers.h
Sun Mar 20 2022
by Valentin
-----------
Prototypes for funct.cpp 
*/

#ifndef HEADER_FUNCTHEADER
#define HEADER_FUNCTHEADER

#include "headers.h"
#include "LTexture.h"
#include "LButton.h"

bool loadTitle(LTexture titleTexture[]);
bool loadMenu(LTexture menuTextures[]);
void loadMenuHighlight(LTexture menuTexture[], int position = 3);
Mix_Chunk* loadChunk(const char *src);
Mix_Music* loadMusic(const char *src);
void displayTitle(LTexture titleTexture[]);
void displayMenu(LTexture menuTexture[]);
void setButtons(LButton buttons[], LTexture textures[]);
void unsetButtons(LButton menuButtons[TOTAL_MENU_ITEMS]);
void renderWIP();
void wait10s();
void flushEvents();

#endif 