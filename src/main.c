/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/04 22:54:27 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/05 10:12:21 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "cluster.h"
#include "MLX.h"

//Provide absolute path to bot files to play the game. If empty inputs are controlled with the keyboard instead.
int main(int argc, char **argv) {
	//Keyboard input
	if (argc == 1) {
		;
	}
	else if (argc == 3) {
		
	}
	else {
		return (1);
	}
	
	return (0);
}
