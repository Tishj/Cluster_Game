/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 09:53:25 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/08 14:49:43 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "Error.h"
#include <stdbool.h>

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

static bool	slot_index_check(int slot_index) {
	return (slot_index >= 0 && slot_index <= 6);
}

static bool	color_index_check(int color_index) {
	return (color_index >= 0 && color_index <= 3);
}

static bool cycles_check(int cycles) {
	return (cycles >= 1 && cycles <= 5);
}

Command*	command_parse(char* commandstring) {
	// dprintf(2, "RECEIVED COMMAND: '%s'\n", commandstring);
	char* const	space_pos = strchr(commandstring, ' ');

	//Commandstring does not contain a space;
	if (!space_pos) {
		return command_invalid();
	}
	*space_pos = '\0';
	CommandType	type = str2cmp((char**)valid_commands, sizeof(valid_commands) / sizeof(valid_commands[0]), commandstring);
	switch (type) {
		case CMD_INVALID: {
			return command_invalid();
		};
		case CMD_ROTATE: {
			int cycles;
			if (sscanf(space_pos + 1, "%d\n", &cycles) == -1 || !cycles_check(cycles)) {
				return command_invalid();
			}
			return command_rotate(cycles);
		};
		case CMD_PLACE: {
			int slot_index, color_index;
			char space;
			if (sscanf(space_pos + 1, "%d%c%d\n", &slot_index, &space, &color_index) == -1 || !slot_index_check(slot_index) || space != ' ' || !color_index_check(color_index)) {
				return command_invalid();
			}
			return command_place(slot_index, color_index);
		}
	}
}
