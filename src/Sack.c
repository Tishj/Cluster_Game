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
	//Round down to the first multiple of 4
	max_pellet -= (max_pellet % 4);
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
	[COLOR_BLUE0] = "BLUE0",
	[COLOR_RED0] = "RED0",
	[COLOR_BLUE1] = "BLUE1",
	[COLOR_RED1] = "RED1"
};

void	sack_debug(Player* player) {
	int offset = player->color == PLAYER_RED;
	for (size_t i = 0 ; i < COLORS_P_PLAYER; i++) {
		dprintf(STDERR_FILENO, "Hand[%s] = %d\n", color_names[offset + (i * 2)], player->hand[i]);
		dprintf(STDERR_FILENO, "Bag[%s] = %d\n", color_names[player->color + (i * 2)], player->bag[i]);
	}
}

int		remaining_pellets_for_player(Player* player) {
	int total = 0;
	for (size_t i = 0; i < COLORS_P_PLAYER; i++) {
		total += player->bag[i];
	}
	return total;
}

void	sack_drawhand(Player *player, int remaining){
	//Limit it to the max amount to draw per round
	if (remaining > COLORS_P_PLAYER)
		remaining = COLORS_P_PLAYER;
	for (int i = 0; i < remaining; i++) {
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
