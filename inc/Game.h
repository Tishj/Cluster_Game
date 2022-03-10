/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Game.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 23:13:04 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/10 15:34:51 by tbruinem      ########   odam.nl         */
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
#define MATCH_MINIMUM	4
#define SIDE_LENGTH		4
#define HEXAGON_HEIGHT	(WINDOW_HEIGHT / (((SIDE_LENGTH * 2) - 1) + 2))
#define PELLET_RADIUS (HEXAGON_HEIGHT / 3)

#define MAX_TURN_COUNT 20

typedef enum GameResult {
	IN_PROGRESS = -1,
	WIN_BLUE,
	WIN_RED,
	TIE
}	GameResult;

typedef struct GameState {
	PlayerType	current_player;
	size_t		turn_count;
	GameResult	result;
}	GameState;

typedef struct Game {
	Board			board;
	mlx_image_t*	image;
	Player			player[2];
	GameState		state;
	PlayerType		starting_player;
	bool			animating;
}	Game;

void	game_loop(void* param);
void	game_init(Game* game, int argc, char **argv);
void	game_destroy(Game* game);

#endif
