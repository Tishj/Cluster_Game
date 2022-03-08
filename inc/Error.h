/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Error.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 23:00:53 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/08 11:37:05 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

typedef enum {
	MLX_INIT_FAIL,
	MLX_WINDOW_INIT_FAIL,
	MLX_IMAGE_INIT_FAIL,
	MLX_PNG_IMAGE_INIT_FAIL,
	GAME_LEVEL_MISSING,
	MEMORY_ALLOCATION_FAIL,
	FILE_OPEN_FAIL,
	INVALID_MAP_TILE,
    INVALID_SIDE,
	CLIENT_READ_FAIL,
	THREAD_FAIL,
	PIPE_CREATION_FAIL,
}	Error;

#define FATAL(x) fatal(x, __FILE__, __LINE__)

void	fatal(Error error_code, char *file, int line_number);

#endif
