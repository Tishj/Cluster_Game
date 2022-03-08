/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Player.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 23:09:04 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/08 14:42:13 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Player.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "Error.h"
#include "Game.h"
#include "Command.h"
#include <sys/time.h>
#include <pthread.h>

#define GAME_CLIENT "./client"

//Bots get input sent to them from the game process
//Bots sent output to the game process

void*	read_line(void* param) {
	Connection*	connection = param;
	char*	line = NULL;
	size_t	capacity = 0;

	//Blocking - cant guarantee this will stop
	if (getline(&line, &capacity, connection->handle) == -1) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	if (line == NULL) {
		return line;
	}
	//deprecated
	// for (size_t i = 0; line[i]; i++) {
	// 	if (line[i] == '\n')
	// 		line[i] = '\0';
	// }
	return line;
}

Command*	connection_get_command(Connection* connection, size_t timeout) {
	pthread_t		thread;

	//Start a thread that is going to try to read a line of input from the bot
	if (pthread_create(&thread, NULL, read_line, connection) == -1) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}

	wait_duration(timeout);

	//Only timeout if the player is a bot
	if (connection->bot) {
		pthread_cancel(thread);
	}
	char*	line = NULL;
	if (pthread_join(thread, (void**)&line) == -1) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	//If the thread was canceled before it finished, it was timed out (Untested)
	if (line == PTHREAD_CANCELED) {
		return command_invalid();
	}
	Command* command = command_parse(line);
	free(line);
	return command;
}

void	connection_init(Connection* connection, char* abspath, bool bot) {
	connection->bot = bot;
	if (bot == true) {
		if (pipe(connection->input) == -1) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
		if (pipe(connection->output) == -1) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
		pid_t pid = fork();
		if (pid == 0) {
			//Read end of input pipe is connected to STDIN
			dup2(connection->input[READ], STDIN_FILENO);
			close(connection->input[READ]);

			dup2(connection->output[WRITE], STDOUT_FILENO);
			close(connection->output[WRITE]);
			execv(abspath, (char*[]){abspath, NULL});
			exit(1);
		}
		close(connection->input[READ]);
		close(connection->output[WRITE]);
	}
	else {
		if (pipe(connection->output) == -1) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
		pid_t pid = fork();
		if (pid == 0) {
			dup2(connection->output[WRITE], STDOUT_FILENO);
			close(connection->output[WRITE]);
			execv(abspath, (char*[]){abspath, NULL});
			exit(1);
		}
		close(connection->output[WRITE]);
	}
	connection->handle = fdopen(connection->output[READ], "r");
	if (!connection->handle) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
}

void	player_send_input(Player* player, Game* game) {
	if (player->conn.bot != true) {
		return;
	}
	(void)game;
}

Command*	player_get_command(Player* player, Game* game) {
	player_send_input(player, game);
	size_t	timeout_duration = game->state.turn_count ? ROUND_TIMEOUT_DURATION : INITIAL_TIMEOUT_DURATION;
	return connection_get_command(&player->conn, timeout_duration);
}

void	player_init(Player* player, PlayerType color, char* abspath) {
	bool	bot = abspath != NULL;
	connection_init(&player->conn, bot ? abspath : GAME_CLIENT, bot);

	player->color = color;
}
