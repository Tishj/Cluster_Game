/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 11:34:12 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 18:56:22 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Game.h"
#include "MLX.h"
#include "Draw.h"
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include "Player.h"
#include <sys/time.h>
#include "Command.h"

void	init_players(Player* player, int argc, char **argv) {
	for (int i = 0; i < 2; i++) {
		if (i < argc) {
			player_init(player + i, i, argv[i]);
		}
		else {
			player_init(player + i, i, NULL);
		}
	}
}

void	state_init(State* state) {
	state->current_player = PLAYER_BLUE;
	state->turn_count = 0;
	state->winner = PLAYER_NONE;
}

void	game_init(Game* game, int argc, char **argv) {
	bzero(game, sizeof(Game));
	
	state_init(&game->state);

	init_players(game->player, argc-1, argv+1);

	board_init(&game->board);
	game->image = mlx_new_image(mlx(), WINDOW_WIDTH, WINDOW_HEIGHT);

	draw_fill(game->image, CLR_TRANSPARENT);
	mlx_image_to_window(mlx(), game->image, 0,0,0);
	mlx_loop_hook(mlx(), game_loop, game);
}

void	render(Game* game) {
	board_render(&game->board, game->image);
	// draw_circle(game->image, game->board.center.x, game->board.center.y, 15, CLR_RED);
}

void	game_loop(void* param) {
	Game*		game = param;
	State*		state = &game->state;

	if (state->winner != PLAYER_NONE) {
		mlx_close_window(mlx());
	}

	size_t	timeout_duration = state->turn_count ? ROUND_TIMEOUT_DURATION : INITIAL_TIMEOUT_DURATION;

	const Command command = player_get_command(&game->player[state->current_player], game, timeout_duration);
	command_print(command);

	//Reset image, refactor this for the love of god....
	draw_fill(game->image, CLR_TRANSPARENT);
	render(game);

	//Switch to other player (Blue -> Red | Red -> Blue)
	state->current_player = !state->current_player;
}

void	game_destroy(Game* game) {
	mlx_delete_image(mlx(), game->image);
}
