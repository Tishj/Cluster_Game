/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   basic_bot.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/07 19:20:13 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/10 20:57:16 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	char*	line = NULL;
	size_t	capacity = 0;

	getline(&line, &capacity, stdin);
	dprintf(2, "LINE: %s\n", line);
	free(line);
	while (true) {
		printf("PLACE 2 2\n");
		usleep(100);
	}
}
