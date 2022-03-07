/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Game.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 23:13:04 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 18:49:31 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

#include "util.h"
#include "MLX42/MLX42.h"
#include <stddef.h>
#include "Board.h"
#include "Player.h"

#define INITIAL_TIMEOUT_DURATION 1000
#define ROUND_TIMEOUT_DURATION 100

#define HEX_HEIGHT 75

typedef struct State {
	PlayerType	current_player;
	size_t		turn_count;
	PlayerType	winner;
}	State;

typedef struct Game {
	Board			board;
	mlx_image_t*	image;
	Player			player[2];
	State			state;
}	Game;

void	game_loop(void* param);
void	game_init(Game* game, int argc, char **argv);
void	game_destroy(Game* game);

#endif
