/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 11:34:12 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/08 18:48:50 by tbruinem      ########   odam.nl         */
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

void	state_init(GameState* state) {
	state->current_player = PLAYER_BLUE;
	state->turn_count = 0;
	state->result = IN_PROGRESS;
}

void	game_init(Game* game, int argc, char **argv) {
	bzero(game, sizeof(Game));
	game->animating = true;

	state_init(&game->state);

	init_players(game->player, argc-1, argv+1);

	board_init(&game->board);
	game->image = mlx_new_image(mlx(), WINDOW_WIDTH, WINDOW_HEIGHT);

	draw_fill(game->image, CLR_TRANSPARENT);
	mlx_image_to_window(mlx(), game->image, 0,0,0);
	mlx_loop_hook(mlx(), game_loop, game);

	game->starting_player = PLAYER_BLUE;
}

void	render(Game* game) {
	board_render(&game->board, game->image);
	// draw_circle(game->image, game->board.center.x, game->board.center.y, 15, CLR_RED);
}

void	game_execute_command(Game* game, Player* player, Command command) {
	switch (command.type) {
		case CMD_INVALID: {
			game->state.result = !player->color;
			break;
		};
		case CMD_PLACE: {
			board_place(&game->board, command.value, (int)player->color);
			game->animating = true;
			break;
		};
		case CMD_ROTATE: {
			board_update_direction(&game->board, command.value);
			board_rotate(&game->board, game->board.side);
			game->animating = true;
			break;
		};
	}
}

void	game_loop(void* param) {
	Game*		game = param;
	GameState*	state = &game->state;

	//Check for end condition
	if (state->result == IN_PROGRESS && state->turn_count == MAX_TURN_COUNT) {
		state->result = TIE;
	}
	if (state->result != IN_PROGRESS) {
		mlx_close_window(mlx());
	}

	if (game->animating) {
		if (game->board.tween.progress >= 0.99) {
			if (game->board.moving_pellets == NULL)
				game->animating = false;
			else {
				List* iter = game->board.moving_pellets;
				size_t len = list_size(iter);
				dprintf(2, "FALLING\n");
				if (len) {
					for (size_t i = 0; i < len; i++) {
						slot_staggered_fall(&game->board, ((Slot*)iter->content)->position, game->board.side);
						List* delete = iter;
						iter = iter->next;
						list_delete(&game->board.moving_pellets, delete);
					}
					// list_delete(&game->board.moving_pellets, iter);
				}
			}
		}
	}
	else {
		Player*	current_player = &game->player[state->current_player];

		const Command command = player_get_command(current_player, game);
		command_print(command);
		game_execute_command(game, current_player, command);
		board_direction_print(&game->board);

		//Switch to other player
		// Blue -> Red
		// Red  -> Blue
		state->current_player = !state->current_player;
		if (state->current_player == game->starting_player)
			state->turn_count += 1;

	}
	//Reset image, refactor this for the love of god....
	draw_fill(game->image, CLR_TRANSPARENT);
	render(game);
}

void	game_destroy(Game* game) {
	mlx_delete_image(mlx(), game->image);
}
