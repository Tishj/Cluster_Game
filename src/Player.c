/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Player.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 23:09:04 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/06 15:44:41 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Player.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "Error.h"

#define GAME_CLIENT "./client"

//Bots get input sent to them from the game process
//Bots sent output to the game process
void	bot_init(Player* player, char* abspath) {
	if (pipe(player->input) == -1) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	if (pipe(player->output) == -1) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	pid_t pid = fork();
	if (pid == 0) {
		//Read end of input pipe is connected to STDIN
		dup2(player->input[0], STDIN_FILENO);
		close(player->input[0]);

		dup2(player->output[1], STDOUT_FILENO);
		close(player->output[1]);
		execv(abspath, (char*[]){abspath, NULL});
		exit(1);
	}
	close(player->input[0]);
	close(player->output[1]);
}

//Players sent output to the game process
void	player_init(Player* player, char* abspath) {
	player->bot = (abspath != NULL);
	if (player->bot) {
		bot_init(player, abspath);
	}
	else {
		abspath = GAME_CLIENT;

		if (pipe(player->output) == -1) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
		pid_t pid = fork();
		if (pid == 0) {
			dup2(player->output[1], STDOUT_FILENO);
			close(player->output[1]);
			execv(abspath, (char*[]){abspath, NULL});
			exit(1);
		}
		close(player->output[1]);
	}
}
