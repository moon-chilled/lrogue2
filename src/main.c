/*
 * main.c
 *
 * This source herein may be modified and/or distributed by anybody who
 * so desires, with the following restrictions:
 *    1.)  No portion of this notice shall be removed.
 *    2.)  Credit shall not be taken for the creation of this source.
 *    3.)  This code is not to be traded, sold, or used for personal
 *         gain or profit.
 *
 */

#include "rogue.h"
#include "extern.h"
#include <locale.h>

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "en_US.UTF-8");

	if (init(argc, argv)) {		// restored game
		goto playlevel;
	}

	for (;;) {
		clear_level();
		make_level();
		put_objects();
		put_stairs();
		add_traps();
		put_mons();
		put_player(party_room);
		print_stats(STAT_ALL);
playlevel:
		play_level();
		free_stuff(&level_objects);
		free_stuff(&level_monsters);
	}
}
