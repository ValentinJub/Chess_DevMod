#include "playPVAI.h"

extern SDL_Renderer* gRenderer;
extern LTexture gBackgroundTexture;

void playerVersusComputer() {
	LBoardPVAI board;
	//Load tile textures in memory
	if(!(board.loadTileTextures())) {
		printf("Unable to load tile textures!");
	}
	//Load piece textures in memory
	else if(!(board.loadPiecesTextures())) {
		printf("Failed to load pieces textures");
	}
	else if(!(board.loadPauseTexture())) {
		printf("Failed to load pause texture");
	}
	//Read map to initialise piece position
	else if(!(board.initMap())) {
		printf("Failed to load map");
	}
	else {
		board.setTileRectClip();
		board.setPiecesClip();
		board.setButtons();
		bool quit = false;
		
		SDL_Event e;
		while(!(quit)) {
			
			board.playMusic();
			
			while(SDL_PollEvent(&e) > 0) {
				if(e.type == SDL_QUIT) {
					quit = true;
				}
				else if(e.type == SDL_KEYDOWN) {
					if(e.key.keysym.sym == SDLK_ESCAPE) {
						quit = true;
					}
					else if(e.key.keysym.sym == SDLK_SPACE) {
						board.pause();
					}
				}
				if(!(board.isPaused())) {
					//select/unselect/move pieces/check for check/mate
					//This is where all calculations happen
					board.handleEvents(&e);
				}
			}
			
			//paint screen white
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);
			
			gBackgroundTexture.render();
			
			//display tiles
			board.renderTile();
			//display Pieces 
			board.renderPieces();
			//display timer
			board.renderTimer();
			//display score
			board.renderScore();
			//display deadpieces
			board.renderDeadPieces();
			
			if(board.isPaused()) {
				board.renderPause();
			}
			
			if(board.isBlackThinking()) {
				board.pauseWhiteTimer();
				SDL_Delay(1000);
				board.stopBlackThinking();
				board.playMoveSound();
				board.unpauseWhiteTimer();
			}
			
			
			
			SDL_RenderPresent(gRenderer);
			SDL_Delay(16);
			
			//check for victory
			board.checkRemainingPieces();
			if(board.pollVictory()) {
				//Pay victory sound and quit
				board.playVictorySound();
				quit = true;
			}
			//check for defeat by time out
			if(board.pollTimeOut()) quit = true;
		}
	}
} //board.close is automatically called here