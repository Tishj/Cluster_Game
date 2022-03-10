/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 08:55:31 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/10 13:28:47 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "Draw.h"
#include "Board.h"
#include <stddef.h>
#include "util.h"
#include "MLX.h"
#include "Error.h"
#include <assert.h>
#include "vector.h"
#include <strings.h>
#include <string.h>
#include "Sort.h"
#include <math.h>
#include <stdlib.h>
#include <alloca.h>
#include "Game.h"

#define HEXAGON_HEIGHT 75

static const char* side_string_mapping[] = {
	[SIDE_SOUTH] = "South",
	[SIDE_SOUTHEAST] = "South East",
	[SIDE_SOUTHWEST] = "South West",
	[SIDE_NORTHWEST] = "North West",
	[SIDE_NORTH] = "North",
	[SIDE_NORTHEAST] = "North East"
};

void	board_direction_print(BoardSide side) {
	dprintf(2, "Direction: (%d) - %s\n", side, side_string_mapping[side]);
}

void	slot_neighbour_print(Slot* slot) {
	// assert(slot != NULL);
	dprintf(2, "SLOT X:%d|Y:%d\n", (int)slot->position.x, (int)slot->position.y);
	for (size_t i = 0; i < 6; i++) {
		if (slot->neighbours[i])
			dprintf(2, "Neighbour[%s] = X:%d,Y:%d\n", side_string_mapping[i], (int)slot->neighbours[i]->position.x, (int)slot->neighbours[i]->position.y);
	}
}

//'lower_f' prototype
bool	board_closer_to_bottom(void* a, void* b, void* extra) {
	List*	list_a = *(List**)a;
	List*	list_b = *(List**)b;
	
	Slot*	slot_a = ((Pellet*)list_a->content)->slot;
	Slot*	slot_b = ((Pellet*)list_b->content)->slot;

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

BoardSide	side_offset(BoardSide side, int offset) {
	if (offset > 0) {
		return (side + offset) % SIDE_SIZE;
	}
	else {
		return (SIDE_SIZE + side + offset) % SIDE_SIZE;
	}
}

BoardSide	side_invert(BoardSide side) {
	return ((side + 3) % SIDE_SIZE);
}

MatchInfo		match_size(Pellet* pellet, MatchInfo info, BoardSide direction) {
	Slot* neighbour = pellet->slot->neighbours[direction];

	if (!neighbour || !neighbour->pellet || neighbour->pellet->color != pellet->color)
		return info;
	info.size += 1;
	return match_size(neighbour->pellet, info, direction);
}

void	debug_matchinfo(MatchInfo info) {
	static const char* player_colors[] = {
		[PLAYER_BLUE] = "BLUE",
		[PLAYER_RED] = "RED"
	};

	dprintf(2, "MATCH_INFO FOR PLAYER[%s] - Size: %d\n", player_colors[info.color % 2], info.size);
}

int	board_check_match(Board* board) {
	MatchInfo	biggest_match[2] = {};
	//Initialize biggest_match to -1
	for (size_t i = 0; i < 2; i++) {
		biggest_match[i].size = -1;
	}
	if (board->pellets == NULL) {
		return IN_PROGRESS;
	}
	dprintf(2, "CHECKING FOR A MATCH\n");

	//For every pellet, check if it can come up with a match
	for (List* iter = board->pellets; iter; iter = iter->next) {
		Pellet* pellet = iter->content;
		for (size_t side = SIDE_SOUTH; side < SIDE_NORTH; side++) {
			MatchInfo current = {.color = pellet->color, .size = 1};
			current = match_size(pellet, current, side);

			//Get the player color according to the pellet color
			PlayerType	player = current.color % 2;
			if (current.size > biggest_match[player].size) {
				biggest_match[player] = current;
			}
			else if (current.size == biggest_match[player].size && current.color != biggest_match[player].color) {
				//Check which of the two colors has less pellets on the board
			}
		}
	}
	for (size_t i = 0; i < 2; i++) {
		debug_matchinfo(biggest_match[i]);
	}
	int blue_size = biggest_match[PLAYER_BLUE].size;
	int red_size = biggest_match[PLAYER_RED].size;
	//No winner
	if (blue_size < MATCH_MINIMUM && red_size < MATCH_MINIMUM) {
		return IN_PROGRESS;
	}
	//Potential tie
	if (blue_size >= MATCH_MINIMUM && red_size >= MATCH_MINIMUM) {
		//Blue wins
		if (blue_size > red_size) {
			return WIN_BLUE;
		}
		else if (red_size < blue_size) {
			return WIN_RED;
		}
		//Even further potential tie
		else {
			//Check which color has the least pellets of that color on the board -> winner
			//If those are equal -> then it's a tie
		}
	}
	//Blue wins
	if (blue_size >= MATCH_MINIMUM) {
		return WIN_BLUE;
	}
	//Red wins
	if (red_size >= MATCH_MINIMUM) {
		return WIN_RED;
	}
	return IN_PROGRESS;
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

v2	get_neighbour_pos(v2 position, BoardSide side) {
	bool round = ((int)position.x % 2 != 0);

	v2 offset = neighbour_offset[round][side];
	return (v2) {
		.x = position.x + offset.x,
		.y = position.y + offset.y
	};
}

// static const v2 insert_slot[6][7] = {
// 	[SIDE_SOUTH] = {
// 		[0] = {0,2},
// 		[1] = {1,1},
// 		[2] = {2,1},
// 		[3] = {3,0},
// 		[4] = {4,1},
// 		[5] = {5,1},
// 		[6] = {6,2}
// 	},
// 	[SIDE_SOUTHWEST] = {
// 		[0] = {3,0},
// 		[1] = {4,1},
// 		[2] = {5,1},
// 		[3] = {6,2},
// 		[4] = {6,3},
// 		[5] = {6,4},
// 		[6] = {6,5}
// 	},
// 	[SIDE_NORTHWEST] = {
// 		[0] = {6,2},
// 		[1] = {6,3},
// 		[2] = {6,4},
// 		[3] = {6,5},
// 		[4] = {5,5},
// 		[5] = {4,6},
// 		[6] = {3,6}
// 	},
// 	[SIDE_NORTH] = {
// 		[0] = {6,5},
// 		[1] = {5,5},
// 		[2] = {4,6},
// 		[3] = {3,6},
// 		[4] = {2,6},
// 		[5] = {1,5},
// 		[6] = {0,5}
// 	},
// 	[SIDE_NORTHEAST] = {
// 		[0] = {3,6},
// 		[1] = {2,6},
// 		[2] = {1,5},
// 		[3] = {0,5},
// 		[4] = {0,4},
// 		[5] = {0,3},
// 		[6] = {0,2}
// 	},
// 	[SIDE_SOUTHEAST] = {
// 		[0] = {0,5},
// 		[1] = {0,4},
// 		[2] = {0,3},
// 		[3] = {0,2},
// 		[4] = {1,1},
// 		[5] = {2,1},
// 		[6] = {3,0}
// 	},
// };

Slot*	get_insert_slot(Board* board, BoardSide side, size_t index) {
	printf("CURRENT DIRECTION: %s\n", side_string_mapping[side]);
	if (index == (SIDE_LENGTH - 1)) {
		return board->corners[side_invert(side)];
	}
	Slot*		slot;
	BoardSide	travel_direction;
	int			corner_index;
	if (index < (SIDE_LENGTH - 1)) {
		BoardSide	corner_side = side_offset(side_invert(side), -1);
		printf("CORNER SIDE: %s\n", side_string_mapping[corner_side]);
		slot = board->corners[corner_side];
		travel_direction = side_invert(side_offset(side, 1));
		corner_index = 0;
	}
	else {
		BoardSide	corner_side = side_offset(side_invert(side), 1);
		printf("CORNER SIDE: %s\n", side_string_mapping[corner_side]);
		slot = board->corners[corner_side];
		travel_direction = side_invert(side_offset(side, -1));
		corner_index = (SIDE_LENGTH * 2) - 2;
	}
	if (index == 0 || index == (SIDE_LENGTH * 2) - 2)
		return slot;

	slot_neighbour_print(slot);
	board_direction_print(travel_direction);

	int steps = abs(corner_index - (int)index);
	printf("steps: %d\n", steps);

	for (int i = 0; i < steps; i++) {
		printf("%d\n", i);
		slot = slot->neighbours[travel_direction];
		assert(slot != NULL);
	}
	return slot;
}

Pellet*	pellet_new(PelletType color, Slot* slot) {
	Pellet*	pellet = malloc(sizeof(Pellet));
	if (!pellet) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	pellet->color = color;
	pellet->slot = slot;
	return pellet;
}

//REFACTOR
void	board_place(Board* board, size_t index, PelletType color) {
	board->pellets_placed++;
	Slot* slot = get_insert_slot(board, board->side, index);
	Pellet*	pellet = pellet_new(color, slot);
	slot->pellet = pellet;
	list_pushfront(&board->pellets, list_new(pellet));
	list_pushback(&board->moving_pellets, list_new(pellet));
	// slot_fall(board, pos, board->side);
}

bool	pellet_has_reached_bottom(Slot* neighbour) {
	// printf("NEIGHBOUR: X:%d|Y:%d\n", (int)neighbour_pos.x, (int)neighbour_pos.y);
	if (!neighbour) {
		return true;
	}

	//Neighbour is occupied by a color
	const Pellet* neighbour_pellet = neighbour->pellet;
	if (neighbour_pellet != NULL) {
		// printf("NEIGHBOUR OCCUPIED\n");
		return true;
	}
	return false;
}

void	pellet_fall(Pellet* pellet, BoardSide side) {
	Slot*	slot = pellet->slot;
	assert(slot != NULL);

	Slot*	neighbour = slot->neighbours[side];

	while (!pellet_has_reached_bottom(neighbour)) {
		slot->pellet = NULL;
		neighbour->pellet = pellet;
		neighbour = neighbour->neighbours[side];
		pellet->slot = neighbour;
	}
}

//deprecated
// void	slot_fall(Board* board, Slot* slot, BoardSide side) {

// 	assert(slot->color != EMPTY);

// 	v2	neighbour = slot->neighbours[side];

// 	const PelletType color = slot->color;
// 	while (!pellet_has_reached_bottom(board, neighbour)) {
// 		slot->color = EMPTY;
// 		slot = &board->map[(int)neighbour.y][(int)neighbour.x];
// 		slot->color = color;
// 		neighbour = slot->neighbours[side];
// 	}
// }

bool	pellet_staggered_fall(Pellet* pellet, BoardSide side) {
	Slot* slot = pellet->slot;
	assert(slot != NULL);
	v2 position = slot->position;

	dprintf(2, "STAGGERED FALLING X:%d|Y:%d\n", (int)position.x, (int)position.y);
	assert(pellet->color != EMPTY);

	Slot* neighbour = slot->neighbours[side];
	if (pellet_has_reached_bottom(neighbour))
		return true;

	slot->pellet = NULL;
	neighbour->pellet = pellet;
	pellet->slot = neighbour;
	return false;
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

void	board_rotate(Board* board, BoardSide new_side) {
	List*	pellets[board->pellets_placed + 1];
	size_t	pellet_index = 0;

	//Set the new side
	board->side = new_side;

	//Gather pellets
	for (List* iter = board->pellets; iter; iter = iter->next) {
		dprintf(2, "ADDED PELLET\n");
		pellets[pellet_index++] = iter;
	}
	if (!board->pellets_placed) {
		return;
	}

	quicksort(pellets, (Range){0,board->pellets_placed-1}, sizeof(List*), (Lower) {
		.func = board_closer_to_bottom,
		.extra = &board->side
	});
	for (size_t i = 0; i < board->pellets_placed; i++) {
		// dprintf(2, "PELLET[%ld] = X:%d|Y:%d\n", i, (int)pellets[i]->position.x, (int)pellets[i]->position.y);
		list_pushback(&board->moving_pellets, list_new(pellets[i]->content));
		// slot_fall(board, pellets[i]->position, board->side);
	}
}

double	board_get_rotation(Board* board) {
	double rotation_angle = 60 * board->side;
	double radians = deg2rad(rotation_angle);

	return radians;
}

void	board_update_direction(Board* board, int cycles) {
	board->tween.from = board_get_rotation(board);
	board->side = (board->side + cycles) % 6;
	board->tween.progress = 0;
	board->tween.to = board_get_rotation(board);
}

void	board_render(Board* board, mlx_image_t* target) {
	for (List* iter = board->slots; iter; iter = iter->next) {
		Slot* slot = iter->content;
		draw_slot(board, slot, target);
	}
	for (List* iter = board->pellets; iter; iter = iter->next) {
		// dprintf(2, "IM RENDERING A PELLET\n");
		Pellet* pellet = iter->content;
		draw_pellet(board, pellet, target);
	}
	if (board->tween.progress < 1) {
		board->tween.progress += 0.01;
	}
	else {
		board->tween.from = board->tween.to;
		board->tween.progress = 1;
	}
}

Slot*	slot_new(v2 position) {
	static int index = 0;
	Slot*	slot = malloc(sizeof(Slot));
	if (!slot) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	slot->position = position;
	slot->pellet = NULL;
	slot->index = index++;
	get_hex_points(slot->points, HEXAGON_HEIGHT, position.y, position.x);
	bzero(slot->neighbours, sizeof(Slot*) * 6);
	return slot;
}

static Slot* get_corner(Slot* slot, BoardSide side) {
	while (slot->neighbours[side]) {
		slot = slot->neighbours[side];
	}
	return slot;
}

static void	assign_corners(Board* board, Slot* center) {
	// int mid = SIDE_LENGTH;
	// v2	positions[SIDE_SIZE] = {};
	
	// positions[SIDE_NORTH].y = 0;
	// positions[SIDE_NORTH].x = mid;
	// positions[SIDE_NORTHEAST].y = (int)((mid + 1) / 2);
	// positions[SIDE_NORTHEAST].x = (int)((2 * mid) - 1);
	// positions[SIDE_SOUTHEAST].y = (int)(mid * 1.5 + 0.5);
	// positions[SIDE_SOUTHEAST].x = (int)((2 * mid) - 1);
	// positions[SIDE_SOUTH].y = (int)((mid * 2) - 1);
	// positions[SIDE_SOUTH].x = (int)(mid);
	// positions[SIDE_SOUTHWEST].y = (int)(mid * 1.5 + 0.5);
	// positions[SIDE_SOUTHWEST].x = 0;
	// positions[SIDE_NORTHWEST].y = (int)((mid + 1) / 2);
	// positions[SIDE_NORTHWEST].x = 0;

	// for (size_t side = SIDE_SOUTH; side < SIDE_SIZE; side++) {
	// 	board->corners[side] = tmp[(int)positions[side].y][(int)positions[side].x];
	// 	dprintf(2, "CORNER[%s] | X:%d|Y:%d\n", side_string_mapping[side], (int)positions[side].x, (int)positions[side].y);
	// 	slot_neighbour_print(board->corners[side]);
	// }
	for (size_t side = SIDE_SOUTH; side < SIDE_SIZE; side++) {
		board->corners[side] = get_corner(center, side);
	}
}

//TODO: save corners, for placement
static void	create_slots(Board* board) {
	const int	board_height = (SIDE_LENGTH * 2);
	//Could do this more optimally by creating it as needed for every "ring"
	Slot* temp[board_height + 1][board_height + 1];
	List*	new_slots = NULL;

	for (int i = 0; i < board_height; i++) {
		for (int j = 0; j < board_height; j++) {
			temp[i][j] = NULL;
		}
	}
	v2		middle_pos = {
		.x = SIDE_LENGTH - 1,
		.y = SIDE_LENGTH - 1
	};
	Slot*	middle = slot_new(middle_pos);
	slot_neighbour_print(middle);
	temp[(int)middle_pos.y][(int)middle_pos.x] = middle;

	list_pushback(&new_slots, list_new(middle));

	for (size_t depth = 0; depth < SIDE_LENGTH; depth++) {
		//Iterate over the current "ring"
		List*	new_neighbours = NULL;
		for (List* iter = new_slots; iter; iter = iter->next) {
			Slot* slot = iter->content;
			for (size_t side = SIDE_SOUTH; side < SIDE_SIZE; side++) {
				v2 position = slot->position;
				v2 neighbour_position = get_neighbour_pos(position, side);
				dprintf(2, "NEIGHBOUR POS: %d|%d\n", (int)neighbour_position.x, (int)neighbour_position.y);
				//Retrieve the neighbour
				Slot* neighbour;
				if (neighbour_position.x < 0 || neighbour_position.y < 0)
					neighbour = NULL;
				else {
					neighbour = temp[(int)neighbour_position.y][(int)neighbour_position.x];
				}
				//If the neighbour doesnt exist yet, and we're not at the final ring, add it
				if (!neighbour && depth + 1 != SIDE_LENGTH) {
					neighbour = slot_new(neighbour_position);
					list_pushback(&new_neighbours, list_new(neighbour));
					temp[(int)neighbour_position.y][(int)neighbour_position.x] = neighbour;
				}
				slot->neighbours[side] = neighbour;
			}
		}
		list_pushback(&board->slots, new_slots);
		new_slots = new_neighbours;
	}


	Slot* center_slot = temp[SIDE_LENGTH - 1][SIDE_LENGTH - 1];
	assign_corners(board, center_slot);
	board->center = (v2){center_slot->points[0].x + ((center_slot->points[3].x - center_slot->points[0].x) / 2), center_slot->points[0].y};
}

void	board_destroy(Board* board) {
	for (List* iter = board->pellets; iter; iter = iter->next) {
		free(iter->content);
	}
	for (List* iter = board->slots; iter; iter = iter->next) {
		free(iter->content);
	}
	list_clear(board->pellets);
	list_clear(board->slots);
}

void	board_init(Board* board) {
	bzero(board, sizeof(Board));
	board->side = SIDE_SOUTH;
	board->pellets_placed = 0;
	board->tween.progress = 1;
	board->tween.from = 0;

	create_slots(board);
}
