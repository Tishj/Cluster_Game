/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 08:55:45 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/05 16:02:34 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARD_H
# define BOARD_H

#include "Util.h"
#include <stdbool.h>

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
}	Slot;

typedef struct Board {
	Slot		map[7][7];
	BoardSide	side;
}	Board;

void	board_init(Board* board);
bool	board_inside(v2 pos);
void	board_render(Board* board, mlx_image_t* target);
void	board_update_slot(Board* board, int row, int col, PelletType color);

#endif
