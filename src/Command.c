/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 09:53:25 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 22:18:47 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

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

void	command_print(Command command) {
	if (command.type == CMD_INVALID) {
		dprintf(2, "Command - INVALID - %d\n", command.value);
	}
	else {
		dprintf(2, "Command - %s - %d\n", valid_commands[command.type], command.value);
	}
}

Command	command_parse(char* commandstring) {
	// dprintf(2, "RECEIVED COMMAND: '%s'\n", commandstring);
	char* const	space_pos = strchr(commandstring, ' ');
	//Initialize to error
	Command		res = {
		.type = CMD_INVALID,
		.value = -1
	};

	//Commandstring does not contain a space;
	if (!space_pos) {
		return res;
	}
	*space_pos = '\0';
	res.type = str2cmp((char**)valid_commands, sizeof(valid_commands) / sizeof(valid_commands[0]), commandstring);
	//Not a valid command
	if (res.type == CMD_INVALID)
		return res;
	//Has to be a single digit character followed by a nullbyte
	if (isdigit(*(space_pos + 1)) && *(space_pos + 2) == '\0')
		res.value = *(space_pos + 1) - '0';
	else
		res.type = CMD_INVALID;
	return res;
}
