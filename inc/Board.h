/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 08:55:45 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/08 17:14:05 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARD_H
# define BOARD_H

#include "util.h"
#include <stdbool.h>
#include "vector.h"
#include "Draw.h"
#include "List.h"

//(SIDE_SIZE + Rotation) % SIDE_SIZE == BoardSide
typedef enum BoardCycle {
	SIDE_SOUTH,
	SIDE_SOUTHWEST,
	SIDE_NORTHWEST,
	SIDE_NORTH,
	SIDE_NORTHEAST,
	SIDE_SOUTHEAST,
	SIDE_SIZE
}	BoardSide;

//0 and 2 are PLAYER_BLUE
//1 and 3 are PLAYER_RED
//X % 2 == PlayerType
typedef enum PelletType {
	EMPTY = -1,
	BLUE0,
	RED0,
	BLUE1,
	RED1,
}	PelletType;

typedef struct Slot {
	v2			neighbours[6];
	PelletType	color;
	v2			position;
	v2			points[6];
	size_t		index;
}	Slot;

typedef struct Board {
	Slot		map[7][7];
	BoardSide	side;
	size_t		pellets_placed;
	v2			center;
	Tween		tween;
	List*		moving_pellets;
}	Board;

void	board_init(Board* board);
bool	board_inside(v2 pos);
void	board_render(Board* board, mlx_image_t* target);
void	board_update_slot(Board* board, int row, int col, PelletType color);
void	slot_fall(Board* board, v2 position, BoardSide side);
void	slot_staggered_fall(Board* board, v2 position, BoardSide side);
void	board_rotate(Board* board, BoardSide new_side);
void	slot_neighbour_print(Slot* slot);
void	board_update_direction(Board* board, int cycles);
void	board_direction_print(Board* board);

void	board_place(Board* board, size_t index, PelletType color);

#endif
