/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Util.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 10:27:13 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 11:57:25 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

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
