/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Game.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 23:13:04 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/05 12:42:27 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

#include "Util.h"
#include "MLX42/MLX42.h"
#include <stddef.h>
#include "Board.h"

#define SIDE_LENGTH 75

typedef enum PlayerType {
	BLUE,
	RED
}	PlayerType;

typedef struct Game {
	Board			board;
	mlx_image_t*	image;
	size_t			turn_count;
}	Game;

void	game_loop(void* param);
void	game_init(Game* game);

#endif
