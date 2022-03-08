/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   List.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/29 15:46:17 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/08 18:37:12 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "List.h"
#include "Error.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

List	*list_popback(List **list)
{
	List	*last;
	List	*elem;

	last = *list;
	if (*list == NULL)
		return (NULL);
	while (last->next)
		last = last->next;
	if (*list == last)
		return (list_popfront(list));
	elem = last;
	last->prev->next = NULL;
	last = last->prev;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}

void 	list_delete(List** list, List* elem) {
	dprintf(2, "YEET LIST:%p | ELEM:%p | *LIST:%p\n", list, elem, *list);
	if (!elem)
		return;
	//List is empty
	if (*list == NULL)
		return ;
	//first element
	if (*list == elem) {
		*list = elem->next;
		elem->prev = NULL;
		return;
	}
	dprintf(2, "HOW ARE YOU DYING\n");
	if (elem->next)
		elem->next->prev = elem->prev;
	assert(elem->prev != NULL);
	elem->prev->next = elem->next;
	free(elem);
}

size_t	list_size(List* list) {
	size_t	size = 0;
	while (list) {
		list = list->next;
		size++;
	}
	return size;
}

List	*list_popfront(List **list)
{
	List	*first;

	first = *list;
	if (!first)
		return (NULL);
	*list = (*list)->next;
	if (*list)
		(*list)->prev = NULL;
	first->next = NULL;
	first->prev = NULL;
	return (first);
}

void	list_pushback(List **list, List *elem)
{
	List	*iter;

	if (*list == NULL)
	{
		list_pushfront(list, elem);
		return ;
	}
	iter = *list;
	while (iter->next)
		iter = iter->next;
	iter->next = elem;
	elem->prev = iter;
}

void	list_pushfront(List **list, List *elem)
{
	List	*head;

	head = *list;
	*list = elem;
	elem->next = head;
	if (head)
		head->prev = elem;
}

void	list_clear(List *list)
{
	List	*del;

	while (list)
	{
		del = list;
		list = list->next;
		free(del);
	}
}

List	*list_new(void* content)
{
	List	*list;

	list = malloc(sizeof(List));
	if (!list) {
		FATAL(MEMORY_ALLOCATION_FAIL);
	}
	list->content = content;
	list->next = NULL;
	list->prev = NULL;
	return (list);
}
