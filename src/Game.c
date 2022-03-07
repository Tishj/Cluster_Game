/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 11:34:12 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 14:15:50 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Game.h"
#include "MLX.h"
#include "Draw.h"
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

void	game_init(Game* game) {
	bzero(game, sizeof(Game));
	board_init(&game->board);
	game->turn_count = 0;
	game->image = mlx_new_image(mlx(), WINDOW_WIDTH, WINDOW_HEIGHT);

	draw_fill(game->image, CLR_TRANSPARENT);
	mlx_image_to_window(mlx(), game->image, 0,0);
	mlx_loop_hook(mlx(), game_loop, game);
}

void	render(Game* game) {
	board_render(&game->board, game->image);
	draw_circle(game->image, game->board.center.x, game->board.center.y, 15, CLR_RED);
}

void	game_loop(void* param) {
	Game*	game = param;

	draw_fill(game->image, CLR_TRANSPARENT);
	render(game);
	// draw_circle(game->image, WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 30, CLR_RED);
	// draw_pixel(game->image, CLR_WHITE, (v2){WINDOW_WIDTH/2, WINDOW_HEIGHT/2});
}

void	game_destroy(Game* game) {
	mlx_delete_image(mlx(), game->image);
	free(game);
}
