/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Sort.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/06 09:44:05 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/07 12:24:43 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SORT_H
# define SORT_H

#include <stdbool.h>

typedef bool (*lower_f)(void* a, void* b, void* extra);

typedef struct Lower {
	lower_f	func;
	void*	extra;
}	Lower;

void	quicksort(void* arr, Range range, size_t typesize, Lower lower);

#endif
