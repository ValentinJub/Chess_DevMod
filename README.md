### To remember!!

- I set the 5 minutes timer to 3 seconds for test, remember to switch back to 5 minutes.
- I disabled the intro screen
#### Changes applied:

- Deleted unused variables
- Changed the type of some variables to prevent a comparison of different types.
- Fixed a slight error in LboardPvAI where piece2|3|4 were created but not used in the functions.
- Try another font: branda.ttf -> looks good.
- Fixed the music still playing during the pause 
- Added a window icon
- Added a game over screen when white runs out of time 
- Fixed the timer displaying -1 when lost for LBoardPVAI.cpp
- Fix the timer displaying -1 when lost for LBoardPVP.cpp
- Added a game over menu for out of time for PVP
- Add a devmode in the menu -> Button/Texture added
- Fixed the show legal move trailing square
- The settings menu shouldn't revert to default each time it is open.
- Fixed the right click not working in PVAI
- Fixed the right click in PVP 


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

![Alt text](image.png)