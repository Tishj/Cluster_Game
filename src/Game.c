/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 11:34:12 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/11 11:51:52 by tbruinem      ########   odam.nl         */
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
#include "Sack.h"

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
	sack_init(game);

	board_init(&game->board);
	game->image = mlx_new_image(mlx(), WINDOW_WIDTH, WINDOW_HEIGHT);

	draw_fill(game->image, CLR_TRANSPARENT);
	mlx_image_to_window(mlx(), game->image, 0,0,0);
	mlx_loop_hook(mlx(), game_loop, game);

	game->starting_player = PLAYER_BLUE;
}

void	render(Game* game) {
	board_render(&game->board, game->image);
}

void	game_execute_command(Game* game, Player* player, Command* command) {
	switch (command->type) {
		case CMD_INVALID: {
			CommandInvalid* cmd = (void*)command;
			game->state.result = !player->color;
			free(cmd);
			break;
		};
		case CMD_PLACE: {
			game->animating = true;
			CommandPlace* cmd = (void*)command;
			board_place(&game->board, cmd->slot_index, cmd->color_index);
			//Remove the index from the hand
			player->hand[cmd->color_index / PLAYER_SIZE]--;
			//Place all the other pellet(s) back into the bag
			for (size_t i = 0; i < 2; i++) {
				int amount = player->hand[i];
				player->hand[i] = 0;
				player->bag[i] += amount;
			}
			//Add the counter for the board
			game->board.onboard[cmd->color_index]++;

			//Update the amount of pellets they are unaware of yet
			player->missing_pellets++;
			game->player[!game->state.current_player].missing_pellets++;

			free(cmd);
			(void)cmd;
			break;
		};
		case CMD_ROTATE: {
			CommandRotate* cmd = (void*)command;
			board_update_direction(&game->board, cmd->cycles);
			board_rotate(&game->board, game->board.side);
			game->animating = true;
			bzero(player->hand, sizeof(int) * 2);
			free(cmd);
			break;
		};
	}
}

static const char*	match_results[] = {
	[WIN_BLUE] = "Player blue has won!",
	[WIN_RED] = "Player red has won!",
	[TIE] = "The game ended in a tie :/"
};

void	game_loop(void* param) {
	Game*		game = param;
	GameState*	state = &game->state;

	//Check for end condition
	if (state->result == IN_PROGRESS && state->turn_count == MAX_TURN_COUNT) {
		state->result = TIE;
	}
	if (state->result != IN_PROGRESS) {
		dprintf(2, "%s\n", match_results[state->result]);
		mlx_close_window(mlx());
		return;
	}

	if (game->animating) {
		if (game->board.tween.progress >= 1.0) {
			if (!game->board.moving_pellets) {
				game->state.result = board_check_match(&game->board);
				game->animating = false;
			}
			else {
				for (List* iter = game->board.moving_pellets; iter;) {
					Pellet* pellet = iter->content;
					List*	delete = NULL;
					bool reached_bottom = pellet_staggered_fall(pellet, game->board.side);
					if (reached_bottom)
						delete = iter;
					iter = iter->next;
					list_delete(&game->board.moving_pellets, delete, free);
				}
			}
		}
	}
	else {
		Player*	current_player = &game->player[state->current_player];

		int remaining = remaining_pellets_for_player(current_player);
		if (remaining) {
			sack_drawhand(current_player, remaining);
			sack_debug(current_player);
			Command* command = player_get_command(current_player, game);
			command_print(command);
			game_execute_command(game, current_player, command);
			board_direction_print(game->board.side);

			//Switch to other player
			// Blue -> Red
			// Red  -> Blue
			state->current_player = !state->current_player;
			if (state->current_player == game->starting_player)
				state->turn_count += 1;
		}
		else {
			state->result = !current_player->color;
		}
	}
	//Reset image, refactor this for the love of god....
	draw_fill(game->image, CLR_TRANSPARENT);
	render(game);
	// usleep(100000);
}

void	game_destroy(Game* game) {
	mlx_delete_image(mlx(), game->image);
	board_destroy(&game->board);
	for (size_t i = 0; i < 2; i++) {
		player_destroy(&game->player[i]);
	}
	printf("WE DONE CLEANING SHIT\n");
}
