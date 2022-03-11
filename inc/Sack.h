#ifndef SACK_H
# define SACK_H

//includes
#include "Board.h"
#include "Game.h"
#include <stdlib.h>

//Defines
#define COLORS_P_PLAYER 2

void	sack_init(Game* game);
void	sack_debug(Player* player);
void	sack_drawhand(Player *player, int remaining);
int		remaining_pellets_for_player(Player* player);

#endif
