/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Player.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 23:09:04 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/11 12:42:41 by tbruinem      ########   odam.nl         */
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
#include <limits.h>
#include <errno.h>
#include <stdio.h>

#define GAME_CLIENT "./client"

//Bots get input sent to them from the game process
//Bots sent output to the game process

void*	read_line(void* param) {
	Connection*	connection = param;
	char*	line = NULL;
	size_t	capacity = 0;

	//Blocking - cant guarantee this will stop
	if (getline(&line, &capacity, connection->handle) == -1) {
		return NULL;
	}
	if (line == NULL) {
		return line;
	}
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
	//(Not protected because this *should* never be able to fail)
	if (connection->bot) {
		pthread_cancel(thread);
	}
	char*	line = NULL;
	if (pthread_join(thread, (void**)&line) == -1) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	//If the thread was canceled before it finished, it was timed out (Untested)
	if (line == PTHREAD_CANCELED) {
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
		if (pid == -1) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
		if (pid == 0) {
			//Read end of input pipe is connected to STDIN
			if (dup2(connection->input[READ], STDIN_FILENO) == -1) {
				FATAL(MEMORY_ALLOCATION_FAIL);
			}
			if (close(connection->input[READ]) == -1) {
				FATAL(MEMORY_ALLOCATION_FAIL);
			}

			if (dup2(connection->output[WRITE], STDOUT_FILENO) == -1) {
				FATAL(MEMORY_ALLOCATION_FAIL);
			}
			if (close(connection->output[WRITE]) == -1) {
				FATAL(MEMORY_ALLOCATION_FAIL);
			}

			if (close(connection->output[READ]) == -1) {
				FATAL(MEMORY_ALLOCATION_FAIL);
			}
			if (close(connection->input[WRITE]) == -1) {
				FATAL(MEMORY_ALLOCATION_FAIL);
			}

			execv(abspath[0], abspath);
			exit(1);
		}
		connection->pid = pid;
		if (close(connection->input[READ]) == -1) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
		if (close(connection->output[WRITE]) == -1) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
	}
	else {
		if (pipe(connection->output) == -1) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
		pid_t pid = fork();
		if (pid == -1) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
		if (pid == 0) {
			if (dup2(connection->output[WRITE], STDOUT_FILENO) == -1) {
				FATAL(MEMORY_ALLOCATION_FAIL);
			}
			if (close(connection->output[WRITE]) == -1) {
				FATAL(MEMORY_ALLOCATION_FAIL);
			}

			if (close(connection->output[READ]) == -1) {
				FATAL(MEMORY_ALLOCATION_FAIL);
			}

			if (execv(abspath[0], abspath) == -1) {
				FATAL(MEMORY_ALLOCATION_FAIL);
			}
			exit(1);
		}
		connection->pid = pid;
		if (close(connection->output[WRITE]) == -1) {
			FATAL(MEMORY_ALLOCATION_FAIL);
		}
	}
	dprintf(2, "initializing connection->handle\n");
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
			dprintf(fd, "%d\n", (SIDE_LENGTH*2)-1); //numberOfColumns
			for (int i = 0; i < (SIDE_LENGTH*2)-1; i++) {
				int directions[6];
				for (int side = 0; side < 6; side++) {
					directions[side] = get_insert_slot(&game->board, side, i)->index;
				}
				dprintf(fd, "%d %d %d %d %d %d %d\n", i, directions[0], directions[1], directions[2], directions[3], directions[4], directions[5]);
			}
			//yourColors
			dprintf(fd, "%d\n", COLOR_SIZE/2);
			for (size_t i = 0; i < COLOR_SIZE/2; i++) {
				dprintf(fd, "%ld %d\n", player->color + (i* 2), board_size/4);
			}
			//opponentColors
			dprintf(fd, "%d\n", COLOR_SIZE/2);
			for (size_t i = 0; i < COLOR_SIZE/2; i++) {
				dprintf(fd, "%ld %d\n", !player->color + (i* 2), board_size/4);
			}
		}
		//Normal round input
		dprintf(fd, "%d\n", game->board.side);

		//numberOfValidColumns;
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

		// Send pellets in hand
		int drawn_pellets = 0;
		for (size_t i = 0; i < COLORS_P_PLAYER; i++) {
			drawn_pellets += player->hand[i];
		}
		dprintf(fd, "%d\n", drawn_pellets);
		dprintf(2, "%d\n", drawn_pellets);

		int counter = 0;
		int in_hand[drawn_pellets];
		for (int i = 0; i < COLORS_P_PLAYER; i++) {
			for (int j = 0; j < player->hand[i]; j++) {
				dprintf(2, "PLAYER->HAND[I]: %d | I: %d | J: %d\n", player->hand[i], i, j);
				int color_index = player->color + ((int)i * 2);
				in_hand[counter++] = color_index;
			}
		}

		for (int i = 0; i < drawn_pellets; i++) {
			dprintf(fd, "%d\n", in_hand[i]);
			dprintf(2, "%d\n", in_hand[i]);
		}
		dprintf(fd, "%s", "\n");
		fsync(fd);
		fflush(player->conn.handle);

		dprintf(2, "FINISHED SENDING INPUT\n");
	}
	player->missing_pellets = 0;
	side = game->board.side;
}

Command*	player_get_command(Player* player, Game* game) {
	player_send_input(player, game);
	fflush(player->conn.handle);
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
	if (fclose(connection->handle) == -1) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
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
