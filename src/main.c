/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 22:54:27 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 11:51:53 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX.h"
#include "Game.h"

int main() {
	Game	game;

	game_init(&game);
	board_update_slot(&game.board, 3, 4, BLUE0);
	slot_fall(&game.board, (v2){4,3}, game.board.side);

	mlx_loop(mlx());
	game_destroy(&game);
	mlx_terminate(mlx());
	return (0);
}
