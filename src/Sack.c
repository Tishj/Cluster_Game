#include "Sack.h"

void	sack_generate(Game game){
	int RED_0;
	int RED_1;
	int BLUE_0;
	int BLUE_1;

	RED_0 = 
		
}

size_t	board_tiles(size_t n){
	size_t tot = 1;

	for (size_t i = 1; i < n;  i++){
		tot += 6 * i;
	}
	return (tot);
}