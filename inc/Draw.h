/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Draw.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 12:10:31 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 11:57:25 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAW_H
# define DRAW_H

#include "MLX42/MLX42.h"
#include "util.h"

#define CLR_TRANSPARENT 0x00000000
#define CLR_WHITE		0xffffffff
#define CLR_RED			0xff0000ff

void	draw_pixel(mlx_image_t* target, unsigned int color, v2 pos);
void	draw_line(mlx_image_t* target, unsigned int color, v2 start, v2 end);
void	draw_fill(mlx_image_t* target, unsigned int color);
void	draw_hexagon_sides(mlx_image_t* target, unsigned int color, v2* points);
void	draw_circle(mlx_image_t* target, int xc, int yc, int r, unsigned int color);
void	draw_slot(Slot* slot, mlx_image_t* target);

#endif
