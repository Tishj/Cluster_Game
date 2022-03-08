/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   List.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/08 15:48:05 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/08 15:50:01 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

typedef struct List
{
	struct List	*next;
	struct List	*prev;
	void		*content;
}	List;

#endif
