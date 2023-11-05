# Chess version 0.7 - Devmode

#### Changes applied:

- Deleted unused variables
- Changed the type of some variables to prevent a comparison of different types.
- Fixed a slight error in LboardPvAI where piece2|3|4 were created but not used in the functions.
- Try another font: branda.ttf -> looks good.
- Fixed the music still playing during the pause 
- Devmode menu selection up
- Added a window icon

#### Plan

- Add a devmode in the menu
	- For testing the pawn promotion to Q,R,K,B
- Implement a better pawn promotion system
- Fix the show legal move trailing square
- Add a quick screen when a player lose because of time
- Improve the AI logic, it shouldn't miss to capture important piece nor should it give itself away
- Implement better logging of the AI part.
- Fix the underlining of the settings menu
- Add a music volume selector in the settings
- Replace the GC music with something shorter
	- Add a home screen which would be a top to bottom scroller that gives the impression that chess pieces are falling from the top of the window.
	- Chess pieces falling top to bottom, filling up the screen. When screen is full, next slide.