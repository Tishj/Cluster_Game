/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 11:34:12 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/05 11:53:34 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Game.h"
#include "MLX.h"
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

void	game_init(Game* game) {
	bzero(game, sizeof(Game));
	board_init(&game->board);
	game->turn_count = 0;
	game->image = mlx_new_image(mlx(), WINDOW_WIDTH, WINDOW_HEIGHT);
}

void	game_loop(void* param) {
	Game*	game = param;
	printf("GAME TURN_COUNT: %ld\n", game->turn_count++);
}
