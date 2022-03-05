/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mlx.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/13 17:37:45 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/04 22:59:58 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX.h"
#include <stdbool.h>
#include <stdlib.h>
#include "Error.h"

void*	mlx() {
	static void*	mlx = NULL;
	if (!mlx) {
		mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, false);
		if (!mlx) {
			FATAL(MLX_INIT_FAIL);
		}
	}
	return mlx;
}
