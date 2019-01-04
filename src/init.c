/*
 * init.c
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

char login_name[30];
char *nick_name = "";
char *rest_file = 0;
boolean cant_int = 0, did_int = 0, score_only, init_curses = 0;
boolean save_is_interactive = 1;
boolean ask_quit = 1, show_skull = 1;
char *error_file = "rogue.esave";
char *byebye_string = "Okay, bye bye!";

int init(int argc, char *argv[]) {
	char *pn;

	do_args(argc, argv);
	do_opts();

	pn = md_gln();
	if (!pn) {
		strcpy(login_name, "Example user");
	} else {
		strcpy(login_name, pn);
		free(pn);
	}


	if (!score_only && !rest_file) {
		printf("Hello %s, just a moment while I dig the dungeon...",
		((nick_name[0]) ? nick_name : login_name));
		fflush(stdout);
	}

	initscr();
	if ((LINES < DROWS) || (COLS < DCOLS)) {
		clean_up("must be played on 24 x 80 or better screen");
	}
	start_window();
	init_curses = 1;

	md_heed_signals();

	if (score_only) {
		put_scores((object *) 0, 0);
	}
	srrandom(getseed());
	if (rest_file) {
		restore(rest_file);
		return 1;
	}
	mix_colors();
	get_wand_and_ring_materials();
	make_scroll_titles();

	level_objects.next_object = 0;
	level_monsters.next_monster = 0;
	player_init();
	party_counter = get_rand(1, PARTY_TIME);
	ring_stats(0);
	return 0;
}

void player_init() {
	object *obj;

	rogue.pack.next_object = 0;

	obj = alloc_object();
	get_food(obj, 1);
	add_to_pack(obj, &rogue.pack, 1);

	obj = alloc_object();		// initial armor
	obj->what_is = ARMOR;
	obj->which_kind = RINGMAIL;
	obj->class = RINGMAIL+2;
	obj->is_protected = 0;
	obj->d_enchant = 1;
	add_to_pack(obj, &rogue.pack, 1);
	do_wear(obj);

	obj = alloc_object();		// initial weapons
	obj->what_is = WEAPON;
	obj->which_kind = MACE;
	obj->damage = "2d3";
	obj->hit_enchant = obj->d_enchant = 1;
	obj->identified = 1;
	add_to_pack(obj, &rogue.pack, 1);
	do_wield(obj);

	obj = alloc_object();
	obj->what_is = WEAPON;
	obj->which_kind = BOW;
	obj->damage = "1d2";
	obj->hit_enchant = 1;
	obj->d_enchant = 0;
	obj->identified = 1;
	add_to_pack(obj, &rogue.pack, 1);

	obj = alloc_object();
	obj->what_is = WEAPON;
	obj->which_kind = ARROW;
	obj->quantity = get_rand(25, 35);
	obj->damage = "1d2";
	obj->hit_enchant = 0;
	obj->d_enchant = 0;
	obj->identified = 1;
	add_to_pack(obj, &rogue.pack, 1);
}

void clean_up(char *estr) {
	if (save_is_interactive) {
		if (init_curses) {
			move(DROWS-1, 0);
			refresh();
			stop_window();
		}
		printf("\n%s\n", estr);
	}
	endwin();
	exit(0);
}

void start_window() {
	crmode();
	noecho();
	nonl();
	md_control_keyboard(0);
}

void stop_window() {
	endwin();
	md_control_keyboard(1);
}

void byebye() {
	md_ignore_signals();
	if (ask_quit) {
		quit(1);
	} else {
		clean_up(byebye_string);
	}
	md_heed_signals();
}

void onintr() {
	md_ignore_signals();
	if (cant_int) {
		did_int = 1;
	} else {
		check_message();
		message("interrupt", 1);
	}
	md_heed_signals();
}

void error_save() {
	save_is_interactive = 0;
	save_into_file(error_file);
	clean_up("");
}

void do_args(int argc, char *argv[]) {
	int i, j;

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-s")) {
			score_only = 1;
		} else if (!strcmp(argv[i], "-w")) {
			wizard = 1;
		} else {
			rest_file = argv[i];
		}
	}
}

void do_opts() {
	char *eptr;

	if ((eptr = getenv("ROGUEOPTS"))) {
		for (;;) {
			while ((*eptr) == ' ') {
				eptr++;
			}
			if (!(*eptr)) {
				break;
			}
			if (!strncmp(eptr, "fruit=", 6)) {
				eptr += 6;
				env_get_value(&fruit, eptr, 1);
			} else if (!strncmp(eptr, "file=", 5)) {
				eptr += 5;
				env_get_value(&save_file, eptr, 0);
			} else if (!strncmp(eptr, "nojump", 6)) {
				jump = 0;
			} else if (!strncmp(eptr, "name=", 5)) {
				eptr += 5;
				env_get_value(&nick_name, eptr, 0);
			} else if (!strncmp(eptr, "noaskquit", 9)) {
				ask_quit = 0;
			} else if (!strncmp(eptr, "noskull", 5) ||
					!strncmp(eptr,"notomb", 6)) {
				show_skull = 0;
			}
			while ((*eptr) && (*eptr != ',')) {
				eptr++;
			}
			if (!(*(eptr++))) {
				break;
			}
		}
	}
}

void env_get_value(char **s, char *e, bool add_blank) {
	int i = 0;
	char *t;

	t = e;

	while ((*e) && (*e != ',')) {
		if (*e == ':') {
			*e = ';';		// ':' reserved for score file purposes
		}
		e++;
		if (++i >= 30) {
			break;
		}
	}
	if (!(*s = malloc(i + (add_blank ? 2 : 1)))) {
		clean_up("cannot alloc() memory");
	}
	strncpy(*s, t, i);
	if (add_blank) {
		(*s)[i++] = ' ';
	}
	(*s)[i] = '\0';
}
