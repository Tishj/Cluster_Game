/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cluster.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 22:54:51 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/04 23:06:12 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLUSTER_H
# define CLUSTER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Object {
	int a;
}	Object;

typedef struct BotConnection {
	pid_t	pid;
	int		pipe[2];
}	BotConnection;

typedef struct Engine {
	BotConnection	bot[2];
}	Engine;

#endif
