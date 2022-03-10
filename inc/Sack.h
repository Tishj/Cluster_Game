#ifndef SACK_H
# define SACK_H

//includes
#include "Board.h"
#include "Game.h"
#include <stdlib.h>

//structs
/*
typedef struct Pellet {
	PelletType		color
}	Pellet;
*/
typedef struct p_list{
	enum PelletType	color;
	struct p_list	*next;
}	P_list;

#endif