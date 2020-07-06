----------Minesweeper!!!----------

---Short Introduction---

The game should hopefully not require much readthrough of instructions as
the applicationtself hosts prompted questions to start and play the game 
in a stepwise approach. Regardless, I hope this readme provides a clear 
summary of the game.

***Assignment details below after instructions***


-----Documentation-----

1. How to play minesweeper


  The game requires you to take educated risks revealling cells of an n*n
  map, where each reveal returns information regarding nearby bombs. Each 
  reveal returns the number of bombs adjacent to that cell. The player can 
  then make their guesses to mark the cells with where they believe a bomb 
  is. Once all the bombs are marked the player wins, and if they uncover a
  bomb during the game, the player loses. "Oh no!".

  Example:

	[0] [1] [2] [2] [1]
	[0] [1] [*] [*] [1]
	[0] [1] [2] [2] [1]
	[1] [1] [0] [0] [0]
	[*] [1] [0] [0] [0]

  Where * denotes a marked bomb and the integers denote the number of adjacent bombs

  a. Unoffical-Algorthim 2
	
	The unnofical algorthim or gamemode differs from the standard form by
	returning the distance of the nearest bomb away rather than the number
	adjacent to a cell.

	Example:

	[2] [2] [2] [2] [2]
	[2] [1] [1] [1] [2]
	[2] [1] [*] [1] [2]
	[2] [1] [1] [1] [2]
	[2] [2] [2] [2] [2]


2. Starting the game

  The application opens with a prompt for the player to either start a new
  game by typing 'n' or 'l'.

  a. Creating a new game

	When a player creates a new game, they will be asked for game options,
	starting with the gametype. The gametype will denote the algorithm to
	use when revealling a cell. Gametype 1 is the offical adjacency value,
	and Gametype 2 is the unoofical distance value.
	They will then be asked for a mapzise which denotes the n cells across
	one dimension of the n*n game board.
	The player will then be asked for the bomb/mine count, which is limited
	between 1 and n*n of the maximum cell count.
	Finally the player will be asked for a savefilename, for any manual
	saving. The game will always be saved each turn to 'autosave.txt'.

  b. Loading a game
	
	Loading a game, the player is asked for a filename of a game within the
	same directory as the application. The game will continue where it was last
	left.

  d. Saving the game

	The game can be saved after each turn with the command 's'. It will save to
	it's given filename at the start of a new game or overwrite a loaded game.

  c. Selecting a cell

	A cell point can now be selected with the command 'p'. The console will then 
	require an x and y co-ordinate.
	The selection can be cancelled with the command 'c'.

  d. Revealing a cell

	Once a cell is selected it can be revealed with the command 'r'.
 	If a bomb is tripped it will end the game.

  e. Marking a cell

	Once a cell is selected it can be marked with an * by the command 'm'.

----------Assignment details and explanation of program----------

  a. Dynamic board size and mines/adjustable console size

	The game is built to ask for map size and mine count, both with deliberate
	limitations. The mapsize is forced to be over 3*3 and under 20*20 and the
	console window will scale to ensure it can be drawn.
	(See void 'ChangeWindowSize')
	Since it requires a window handle, I fear this maybe limited to only windows
	operating systems.
	The mine count will therefore be limited between 1 and the number of cells
	on the board.

  b. Two gamemodes/algorithms

  	At first I made the mistake of assuming I knew how to play minesweeper, 
  	this was not actually the case. I orginally thought it worked by 
  	detecting the proximity to the mines, rather than the count of adjacent
  	bombs. So rather than reinventing it all, the application now contains 2
  	game modes using 2 algorithms.
  	In the source code, algorithm 1 'algo1' denotes the standard minesweeper
  	gamemode, where it returns the number of adjacent bombs to the selected
  	square. 'algo2' denotes the distance algorithm and returns how far away 
	the nearest bomb is.

	Examples:

	[0] [1] [2] [2] [1]
	[0] [1] [*] [*] [1]
	[0] [1] [2] [2] [1]
	[1] [1] [0] [0] [0]
	[*] [1] [0] [0] [0]

	Offical above-algorithm 1
	Unoffical below-algorthim 2

	[2] [2] [2] [2] [2]
	[2] [1] [1] [1] [2]
	[2] [1] [*] [1] [2]
	[2] [1] [1] [1] [2]
	[2] [2] [2] [2] [2]

	Both alogthims share the a function which returns a 2D int vector list
	for the co-ordinates of a sqare xyz distance away from the selected spot.
	Algorithm 2 delcares this function of xyz distance until it finds a bomb
        	and returns that distance for the cell and algorithm 1 declares this
	function of distance 1 and just checks all of those cells for a bomb.
	(See functions 'GetSquare', 'GetAdjacencyValue' and 'GetProximityValue')

  c.  Draw map and mine map seperate/Updates drawmap as you go

	The game doesn't pre-determine the values of the cells but rather uses
	a map for the mines (denoting a mine as 1 and a null value as 0) and uses
	a draw map (intially of '?' question marks) which has a value returned to
	a cell per reveal. 
	This was done to allow for easy saving of the game progress, especially
	with 2 gamemodes.

  d. Save and load ability

	The game can be saved, where every new game has a prompt for the save
	filename into the same directory of the program. The game also autosaves to
	an 'autosave.txt' within the same directory per move.
	The game can be loaded by filename at the launch of the program or end of
	a game when it asks for a new game or to load one.
	The data saved is the gametype, the mapsize, the minecount, the minemap and
	the draw map.
	(See voids 'SaveGame' and 'LoadGame')