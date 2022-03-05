/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Util.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 10:26:07 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/05 12:09:04 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

#include <stddef.h>

typedef struct v2 {
	float	x;
	float	y;
}	v2;

v2	v2add(v2 a, v2 b);
int	max(int a, int b);
int	min(int a, int b);
int abs(int a);

#endif
