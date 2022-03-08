/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Animation.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/08 15:41:04 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/08 15:51:27 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATION_H
# define ANIMATION_H

#include <stddef.h>
#include "MLX42/MLX42.h"

//Prototype to follow for the animation function
typedef void (*anim_f)(mlx_image_t* image, void* param);

//
typedef struct Animation {
	size_t			start_frame; //When the animation should be started
	mlx_image_t*	image;
	anim_f			func;
	void*			param;
}	Animation;

//All the animations that are scheduled for a game turn
typedef struct Transition {

}	Transition;

#endif
