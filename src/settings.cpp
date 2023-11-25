/*
settings.cpp
Sun Mar 20 2022
by Valentin
-----------
Settings menu allowing to select various options such as:

- Showing legal moves 
- Play with a timer
- Tile/Piece color/design
- Choice of different music? (playing in loop?) or a set of music playing
- etc....
*/

#include "settings.h"

extern SDL_Renderer* gRenderer;
extern LTexture gBackgroundTexture;

void settings() {
	LMenu menuSettings;
	
	SDL_Event e;
	
	while(menuSettings.getRun()) {
		SDL_Point mouse;
		SDL_GetMouseState( &mouse.x, &mouse.y );
		while(SDL_PollEvent(&e) > 0) {
			menuSettings.handleEvent(&e, mouse);
		}

		if(menuSettings.getMouseFollow()) {
			menuSettings.handleSliderMotion(mouse);
		}
	
		SDL_SetRenderDrawColor(gRenderer, 0XFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		
		gBackgroundTexture.render();
		
		menuSettings.renderLeftTexture();
		menuSettings.renderClickableTexture();
		// menuSettings.renderPieceTheme();
		menuSettings.renderSlider();
		
		
		menuSettings.outlineSelected();
		
		// to check if hitbox is set properly
		// menuSettings.drawButtons();
		
		SDL_RenderPresent(gRenderer);
	}
}