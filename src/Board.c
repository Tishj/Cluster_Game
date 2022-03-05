/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 08:55:31 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/05 15:59:52 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "Board.h"
#include <stddef.h>
#include "Util.h"
#include "Draw.h"
#include "MLX.h"

typedef struct Range {
	size_t start;
	size_t end;
}	Range;

static v2	no_neighbour = {-1,-1};

bool	board_inside(v2 pos) {
	const int col = pos.x;
	const int row = pos.y;

	//Out of bounds
	if (row < 0 || row > 6)
		return false;
	if (col < 0 || col > 6)
		return false;

	if (row >= 2 && row <= 5)
		return true;

	if (row == 0 && col != 3)
		return false;
	if (row == 1 && (col < 1 || col > 5))
		return false;
	if (row == 6 && (col < 2 || col > 4))
		return false;
	return true;
}

//Y offset of neighbours is based on the column being round or not
static const v2* neighbour_offset[] = {
	(v2[]){
		[SIDE_SOUTH] =		{ 0,  1},
		[SIDE_SOUTHWEST] =	{-1,  0},
		[SIDE_NORTHWEST] =	{-1, -1},
		[SIDE_NORTH] =		{ 0, -1},
		[SIDE_NORTHEAST] =	{ 1, -1},
		[SIDE_SOUTHEAST] =	{ 1,  0}
	},
	(v2[]){
		[SIDE_SOUTH] =		{ 0,  1},
		[SIDE_SOUTHWEST] =	{-1,  1},
		[SIDE_NORTHWEST] =	{-1,  0},
		[SIDE_NORTH] =		{ 0, -1},
		[SIDE_NORTHEAST] =	{ 1,  0},
		[SIDE_SOUTHEAST] =	{ 1,  1}
	},
};

void	slot_init(Slot* slot, int row, int col) {
	const v2*	offsets = neighbour_offset[col % 2 == 0];

	slot->position = (v2){col, row};
	slot->color = EMPTY;
	for (size_t i = SIDE_SOUTH; i < SIDE_SIZE; i++) {
		const v2 offset = offsets[i];
		v2 pos = (v2){slot->position.x + offset.x, slot->position.y + offset.y};
		if (board_inside(pos)) {
			slot->neighbours[i] = pos;
		}
		else {
			slot->neighbours[i] = no_neighbour;
		}
	}
}

static const Range ranges[] = {
	[0] = {
		.start = 3,
		.end = 4
	},
	[1] = {
		.start = 1,
		.end = 6
	},
	[2] = {
		.start = 0,
		.end = 7
	},
	[3] = {
		.start = 0,
		.end = 7
	},
	[4] = {
		.start = 0,
		.end = 7
	},
	[5] = {
		.start = 0,
		.end = 7
	},
	[6] = {
		.start = 2,
		.end = 5
	}
};

void	board_render(Board* board, mlx_image_t* target) {
	for (size_t row = 0; row < 7; row++) {
		for (size_t col = ranges[row].start; col < ranges[row].end; col++) {
			draw_slot(&board->map[row][col], target);
		}
	}
}

void	board_init(Board* board) {
	board->side = SIDE_SOUTH;

	for (size_t row = 0; row < 7; row++) {
		for (size_t col = ranges[row].start; col < ranges[row].end; col++) {
			slot_init(&board->map[row][col], row, col);
		}
	}
}
