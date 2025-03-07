# Launch flags

## Optional flags
- '-h' will hide the intro screen
- '-l' + '<trace|debug|info|warn|error|critical>' will set the log level


### To remember!!

- I set the 5 minutes timer to 3 seconds for test, remember to switch back to 5 minutes.

#### Changes applied:

- Add a devmode in the menu -> Button/Texture added
- Added a game over menu for out of time for PVP
- Added a game over screen when white runs out of time 
- Added a window icon
- Changed the type of some variables to prevent a comparison of different types.
- Deleted unused variables
- Fix the timer displaying -1 when lost for LBoardPVP.cpp
- Fixed a slight error in LboardPvAI where piece2|3|4 were created but not used in the functions.
- Fixed the music still playing during the pause 
- Fixed the right click in PVP 
- Fixed the right click not working in PVAI
- Fixed the show legal move trailing square
- Fixed the timer displaying -1 when lost for LBoardPVAI.cpp
- The settings menu shouldn't revert to default each time it is open.
- Try another font: branda.ttf -> looks good.


#### Plan
- Add stalemate
- Review show legal move results, it seems like it is adding more than it should
- Add a win screen when checkmate
- Implement a better pawn promotion system
- Add a devmode in the menu ->  Code the logic and the mode.
	- For testing the pawn promotion to Q,R,K,B
	- For any type of testing.
- Improve the AI logic, it shouldn't miss to capture important piece nor should it give itself away
- Implement better logging of the AI part.
- Fix the underlining of the settings menu

- Replace the GC music with something shorter
	- Add a home screen which would be a top to bottom scroller that gives the impression that chess pieces are falling from the top of the window.
	- Chess pieces falling top to bottom, filling up the screen. When screen is full, next slide?

- Add a music volume selector in the settings
	- Use a horizontal bar and a circle, the circle can slide left to right limited by the horizontal bar x position.
