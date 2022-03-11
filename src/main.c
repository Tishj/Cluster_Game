/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 22:54:27 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/11 11:45:04 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX.h"
#include "Game.h"
// #include "Communication.h"

int main(int argc, char **argv) {
	Game	game;

	srand(time(NULL));
	game_init(&game, argc, argv);
	// simulate_game(&game.board);
	// print_initial_state(&game);

	
	mlx_loop(mlx());
	game_destroy(&game);
	mlx_terminate(mlx());
	printf("RIGHT BEFORE RETURN\n");
	return (0);
}
