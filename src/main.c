/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 22:54:27 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 19:37:25 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX.h"
#include "Game.h"

void	simulate_game(Board* board) {
	board_update_slot(board, 3, 0, RED1);
	board_update_slot(board, 3, 1, BLUE0);
	board_update_slot(board, 3, 2, BLUE1);
	board_update_slot(board, 3, 3, RED0);
	board_update_slot(board, 3, 4, BLUE0);

	board_rotate(board, SIDE_SOUTH);
	// slot_neighbour_print(&board->map[2][2]);
	slot_neighbour_print(&board->map[3][3]);
	slot_neighbour_print(&board->map[2][2]);
	// slot_fall(board, (v2){2,2}, SIDE_NORTHWEST);
	dprintf(2, "COLOR of X:1|Y:2 = %d\n", board->map[2][1].color);
}

int main(int argc, char **argv) {
	Game	game;

	game_init(&game, argc, argv);
	simulate_game(&game.board);

	mlx_loop(mlx());
	game_destroy(&game);
	mlx_terminate(mlx());
	return (0);
}
