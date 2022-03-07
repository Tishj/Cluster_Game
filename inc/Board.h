/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 08:55:45 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 18:46:11 by limartin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARD_H
# define BOARD_H

#include "util.h"
#include <stdbool.h>
#include "vector.h"

//(DIR_SIZE + Rotation) % DIR_SIZE == BoardSide
typedef enum BoardCycle {
	DIR_SOUTH,
	DIR_SOUTHWEST,
	DIR_NORTHWEST,
	DIR_NORTH,
	DIR_NORTHEAST,
	DIR_SOUTHEAST,
	DIR_SIZE
}	BoardSide; //deprectaed by facing?

typedef enum Facing {
	CENTRE, 
	SOUTH,
	SOUTHWEST,
	NORTHWEST,
	NORTH,
	NORTHEAST,
	SOUTHEAST,
	SIZE
}	Facing;

//0 and 2 are BLUE
//1 and 3 are RED
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
}	Slot;

typedef struct Board {
	Slot		map[7][7];
	BoardSide	side;
	size_t		pellets_placed;
	v2			center; // deprecated by compass?
	v2			compass[SIZE];
}	Board;

void	board_init(Board* board);
bool	board_inside(v2 pos);
void	board_render(Board* board, mlx_image_t* target);
void	board_update_slot(Board* board, int row, int col, PelletType color);
void	slot_fall(Board* board, v2 position, BoardSide side);
void	board_rotate(Board* board, BoardSide new_side);
void	slot_neighbour_print(Slot* slot);
void	calibrate_compass(Board* board, int side_length);

#endif
