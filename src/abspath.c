/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   abspath.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/09 22:28:06 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/09 22:50:46 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#ifndef PATH_MAX
# define PATH_MAX 1024
#endif

static char*	try_expand(char *entry, char *program)
{
	char		tmp[PATH_MAX];
	struct stat	unused;
	char*		ret;

	ret = memcpy(tmp, entry, strlen(entry));
	ret += strlen(entry);
	*ret = '/';
	ret = memcpy(ret + 1, program, strlen(program));
	ret += strlen(program);
	*ret = '\0';

	if (stat(tmp, &unused) != -1)
	{
		return strdup(tmp);
	}
	return (NULL);
}

char*	expand_path(char *program)
{
	char	*entry;
	char	path[PATH_MAX] = {};

	memcpy(path, getenv("PATH"), strlen(getenv("PATH")));
	entry = strtok(path, ":");
	while (entry)
	{
		char* expanded = try_expand(entry, program);
		if (expanded != NULL) {
			free(program);
			return expanded;
		}
		entry = strtok(NULL, ":");
	}
	return program;
}
