/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   basic_bot.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/07 19:20:13 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/11 01:12:10 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

// ### Initialization Input

// First line: `numberOfCells`  
// Next `numberOfCells` lines: 7 space-separated integers:  
// - `index` for the cell's index.  
// - 6 `neigh` variables, one for each direction, containing the index of a neighboring cell or -1 if is there is no neighbor.  

// Next line: An integer `yourColors`  
// Next `yourColors` lines: 2 space-separated integers:
// - `index` for the color's index.  
// - `maxAmount` for the max amount of pellets of that color.  

// Next line: An integer `opponentColors`  
// Next `opponentColors` lines: 2 space-separated integers:
// - `index` for the color's index.  
// - `maxAmount` for the max amount of pellets of that color.  



// ### Input for One Game Turn
// First line: An integer `direction` (between 0 and 5): the direction of gravity.  

// Next line: `numberOfValidInsertSlots`  
// next `numberOfValidInsertSlots` lines: 2 space-separated integers  
// - `column`: the column to use for a PLACE command  
// - `cellIndex`: the cell that correspond to the top of the column, according to the current gravity  

// Next line: `numberOfNewPellets`  
// next `numberOfNewPellets` lines: 4 space-separated integers:  
// - `index`: index of the new pellet.  
// - `cellIndex`: index of the cell the pellet is currently on.  
// - `colorIndex`: index of the pellet's color.  
// - `isMine`: 1 if you are the owner of this pellet, 0 otherwise.

// Next line: `numberOfChangedPellets`  
// next `numberOfChangedPellets` lines: 2 space-separated integers:  
// - `index`: index of the pellet.   
// - `cellIndex`: index of the cell the pellet is currently on.  

// Next line: `numberOfPelletsInHand`
// next `numberOfPelletsInHand` lines: 1 integer:
// - `colorIndex`: index of the pellet's color.

int main() {
	// char	buf[50 + 1];
	int fd = open("bot.log", O_CREAT | O_WRONLY | O_TRUNC, 0644);

	int numberOfCells;
	scanf("%d\n", &numberOfCells);dprintf(fd, "numberOfCells %d\n", numberOfCells);
	for (int i = 0; i < numberOfCells; i++) {
		int index;
		int n0,n1,n2,n3,n4,n5;
		scanf("%d %d %d %d %d %d %d\n", &index, &n0, &n1, &n2, &n3, &n4, &n5);dprintf(fd, "index %d n0 %d n1 %d n2 %d n3 %d n4 %d n5 %d\n", index, n0, n1, n2, n3, n4, n5);
	}

	int yourColors;
	scanf("%d\n", &yourColors);dprintf(fd, "yourColors %d\n", yourColors);
	for (int i = 0; i < yourColors; i++) {
		int colorIndex;
		int colorAmount;
		scanf("%d %d\n", &colorIndex, &colorAmount); dprintf(fd, "colorIndex %d colorAmount %d\n", colorIndex, colorAmount);
	}
	int opponentColors;
	scanf("%d\n", &opponentColors);dprintf(fd, "opponentColors %d\n", opponentColors);
	for (int i = 0; i < opponentColors; i++) {
		int colorIndex;
		int colorAmount;
		scanf("%d %d\n", &colorIndex, &colorAmount);dprintf(fd, "colorIndex %d colorAmount %d\n", colorIndex, colorAmount);
	}
	while (true) {
		// char*	line = NULL;
		// size_t	cap = 0;
		// fflush(stdin);
		// fflush(stdout);
		// int ret = getline(&line, &cap, stdin);
		// dprintf(2, "RECEIVED LINE: %s | RET: %d", line, ret);
		// free(line);
	// 	dprintf(2, "NEXT ITERATION\n");
		int direction;
		scanf("%d\n", &direction);dprintf(fd, "direction %d\n", direction);

		int numberOfValidInsertSlots;
		scanf("%d\n", &numberOfValidInsertSlots);dprintf(fd, "numberOfValidInsertSlots %d\n", numberOfValidInsertSlots);
		for (int i = 0; i < numberOfValidInsertSlots; i++) {
			int column, cellIndex;
			scanf("%d %d\n", &column, &cellIndex);dprintf(fd, "column %d cellIndex %d\n", column, cellIndex);
		}

		int numberOfNewPellets;
		scanf("%d\n", &numberOfNewPellets);dprintf(fd, "numberOfNewPellets %d\n", numberOfNewPellets);
		for (int i = 0; i < numberOfNewPellets; i++) {
			int index, cellIndex, colorIndex, isMine;
			scanf("%d %d %d %d\n", &index, &cellIndex, &colorIndex, &isMine);dprintf(fd, "index %d cellIndex %d colorIndex %d isMine %d\n", index, cellIndex, colorIndex, isMine);
		}

		int numberOfChangedPellets;
		scanf("%d\n", &numberOfChangedPellets);dprintf(fd, "numberOfChangedPellets %d\n", numberOfChangedPellets);
		for (int i = 0; i < numberOfChangedPellets; i++) {
			int index, cellIndex;
			scanf("%d %d\n", &index, &cellIndex);dprintf(fd, "index %d cellIndex %d\n", index, cellIndex);
		}

		int numberOfPelletsInHand;
		scanf("%d\n", &numberOfPelletsInHand);dprintf(fd, "numberOfPelletsInHand %d\n", numberOfPelletsInHand);

		//TODO: figure out why this is causing the loop to halt
		// for (int k = 0; k < numberOfPelletsInHand; k++) {
		// 	int colorIndex;
		// 	// dprintf(fd, "INDEX OF PELLETS IN HAND ITERATION: %d\n", k);
		// 	scanf("%d\n", &colorIndex);
		// 	dprintf(fd, "colorIndex %d | %d\n", colorIndex, k);
		// 	// dprintf(fd, "\n");
		// }


	// 	// close(fd);
	// 	// read(STDIN_FILENO, buf, 50);
		fflush(stdout);
		fflush(stdin);
		printf("PLACE 2 2\n");
		dprintf(2, "BOT LOOP\n");
		fflush(stdout);
	}
}
