/*
 * rogue.h
 *
 * This source herein may be modified and/or distributed by anybody who
 * so desires, with the following restrictions:
 *    1.)  This notice shall not be removed.
 *    2.)  Credit shall not be taken for the creation of this source.
 *    3.)  This code is not to be traded, sold, or used for personal
 *         gain or profit.
 *
 */

#ifndef ROGUE_H
#define ROGUE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>

#define boolean char

#define NOTHING		((unsigned int) 0x0)
#define OBJECT		((unsigned int) 0x1)
#define MONSTER		((unsigned int) 0x2)
#define STAIRS		((unsigned int) 0x4)
#define HORWALL		((unsigned int) 0x8)
#define VERTWALL	((unsigned int) 0x10)
#define DOOR		((unsigned int) 0x20)
#define FLOOR		((unsigned int) 0x40)
#define TUNNEL		((unsigned int) 0x80)
#define TRAP		((unsigned int) 0x100)
#define HIDDEN		((unsigned int) 0x200)

#define ARMOR		((unsigned int) 0x1)
#define WEAPON		((unsigned int) 0x2)
#define SCROLL		((unsigned int) 0x4)
#define POTION		((unsigned int) 0x8)
#define GOLD		((unsigned int) 0x10)
#define FOOD		((unsigned int) 0x20)
#define WAND		((unsigned int) 0x40)
#define RING		((unsigned int) 0x80)
#define AMULET		((unsigned int) 0x100)
#define ALL_OBJECTS	((unsigned int) (ARMOR | WEAPON | SCROLL | POTION | GOLD | FOOD | WAND | RING | AMULET))

#define LEATHER 0
#define RINGMAIL 1
#define SCALE 2
#define CHAIN 3
#define BANDED 4
#define SPLINT 5
#define PLATE 6
#define ARMORS 7

#define BOW 0
#define DART 1
#define ARROW 2
#define DAGGER 3
#define SHURIKEN 4
#define MACE 5
#define LONG_SWORD 6
#define TWO_HANDED_SWORD 7
#define WEAPONS 8

#define MAX_PACK_COUNT 24

#define PROTECT_ARMOR 0
#define HOLD_MONSTER 1
#define ENCH_WEAPON 2
#define ENCH_ARMOR 3
#define IDENTIFY 4
#define TELEPORT 5
#define SLEEP 6
#define SCARE_MONSTER 7
#define REMOVE_CURSE 8
#define CREATE_MONSTER 9
#define AGGRAVATE_MONSTER 10
#define MAGIC_MAPPING 11
#define SCROLLS 12

#define INCREASE_STRENGTH 0
#define RESTORE_STRENGTH 1
#define HEALING 2
#define EXTRA_HEALING 3
#define POISON 4
#define RAISE_LEVEL 5
#define BLINDNESS 6
#define HALLUCINATION 7
#define DETECT_MONSTER 8
#define DETECT_OBJECTS 9
#define CONFUSION 10
#define LEVITATION 11
#define HASTE_SELF 12
#define SEE_INVISIBLE 13
#define POTIONS 14

#define TELE_AWAY 0
#define SLOW_MONSTER 1
#define CONFUSE_MONSTER 2
#define INVISIBILITY 3
#define POLYMORPH 4
#define HASTE_MONSTER 5
#define PUT_TO_SLEEP 6
#define MAGIC_MISSILE 7
#define CANCELLATION 8
#define DO_NOTHING 9
#define WANDS 10

#define STEALTH 0
#define R_TELEPORT 1
#define REGENERATION 2
#define SLOW_DIGEST 3
#define ADD_STRENGTH 4
#define SUSTAIN_STRENGTH 5
#define DEXTERITY 6
#define ADORNMENT 7
#define R_SEE_INVISIBLE 8
#define MAINTAIN_ARMOR 9
#define SEARCHING 10
#define RINGS 11

#define RATION 0
#define FRUIT 1

#define NOT_USED	((unsigned int) 0x0)
#define BEING_WIELDED	((unsigned int) 0x1)
#define BEING_WORN	((unsigned int) 0x2)
#define ON_LEFT_HAND	((unsigned int) 0x4)
#define ON_RIGHT_HAND	((unsigned int) 0x8)
#define ON_EITHER_HAND	((unsigned int) (ON_LEFT_HAND | ON_RIGHT_HAND))
#define BEING_USED	((unsigned int) (NOT_USED | BEING_WIELDED | BEING_WORN | ON_LEFT_HAND | ON_RIGHT_HAND))

#define NO_TRAP -1
#define TRAP_DOOR 0
#define BEAR_TRAP 1
#define TELE_TRAP 2
#define DART_TRAP 3
#define SLEEPING_GAS_TRAP 4
#define RUST_TRAP 5
#define TRAPS 6

#define STEALTH_FACTOR 3
#define R_TELE_PERCENT 8

#define UNIDENTIFIED	((unsigned int) 0x0)	// MUST BE ZERO!
#define IDENTIFIED	((unsigned int) 0x1)
#define CALLED		((unsigned int) 0x2)

#define DROWS 24
#define DCOLS 80
#define MAX_TITLE_LENGTH 30
#define MORE "-more-"
#define MAXSYLLABLES 40
#define MAX_METAL 14
#define WAND_MATERIALS 30
#define GEMS 14

#define GOLD_PERCENT 46

typedef struct id id;
struct id {
	int value;
	char *title;
	char *real;
	unsigned int id_status;
};

/* The following #defines provide more meaningful names for some of the
 * struct object fields that are used for monsters.  This, since each monster
 * and object (scrolls, potions, etc) are represented by a struct object.
 * Ideally, this should be handled by some kind of union structure.
 */

#define m_damage damage
#define hp_to_kill quantity
#define m_char ichar
#define first_level is_protected
#define last_level is_cursed
#define m_hit_chance class
#define stationary_damage identified
#define drop_percent which_kind
#define trail_char d_enchant
#define slowed_toggle quiver
#define moves_confused hit_enchant
#define nap_length picked_up
#define disguise what_is
#define next_monster next_object

typedef struct object object;
struct object {			// comment is monster meaning
	unsigned long m_flags;	// monster flags
	char damage[64];		// damage it does
	int quantity;		// hit points to kill
	int ichar;		// 'A' is for aquatar
	int kill_exp;		// exp for killing it
	int is_protected;	// level starts
	int is_cursed;		// level ends
	int class;		// chance of hitting you
	int identified;		// 'F' damage, 1,2,3...
	unsigned int which_kind; // item carry/drop %
	int o_row, o_col, o;	// o is how many times stuck at o_row, o_col
	int row, col;		// current row, col
	int d_enchant;		// room char when detect_monster
	int quiver;		// monster slowed toggle
	int trow, tcol;		// target row, col
	int hit_enchant;	// how many moves is confused
	unsigned int what_is;	// imitator's charactor (?!%:
	int picked_up;		// sleep from wand of sleep
	unsigned int in_use_flags;
	struct object *next_object;	// next monster
};

#define INIT_HP 12

typedef struct fighter fighter;
struct fighter {
	object *armor;
	object *weapon;
	object *left_ring, *right_ring;
	int hp_current;
	int hp_max;
	int str_current;
	int str_max;
	object pack;
	long gold;
	int exp;
	long exp_points;
	int row, col;
	int fchar;
	int moves_left;
};

typedef struct door door;
struct door {
	int oth_room;
	int oth_row,
	oth_col;
	int door_row,
	door_col;
};

typedef struct room room;
struct room {
	char bottom_row, right_col, left_col, top_row;
	door doors[4];
	unsigned int is_room;
};

#define MAXROOMS 9
#define BIG_ROOM 10

#define NO_ROOM -1

#define PASSAGE -3		// cur_room value

#define AMULET_LEVEL 26

#define R_NOTHING	((unsigned int) 0x1)
#define R_ROOM		((unsigned int) 0x2)
#define R_MAZE		((unsigned int) 0x4)
#define R_DEADEND	((unsigned int) 0x8)
#define R_CROSS		((unsigned int) 0x10)

#define MAX_EXP_LEVEL 21
#define MAX_EXP 10000000L
#define MAX_GOLD 900000
#define MAX_ARMOR 99
#define MAX_HP 800
#define MAX_STRENGTH 99
#define LAST_DUNGEON 99

#define STAT_LEVEL	0x1
#define STAT_GOLD	0x2
#define STAT_HP		0x4
#define STAT_STRENGTH	0x8
#define STAT_ARMOR	0x10
#define STAT_EXP	0x20
#define STAT_HUNGER	0x40
#define STAT_LABEL	0x80
#define STAT_ALL	(STAT_LEVEL | STAT_GOLD | STAT_HP | STAT_STRENGTH | STAT_ARMOR | STAT_EXP | STAT_HUNGER | STAT_LABEL)

#define PARTY_TIME 10	// one party somewhere in each 10 level span

#define MAX_TRAPS 10	// maximum traps per level

#define HIDE_PERCENT 12

typedef struct trap trap;
struct trap {
	int trap_type;
	int trap_row, trap_col;
};

extern fighter rogue;
extern room rooms[];
extern trap traps[];
extern unsigned int dungeon[DROWS][DCOLS];
extern object level_objects;

extern id id_scrolls[];
extern id id_potions[];
extern id id_wands[];
extern id id_rings[];
extern id id_weapons[];
extern id id_armors[];

extern object mon_tab[];
extern object level_monsters;

#define MONSTERS 26

#define HASTED				0x1L
#define SLOWED				0x2L
#define INVISIBLE			0x4L
#define ASLEEP				0x8L
#define WAKENS				0x10L
#define WANDERS				0x20L
#define FLIES				0x40L
#define FLITS				0x80L
#define CAN_FLIT			0x100L		// can, but usually doesn't, flit
#define CONFUSED	 		0x200L
#define RUSTS				0x400L
#define HOLDS				0x800L
#define FREEZES				0x1000L
#define STEALS_GOLD			0x2000L
#define STEALS_ITEM			0x4000L
#define STINGS			   	0x8000L
#define DRAINS_LIFE		   	0x10000L
#define DROPS_LEVEL		   	0x20000L
#define SEEKS_GOLD		  	0x40000L
#define FREEZING_ROGUE	 	 	0x80000L
#define RUST_VANISHED	  		0x100000L
#define CONFUSES			0x200000L
#define IMITATES			0x400000L
#define FLAMES				0x800000L
#define STATIONARY			0x1000000L	// damage will be 1,2,3,...
#define NAPPING				0x2000000L	// can't wake up for a while
#define ALREADY_MOVED			0x4000000L

#define SPECIAL_HIT		(RUSTS|HOLDS|FREEZES|STEALS_GOLD|STEALS_ITEM|STINGS|DRAINS_LIFE|DROPS_LEVEL)

#define WAKE_PERCENT 45
#define FLIT_PERCENT 33
#define PARTY_WAKE_PERCENT 75

#define HYPOTHERMIA 1
#define STARVATION 2
#define POISON_DART 3
#define QUIT 4
#define WIN 5

#define UP 0
#define UPRIGHT 1
#define RIGHT 2
#define RIGHTDOWN 3
#define DOWN 4
#define DOWNLEFT 5
#define LEFT 6
#define LEFTUP 7
#define DIRS 8

#define ROW1 7
#define ROW2 15

#define COL1 26
#define COL2 52

#define MOVED 0
#define MOVE_FAILED -1
#define STOPPED_ON_SOMETHING -2
#define CANCEL '\033'
#define LIST '*'

#define HUNGRY 300
#define WEAK 150
#define FAINT 20
#define STARVE 0

#define MIN_ROW 1

typedef struct rogue_time rogue_time;
struct rogue_time {
	int year;	// >= 1987
	int month;	// 1 - 12
	int day;	// 1 - 31
	int hour;	// 0 - 23
	int minute;	// 0 - 59
	int second;	// 0 - 59
};
#endif // ROGUE_H
