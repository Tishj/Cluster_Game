/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   basic_bot.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/07 19:20:13 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/10 17:57:39 by limartin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	char*	line = NULL;
	size_t	capacity = 0;

	dprintf(2, "I'm gonna block\n");
	getline(&line, &capacity, stdin);
	printf("or am I?\n");
	dprintf(2, "LINE: %s\n", line);
	free(line);
	while (true) {
		
		printf("PLACE 1 1\n");
		usleep(100);
	}
}
