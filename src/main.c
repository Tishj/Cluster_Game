/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 22:54:27 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 16:51:51 by limartin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX.h"
#include "Game.h"

void	simulate_game(Board* board) {
	board_update_slot(board, 3, 0, BLUE0);
	board_update_slot(board, 3, 1, BLUE0);
	board_update_slot(board, 3, 2, BLUE0);
	board_update_slot(board, 3, 3, BLUE0);
	board_update_slot(board, 3, 4, BLUE0);

	// board_rotate(board, DIR_NORTH);
	// slot_neighbour_print(&board->map[2][2]);
	// slot_neighbour_print(&board->map[3][3]);
	// slot_neighbour_print(&board->map[2][2]);
	// slot_fall(board, (v2){2,2}, DIR_NORTHWEST);
	printf("COLOR of X:1|Y:2 = %d\n", board->map[2][1].color);
}

int main() {
	Game	game;

	game_init(&game);
	simulate_game(&game.board);

	mlx_loop(mlx());
	game_destroy(&game);
	mlx_terminate(mlx());
	return (0);
}
