/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 08:55:45 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/10 13:38:43 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARD_H
# define BOARD_H

#include "util.h"
#include <stdbool.h>
#include "vector.h"
#include "Draw.h"
#include "List.h"

#define MATCH_MINIMUM 4
#define SIDE_LENGTH 6

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

// typedef struct Hexagon {

// }	Hexagon;

typedef struct MatchInfo {
	int			size;
	PelletType	color;
}	MatchInfo;

typedef struct Pellet {
	PelletType	color;
	Slot*		slot;
	bool		moving; //for falling purposes
}	Pellet;

typedef struct Slot {
	Slot*		neighbours[6];
	// PelletType	color;
	v2			position;
	v2			points[6];
	size_t		index;
	Pellet*		pellet; //Needs to have this, otherwise we cant check if the neighbour of a pellet already contains another pellet
}	Slot;

typedef struct Board {
	List*		pellets;
	List*		slots;
	BoardSide	side;
	size_t		pellets_placed;
	v2			center;
	Tween		tween;
	Slot*		corners[6];
	List*		moving_pellets;
}	Board;

void	board_init(Board* board);
bool	board_inside(v2 pos);
void	board_render(Board* board, mlx_image_t* target);
// void	board_update_slot(Board* board, int row, int col, PelletType color);
void	pellet_fall(Pellet* pellet, BoardSide side);
bool	pellet_staggered_fall(Pellet* pellet, BoardSide side);
void	board_rotate(Board* board, BoardSide new_side);
void	slot_neighbour_print(Slot* slot);
void	board_update_direction(Board* board, int cycles);
void	board_direction_print(BoardSide side);
void	draw_pellet(Board* board, Pellet* pellet, mlx_image_t* target);
void	board_destroy(Board* board);
int		board_check_match(Board* board);

void	board_place(Board* board, size_t index, PelletType color);

#endif
