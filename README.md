# CLUSTER

## - What?
A 1v1 game written in a week time, using C and the MLX42 (GLFW wrapper) library  
The game is made to be played by bots, in a turn based fashion.  
Each turn the bot receives input, and has X miliseconds to respond with a valid move  

Brief description:  
The game is similar to the popular game Connect 4, with the same goal; connect 4 tokens to win  
The twist is this is a hexagonal grid, and it's also possible to rotate this grid, which will make the tokens fall according to the new gravity

## - How?
PREREQUISITES:  
Need to have installed the requirements for MLX42 (found here https://github.com/W2Codam/MLX42)

INSTALLATION:  
`make`

USAGE:  
`./cluster BOT_PROGRAM_PATH1 [BOT_PROGRAM_PATH2]`  
The second path is optional, allowing you to play against a bot manually (without a timeout)

VALID MOVES:  
`PLACE column color`  
`ROTATE cycles`

If an invalid move is made, or a bot times out, the opposing player wins and the game is over.

## Mappings
`direction`  
0 - South  
1 - SouthWest  
2 - NorthWest  
3 - North  
4 - NorthEast  
5 - SouthEast  
## Input data

### Initialization Input

First line: `numberOfCells`  
Next `numberOfCells` lines: 7 space-separated integers:  
- `index` for the cell's index.  
- 6 `neigh` variables, one for each direction, containing the index of a neighboring cell or -1 if is there is no neighbor.  

Next line: An integer `numberOfColors`  
Next `numberOfColors` lines: 4 space-separated integers:
- `index` for the color's index.  
- `maxAmount` for the max amount of pellets of that color.  

### Input for One Game Turn
First line: An integer `direction` (between 0 and 5): the direction of gravity.  
Next line: `numberOfNewPellets`  
next `numberOfNewPellets` lines: 4 space-separated integers:  
- `index`: index of the new pellet.  
- `cellIndex`: index of the cell the pellet is currently on.  
- `colorIndex`: index of the pellet's color.  
- `isMine`: 1 if you are the owner of this pellet, 0 otherwise.

Next line: `numberOfChangedPellets`  
next `numberOfChangedPellets` lines: 2 space-separated integers:  
- `index`: index of the pellet.   
- `cellIndex`: index of the cell the pellet is currently on.  

Next line: `numberOfPelletsInHand`
next `numberOfPelletsInHand` lines: 1 integer:
- `colorIndex`: index of the pellet's color.
