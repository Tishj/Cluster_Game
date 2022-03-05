/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 22:54:27 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/05 16:03:33 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "cluster.h"
#include "MLX.h"
#include "Game.h"

//Provide absolute path to bot files to play the game. If empty inputs are controlled with the keyboard instead.
int main(int argc, char **argv) {
	(void)argv;
	//Keyboard input
	if (argc == 1) {
		;
	}
	//Bot input
	else if (argc == 3) {
		;
	}
	else {
		return (1);
	}
	Game	game;

	game_init(&game);
	board_update_slot(&game.board, 3, 4, BLUE0);
	mlx_loop_hook(mlx(), game_loop, &game);
	mlx_loop(mlx());
	return (0);
}
