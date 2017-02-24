#ifndef EXTERN_H
#define EXTERN_H

#define boolean char

/* hit.c */
void mon_hit(object*, char*, boolean);
void rogue_hit(object*, boolean);
void rogue_damage(int, object*);
int get_damage(char*, boolean);
int get_w_damage(object*);
int get_number(char*);
long lget_number(char*);
int to_hit(object*);
int damage_for_strength();
boolean mon_damage(object*, int);
void fight(boolean);
void get_dir_rc(int, int*, int*, boolean);
int get_hit_chance(object*);
int get_weapon_damage(object*);


/* init.c */
int init(int, char*[]);
void player_init();
void clean_up(char*);
void start_window();
void stop_window();
void byebye();
void onintr();
void error_save();
void do_args(int, char*[]);
void do_opts();
void env_get_value(char **s, char *e, boolean add_blank);


/* instructions.c */
void Instructions();


/* inventory.c */
void inventory(object*, unsigned int);
void mix_colors();
void make_scroll_titles();
void get_desc(object*, char*);
void get_wand_and_ring_materials();
void single_inv(int);
id *get_id_table(object*);
void inv_armor_weapon(boolean);


/* level.c */
void make_level();
void make_room(int, int, int, int);
boolean connect_rooms(int, int);
void clear_level();
void put_door(room*, int, int*, int*);
void draw_simple_passage(int, int, int, int, int);

/* tentative, might change because inputs weren't originally specified */
boolean same_row(int, int);
boolean same_col(int, int);

void add_mazes();
void fill_out_level();
void fill_it(int, boolean);
void recursive_deadend(int, int*, int, int);
boolean mask_room(int, int*, int*, unsigned int);
void make_maze(int, int, int, int, int, int);
void hide_boxed_passage(int, int, int, int, int);
void put_player(int);
boolean drop_check();
boolean check_up();
void add_exp(int, boolean);
int get_exp_level(long);
int hp_raise();
void show_average_hp();
void mix_random_rooms();


/* machdep.c */
void md_control_keyboard(int);
void md_heed_signals();
void md_ignore_signals();
int md_get_file_id(char*);
int md_link_count(char*);
void md_gct(rogue_time*);
void md_gfmt(char*, rogue_time*);
boolean md_df(char*);
char *md_gln();
int md_gseed();


/* main.c */
void turn_into_games();
void turn_into_user();


/* message.c */
void message(char*, boolean);
void remessage();
void check_message();
int get_input_line(char*, char*, char*, char*, boolean, boolean);
int rgetchar();
void print_stats(int);
void pad(char*, int);
void save_screen();
void sound_bell();
boolean is_digit(int);
int r_index(char*, int, boolean);


/* monster.c */
void put_mons();
object *gr_monster(object*, int);
void mv_mons();
void party_monsters(int, int);
int gmc_row_col(int, int);
int gmc(object*);
void mv_monster(object*, int, int);
boolean mtry(object*, int, int);
void move_mon_to(object*, int, int);
boolean mon_can_go(object*, int, int);
void wake_up(object*);
void wake_room(int, boolean, int, int);
char *mon_name(object*);
boolean rogue_is_around(int, int);
void wanderer();
void show_monsters();
void create_monster();
void put_m_at(int, int, object*);
void aim_monster(object*);
boolean rogue_can_see(int, int);
boolean move_confused(object*);
boolean flit(object*);
char gr_obj_char();
boolean no_room_for_monster(int);
void aggravate();
boolean mon_sees(object*, int, int);
void mv_aquatars();


/* move.c */
int one_move_rogue(int, int);
void multiple_move_rogue(char);
boolean is_passable(int, int);
boolean next_to_something(int, int);
boolean can_move(int, int, int, int);
void move_onto();
boolean is_direction(char);
boolean check_hunger(boolean);
boolean reg_move();
void rest(int);
int gr_dir();
void heal();


/* object.c */
void put_objects();
void put_gold();
void plant_gold(int, int, boolean);
void place_at(object*, int, int);
object *object_at(object*, int, int);
object *get_letter_object(int);
void free_stuff(object*);
char *name_of(object*);
object *gr_object();
unsigned int gr_what_is();
void gr_scroll(object*);
void gr_potion(object*);
void gr_weapon(object*, boolean);
void gr_armor(object*);
void gr_wand(object*);
void get_food(object*, boolean);
void put_stairs();
int get_armor_class(object*);
object *alloc_object();
void free_object(object*);
void make_party();
void show_objects();
void put_amulet();
void rand_place(object*);
void new_object_for_wizard();
int next_party();


/* pack.c */
object *add_to_pack(object*, object*, boolean);
void take_from_pack(object*, object*);
object *pick_up(int, int, int*);
void drop();
object *check_duplicate(object*, object*);
char next_avail_ichar();
void wait_for_ack();
int pack_letter(char*, unsigned int);
void take_off();
void wear();
void unwear(object*);
void do_wear(object*);
void wield();
void do_wield(object*);
void unwield(object*);
void call_it();
int pack_count(object*);
boolean mask_pack(object*, unsigned int);
boolean is_pack_letter(int*, unsigned int*);
boolean has_amulet();
void kick_into_pack();


/* play.c */
void play_level();


/* random.c */
void srrandom(int);
long rrandom();
int get_rand(int, int);
int rand_percent(int);
boolean coin_toss();


/* ring.c */
void put_on_ring();
void do_put_on(object*, boolean);
void remove_ring();
void un_put_on(object*);
void gr_ring(object*, boolean);
void inv_rings();
void ring_stats(boolean);


/* room.c */
void light_up_room(int);
void light_passage(int, int);
void darken_room(int);
char get_dungeon_char(int, int);
char get_mask_char(unsigned int);
void gr_row_col(int*, int*, unsigned int);
int gr_room();
int party_objects(int);
int get_room_number(int, int);
boolean is_all_connected();
void visit_rooms(int);
void draw_magic_map();
void dr_course(object*, boolean, int, int);
boolean get_oth_room(int, int*, int*);


/* save.c */
void save_game();
void save_into_file(char*);
void restore(char*);
void write_pack(object*, FILE*);
void read_pack(object*, FILE*, boolean);
void rw_dungeon(FILE*, boolean);
void rw_id(id[], FILE*, int, boolean);
void rw_id_alloc(id[], FILE*, int, boolean);
void write_string(char*, FILE*);
void read_string(char*, FILE*);
void rw_rooms(FILE*, boolean);
void r_read(FILE*, char*, int);
void r_write(FILE*, char*, int);
boolean has_been_touched(rogue_time*, rogue_time*);


/* score.c */
void killed_by(object*, int);
void win();
void quit(boolean);
void put_scores(object*, int);
void insert_score(char[][82], char[][30], char*, int, int, object*, boolean);
boolean is_vowel(char);
void sell_pack();
int get_value(object*);
void id_all();
int name_cmp(char*, char*);
void xxxx(char*, int);
long xxx(boolean);
void nickize(char*, char*, char*);
void center(int, char*);
void sf_error();


/* spec_hit.c */
void special_hit(object*);
void rust(object*);
void freeze(object*);
void steal_gold(object*);
void steal_item(object*);
void disappear(object*);
void cough_up(object*);
boolean try_to_cough(int, int, object*);
boolean seek_gold(object*);
boolean gold_at(int, int);
void check_gold_seeker(object*);
boolean check_imitator(object*);
boolean imitating(int, int);
void sting(object*);
void drop_level();
void drain_life();
boolean m_confuse(object*);
boolean flame_broil(object*);
void get_closer(int*, int*, int, int);


/* throw.c */
void throw();
boolean throw_at_monster(object*, object*);
object *get_thrown_at_monster(object*, int, int*, int*);
void flop_weapon(object*, int, int);
void rand_around(int, int*, int*);


/* trap.c */
int trap_at(int, int);
void trap_player(int, int);
void add_traps();
void id_trap();
void show_traps();
void search(int, boolean);


/* use.c */
void quaff();
void read_scroll();
void vanish(object*, int, object*);
void potion_heal(boolean);
void identify();
void eat();
void hold_monster();
void tele();
void hallucinate();
void unhallucinate();
void unblind();
void relight();
void take_a_nap();
void go_blind();
char *get_ench_color();
void confuse();
void unconfuse();
void uncurse_all();


/* zap.c */
void zapp();
object *get_zapped_monster(int, int*, int*);
void zap_monster(object*, unsigned int);
void tele_away(object*);
void wizardize();




#endif /* EXTERN_H */
