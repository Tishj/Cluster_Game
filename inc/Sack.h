#ifndef SACK_H
# define SACK_H

//includes
#include "Board.h"
#include "Game.h"
#include <stdlib.h>

//Defines
#define COLORS_P_PLAYER 2
#define MAX_RAND 32767

void	sack_init(Game* game);
void	sack_debug(Player* player);
void	sack_drawhand(Player *player);
int sack_probaility(Player* player);

#endif
