/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Util.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 10:26:07 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/05 10:26:57 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

typedef struct v2 {
	float	x;
	float	y;
}	v2;

v2	v2add(v2 a, v2 b);

#endif
