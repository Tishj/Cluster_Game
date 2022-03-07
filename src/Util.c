/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 10:27:13 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/08 00:09:52 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include <math.h>
#include <sys/time.h>
#include <unistd.h>

void	wait_duration(size_t duration) {
	const unsigned long start_msec = time_msec();

	unsigned long current_time = time_msec();
	while (current_time - start_msec < duration) {
		usleep(100);
		current_time = time_msec();
	}
}

unsigned long	time_msec(void)
{
	struct timeval	time;
	unsigned long	ret;

	gettimeofday(&time, NULL);
	ret = (time.tv_sec * 1000);
	ret += (time.tv_usec / 1000);
	return (ret);
}

v2 rotate_point(float cx,float cy,float angle, v2 p)
{
	float s = sin(angle);
	float c = cos(angle);

	// translate point back to origin:
	p.x -= cx;
	p.y -= cy;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + cx;
	p.y = ynew + cy;
	return p;
}

v2	v2rotate(v2 a, double radians) {
	v2 new;

	new.x = a.x * cos(radians) - a.y * sin(radians);
	new.y = a.x * sin(radians) + a.y * cos(radians);
	return new;
}

v2	v2add(v2 a, v2 b) {
	return (v2){a.x + b.x, a.y + b.y};
}

int	max(int a, int b) {
	if (a > b)
		return a;
	return b;
}

int	min(int a, int b) {
	if (a < b)
		return a;
	return b;
}

int abs(int a) {
	return a > 0 ? a : a * -1;
}

int		sign(int a) {
	if (a < 0)
		return -1;
	return 1;
}

float	lerp(float a, float b, float t) {
	return a + (t * (b-a));
}

double		deg2rad(double angle)
{
	return (angle * (M_PI / 180));
}
