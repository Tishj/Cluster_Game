/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 08:55:31 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 17:17:28 by tbruinem      ########   odam.nl         */
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
#include <math.h>

#define HEXAGON_HEIGHT 75

static const char* side_string_mapping[] = {
	[SIDE_SOUTH] = "South",
	[SIDE_SOUTHEAST] = "South East",
	[SIDE_SOUTHWEST] = "South West",
	[SIDE_NORTHWEST] = "North West",
	[SIDE_NORTH] = "North",
	[SIDE_NORTHEAST] = "North East"
};

static v2	no_neighbour = {-1,-1};

void	slot_neighbour_print(Slot* slot) {
	printf("SLOT X:%d|Y:%d\n", (int)slot->position.x, (int)slot->position.y);
	for (size_t i = 0; i < 6; i++) {
		printf("Neighbour[%s] = X:%d,Y:%d\n", side_string_mapping[i], (int)slot->neighbours[i].x, (int)slot->neighbours[i].y);
	}
}

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
	//Round
	(v2[]){
		[SIDE_SOUTH] =		{ 0,  1},
		[SIDE_SOUTHWEST] =	{-1,  0},
		[SIDE_NORTHWEST] =	{-1, -1},
		[SIDE_NORTH] =		{ 0, -1},
		[SIDE_NORTHEAST] =	{ 1, -1},
		[SIDE_SOUTHEAST] =	{ 1,  0}
	},
	//Odd
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
	// printf("NEIGHBOUR: X:%d|Y:%d\n", (int)neighbour_pos.x, (int)neighbour_pos.y);

	//No neighbour on this side
	if (neighbour_pos.x == no_neighbour.x && neighbour_pos.y == no_neighbour.y) {
		// printf("NO NEIGHBOUR\n");
		return true;
	}
	//Neighbour is occupied by a color
	const PelletType color = board->map[(int)neighbour_pos.y][(int)neighbour_pos.x].color;
	if (color != EMPTY) {
		// printf("NEIGHBOUR OCCUPIED\n");
		return true;
	}
	return false;
}

void	slot_fall(Board* board, v2 position, BoardSide side) {
	Slot* slot = &board->map[(int)position.y][(int)position.x];
	assert(slot->color != EMPTY);

	v2	neighbour = slot->neighbours[side];

	const PelletType color = slot->color;
	while (!pellet_has_reached_bottom(board, neighbour)) {
		slot->color = EMPTY;
		slot = &board->map[(int)neighbour.y][(int)neighbour.x];
		slot->color = color;
		neighbour = slot->neighbours[side];
	}
}

float hex_width(float height)
{
	return (float)(4 * (height / 2 / sqrt(3)));
}

void	get_hex_points(v2* points, float height, float row, float col)
{
	// Start with the leftmost corner of the upper left hexagon.
	float width = hex_width(height);
	float y = height / 2;
	float x = 0;

	// Move down the required number of rows.
	y += row * height;

	// If the column is odd, move down half a hex more.
	if ((int)col % 2 == 1) y += height / 2;

	// Move over for the column number.
	x += col * (width * 0.75f);

	// Generate the points.
	points[0] = (v2){(int)x,(int)y};
	points[1] = (v2){(int)(x + width * 0.25),(int)(y - height / 2)};
	points[2] = (v2){(int)(x + width * 0.75),(int)(y - height / 2)};
	points[3] = (v2){(int)(x + width),(int)y};
	points[4] = (v2){(int)(x + width * 0.75),(int)(y + height / 2)};
	points[5] = (v2){(int)(x + width * 0.25),(int)(y + height / 2)};
}

void	slot_init(Slot* slot, int row, int col) {
	bool round = (col % 2 != 0);

	slot->position = (v2){col, row};
	slot->color = EMPTY;
	get_hex_points(slot->points, HEXAGON_HEIGHT, slot->position.y, slot->position.x);
	for (size_t i = SIDE_SOUTH; i < SIDE_SIZE; i++) {
		v2 pos = neighbour_offset[round][i];
		pos.x += col;
		pos.y += row;
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
			draw_slot(board, (v2){col, row}, target);
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
	Slot* center_slot = &board->map[3][3];
	board->center = (v2){center_slot->points[0].x + ((center_slot->points[3].x - center_slot->points[0].x) / 2), center_slot->points[0].y};
}
