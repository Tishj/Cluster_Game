/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   starterkit.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/07 19:20:13 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/11 12:43:15 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

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
	int numberOfCells;
	scanf("%d\n", &numberOfCells);
	for (int i = 0; i < numberOfCells; i++) {
		int index;
		int n0,n1,n2,n3,n4,n5;
		scanf("%d %d %d %d %d %d %d\n", &index, &n0, &n1, &n2, &n3, &n4, &n5);
	}

	int numberOfColumns;
	scanf("%d\n", &numberOfColumns);
	for (int i = 0; i < numberOfColumns; i++) {
		int index, d0, d1, d2, d3, d4, d5;
		scanf("%d %d %d %d %d %d %d\n", &index, &d0, &d1, &d2, &d3, &d4, &d5);
	}

	int yourColors;
	// int pellet_types[yourColors * 2];
	scanf("%d\n", &yourColors);
	for (int i = 0; i < yourColors; i++) {
		int colorIndex;
		int colorAmount;
		scanf("%d %d\n", &colorIndex, &colorAmount);
		// pellet_types[colorIndex] = true;
	}
	int opponentColors;
	scanf("%d\n", &opponentColors);
	for (int i = 0; i < opponentColors; i++) {
		int colorIndex;
		int colorAmount;
		scanf("%d %d\n", &colorIndex, &colorAmount);
		// pellet_types[colorIndex] = false;
	}
	while (true) {

		int color_to_play;
		int column_to_play;

		int direction;
		scanf("%d\n", &direction);

		int numberOfValidColumns;
		scanf("%d\n", &numberOfValidColumns);
		for (int i = 0; i < numberOfValidColumns; i++) {
			int column, cellIndex;
			scanf("%d %d\n", &column, &cellIndex);
			column_to_play = column;
		}

		int numberOfNewPellets;
		scanf("%d\n", &numberOfNewPellets);
		for (int i = 0; i < numberOfNewPellets; i++) {
			int index, cellIndex, colorIndex, isMine;
			scanf("%d %d %d %d\n", &index, &cellIndex, &colorIndex, &isMine);
		}

		int numberOfChangedPellets;
		scanf("%d\n", &numberOfChangedPellets);
		for (int i = 0; i < numberOfChangedPellets; i++) {
			int index, cellIndex;
			scanf("%d %d\n", &index, &cellIndex);
		}

		int numberOfPelletsInHand;
		scanf("%d\n", &numberOfPelletsInHand);
		for (int i = 0; i < numberOfPelletsInHand; i++) {
			int colorIndex;
			char newline;
			scanf("%d%c", &colorIndex, &newline);
			color_to_play = colorIndex;
		}
		fflush(stdout);
		fflush(stdin);
		printf("PLACE %d %d\n", column_to_play, color_to_play);
		fflush(stdout);
	}
}
