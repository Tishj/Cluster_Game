/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Game.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 23:13:04 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/05 09:06:11 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

#include "cute_math2d.h"

typedef enum PlayerType {
	BLUE,
	RED
} PlayerType;

typedef enum Command {
	PLACE0,
	PLACE1,
	PLACE2,
	PLACE3,
	PLACE4,
	PLACE5,
	PLACE6,
	ROTATE
}	Command;

#endif
