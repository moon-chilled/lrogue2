/*
 * save.c
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

int write_failed = 0;
char *save_file = "";

void save_game() {
	char fname[64];

	if (!get_input_line("file name?", save_file, fname, "game not saved", 0, 1)) {
		return;
	}
	check_message();
	message(fname, 0);
	save_into_file(fname);
}

void save_into_file(char *sfile) {
	FILE *fp;
	char name_buffer[80];
	char *hptr;
	rogue_time rt_buf;

	if (sfile[0] == '~') {
		if ((hptr = getenv("HOME"))) {
			strcpy(name_buffer, hptr);
			strcat(name_buffer, sfile+1);
			sfile = name_buffer;
		}
	}
	if (!(fp = fopen(sfile, "w"))) {
		message("problem accessing the save file", 0);
		return;
	}
	md_ignore_signals();
	write_failed = 0;
	r_write(fp, (char *) &detect_monster, sizeof(detect_monster));
	r_write(fp, (char *) &cur_level, sizeof(cur_level));
	r_write(fp, (char *) &max_level, sizeof(max_level));
	write_string(hunger_str, fp);
	r_write(fp, (char *) &party_room, sizeof(party_room));
	r_write(fp, (char *) &party_counter, sizeof(party_counter));
	write_pack(&level_monsters, fp);
	write_pack(&level_objects, fp);
	rw_dungeon(fp, 1);
	r_write(fp, (char *) &foods, sizeof(foods));
	r_write(fp, (char *) &rogue, sizeof(fighter));
	write_pack(&rogue.pack, fp);
	rw_id_alloc(id_potions, fp, POTIONS, 1);
	rw_id_alloc(id_scrolls, fp, SCROLLS, 1);
	rw_id_alloc(id_wands, fp, WANDS, 1);
	rw_id_alloc(id_rings, fp, RINGS, 1);
	r_write(fp, (char *) traps, (MAX_TRAPS * sizeof(trap)));
	r_write(fp, (char *) is_wood, (WANDS * sizeof(boolean)));
	r_write(fp, (char *) &cur_room, sizeof(cur_room));
	rw_rooms(fp, 1);
	r_write(fp, (char *) &being_held, sizeof(being_held));
	r_write(fp, (char *) &bear_trap, sizeof(bear_trap));
	r_write(fp, (char *) &halluc, sizeof(halluc));
	r_write(fp, (char *) &blind, sizeof(blind));
	r_write(fp, (char *) &confused, sizeof(confused));
	r_write(fp, (char *) &levitate, sizeof(levitate));
	r_write(fp, (char *) &haste_self, sizeof(haste_self));
	r_write(fp, (char *) &see_invisible, sizeof(see_invisible));
	r_write(fp, (char *) &detect_monster, sizeof(detect_monster));
	r_write(fp, (char *) &wizard, sizeof(wizard));
	r_write(fp, (char *) &score_only, sizeof(score_only));
	r_write(fp, (char *) &m_moves, sizeof(m_moves));
	rt_buf = md_gct();
	rt_buf.second += 10;		// allow for some processing time
	r_write(fp, (char *) &rt_buf, sizeof(rt_buf));
	fclose(fp);

	if (write_failed) {
		md_df(sfile);	// delete file
	} else {
		clean_up("");
	}
}

void restore(char *fname) {
	FILE *fp;
	rogue_time saved_time, mod_time;
	char buf[4];
	char tbuf[40];

	if (!(fp = fopen(fname, "r"))) {
		clean_up("cannot open file");
	}
	r_read(fp, (char *) &detect_monster, sizeof(detect_monster));
	r_read(fp, (char *) &cur_level, sizeof(cur_level));
	r_read(fp, (char *) &max_level, sizeof(max_level));
	read_string(hunger_str, fp);

	r_read(fp, (char *) &party_room, sizeof(party_room));
	r_read(fp, (char *) &party_counter, sizeof(party_counter));
	read_pack(&level_monsters, fp, 0);
	read_pack(&level_objects, fp, 0);
	rw_dungeon(fp, 0);
	r_read(fp, (char *) &foods, sizeof(foods));
	r_read(fp, (char *) &rogue, sizeof(fighter));
	read_pack(&rogue.pack, fp, 1);
	rw_id_alloc(id_potions, fp, POTIONS, 0);
	rw_id_alloc(id_scrolls, fp, SCROLLS, 0);
	rw_id_alloc(id_wands, fp, WANDS, 0);
	rw_id_alloc(id_rings, fp, RINGS, 0);
	r_read(fp, (char *) traps, (MAX_TRAPS * sizeof(trap)));
	r_read(fp, (char *) is_wood, (WANDS * sizeof(boolean)));
	r_read(fp, (char *) &cur_room, sizeof(cur_room));
	rw_rooms(fp, 0);
	r_read(fp, (char *) &being_held, sizeof(being_held));
	r_read(fp, (char *) &bear_trap, sizeof(bear_trap));
	r_read(fp, (char *) &halluc, sizeof(halluc));
	r_read(fp, (char *) &blind, sizeof(blind));
	r_read(fp, (char *) &confused, sizeof(confused));
	r_read(fp, (char *) &levitate, sizeof(levitate));
	r_read(fp, (char *) &haste_self, sizeof(haste_self));
	r_read(fp, (char *) &see_invisible, sizeof(see_invisible));
	r_read(fp, (char *) &detect_monster, sizeof(detect_monster));
	r_read(fp, (char *) &wizard, sizeof(wizard));
	r_read(fp, (char *) &score_only, sizeof(score_only));
	r_read(fp, (char *) &m_moves, sizeof(m_moves));
	r_read(fp, (char *) &saved_time, sizeof(saved_time));

	if (fread(buf, sizeof(char), 1, fp) > 0) {
		message("extra characters in file; bug, probably", 1);
	}

	if ((!wizard) && !md_df(fname)) {
		clean_up("cannot delete file");
	}
	msg_cleared = 0;
	ring_stats(0);
	fclose(fp);
}

void write_pack(object *pack, FILE *fp) {
	object t;

	while ((pack = pack->next_object)) {
		r_write(fp, (char *) pack, sizeof(object));
	}
	t.ichar = t.what_is = 0;
	r_write(fp, (char *) &t, sizeof(object));
}

void read_pack(object *pack, FILE *fp, boolean is_rogue) {
	object read_obj, *new_obj;

	for (;;) {
		r_read(fp, (char *) &read_obj, sizeof(object));
		if (read_obj.ichar == 0) {
			pack->next_object = (object *) 0;
			break;
		}
		new_obj = alloc_object();
		*new_obj = read_obj;
		if (is_rogue) {
			if (new_obj->in_use_flags & BEING_WORN) {
				do_wear(new_obj);
			} else if (new_obj->in_use_flags & BEING_WIELDED) {
				do_wield(new_obj);
			} else if (new_obj->in_use_flags & (ON_EITHER_HAND)) {
				do_put_on(new_obj, ((new_obj->in_use_flags & ON_LEFT_HAND) ? 1 : 0));
			}
		}
		pack->next_object = new_obj;
		pack = new_obj;
	}
}

void rw_dungeon(FILE *fp, boolean rw) {
	int i, j;
	char buf[DCOLS];

	for (i = 0; i < DROWS; i++) {
		if (rw) {
			r_write(fp, (char *) dungeon[i], (DCOLS * sizeof(dungeon[0][0])));
			for (j = 0; j < DCOLS; j++) {
				buf[j] = mvinch(i, j);
			}
			r_write(fp, buf, DCOLS);
		} else {
			r_read(fp, (char *) dungeon[i], (DCOLS * sizeof(dungeon[0][0])));
			r_read(fp, buf, DCOLS);
			for (j = 0; j < DCOLS; j++) {
				mvaddch(i, j, buf[j]);
			}
		}
	}
}

void rw_id(id id_table[], FILE *fp, int n, boolean wr) {
	int i;

	for (i = 0; i < n; i++) {
		if (wr) {
			r_write(fp, (char *) &(id_table[i].value), sizeof(int));
			r_write(fp, (char *) &(id_table[i].id_status),
					sizeof(unsigned int));
			write_string(id_table[i].title, fp);
		} else {
			r_read(fp, (char *) &(id_table[i].value), sizeof(int));
			r_read(fp, (char *) &(id_table[i].id_status),
					sizeof(unsigned int));
			read_string(id_table[i].title, fp);
		}
	}
}

void rw_id_alloc(id id_table[], FILE *fp, int n, boolean wr) {
	int i;

	for (i = 0; i < n; i++) {
		if (wr) {
			r_write(fp, (char *) &(id_table[i].value), sizeof(int));
			r_write(fp, (char *) &(id_table[i].id_status),
					sizeof(unsigned int));
			write_string(id_table[i].title, fp);
		} else {
			r_read(fp, (char *) &(id_table[i].value), sizeof(int));
			r_read(fp, (char *) &(id_table[i].id_status),
					sizeof(unsigned int));
			id_table[i].title=(char *)malloc(80);
			read_string(id_table[i].title, fp);
		}
	}
}

void write_string(char *s, FILE *fp) {
	int n;

	n = strlen(s) + 1;
	r_write(fp, (char *) &n, sizeof(int));
	r_write(fp, s, n);
}

void read_string(char *s, FILE *fp) {
	int n;

	r_read(fp, (char *) &n, sizeof(int));
	r_read(fp, s, n);
}

void rw_rooms(FILE *fp, boolean rw) {
	int i;

	for (i = 0; i < MAXROOMS; i++) {
		rw ? r_write(fp, (char *) (rooms + i), sizeof(room)) :
			r_read(fp, (char *) (rooms + i), sizeof(room));
	}
}

void r_read(FILE *fp, char *buf, int n) {
	if (fread(buf, sizeof(char), n, fp) != n) {
		clean_up("read() failed, don't know why");
	}
}

void r_write(FILE *fp, char *buf, int n) {
	if (!write_failed) {
		if (fwrite(buf, sizeof(char), n, fp) != n) {
			message("write() failed, don't know why", 0);
			sound_bell();
			write_failed = 1;
		}
	}
}
