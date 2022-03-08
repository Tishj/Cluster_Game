/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 09:50:10 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/08 14:30:31 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

typedef enum CommandType {
	CMD_INVALID = -1,
	CMD_PLACE,
	CMD_ROTATE,
}	CommandType;

typedef struct Command {
	CommandType	type;
}	Command;

typedef struct CommandInvalid {
	Command	base;
}	CommandInvalid;

typedef struct	CommandPlace {
	Command	base;
	int	slot_index;
	int	color_index;
}	CommandPlace;

typedef struct CommandRotate {
	Command	base;
	int	cycles;
}	CommandRotate;

Command*	command_invalid(void);
Command*	command_place(int slot_index, int color_index);
Command*	command_rotate(int cycles);

Command*	command_parse(char* commandstring);
void		command_print(Command* command);


#endif
