/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Player.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 23:09:04 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/10 23:25:41 by tbruinem      ########   odam.nl         */
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
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Sack.h"

#define GAME_CLIENT "./client"

//Bots get input sent to them from the game process
//Bots sent output to the game process

void*	read_line(void* param) {
	Connection*	connection = param;
	char*	line = NULL;
	size_t	capacity = 0;

	//Blocking - cant guarantee this will stop
	dprintf(2, "READING INPUT FROM BOT\n");
	if (getline(&line, &capacity, connection->handle) == -1) {
		return NULL;
	}
	dprintf(2, "DONE READING INPUT FROM BOT\n");
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

char*	connection_get_command(Connection* connection, size_t timeout) {
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
		// dprintf(2, "BOT TIMED OUT\n");
		return NULL;
	}
	return line;
}

void	connection_init(Connection* connection, char** abspath, bool bot) {
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
			execv(abspath[0], abspath);
			exit(1);
		}
		close(connection->input[READ]);
		close(connection->output[WRITE]);
		// connection->in = fdopen(connection->input[WRITE], "w");
		// if (!connection->in) {
		// 	FATAL(MEMORY_ALLOCATION_FAIL);
		// }
	}
	else {
		if (pipe(connection->output) == -1) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
		pid_t pid = fork();
		if (pid == 0) {
			dup2(connection->output[WRITE], STDOUT_FILENO);
			close(connection->output[WRITE]);
			execv(abspath[0], abspath);
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
	static BoardSide	side = SIDE_SOUTH;

	if (player->conn.bot == true) {
		int fd = player->conn.input[WRITE];
		//Initial input
		if (!game->state.turn_count) {
			print_slots(game->board.slots);
			int board_size = get_board_size();
			dprintf(fd, "%d\n", board_size);
			for (List* iter = game->board.slots; iter; iter = iter->next) {
				Slot* slot = iter->content;
				int	neighbour_indices[6] = {};
				for (size_t i = 0; i < 6; i++) {
					neighbour_indices[i] = slot->neighbours[i] ? (int)slot->neighbours[i]->index : -1;
				}
				dprintf(fd, "%d %d %d %d %d %d %d\n", (int)slot->index, neighbour_indices[0], neighbour_indices[1], neighbour_indices[2], neighbour_indices[3], neighbour_indices[4], neighbour_indices[5]);
			}
			dprintf(fd, "%d\n", COLOR_SIZE/2);
			for (size_t i = 0; i < COLOR_SIZE/2; i++) {
				dprintf(fd, "%ld %d\n", player->color + (i* 2), board_size/4);
			}
			dprintf(fd, "%d\n", COLOR_SIZE/2);
			for (size_t i = 0; i < COLOR_SIZE/2; i++) {
				dprintf(fd, "%ld %d\n", !player->color + (i* 2), board_size/4);
			}
		}
		//Normal round input
		dprintf(fd, "%d\n", game->board.side);

		//numberOfValidInsertSlots;
		Slot*	indices[(SIDE_LENGTH*2)-1] = {};
		size_t	valid_slots = 0;
		for (size_t i = 0; i < (SIDE_LENGTH*2)-1; i++) {
			indices[i] = get_insert_slot(&game->board, game->board.side, i);
			if (indices[i]->pellet) {
				indices[i] = NULL;
			}
			else {
				valid_slots++;
			}
		}
		dprintf(fd, "%ld\n", valid_slots);
		for (size_t i = 0; i < (SIDE_LENGTH*2)-1; i++) {
			if (!indices[i])
				continue;
			dprintf(fd, "%ld %ld\n", i, indices[i]->index);
		}

		//Rotation happened
		if (game->board.side != side) {
			dprintf(fd, "0\n"); //numberOfNewPellets
			dprintf(fd, "%ld\n", game->board.pellets_placed);
			for (List* iter = game->board.pellets; iter; iter = iter->next) {
				Pellet* pellet = iter->content;
				dprintf(fd, "%ld %ld\n", pellet->index, pellet->slot->index);
			}
		}
		else {
			List* iter = game->board.pellets;
			while (iter && iter->next) {
				iter = iter->next;
			}
			dprintf(fd, "%ld\n", player->missing_pellets);
			for (size_t i = 0; i < player->missing_pellets; i++) {
				Pellet* pellet = iter->content;
				bool is_mine = (pellet->color % 2) == player->color;
				dprintf(fd, "%ld %ld %d %d\n", pellet->index, pellet->slot->index, pellet->color, (int)is_mine);
				iter = iter->prev;
			}
			dprintf(fd, "0\n"); //numberOfChangedPellets
		}
		//Send pellets in hand
		int drawn_pellets = 0;
		for (size_t i = 0; i < COLORS_P_PLAYER; i++) {
			drawn_pellets += player->hand[i];
		}
		dprintf(fd, "%d\n", drawn_pellets);
		for (size_t i = 0; i < COLORS_P_PLAYER; i++) {
			for (int j = 0; j < player->hand[i]; j++) {
				dprintf(2, "PLAYER->HAND[I]: %d | I: %ld | J: %d\n", player->hand[i], i, j);
				dprintf(fd, "%d\n", player->color + ((int)i * 2));
				dprintf(2, "%d\n", player->color + ((int)i * 2));
			}
		}
		dprintf(2, "FINISHED SENDING INPUT\n");
	}
	player->missing_pellets = 0;
	side = game->board.side;
}

Command*	player_get_command(Player* player, Game* game) {
	player_send_input(player, game);
	size_t	timeout_duration = game->state.turn_count ? ROUND_TIMEOUT_DURATION : INITIAL_TIMEOUT_DURATION;
	char* line = connection_get_command(&player->conn, timeout_duration);
	Command* command = command_parse(line, player, &game->board);
	free(line);
	return command;
}

char**	get_abspath(char* program) {
	if (!program) {
		char**	arguments = malloc(sizeof(char*) * 2);
		if (!arguments) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
		arguments[0] = strdup(GAME_CLIENT);
		if (!arguments[0]) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
		arguments[1] = NULL;
		return arguments;
	}
	char**	arguments = split(program, ' ');
	if (!arguments) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	arguments[0] = expand_path(arguments[0]);
	return arguments;
}

void	connection_destroy(Connection* connection) {
	kill(connection->pid, SIGKILL);
	waitpid(connection->pid, NULL, 0);
}

void	player_destroy(Player* player) {
	connection_destroy(&player->conn);
}

void	player_init(Player* player, PlayerType color, char* program) {
	bool	bot = program != NULL;
	char**	abspath = get_abspath(program);
	connection_init(&player->conn, abspath, bot);
	print_str2(abspath);
	for (size_t i = 0; abspath[i]; i++) {
		free(abspath[i]);
	}
	free(abspath);
	player->missing_pellets = 0;
	player->color = color;
}
