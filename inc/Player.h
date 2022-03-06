/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Player.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 23:13:40 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/06 14:59:13 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_H
# define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct Player {
	pid_t	pid;
	int		input[2];
	int		output[2];
	bool	bot;
}	Player;

#endif
