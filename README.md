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

First line: An integer `direction` (between 0 and 5): the direction of gravity.  
Next line: `numberOfCells`  
Next `numberOfCells` lines: 7 space-separated integers:  
`index` for the cell's index.  
6 `neigh` variables, one for each direction, containing the index of a neighboring cell or -1 if is there is no neighbor.  
Next line: An integer `numberOfColors`  
Next `numberOfColors` lines: 4 space-separated integers:
`index` for the color's index.  
`maxAmount` for the max amount of tokens of that color.  


### Input for One Game Turn
<!-- First line: An integer `direction`: the current direction (0-6)
Next line: An integer nutrients: the current nutrient value of the forest.  
Next line: 2 space-separated integers:  
mySun: your current sun points.  
myScore: your current score.  
Next line: 3 space-separated integers:  
oppSun: your opponent's sun points.  
oppScore: your opponent's score.  
oppIsWaiting: equals 1 if your opponent is asleep, 0 otherwise.  
Next line: An integer numberOfTrees for the current number of trees in the forest.  
Next numberOfTrees lines: 4 space-separated integers to describe each tree:  
cellIndex: the index of the cell this tree is on.  
size: the size of the tree. From 0 (seed) to 3 (large tree).  
isMine: 1 if you are the owner of this tree, 0 otherwise.  
isDormant: 1 if this tree is dormant, 0 otherwise.  
Next line: An integer numberOfPossibleActions for the number of legal moves you can make this turn.  
Next numberOfPossibleActions lines: A string possibleAction containing one of the actions you can output this turn.  
This list is provided to help you get started.   -->
