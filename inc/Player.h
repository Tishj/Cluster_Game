/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Player.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 23:13:40 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 22:52:55 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_H
# define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Command.h"

typedef enum PlayerType {
	PLAYER_BLUE,
	PLAYER_RED
}	PlayerType;

typedef struct Connection {
	pid_t		pid;
	int			input[2];
	int			output[2];
	bool		bot;
	FILE*		handle;
}	Connection;

typedef struct Player {
	PlayerType	color;
	Connection	conn;
}	Player;

//fwd declare
typedef struct Game Game;

void	player_init(Player* player, PlayerType color, char* abspath);
Command	player_get_command(Player* player, Game* game);

#endif
