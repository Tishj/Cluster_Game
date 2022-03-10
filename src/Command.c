/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 09:53:25 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/10 20:51:28 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "Error.h"
#include <stdbool.h>
#include "Board.h"
#include "Game.h"

const char* valid_commands[] = {
	[CMD_PLACE] = "PLACE",
	[CMD_ROTATE] = "ROTATE"
};

int	str2cmp(char** str2, size_t size, char* str) {
	for (size_t i = 0; i < size; i++) {
		if (!strcmp(str2[i], str))
			return i;
	}
	return -1;
}

void	command_print(Command* command) {
	switch (command->type) {
		case CMD_INVALID: {
			dprintf(2, "Command - INVALID\n");
			break;
		};
		case CMD_PLACE: {
			CommandPlace* cmd = (void*)command;
			dprintf(2, "Command - PLACE - SLOT:%d - COLOR:%d\n", cmd->slot_index, cmd->color_index);
			break;
		};
		case CMD_ROTATE: {
			CommandRotate* cmd = (void*)command;
			dprintf(2, "Command - ROTATE - CYCLES:%d\n", cmd->cycles);
		}
	}
}

Command*	command_invalid() {
	CommandInvalid*	cmd = malloc(sizeof(CommandInvalid));
	if (!cmd) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	cmd->base.type = CMD_INVALID;
	return (void*)cmd;
}

Command*	command_place(int slot_index, int color_index) {
	CommandPlace*	cmd = malloc(sizeof(CommandPlace));
	if (!cmd) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	cmd->base.type = CMD_PLACE;
	cmd->color_index = color_index;
	cmd->slot_index = slot_index;
	return (void*)cmd;
}

Command*	command_rotate(int cycles) {
	CommandRotate*	cmd = malloc(sizeof(CommandRotate));
	if (!cmd) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	cmd->base.type = CMD_ROTATE;
	cmd->cycles = cycles;
	return (void*)cmd;
}

static bool	slot_index_check(int slot_index, Board* board) {
	const int max_index = (SIDE_LENGTH * 2) - 2;
	dprintf(2, "MAX_INDEX: %d\n", max_index);
	if (slot_index < 0 || slot_index > max_index)
		return false;
	Slot*	slot = get_insert_slot(board, board->side, slot_index);
	if (!slot)
		return false;
	if (slot->pellet != NULL)
		return false;
	return true;
}

//Take player to verify that the color is one chosen for them
static bool	color_index_check(int color_index, Player* player) {
	if (color_index < 0 || color_index > 3) {
		dprintf(2, "outside of valid range\n");
		return false;
	}
	// if (player->color == PLAYER_RED) {
	// 	color_index--;
	// }
	if (color_index % 2 != (int)player->color) {
		dprintf(2, "NOT YOUR COLOR!!\n");
		return false;
	}
	return (player->hand[color_index / 2] != 0);
}

static bool cycles_check(int cycles) {
	return (cycles >= 1 && cycles <= 5);
}

//Needs board to check for invalid placement
Command*	command_parse(char* commandstring, Player* player, Board* board) {
	if (!commandstring) {
		return command_invalid();
	}
	dprintf(2, "RECEIVED COMMAND: '%s'\n", commandstring);
	char* const	space_pos = strchr(commandstring, ' ');

	//Commandstring does not contain a space;
	if (!space_pos) {
		dprintf(2, "INVALID 1\n");
		return command_invalid();
	}
	*space_pos = '\0';
	CommandType	type = str2cmp((char**)valid_commands, sizeof(valid_commands) / sizeof(valid_commands[0]), commandstring);
	switch (type) {
		case CMD_INVALID: {
			dprintf(2, "INVALID 2\n");
			return command_invalid();
		};
		case CMD_ROTATE: {
			int cycles;
			if (sscanf(space_pos + 1, "%d\n", &cycles) == -1 || !cycles_check(cycles)) {
				dprintf(2, "INVALID 3\n");
				return command_invalid();
			}
			return command_rotate(cycles);
		};
		case CMD_PLACE: {
			int slot_index, color_index;
			char space;
			if (sscanf(space_pos + 1, "%d%c%d\n", &slot_index, &space, &color_index) == -1 || 
				!slot_index_check(slot_index, board) ||
				space != ' ' ||
				!color_index_check(color_index, player)) {
				return command_invalid();
			}
			return command_place(slot_index, color_index);
		}
	}
	return command_invalid();
}
