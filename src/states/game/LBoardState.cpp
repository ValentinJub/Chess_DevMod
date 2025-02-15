#include "states/game/LBoardState.h"

extern SDL_Renderer* gRenderer;
extern LTexture* gBackgroundTexture;

LBoardState::LBoardState() {
	
}

LBoardState::~LBoardState() {
	mBoard->free();
	mBoard = NULL;
}

void LBoardState::enter(LObserver* observer) {
	mBoard = new LBoardPVP(observer);
}

void LBoardState::exit() {
	mBoard->free();
}

void LBoardState::update() {
	// mBoard->update();
}

void LBoardState::render() {
	// mBoard->render();
}


void playPVP() {
	LBoardPVP board(nullptr);
	//Load tile textures in memory
	if(!(board.loadTileTextures())) {
		spdlog::error("Unable to load tile textures!");
	}
	//Load piece textures in memory
	else if(!(board.loadPiecesTextures())) {
		spdlog::error("Failed to load pieces textures");
	}
	else if(!(board.loadPauseTexture())) {
		spdlog::error("Failed to load pause texture");
	}
	//Read map to initialise piece position
	else if(!(board.initMap())) {
		spdlog::error("Failed to load map");
	}
	else {
		board.setTileRectClip();
		board.setPiecesClip();
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
					//select/unselect/move pieces
					board.handleEvents(&e);
				}
			}
			
			//paint screen white
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);
			
			gBackgroundTexture->render();
			
			//establish possible moves if a piece is selected
			board.showLegalMove();
			
			//display tiles
			board.renderTile();
			//display Pieces 
			board.renderPieces();
			//display timer and checks for time remaining
			board.renderTimer();
			//display score
			board.renderScore();
			//display dead pieces
			board.renderDeadPieces();
			
			//set buttons according to piece pos
			board.setButtons();
			
			if(board.isPaused()) {
				board.renderPause();
			}
			
			SDL_RenderPresent(gRenderer);
			SDL_Delay(16);
			
			//check for victory
			if(board.pollVictory()) {
				//Pay victory sound and quit
				board.playVictorySound();
				quit = true;
			}
			//check for defeat by time out
			if(board.pollTimeOut()) {
				SDL_Delay(1750);
				quit = true;
			}
		}
	}
} //board.close is automatically called here