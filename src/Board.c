/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 08:55:31 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/05 09:40:31 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Board.h"

bool	board_inside(v2 pos) {
	const int col = pos.x;
	const int row = pos.y;

	//Out of bounds
	if (row < 0 || row > 6)
		return false;
	if (col < 0 || col > 6)
		return false;

	//Anything other than the first 3 rows (0,1,2) is valid at this point
	if (row >= 3)
		return true;

	if (row == 0 && col != 3)
		return false;
	if (row == 1 && (col < 2 || col > 4))
		return false;
	if (row == 2 && (col < 1 || col > 5))
		return false;
	return true;
}

static const v2 neighbour_offset[] = {
	[SOUTH] =		{0,1},
	[SOUTHWEST] =	{-1,1},
	[NORTHWEST] =	{-1,0},
	[NORTH] =		{0,-1},
	[NORTHEAST] =	{1,0},
	[SOUTHEAST] =	{1,1}
};

void	slot_init(Slot* slot, int row, int col) {
	slot->position = v2(col, row);
	slot->color = EMPTY;
	for (size_t i = SIDE_SOUTH; i < SIDE_SIZE; i++) {
		const v2 offset = neighbour_offset[i];
		v2 pos = (v2){slot->position.x + offset.x, slot->position.y + offset.y};
		if (board_inside(pos)) {
			slot->neighbours[i] = pos;
		}
		else {
			slot->neighbours[i] = (v2){-1,-1};
		}
	}
}

void	board_init(Board* board) {
	board->side = SIDE_SOUTH;

	for (size_t row = 0; row < 7; row++) {
		if (row == 0) {
			slot_init(&board->slots[row][3]);
		}
		else if (row == 1) {
			for (size_t col = 2; col < 5; col++) {
				slot_init(&board->slots[row][col]);
			}
		}
		else if (row == 2) {
			for (size_t col = 1; col < 6; col++) {
				slot_init(&board->slots[row][col]);
			}
		}
		else {
			for (size_t col = 0; col < 7; col++) {
				slot_init(&board->slots[row][col]);
			}
		}
	}
}
