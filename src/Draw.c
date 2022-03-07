/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Draw.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 11:59:23 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 16:08:15 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "Board.h"
#include "util.h"
#include "Draw.h"
#include "MLX.h"
#include "Game.h"
#include <math.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

void	draw_pixel(mlx_image_t* target, unsigned int color, v2 pos) {
	if ((pos.x >= 0 && pos.x < target->width) && (pos.y >= 0 && pos.y < target->height))
		mlx_put_pixel(target, pos.x, pos.y, color);
}

//Bresenham
void	draw_line(mlx_image_t* target, unsigned int color, v2 start, v2 end) {
	// printf("START(X:%f|Y:%f) - END(X:%f|Y:%f)\n", start.x, start.y, end.x, end.y);

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

void draw_circle_points(mlx_image_t* target, int xc, int yc, int x, int y, unsigned int color)
{
	draw_pixel(target, color, (v2){xc+x,yc+y});
	draw_pixel(target, color, (v2){xc-x, yc+y});
	draw_pixel(target, color, (v2){xc+x, yc-y});
	draw_pixel(target, color, (v2){xc-x, yc-y});
	draw_pixel(target, color, (v2){xc+y, yc+x});
	draw_pixel(target, color, (v2){xc-y, yc+x});
	draw_pixel(target, color, (v2){xc+y, yc-x});
	draw_pixel(target, color, (v2){xc-y, yc-x});
}

void draw_circle(mlx_image_t* target, int xc, int yc, int r, unsigned int color)
{
	for(int y=-r; y<=r; y++)
    	for(int x=-r; x<=r; x++)
        	if(x*x+y*y <= r*r)
            	draw_pixel(target, color, (v2){xc + x, yc+y});
}

void	draw_hexagon_sides(mlx_image_t* target, unsigned int color, v2* points) {
	for (size_t i = 0; i < 6; i++) {
		draw_line(target, color, points[i], points[(i+1)%6]);
	}
}

static const unsigned int color_mapping[] = {
	[BLUE0] =	0x55c1dbff,
	[RED0] =	0xb03d2bff
};

void	draw_slot(Board* board, v2 pos, mlx_image_t* target) {
	Slot* slot = &board->map[(int)pos.y][(int)pos.x];

	//Create temporary copy of the points
	v2	points[6];
	memcpy(points, slot->points, sizeof(v2) * 6);

	//Apply rotation to the points
	double rotation_angle = 60 * board->side;
	double radians = deg2rad(rotation_angle);

	// for (size_t i = 0; i < 6; i++) {
	// 	// //Offset the point based on the pivot
	// 	// v2 position = {
	// 	// 	.x = points[i].x - board->center.x,
	// 	// 	.y = points[i].y - board->center.y
	// 	// };

	// 	// //Rotate the position by x degrees
	// 	// position = v2rotate(position, radians);

	// 	// //Re-add the pivot back to the point
	// 	// points[i].x = position.x + board->center.x;
	// 	// points[i].y = position.y + board->center.y;
	// 	points[i] = rotate_point(board->center.x, board->center.y, radians, points[i]);
	// }

	draw_hexagon_sides(target, CLR_RED, points);
	(void)radians;

	//Draw pellet/token if present
	if (slot->color == EMPTY)
		return;
	v2 middle = (v2){slot->points[0].x + ((slot->points[3].x - slot->points[0].x) / 2), slot->points[0].y};
	middle = rotate_point(board->center.x, board->center.y, -radians, middle);
	draw_circle(target, middle.x, middle.y, 30, color_mapping[slot->color]);
}
