/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 08:55:31 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 12:57:48 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "Board.h"
#include <stddef.h>
#include "util.h"
#include "Draw.h"
#include "MLX.h"
#include "Error.h"
#include <assert.h>
#include "vector.h"
#include <strings.h>
#include <string.h>
#include "Sort.h"

static v2	no_neighbour = {-1,-1};

//'lower_f' prototype
bool	board_closer_to_bottom(void* a, void* b, void* extra) {
	Slot*	slot_a = *(Slot**)a;
	Slot*	slot_b = *(Slot**)b;

	// printf("Slot_A: %p\n", slot_a);
	// printf("Slot_B: %p\n", slot_b);

	BoardSide	side = *(BoardSide*)extra;
	switch (side) {
		case SIDE_SOUTH: {
			return (slot_a->position.y > slot_b->position.y);
		};
		case SIDE_SOUTHWEST: {
			return (slot_a->position.x < slot_b->position.x);
		};
		case SIDE_NORTHWEST: {
			return (slot_a->position.x < slot_b->position.x);
		};
		case SIDE_NORTH: {
			return (slot_a->position.y < slot_b->position.y);
		};
		case SIDE_NORTHEAST: {
			return (slot_a->position.x > slot_b->position.x);
		};
		case SIDE_SOUTHEAST: {
			return (slot_a->position.x > slot_b->position.x);
		};
		default: {
			FATAL(INVALID_SIDE);
		}
	}
	return false;
}

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

bool	pellet_has_reached_bottom(Board* board, v2 neighbour_pos) {
	//No neighbour on this side
	if (neighbour_pos.x == no_neighbour.x && neighbour_pos.y == no_neighbour.y) {
		return true;
	}
	//Neighbour is occupied by a color
	if (board->map[(int)neighbour_pos.y][(int)neighbour_pos.x].color != EMPTY) {
		return true;
	}
	return false;
}

void	slot_fall(Board* board, v2 position, BoardSide side) {
	Slot* slot = &board->map[(int)position.y][(int)position.x];
	v2	neighbour = slot->neighbours[side];

	//fall should never be called for an empty slot
	assert(slot->color != EMPTY);

	const PelletType color = slot->color;
	while (!pellet_has_reached_bottom(board, neighbour)) {
		slot->color = EMPTY;
		slot = &board->map[(int)neighbour.y][(int)neighbour.x];
		slot->color = color;
		neighbour = slot->neighbours[side];
	}
}

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

void	board_rotate(Board* board, BoardSide new_side) {
	Slot*	pellets[board->pellets_placed];
	size_t	pellet_index = 0;

	//Set the new side
	board->side = new_side;

	//Collect the pellets
	for (size_t row = 0; row < 7 && pellet_index < board->pellets_placed; row++) {
		for (size_t col = ranges[row].start; col < ranges[row].end && pellet_index < board->pellets_placed; col++) {
			if (board->map[row][col].color != EMPTY) {
				pellets[pellet_index] = &board->map[row][col];
				pellet_index++;
			}
		}
	}
	//Sort them by closest to bottom first
	quicksort(pellets, (Range){0,board->pellets_placed-1}, sizeof(Slot*), (Lower) {
		.func = board_closer_to_bottom,
		.extra = &board->side
	});
	//Let them all fall
	for (size_t i = 0; i < board->pellets_placed; i++) {
		slot_fall(board, pellets[i]->position, board->side);
	}
}

void	board_render(Board* board, mlx_image_t* target) {
	for (size_t row = 0; row < 7; row++) {
		for (size_t col = ranges[row].start; col < ranges[row].end; col++) {
			draw_slot(&board->map[row][col], target);
		}
	}
}

void	board_update_slot(Board* board, int row, int col, PelletType color) {
	if (board->map[row][col].color == EMPTY) {
		board->pellets_placed++;
	}
	board->map[row][col].color = color;
}

void	board_init(Board* board) {
	bzero(board, sizeof(Board));
	board->side = SIDE_SOUTH;
	board->pellets_placed = 0;

	for (size_t row = 0; row < 7; row++) {
		for (size_t col = ranges[row].start; col < ranges[row].end; col++) {
			slot_init(&board->map[row][col], row, col);
		}
	}
}
