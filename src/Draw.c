/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Draw.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 11:59:23 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/05 15:02:17 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "Util.h"
#include "Draw.h"
#include "MLX.h"
#include "Game.h"
#include <math.h>
#include <stdio.h>

void	draw_pixel(mlx_image_t* target, unsigned int color, v2 pos) {
	if ((pos.x >= 0 && pos.x < target->width) && (pos.y >= 0 && pos.y < target->height))
		mlx_put_pixel(target, pos.x, pos.y, color);
}

//Bresenham
void	draw_line(mlx_image_t* target, unsigned int color, v2 start, v2 end) {
	printf("START(X:%f|Y:%f) - END(X:%f|Y:%f)\n", start.x, start.y, end.x, end.y);

	int dx = abs((int)(end.x - start.x)), sx = start.x < end.x ? 1 : -1;
	int dy = abs((int)(end.y - start.y)), sy = start.y < end.y ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;

	while (draw_pixel(target, color, (v2){start.x, start.y}), start.x != end.x || start.y != end.y) {
		int e2 = err;
		if (e2 > -dx) {
			err -= dy;
			start.x += sx;
		}
		if (e2 <  dy) {
			err += dx;
			start.y += sy;
		}
	}
}

void	draw_fill(mlx_image_t* target, unsigned int color) {
	for (unsigned int y = 0; y < target->height; y++) {
		for (unsigned int x = 0; x < target->width; x++) {
			draw_pixel(target, color, (v2){x, y});
		}
	}
}

float HexWidth(float height)
{
	return (float)(4 * (height / 2 / sqrt(3)));
}

void	get_hex_points(v2* points, float height, float row, float col)
{
	// Start with the leftmost corner of the upper left hexagon.
	float width = HexWidth(height);
	float y = height / 2;
	float x = 0;

	// Move down the required number of rows.
	y += row * height;

	// If the column is odd, move down half a hex more.
	if ((int)col % 2 == 1) y += height / 2;

	// Move over for the column number.
	x += col * (width * 0.75f);

	// Generate the points.
	points[0] = (v2){(int)x,(int)y};
	points[1] = (v2){(int)(x + width * 0.25),(int)(y - height / 2)};
	points[2] = (v2){(int)(x + width * 0.75),(int)(y - height / 2)};
	points[3] = (v2){(int)(x + width),(int)y};
	points[4] = (v2){(int)(x + width * 0.75),(int)(y + height / 2)};
	points[5] = (v2){(int)(x + width * 0.25),(int)(y + height / 2)};
}

void	draw_hexagon_sides(mlx_image_t* target, unsigned int color, size_t row, size_t col) {
	v2	points[6];

	get_hex_points(points, 75, row, col);
	for (size_t i = 0; i < 6; i++) {
		draw_line(target, color, points[i], points[(i+1)%6]);
	}
}
