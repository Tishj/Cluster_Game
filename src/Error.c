/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 23:00:42 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 11:25:32 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Error.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdnoreturn.h>

static const char* ErrorString[] = {
	[MLX_INIT_FAIL] = "MLX_INIT_FAIL",
	[MLX_WINDOW_INIT_FAIL] = "MLX_WINDOW_INIT_FAIL",
	[MLX_IMAGE_INIT_FAIL] = "MLX_IMAGE_INIT_FAIL",
	[MLX_PNG_IMAGE_INIT_FAIL] = "MLX_PNG_IMAGE_INIT_FAIL",
	[GAME_LEVEL_MISSING] = "GAME LEVEL MISSING",
	[MEMORY_ALLOCATION_FAIL] = "MEMORY_ALLOCATION_FAIL",
	[FILE_OPEN_FAIL] = "FILE_OPEN_FAIL",
	[INVALID_MAP_TILE] = "INVALID MAP TILE",
    [INVALID_SIDE] = "INVALID BOARD SIDE"
};

//Dont call this directly, use FATAL(error_code) instead
noreturn void	fatal(Error error_code, char* file, int line_number) {
	printf("FATAL ERROR: %s(%u) - Encountered in file '%s' on line %u\n", ErrorString[error_code], error_code, file, line_number);
	exit(1);
}
