/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   List.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/08 15:48:05 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/09 16:08:05 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

#include <stddef.h>

typedef struct List
{
	struct List	*next;
	struct List	*prev;
	void		*content;
}	List;

void 	list_delete(List** list, List* elem, void (*del_f)(void*));
List	*list_popback(List **list);
List	*list_popfront(List **list);
void	list_pushback(List **list, List *elem);
void	list_pushfront(List **list, List *elem);
List	*list_new(void* content);
void	list_clear(List *list);
size_t	list_size(List* list);

#endif
