#include "Sack.h"
#include "Player.h"

size_t	board_tiles(void){
	size_t tot = 1;

	for (size_t i = 1; i < SIDE_LENGTH; i++){
		tot += 6 * i;
	}
	return (tot);
}

void	sack_init(Game* game){
	// size_t RED_B_0	0
	// size_t RED_B_1	1
	// size_t BLUE_B_0	2
	// size_t BLUE_B_1	3

	size_t	max_pellet = (board_tiles()/4);
	for (int j = 0; j < PLAYER_SIZE; j++){
		for (int i = 0; i < 2; i++){
			game->player[j].bag[i] = max_pellet;
		}
	}
}

//Check to see if bags are empty. in main loop
bool	all_bags_empty(int bags[COLORS_P_PLAYER]) {
	bool empty = true;
	for (size_t i = 0; i < COLORS_P_PLAYER; i++) {
		if (bags[i]) {
			empty = false;
			break;
		}
	}
	return empty;
}

static const char* color_names[] = {
	[BLUE0] = "BLUE0",
	[RED0] = "RED0",
	[BLUE1] = "BLUE1",
	[RED1] = "RED1"
};

void	sack_debug(Player* player) {
	int offset = player->color == PLAYER_RED;
	for (size_t i = 0 ; i < COLORS_P_PLAYER; i++) {
		dprintf(STDERR_FILENO, "Hand[%s] = %d\n", color_names[offset + (i * 2)], player->hand[i]);
	}
}

void	sack_drawhand(Player *player){
	for (size_t i = 0; i < COLORS_P_PLAYER; i++) {
		while (true) {
			int color = rand() % 2;
			//
			if (player->bag[color]) {
				player->bag[color]--;
				player->hand[color]++;
				break;
			}
		}
	}
}
