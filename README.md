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
