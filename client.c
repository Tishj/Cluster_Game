/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/05 23:42:39 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 19:02:57 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main() {
	extern int errno;

	while (true) {
		char*	line = NULL;
		size_t	bytes = 0;
		if (getline(&line, &bytes, stdin) == -1) {

			perror("getline");
			dprintf(2, "(%d) CLIENT DIED TRYING TO READ INPUT\n", errno);
			exit(1);
		}
		write(STDOUT_FILENO, line, strlen(line));
		free(line);
	}
}
