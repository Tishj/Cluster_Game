/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Draw.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 11:59:23 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/10 15:11:22 by tbruinem      ########   odam.nl         */
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
#include "Game.h"

void	draw_pixel(mlx_image_t* target, unsigned int color, v2 pos) {
	if ((pos.x >= 0 && pos.x < target->width) && (pos.y >= 0 && pos.y < target->height))
		mlx_put_pixel(target, pos.x, pos.y, color);
}

//Bresenham
void	draw_line(mlx_image_t* target, unsigned int color, v2 start, v2 end) {
	int x1 = start.x;
	int y1 = start.y;
	int x2 = end.x;
	int y2 = end.y;
	int dy = y2 - y1;
	int dx = x2 - x1;
	int x, y;

	if (abs(dy) > abs(dx)) {
		// since there is a greater change in y than x we must
		// loop in y, calculate x and draw
		for (y=y1; y != y2; y += sign(dy)) {
			x = x1 + (y - y1) * dx / dy;
			draw_pixel(target, color, (v2){x,y});
		}
	}
	 
	else {
		// since there is a greater (or equal) change in x than y we must
		// loop in x, calculate y and draw
		for (x=x1; x != x2; x += sign(dx)) {
			y = y1 + (x - x1) * dy / dx;
			draw_pixel(target, color, (v2){x,y});
		}
	}
 
	// draw the last pixel
	draw_pixel(target, color, (v2){x,y});
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
	[RED0] =	0xb03d2bff,
	[BLUE1] =	0x0000ffff,
	[RED1] =	0xff0000ff
};

void	draw_pellet(Board* board, Pellet* pellet, mlx_image_t* target) {
	Slot*	slot = pellet->slot;
	assert(slot != NULL);

	double rotation = lerp(board->tween.from, board->tween.to, board->tween.progress);

	v2 middle = (v2){slot->points[0].x + ((slot->points[3].x - slot->points[0].x) / 2), slot->points[0].y};
	middle = rotate_point(board->center.x, board->center.y, -rotation, middle);
	draw_circle(target, middle.x, middle.y, PELLET_RADIUS, color_mapping[pellet->color]);
}

void	draw_slot(Board* board, Slot* slot, mlx_image_t* target) {

	//Create temporary copy of the points
	v2	points[6];
	memcpy(points, slot->points, sizeof(v2) * 6);

	double rotation = lerp(board->tween.from, board->tween.to, board->tween.progress);

	for (size_t i = 0; i < 6; i++) {
		points[i] = rotate_point(board->center.x, board->center.y, -rotation, points[i]);
	}
	draw_hexagon_sides(target, 0xffffffaa, points);
}
