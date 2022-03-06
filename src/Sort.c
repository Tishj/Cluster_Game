/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Sort.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/06 09:43:48 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/06 10:16:45 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Sort.h"
#include <stddef.h>
#include <stdbool.h>
#include <strings.h>
#include <assert.h>
#include <string.h>
#include "Util.h"

static void	swap(void* a, void *b, size_t typesize) {
	char	tmp[typesize];
	memcpy(tmp, a, typesize);
	memcpy(a, b, typesize);
	memcpy(b, tmp, typesize);
}

static size_t	partition(char* arr, Range range, size_t typesize, Lower lower) {
	const size_t	pivot_position = (range.end * typesize);
	char*			index = arr + (range.start * typesize);

	for (size_t i = range.start; i < range.end; i++) {
		if (lower.func(&arr[i], &arr[pivot_position], lower.extra)) {
			swap(index, arr + (i * typesize), typesize);
			index += typesize;
		}
	}
	swap(arr + pivot_position, index, typesize);
	return (index - arr) / typesize;
}

void	quicksort(void* arr, Range range, size_t typesize, Lower lower) {
	if (range.start >= range.end) {
		return;
	}
	const size_t	pivot_index = partition(arr, range, typesize, lower);
	const Range lower_range = {
		.start = range.start,
		.end = pivot_index - 1
	};
	const Range upper_range = {
		.start = pivot_index + 1,
		.end = range.end
	};
	quicksort(arr, lower_range, typesize, lower);
	quicksort(arr, upper_range, typesize, lower);
}
