/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   util.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 10:26:07 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/09 22:46:14 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

#include <stddef.h>

typedef enum {
	READ,
	WRITE,
}	IO;

typedef struct v2 {
	float	x;
	float	y;
}	v2;

typedef struct Range {
	size_t start;
	size_t end;
}	Range;

v2	v2rotate(v2 a, double radians);
v2	v2add(v2 a, v2 b);
int	max(int a, int b);
int	min(int a, int b);
int abs(int a);
double		deg2rad(double angle);
v2 rotate_point(float cx,float cy,float angle, v2 p);
unsigned long	time_msec(void);
void	wait_duration(size_t duration);
int		sign(int a);
float	lerp(float a, float b, float t);
char	**split(const char *s, char c);
char*	expand_path(char *program);
void	print_str2(char** str);

#endif
