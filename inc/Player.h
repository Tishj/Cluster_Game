/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Player.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 23:13:40 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/10 12:50:00 by tbruinem      ########   odam.nl         */
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
	int 		hand[2]; //Pellets currently in hand
	int			bag[2];	//Number of each pellet left in bag
	Connection	conn;
}	Player;

//fwd declare
typedef struct Game Game;

void		player_init(Player* player, PlayerType color, char* abspath);
Command*	player_get_command(Player* player, Game* game);
void	player_destroy(Player* player);

#endif
