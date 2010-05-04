/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/****************************************************************************
 * [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   *
 * -----------------------------------------------------------|   (0...0)   *
 * SMAUG 1.0 (C) 1994, 1995, 1996, 1998  by Derek Snider      |    ).:.(    *
 * -----------------------------------------------------------|    {o o}    *
 * SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,      |~'~.VxvxV.~'~*
 * Tricops and Fireblade                                      |             *
 * ------------------------------------------------------------------------ *
 * Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
 * Chastain, Michael Quan, and Mitchell Tse.                                *
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
 * Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
 * ------------------------------------------------------------------------ *
 * 			Database management module			    *
 ****************************************************************************/

#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#ifndef WIN32
#include <dirent.h>
#else
#define strcasecmp strcmp
#endif
#include "mud.h"

void init_supermob(  );

extern void load_world( CHAR_DATA * ch );
void *zlib_alloc( void *opaque, unsigned int items, unsigned int size );
void zlib_free( void *opaque, void *address );

/*
 * Globals.
 */
#ifdef COMPRESSION_TEST
ROOM_DECOMP *first_decompress;
ROOM_DECOMP *last_decompress;
#endif

WIZENT *first_wiz;
WIZENT *last_wiz;

time_t last_restore_all_time = 0;

HELP_DATA *first_help;
HELP_DATA *last_help;

SHOP_DATA *first_shop;
SHOP_DATA *last_shop;

REPAIR_DATA *first_repair;
REPAIR_DATA *last_repair;

TELEPORT_DATA *first_teleport;
TELEPORT_DATA *last_teleport;

PROJECT_DATA *first_project;
PROJECT_DATA *last_project;

OBJ_DATA *extracted_obj_queue;
EXTRACT_CHAR_DATA *extracted_char_queue;

char bug_buf[2 * MAX_INPUT_LENGTH];
CHAR_DATA *first_char;
CHAR_DATA *last_char;
char *help_greeting;
char *help_greeting2;   /* Josh */
char log_buf[2 * MAX_INPUT_LENGTH];

OBJ_DATA *first_object;
OBJ_DATA *last_object;
TIME_INFO_DATA time_info;
WEATHER_DATA weather_info;

int weath_unit;   /* global weather param */
int rand_factor;
int climate_factor;
int neigh_factor;
int max_vector;

int cur_qobjs;
int cur_qchars;
int nummobsloaded;
int numobjsloaded;
int physicalobjects;
int last_pkroom;

AUCTION_DATA *auction;  /* auctions */

/* weaponry */
short gsn_pugilism;
short gsn_long_blades;
short gsn_short_blades;
short gsn_flexible_arms;
short gsn_talonous_arms;
short gsn_maces;
short gsn_shieldwork;
short gsn_missile_weapons;

/* thief */
short gsn_detrap;
short gsn_backstab;
short gsn_circle;
short gsn_dodge;
short gsn_hide;
short gsn_peek;
short gsn_pick_lock;
short gsn_sneak;
short gsn_steal;
short gsn_gouge;
short gsn_poison_weapon;

/* thief & warrior */
short gsn_disarm;
short gsn_enhanced_damage;
short gsn_kick;
short gsn_parry;
short gsn_rescue;
short gsn_second_attack;
short gsn_third_attack;
short gsn_fourth_attack;
short gsn_fifth_attack;
short gsn_sixth_attack;
short gsn_seventh_attack;
short gsn_eighth_attack;
short gsn_dual_wield;
short gsn_punch;
short gsn_bash;
short gsn_stun;
short gsn_bashdoor;
short gsn_grip;
short gsn_berserk;
short gsn_hitall;
short gsn_tumble;

/* vampire */
short gsn_feed;
short gsn_bloodlet;
short gsn_broach;
short gsn_mistwalk;

/* other   */
short gsn_bleeding;
short gsn_aid;
short gsn_track;
short gsn_search;
short gsn_dig;
short gsn_mount;
short gsn_bite;
short gsn_claw;
short gsn_sting;
short gsn_tail;
short gsn_scribe;
short gsn_brew;
short gsn_climb;
short gsn_cook;
short gsn_scan;
short gsn_slice;
short gsn_study;
short gsn_assassinate;
short gsn_hunt;
short gsn_meditate;
short gsn_ruminate;
short gsn_trance;

/* spell casting skills -- Scion */
short gsn_second_cast;
short gsn_third_cast;
short gsn_fourth_cast;
short gsn_fifth_cast;
short gsn_sixth_cast;
short gsn_seventh_cast;
short gsn_eighth_cast;

/* spells */
short gsn_aqua_breath;
short gsn_blindness;
short gsn_charm_person;
short gsn_curse;
short gsn_invis;
short gsn_mass_invis;
short gsn_poison;
short gsn_sleep;
short gsn_possess;
short gsn_fireball;
short gsn_chill_touch;
short gsn_lightning_bolt;
short gsn_haste;
short gsn_slow;
short gsn_silence;

/* languages */
short gsn_common;
short gsn_elven;
short gsn_dwarven;
short gsn_pixie;
short gsn_ogre;
short gsn_orcish;
short gsn_trollish;
short gsn_goblin;
short gsn_halfling;

/* for searching */
short gsn_first_spell;
short gsn_first_skill;
short gsn_first_weapon;
short gsn_first_tongue;
short gsn_top_sn;

/* For styles?  Trying to rebuild from some kind of accident here - Blod */
short gsn_style_evasive;
short gsn_style_defensive;
short gsn_style_standard;
short gsn_style_aggressive;
short gsn_style_berserk;
/* sharpen skill --Cronel */
short gsn_sharpen;
short gsn_listen;

/*
 * Locals.
 */
MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
OBJ_INDEX_DATA *obj_index_hash[MAX_KEY_HASH];
ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];

AREA_DATA *first_area;
AREA_DATA *last_area;
AREA_DATA *first_area_name;   /*Used for alphanum. sort */
AREA_DATA *last_area_name;
AREA_DATA *first_build;
AREA_DATA *last_build;
AREA_DATA *first_asort;
AREA_DATA *last_asort;
AREA_DATA *first_bsort;
AREA_DATA *last_bsort;

SYSTEM_DATA sysdata;

int top_affect;
int top_area;
int top_ed;
int top_exit;
int top_help;
int top_mob_index;
int top_obj_index;
int top_reset;
int top_room;
int top_shop;
int top_repair;
int top_vroom;

/*
 * Semi-locals.
 */
bool fBootDb;
FILE *fpArea;
char strArea[MAX_INPUT_LENGTH];

/*
 * Local booting procedures.
 */
void load_timedata args( ( void ) );
void init_mm args( ( void ) );
void boot_log args( ( const char *str, ... ) );
void load_buildlist args( ( void ) );
bool load_systemdata args( ( SYSTEM_DATA * sys ) );
void load_watchlist args( ( void ) );
void load_reserved args( ( void ) );
void load_filtered args( ( void ) );
void initialize_economy args( ( void ) );
void fix_exits args( ( void ) );
void sort_reserved args( ( RESERVE_DATA * pRes ) );
void sort_filtered args( ( FILTER_DATA * pFlt ) );
void load_weatherdata args( ( void ) );
void load_projects args( ( void ) );
PROJECT_DATA *read_project( FILE * fp );
NOTE_DATA *read_log args( ( FILE * fp ) );

/*
 * External booting function
 */
void load_corpses args( ( void ) );
void renumber_put_resets( ROOM_INDEX_DATA * room );
void wipe_resets( ROOM_INDEX_DATA * room );
void load_disabled args( ( void ) );
void make_immlist args( ( void ) );
void make_immweb args( ( void ) );
void load_changes args( ( void ) );
void load_global_boards( void ); /* load all boards */

/*
 * MUDprogram locals
 */
int mprog_name_to_type args( ( char *name ) );
MPROG_DATA *mprog_file_read args( ( char *f, MPROG_DATA * mprg, MOB_INDEX_DATA * pMobIndex ) );
MPROG_DATA *oprog_file_read args( ( char *f, MPROG_DATA * mprg, OBJ_INDEX_DATA * pObjIndex ) );
MPROG_DATA *rprog_file_read args( ( char *f, MPROG_DATA * mprg, ROOM_INDEX_DATA * pRoomIndex ) );
void load_mudprogs args( ( AREA_DATA * tarea, FILE * fp ) );
void load_objprogs args( ( AREA_DATA * tarea, FILE * fp ) );
void load_roomprogs args( ( AREA_DATA * tarea, FILE * fp ) );
void mprog_read_programs args( ( FILE * fp, MOB_INDEX_DATA * pMobIndex ) );
void oprog_read_programs args( ( FILE * fp, OBJ_INDEX_DATA * pObjIndex ) );
void rprog_read_programs args( ( FILE * fp, ROOM_INDEX_DATA * pRoomIndex ) );

void shutdown_mud( char *reason )
{
   FILE *fp;

   if( ( fp = fopen( SHUTDOWN_FILE, "a" ) ) != NULL )
   {
      fprintf( fp, "%s\n", reason );
      fclose( fp );
   }
}


/*
 * Big mama top level function.
 */
void boot_db( bool fCopyOver )
{
   short wear, x;

   show_hash( 32 );
   unlink( BOOTLOG_FILE );
   boot_log( "---------------------[ Boot Log ]--------------------" );

   log_string( "Loading commands" );
   load_commands(  );
   load_mudchannels(  );

   log_string( "Loading sysdata configuration..." );

   /*
    * default values 
    */
   sysdata.read_all_mail = LEVEL_DEMI;
   sysdata.read_mail_free = LEVEL_IMMORTAL;
   sysdata.write_mail_free = LEVEL_IMMORTAL;
   sysdata.take_others_mail = LEVEL_DEMI;
   sysdata.muse_level = LEVEL_DEMI;
   sysdata.think_level = LEVEL_HIGOD;
   sysdata.build_level = LEVEL_DEMI;
   sysdata.log_level = LEVEL_LOG;
   sysdata.level_modify_proto = LEVEL_LESSER;
   sysdata.level_override_private = LEVEL_GREATER;
   sysdata.level_mset_player = LEVEL_LESSER;
   sysdata.stun_plr_vs_plr = 65;
   sysdata.stun_regular = 15;
   sysdata.gouge_nontank = 0;
   sysdata.gouge_plr_vs_plr = 0;
   sysdata.bash_nontank = 0;
   sysdata.bash_plr_vs_plr = 0;
   sysdata.dodge_mod = 2;
   sysdata.parry_mod = 2;
   sysdata.tumble_mod = 4;
   sysdata.dam_plr_vs_plr = 100;
   sysdata.dam_plr_vs_mob = 100;
   sysdata.dam_mob_vs_plr = 100;
   sysdata.dam_mob_vs_mob = 100;
   sysdata.webport = 4055;
   sysdata.homepage = STRALLOC( "unknown" );
   sysdata.level_getobjnotake = LEVEL_GREATER;
   sysdata.save_frequency = 20;  /* minutes */
   sysdata.double_exp = FALSE;
   sysdata.bestow_dif = 5;
   sysdata.check_imm_host = 1;
   sysdata.morph_opt = 1;
   sysdata.save_pets = 0;
   sysdata.save_flags = SV_DEATH | SV_PASSCHG | SV_AUTO | SV_PUT | SV_DROP | SV_GIVE | SV_AUCTION | SV_ZAPDROP | SV_IDLE;
   sysdata.wizlock = FALSE;
   sysdata.mplimit = 5; /* Default mplimit */
   sysdata.pklimit = 2; /* Default pklimit */

   if( !load_systemdata( &sysdata ) )
   {
      log_string( "Not found.  Creating new configuration." );
      sysdata.alltimemax = 0;
      sysdata.mud_name = str_dup( "(Name not set)" );
   }
   log_string( "Loading illegal pkill data" );
   load_pkdata(  );

   log_string( "Loading socials" );
   load_socials(  );

   log_string( "Loading skill table" );
   load_skill_table(  );
   sort_skill_table(  );
   remap_slot_numbers(  ); /* must be after the sort */

   gsn_first_spell = 0;
   gsn_first_skill = 0;
   gsn_first_weapon = 0;
   gsn_first_tongue = 0;
   gsn_top_sn = top_sn;

   for( x = 0; x < top_sn; x++ )
      if( !gsn_first_spell && skill_table[x]->type == SKILL_SPELL )
         gsn_first_spell = x;
      else if( !gsn_first_skill && skill_table[x]->type == SKILL_SKILL )
         gsn_first_skill = x;
      else if( !gsn_first_weapon && skill_table[x]->type == SKILL_WEAPON )
         gsn_first_weapon = x;
      else if( !gsn_first_tongue && skill_table[x]->type == SKILL_TONGUE )
         gsn_first_tongue = x;

   log_string( "Loading classes" );
   load_classes(  );

   log_string( "Loading races" );
   load_races(  );

   /*
    * Extended News - 12/15/01 - Nopey 
    */
   log_string( "Loading extended news data" );
   load_news(  );

   log_string( "Loading Changes" );
   load_changes(  );

   log_string( "Loading herb table" );
   load_herb_table(  );

   log_string( "Loading tongues" );
   load_tongues(  );

   log_string( "Making wizlist" );
   make_wizlist(  );
   make_webwiz(  );

   log_string( "Loading Hometowns..." );
   load_hometowns(  );

   log_string( "Loading Nations..." );
   load_nations(  );

   log_string( "Making guestlist" );
   make_guestlist(  );

   log_string( "Initializing request pipe" );
   init_request_pipe(  );

   fBootDb = TRUE;

   nummobsloaded = 0;
   numobjsloaded = 0;
   physicalobjects = 0;
   sysdata.maxplayers = 0;
   sysdata.totalplayers = 0;
   first_object = NULL;
   last_object = NULL;
   first_char = NULL;
   last_char = NULL;
   first_area = NULL;
   first_area_name = NULL; /*Used for alphanum. sort */
   last_area_name = NULL;
   last_area = NULL;
   first_build = NULL;
   last_area = NULL;
   first_shop = NULL;
   last_shop = NULL;
   first_repair = NULL;
   last_repair = NULL;
   first_teleport = NULL;
   last_teleport = NULL;
   first_asort = NULL;
   last_asort = NULL;
   extracted_obj_queue = NULL;
   extracted_char_queue = NULL;
   cur_qobjs = 0;
   cur_qchars = 0;
   cur_char = NULL;
   cur_obj = 0;
   cur_obj_serial = 0;
   cur_char_died = FALSE;
   cur_obj_extracted = FALSE;
   cur_room = NULL;
   quitting_char = NULL;
   loading_char = NULL;
   saving_char = NULL;
   last_pkroom = 1;
   immortal_host_start = NULL;
   immortal_host_end = NULL;
   first_ban_class = NULL;
   last_ban_class = NULL;
   first_ban_race = NULL;
   last_ban_race = NULL;
   first_ban = NULL;
   last_ban = NULL;

   CREATE( auction, AUCTION_DATA, 1 );
   auction->item = NULL;
   auction->hist_timer = 0;
   for( x = 0; x < AUCTION_MEM; x++ )
      auction->history[x] = NULL;

   weath_unit = 10;
   rand_factor = 2;
   climate_factor = 1;
   neigh_factor = 3;
   max_vector = weath_unit * 3;

   for( wear = 0; wear < MAX_WEAR; wear++ )
      for( x = 0; x < MAX_LAYERS; x++ )
         save_equipment[wear][x] = NULL;



   /*
    * Init random number generator.
    */
   log_string( "Initializing random number generator" );
   init_mm(  );

   /*
    * Set time and weather.
    */
   {

      log_string( "Setting time and weather" );

      load_timedata(  );

      if( time_info.hour < 5 )
         time_info.sunlight = SUN_DARK;
      else if( time_info.hour < 6 )
         time_info.sunlight = SUN_RISE;
      else if( time_info.hour < 19 )
         time_info.sunlight = SUN_LIGHT;
      else if( time_info.hour < 20 )
         time_info.sunlight = SUN_SET;
      else
         time_info.sunlight = SUN_DARK;

      /*
       * weather_info.change  = 0;
       * weather_info.mmhg = 960;
       * if ( time_info.month >= 7 && time_info.month <=12 )
       * weather_info.mmhg += number_range( 1, 50 );
       * else
       * weather_info.mmhg += number_range( 1, 80 );
       * 
       * if ( weather_info.mmhg <=  980 ) weather_info.sky = SKY_LIGHTNING;
       * else if ( weather_info.mmhg <= 1000 ) weather_info.sky = SKY_RAINING;
       * else if ( weather_info.mmhg <= 1020 ) weather_info.sky = SKY_CLOUDY;
       * else                                  weather_info.sky = SKY_CLOUDLESS;
       */
   }

   /*
    * Assign gsn's for skills which need them.
    */
   {
      log_string( "Assigning gsn's" );
      ASSIGN_GSN( gsn_style_evasive, "evasive style" );
      ASSIGN_GSN( gsn_style_defensive, "defensive style" );
      ASSIGN_GSN( gsn_style_standard, "standard style" );
      ASSIGN_GSN( gsn_style_aggressive, "aggressive style" );
      ASSIGN_GSN( gsn_style_berserk, "berserk style" );

      ASSIGN_GSN( gsn_meditate, "meditate" );
      ASSIGN_GSN( gsn_trance, "trance" );
      ASSIGN_GSN( gsn_ruminate, "ruminate" );
      ASSIGN_GSN( gsn_pugilism, "pugilism" );
      ASSIGN_GSN( gsn_long_blades, "long blades" );
      ASSIGN_GSN( gsn_short_blades, "short blades" );
      ASSIGN_GSN( gsn_flexible_arms, "flexible arms" );
      ASSIGN_GSN( gsn_talonous_arms, "talonous arms" );
      ASSIGN_GSN( gsn_maces, "maces" );
      ASSIGN_GSN( gsn_missile_weapons, "missile weapons" );
      ASSIGN_GSN( gsn_detrap, "detrap" );
      ASSIGN_GSN( gsn_backstab, "backstab" );
      ASSIGN_GSN( gsn_circle, "circle" );
      ASSIGN_GSN( gsn_tumble, "tumble" );
      ASSIGN_GSN( gsn_dodge, "dodge" );
      ASSIGN_GSN( gsn_hide, "hide" );
      ASSIGN_GSN( gsn_peek, "peek" );
      ASSIGN_GSN( gsn_pick_lock, "pick lock" );
      ASSIGN_GSN( gsn_sneak, "sneak" );
      ASSIGN_GSN( gsn_steal, "steal" );
      ASSIGN_GSN( gsn_gouge, "gouge" );
      ASSIGN_GSN( gsn_poison_weapon, "poison weapon" );
      ASSIGN_GSN( gsn_disarm, "disarm" );
      ASSIGN_GSN( gsn_enhanced_damage, "enhanced damage" );
      ASSIGN_GSN( gsn_kick, "kick" );
      ASSIGN_GSN( gsn_parry, "parry" );
      ASSIGN_GSN( gsn_rescue, "rescue" );
      ASSIGN_GSN( gsn_second_attack, "second attack" );
      ASSIGN_GSN( gsn_third_attack, "third attack" );
      ASSIGN_GSN( gsn_fourth_attack, "fourth attack" );
      ASSIGN_GSN( gsn_fifth_attack, "fifth attack" );
      ASSIGN_GSN( gsn_sixth_attack, "sixth attack" );
      ASSIGN_GSN( gsn_seventh_attack, "seventh attack" );
      ASSIGN_GSN( gsn_eighth_attack, "eighth attack" );
      ASSIGN_GSN( gsn_dual_wield, "dual wield" );
      ASSIGN_GSN( gsn_punch, "punch" );
      ASSIGN_GSN( gsn_bash, "bash" );
      ASSIGN_GSN( gsn_stun, "stun" );
      ASSIGN_GSN( gsn_bashdoor, "doorbash" );
      ASSIGN_GSN( gsn_grip, "grip" );
      ASSIGN_GSN( gsn_berserk, "berserk" );
      ASSIGN_GSN( gsn_hitall, "hitall" );
      ASSIGN_GSN( gsn_feed, "feed" );
      ASSIGN_GSN( gsn_bloodlet, "bloodlet" );
      ASSIGN_GSN( gsn_broach, "broach" );
      ASSIGN_GSN( gsn_mistwalk, "mistwalk" );
      ASSIGN_GSN( gsn_bleeding, "bandage" );
      ASSIGN_GSN( gsn_aid, "aid" );
      ASSIGN_GSN( gsn_track, "track" );
      ASSIGN_GSN( gsn_search, "search" );
      ASSIGN_GSN( gsn_dig, "dig" );
      ASSIGN_GSN( gsn_mount, "mount" );
      ASSIGN_GSN( gsn_bite, "bite" );
      ASSIGN_GSN( gsn_claw, "claw" );
      ASSIGN_GSN( gsn_sting, "sting" );
      ASSIGN_GSN( gsn_tail, "tail" );
      ASSIGN_GSN( gsn_scribe, "scribe" );
      ASSIGN_GSN( gsn_brew, "brew" );
      ASSIGN_GSN( gsn_climb, "climb" );
      ASSIGN_GSN( gsn_cook, "cook" );
      ASSIGN_GSN( gsn_scan, "scan" );
      ASSIGN_GSN( gsn_slice, "slice" );
      ASSIGN_GSN( gsn_fireball, "fireball" );
      ASSIGN_GSN( gsn_chill_touch, "chill touch" );
      ASSIGN_GSN( gsn_lightning_bolt, "lightning bolt" );
      ASSIGN_GSN( gsn_aqua_breath, "aqua breath" );
      ASSIGN_GSN( gsn_blindness, "blindness" );
      ASSIGN_GSN( gsn_charm_person, "charm person" );
      ASSIGN_GSN( gsn_curse, "curse" );
      ASSIGN_GSN( gsn_invis, "invis" );
      ASSIGN_GSN( gsn_mass_invis, "mass invis" );
      ASSIGN_GSN( gsn_poison, "poison" );
      ASSIGN_GSN( gsn_study, "study" );
      ASSIGN_GSN( gsn_sleep, "sleep" );
      ASSIGN_GSN( gsn_possess, "possess" );
      ASSIGN_GSN( gsn_common, "common" );
      ASSIGN_GSN( gsn_elven, "elven" );
      ASSIGN_GSN( gsn_dwarven, "dwarven" );
      ASSIGN_GSN( gsn_pixie, "pixie" );
      ASSIGN_GSN( gsn_ogre, "ogre" );
      ASSIGN_GSN( gsn_orcish, "orcish" );
      ASSIGN_GSN( gsn_trollish, "trollese" );
      ASSIGN_GSN( gsn_goblin, "goblin" );
      ASSIGN_GSN( gsn_halfling, "halfling" );
      ASSIGN_GSN( gsn_sharpen, "sharpen" );  /* sharpen skill --Cronel */
      ASSIGN_GSN( gsn_haste, "haste" );
      ASSIGN_GSN( gsn_slow, "slow" );
      ASSIGN_GSN( gsn_silence, "silence" );
      ASSIGN_GSN( gsn_second_cast, "second cast" );
      ASSIGN_GSN( gsn_third_cast, "third cast" );
      ASSIGN_GSN( gsn_fourth_cast, "fourth cast" );
      ASSIGN_GSN( gsn_fifth_cast, "fifth cast" );
      ASSIGN_GSN( gsn_sixth_cast, "sixth cast" );
      ASSIGN_GSN( gsn_seventh_cast, "seventh cast" );
      ASSIGN_GSN( gsn_eighth_cast, "eighth cast" );
      ASSIGN_GSN( gsn_assassinate, "assassinate" );
      ASSIGN_GSN( gsn_listen, "listen" );
      ASSIGN_GSN( gsn_hunt, "hunt" );
   }

   log_string( "Reading in plane file..." );
   load_planes(  );

   /*
    * Read in all the area files.
    */
   {
      FILE *fpList;

      log_string( "Reading in area files..." );
      if( ( fpList = fopen( AREA_LIST, "r" ) ) == NULL )
      {
         perror( AREA_LIST );
         shutdown_mud( "Unable to open area list" );
         exit( 1 );
      }

      for( ;; )
      {
         strcpy( strArea, fread_word( fpList ) );
         if( strArea[0] == '$' )
            break;

         load_area_file( last_area, strArea );

      }
      fclose( fpList );
   }

   log_string( "Making sure rooms are planed..." );
   check_planes( NULL );

/* hiscore shtuff -- Scion */
   log_string( "Loading high scores..." );
   load_hiscores(  );
/* end hiscore */

   /*
    *   initialize supermob.
    *    must be done before reset_area!
    *
    */
   init_supermob(  );
   /*
    * Has some bad memory bugs in it
    */
#if 0
   init_profanity_checker(  );   /* as good a place as any */
#endif
   /*
    * Fix up exits.
    * Declare db booting over.
    * Reset all areas once.
    * Load up the notes file.
    */
   {
      log_string( "Fixing exits" );
      fix_exits(  );
      fBootDb = FALSE;
      log_string( "Initializing economy" );
      initialize_economy(  );
      log_string( "Resetting areas" );

      area_update(  );  /* Either area_update or load_world must be commented out at all times */
      /*
       * load_world(NULL); 
       *//*
       * Add the flag plrbld to objs that mobs can not carry 
       */
/* Also uncomment save_world and load_world in save.c */
      log_string( "Loading buildlist" );
      load_buildlist(  );
      log_string( "Loading quest bits" );
      load_bits(  );
      log_string( "Loading Local boards" );
      load_boards(  );
      log_string( "Loading Global Boards" );
      load_global_boards(  );
      log_string( "Loading clans" );
      load_clans(  );
      log_string( "Loading deities" );
      load_deity(  );
      log_string( "Loading councils" );
      load_councils(  );
      log_string( "Loading watches" );
      load_watchlist(  );
      log_string( "Loading bans" );
      load_banlist(  );
      log_string( "Loading reserved names" );
      load_reserved(  );
      log_string( "Loading filtered words" );
      load_filtered(  );
      log_string( "Loading corpses" );
      load_corpses(  );
      log_string( "Loading slay tables" );   /* Online slay tables - Samson 8-3-98 */
      load_slays(  );
      log_string( "Loading Immortal Hosts" );
      load_imm_host(  );
      log_string( "Loading Projects" );
      load_projects(  );
      log_string( "Loading disabled commands" );
      load_disabled(  );
/* Morphs MUST be loaded after class and race tables are set up --Shaddai */
      log_string( "Loading Morphs" );
      load_morphs(  );
      log_string( "Loading Housing System, Home Accessories Data," " and Home Auctioning System" );
      load_homedata(  );
      load_accessories(  );
      load_homebuy(  );
      MOBtrigger = TRUE;
      if( fCopyOver )
      {
         log_string( "Running copyover_recover." );
         copyover_recover(  );
      }
   }

   /*
    * Initialize area weather data 
    */
   load_weatherdata(  );
   init_area_weather(  );
   /*
    * init_maps ( ); 
    */

   return;
}



/*
 * Load an 'area' header line.
 */
void load_area( FILE * fp )
{
   AREA_DATA *pArea;

   CREATE( pArea, AREA_DATA, 1 );
   pArea->first_room = pArea->last_room = NULL;
   pArea->name = fread_string_nohash( fp );
   pArea->author = STRALLOC( "unknown" );
   pArea->filename = str_dup( strArea );
   pArea->age = 15;
   pArea->nplayer = 0;
   pArea->low_r_vnum = 0;
   pArea->low_o_vnum = 0;
   pArea->low_m_vnum = 0;
   pArea->hi_r_vnum = 0;
   pArea->hi_o_vnum = 0;
   pArea->hi_m_vnum = 0;
   pArea->low_soft_range = 0;
   pArea->hi_soft_range = MAX_LEVEL;
   pArea->low_hard_range = 0;
   pArea->hi_hard_range = MAX_LEVEL;

   /*
    * initialize weather data - FB 
    */
   CREATE( pArea->weather, WEATHER_DATA, 1 );
   pArea->weather->temp = 0;
   pArea->weather->precip = 0;
   pArea->weather->wind = 0;
   pArea->weather->temp_vector = 0;
   pArea->weather->precip_vector = 0;
   pArea->weather->wind_vector = 0;
   pArea->weather->climate_temp = 2;
   pArea->weather->climate_precip = 2;
   pArea->weather->climate_wind = 2;
   pArea->weather->first_neighbor = NULL;
   pArea->weather->last_neighbor = NULL;
   pArea->weather->echo = NULL;
   pArea->weather->echo_color = AT_GREY;
   area_version = 0;
   LINK( pArea, first_area, last_area, next, prev );
   top_area++;
   return;
}


/* Load the version number of the area file if none exists, then it
 * is set to version 0 when #AREA is read in which is why we check for
 * the #AREA here.  --Shaddai
 */

void load_version( AREA_DATA * tarea, FILE * fp )
{
   if( !tarea )
   {
      bug( "Load_author: no #AREA seen yet." );
      if( fBootDb )
      {
         shutdown_mud( "No #AREA" );
         exit( 1 );
      }
      else
         return;
   }

   area_version = fread_number( fp );
   return;
}

/*
 * Load an author section. Scryn 2/1/96
 */
void load_author( AREA_DATA * tarea, FILE * fp )
{
   if( !tarea )
   {
      bug( "Load_author: no #AREA seen yet." );
      if( fBootDb )
      {
         shutdown_mud( "No #AREA" );
         exit( 1 );
      }
      else
         return;
   }

   if( tarea->author )
      STRFREE( tarea->author );
   tarea->author = fread_string( fp );
   return;
}

/*
 * Load an economy section. Thoric
 */
void load_economy( AREA_DATA * tarea, FILE * fp )
{
   if( !tarea )
   {
      bug( "Load_economy: no #AREA seen yet." );
      if( fBootDb )
      {
         shutdown_mud( "No #AREA" );
         exit( 1 );
      }
      else
         return;
   }

   tarea->high_economy = fread_number( fp );
   tarea->low_economy = fread_number( fp );
   return;
}

/* Reset Message Load, Rennard */
void load_resetmsg( AREA_DATA * tarea, FILE * fp )
{
   if( !tarea )
   {
      bug( "Load_resetmsg: no #AREA seen yet." );
      if( fBootDb )
      {
         shutdown_mud( "No #AREA" );
         exit( 1 );
      }
      else
         return;
   }

   if( tarea->resetmsg )
      DISPOSE( tarea->resetmsg );
   tarea->resetmsg = fread_string_nohash( fp );
   return;
}

/*
 * Load area flags. Narn, Mar/96 
 */
void load_flags( AREA_DATA * tarea, FILE * fp )
{
   char *ln;
   int x1, x2;

   if( !tarea )
   {
      bug( "Load_flags: no #AREA seen yet." );
      if( fBootDb )
      {
         shutdown_mud( "No #AREA" );
         exit( 1 );
      }
      else
         return;
   }
   ln = fread_line( fp );
   x1 = x2 = 0;
   sscanf( ln, "%d %d", &x1, &x2 );
   tarea->flags = x1;
   tarea->reset_frequency = x2;
   if( x2 )
      tarea->age = x2;
   return;
}

/*
 * Adds a help page to the list if it is not a duplicate of an existing page.
 * Page is insert-sorted by keyword.			-Thoric
 * (The reason for sorting is to keep do_hlist looking nice)
 */
void add_help( HELP_DATA * pHelp )
{
   HELP_DATA *tHelp;
   int match;

   for( tHelp = first_help; tHelp; tHelp = tHelp->next )
      if( pHelp->level == tHelp->level && strcmp( pHelp->keyword, tHelp->keyword ) == 0 )
      {
         bug( "add_help: duplicate: %s.  Deleting.", pHelp->keyword );
         STRFREE( pHelp->text );
         STRFREE( pHelp->keyword );
         DISPOSE( pHelp );
         return;
      }
      else
         if( ( match = strcmp( pHelp->keyword[0] == '\'' ? pHelp->keyword + 1 : pHelp->keyword,
                               tHelp->keyword[0] == '\'' ? tHelp->keyword + 1 : tHelp->keyword ) ) < 0
             || ( match == 0 && pHelp->level > tHelp->level ) )
      {
         if( !tHelp->prev )
            first_help = pHelp;
         else
            tHelp->prev->next = pHelp;
         pHelp->prev = tHelp->prev;
         pHelp->next = tHelp;
         tHelp->prev = pHelp;
         break;
      }

   if( !tHelp )
      LINK( pHelp, first_help, last_help, next, prev );

   top_help++;
}

void load_helps( FILE * fp )
{
   HELP_DATA *pHelp;

   for( ;; )
   {
      CREATE( pHelp, HELP_DATA, 1 );
      pHelp->level = fread_number( fp );
      pHelp->keyword = fread_string( fp );
      if( pHelp->keyword[0] == '$' )
      {
         STRFREE( pHelp->keyword );
         DISPOSE( pHelp );
         break;
      }
      pHelp->text = fread_string( fp );
      if( pHelp->keyword[0] == '\0' )
      {
         STRFREE( pHelp->text );
         STRFREE( pHelp->keyword );
         DISPOSE( pHelp );
         continue;
      }

      if( !str_cmp( pHelp->keyword, "greeting" ) )
         help_greeting = pHelp->text;
      if( !str_cmp( pHelp->keyword, "greeting2" ) )   /* Josh */
         help_greeting2 = pHelp->text;
      add_help( pHelp );
   }
   return;
}

/*
 * Add a character to the list of all characters		-Thoric
 */
void add_char( CHAR_DATA * ch )
{
   LINK( ch, first_char, last_char, next, prev );
   /*
    * REMOVE THIS KRATAS 
    */
//    CHECK_LINKS( first_char, last_char, next, prev, CHAR_DATA );
}


/*
 * Load a mob section.
 */
void load_mobiles( AREA_DATA * tarea, FILE * fp )
{
   MOB_INDEX_DATA *pMobIndex;
   char *ln;
   int x1, x2, x3, x4, x5, x6, x7, x8;

   if( !tarea )
   {
      bug( "Load_mobiles: no #AREA seen yet." );
      if( fBootDb )
      {
         shutdown_mud( "No #AREA" );
         exit( 1 );
      }
      else
         return;
   }

   for( ;; )
   {
      char buf[MAX_STRING_LENGTH];
      int vnum;
      char letter;
      int iHash;
      bool oldmob;
      bool tmpBootDb;
      int tmpgold = 0;

      letter = fread_letter( fp );
      if( letter != '#' )
      {
         bug( "Load_mobiles: # not found." );
         if( fBootDb )
         {
            shutdown_mud( "# not found" );
            exit( 1 );
         }
         else
            return;
      }

      vnum = fread_number( fp );
      if( vnum == 0 )
         break;

      tmpBootDb = fBootDb;
      fBootDb = FALSE;
      if( get_mob_index( vnum ) )
      {
         if( tmpBootDb )
         {
            bug( "Load_mobiles: vnum %d duplicated.", vnum );
            shutdown_mud( "duplicate vnum" );
            exit( 1 );
         }
         else
         {
            pMobIndex = get_mob_index( vnum );
            sprintf( buf, "Cleaning mobile: %d", vnum );
            log_string_plus( buf, LOG_BUILD, sysdata.log_level );
            clean_mob( pMobIndex );
            oldmob = TRUE;
         }
      }
      else
      {
         oldmob = FALSE;
         CREATE( pMobIndex, MOB_INDEX_DATA, 1 );
      }
      fBootDb = tmpBootDb;

      pMobIndex->vnum = vnum;
      if( fBootDb )
      {
         if( !tarea->low_m_vnum )
            tarea->low_m_vnum = vnum;
         if( vnum > tarea->hi_m_vnum )
            tarea->hi_m_vnum = vnum;
      }
      pMobIndex->player_name = fread_string( fp );
      pMobIndex->short_descr = fread_string( fp );
      pMobIndex->long_descr = fread_string( fp );
      pMobIndex->description = fread_string( fp );

      pMobIndex->long_descr[0] = UPPER( pMobIndex->long_descr[0] );
      pMobIndex->description[0] = UPPER( pMobIndex->description[0] );

      pMobIndex->act = fread_bitvector( fp );
      xSET_BIT( pMobIndex->act, ACT_IS_NPC );
      pMobIndex->affected_by = fread_bitvector( fp );
      pMobIndex->pShop = NULL;
      pMobIndex->rShop = NULL;
      pMobIndex->alignment = fread_number( fp );
      letter = fread_letter( fp );
      pMobIndex->level = fread_number( fp );
      /*
       * I dont know how Dystopia or whatever pulled of a level 880 mob
       * but it stops here - shogar)
       */
      if( pMobIndex->level > ( LEVEL_AVATAR + 5 ) )
      {
         bug( "Mob %d was set for level %d ... Resetting to level %d", vnum, pMobIndex->level, LEVEL_AVATAR );
         pMobIndex->level = LEVEL_AVATAR;
      }

      pMobIndex->mobthac0 = fread_number( fp );
      pMobIndex->ac = fread_number( fp );
      pMobIndex->hitnodice = fread_number( fp );
      /*
       * 'd'      
       */ fread_letter( fp );
      pMobIndex->hitsizedice = fread_number( fp );
      /*
       * '+'      
       */ fread_letter( fp );
      pMobIndex->hitplus = fread_number( fp );
      pMobIndex->damnodice = fread_number( fp );
      /*
       * 'd'      
       */ fread_letter( fp );
      pMobIndex->damsizedice = fread_number( fp );
      /*
       * '+'      
       */ fread_letter( fp );
      pMobIndex->damplus = fread_number( fp );
      if( area_version <= 1 )
      {
         tmpgold = fread_number( fp );
         pMobIndex->exp = fread_number( fp );

         /*
          * Convert to new system -Druid
          */
         pMobIndex->gold = 0;
         pMobIndex->silver = 0;
         pMobIndex->copper = 0;

         pMobIndex->gold = tmpgold / 10000;
         tmpgold = tmpgold % 10000;
         pMobIndex->silver = tmpgold / 100;
         tmpgold = tmpgold % 100;
         pMobIndex->copper = tmpgold;
      }
      else
      {
         pMobIndex->exp = fread_number( fp );
         pMobIndex->gold = fread_number( fp );
         pMobIndex->silver = fread_number( fp );
         pMobIndex->copper = fread_number( fp );
      }

      /*
       * pMobIndex->position     = fread_number( fp ); 
       */
      pMobIndex->position = fread_number( fp );
      if( pMobIndex->position < 100 )
      {
         switch ( pMobIndex->position )
         {
            default:
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
               break;
            case 5:
               pMobIndex->position = 6;
               break;
            case 6:
               pMobIndex->position = 8;
               break;
            case 7:
               pMobIndex->position = 9;
               break;
            case 8:
               pMobIndex->position = 12;
               break;
            case 9:
               pMobIndex->position = 13;
               break;
            case 10:
               pMobIndex->position = 14;
               break;
            case 11:
               pMobIndex->position = 15;
               break;
         }
      }
      else
      {
         pMobIndex->position -= 100;
      }

      /*
       * pMobIndex->defposition     = fread_number( fp ); 
       */
      pMobIndex->defposition = fread_number( fp );
      if( pMobIndex->defposition < 100 )
      {
         switch ( pMobIndex->defposition )
         {
            default:
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
               break;
            case 5:
               pMobIndex->defposition = 6;
               break;
            case 6:
               pMobIndex->defposition = 8;
               break;
            case 7:
               pMobIndex->defposition = 9;
               break;
            case 8:
               pMobIndex->defposition = 12;
               break;
            case 9:
               pMobIndex->defposition = 13;
               break;
            case 10:
               pMobIndex->defposition = 14;
               break;
            case 11:
               pMobIndex->defposition = 15;
               break;
         }
      }
      else
      {
         pMobIndex->defposition -= 100;
      }


      /*
       * Back to meaningful values.
       */
      pMobIndex->sex = fread_number( fp );

      if( letter != 'S' && letter != 'C' )
      {
         bug( "Load_mobiles: vnum %d: letter '%c' not S or C.", vnum, letter );
         shutdown_mud( "bad mob data" );
         exit( 1 );
      }
      if( letter == 'C' )  /* Realms complex mob   -Thoric */
      {
         pMobIndex->perm_str = fread_number( fp );
         pMobIndex->perm_int = fread_number( fp );
         pMobIndex->perm_wis = fread_number( fp );
         pMobIndex->perm_dex = fread_number( fp );
         pMobIndex->perm_con = fread_number( fp );
         pMobIndex->perm_cha = fread_number( fp );
         pMobIndex->perm_lck = fread_number( fp );
         pMobIndex->saving_poison_death = fread_number( fp );
         pMobIndex->saving_wand = fread_number( fp );
         pMobIndex->saving_para_petri = fread_number( fp );
         pMobIndex->saving_breath = fread_number( fp );
         pMobIndex->saving_spell_staff = fread_number( fp );
         ln = fread_line( fp );
         x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = 0;
         sscanf( ln, "%d %d %d %d %d %d %d", &x1, &x2, &x3, &x4, &x5, &x6, &x7 );
         pMobIndex->race = x1;
         pMobIndex->class = x2;
         pMobIndex->height = x3;
         pMobIndex->weight = x4;
         pMobIndex->speaks = x5;
         pMobIndex->speaking = x6;
         pMobIndex->numattacks = x7;

         if( !pMobIndex->speaks )
            pMobIndex->speaks = race_table[pMobIndex->race]->language | LANG_COMMON;
         if( !pMobIndex->speaking )
            pMobIndex->speaking = race_table[pMobIndex->race]->language;

#ifndef XBI
         ln = fread_line( fp );
         x1 = x2 = x3 = x4 = x5 = x6 = x7 = x8 = 0;
         sscanf( ln, "%d %d %d %d %d %d %d %d", &x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8 );
         pMobIndex->hitroll = x1;
         pMobIndex->damroll = x2;
         pMobIndex->xflags = x3;
         pMobIndex->resistant = x4;
         pMobIndex->immune = x5;
         pMobIndex->susceptible = x6;
         pMobIndex->attacks = x7;
         pMobIndex->defenses = x8;
#else
         pMobIndex->hitroll = fread_number( fp );
         pMobIndex->damroll = fread_number( fp );
         pMobIndex->xflags = fread_number( fp );
         pMobIndex->resistant = fread_number( fp );
         pMobIndex->immune = fread_number( fp );
         pMobIndex->susceptible = fread_number( fp );
         pMobIndex->attacks = fread_bitvector( fp );
         pMobIndex->defenses = fread_bitvector( fp );
#endif
      }
      else
      {
         pMobIndex->perm_str = 13;
         pMobIndex->perm_dex = 13;
         pMobIndex->perm_int = 13;
         pMobIndex->perm_wis = 13;
         pMobIndex->perm_cha = 13;
         pMobIndex->perm_con = 13;
         pMobIndex->perm_lck = 13;
         pMobIndex->race = 0;
         pMobIndex->class = 3;
         pMobIndex->xflags = 0;
         pMobIndex->resistant = 0;
         pMobIndex->immune = 0;
         pMobIndex->susceptible = 0;
         pMobIndex->numattacks = 0;
#ifdef XBI
         xCLEAR_BITS( pMobIndex->attacks );
         xCLEAR_BITS( pMobIndex->defenses );
#else
         pMobIndex->attacks = 0;
         pMobIndex->defenses = 0;
#endif
      }

      letter = fread_letter( fp );
      if( letter == '>' )
      {
         ungetc( letter, fp );
         mprog_read_programs( fp, pMobIndex );
      }
      else
         ungetc( letter, fp );

      if( !oldmob )
      {
         iHash = vnum % MAX_KEY_HASH;
         pMobIndex->next = mob_index_hash[iHash];
         mob_index_hash[iHash] = pMobIndex;
         top_mob_index++;
      }
   }

   return;
}

/*
 * Load an obj section.
 */
void load_objects( AREA_DATA * tarea, FILE * fp )
{
   OBJ_INDEX_DATA *pObjIndex;
   char letter;
   char *ln;
   int x1, x2, x3, x4, x5, x6;
   int tempcost;
   if( !tarea )
   {
      bug( "Load_objects: no #AREA seen yet." );
      if( fBootDb )
      {
         shutdown_mud( "No #AREA" );
         exit( 1 );
      }
      else
         return;
   }

   for( ;; )
   {
      char buf[MAX_STRING_LENGTH];
      int vnum;
      int iHash;
      bool tmpBootDb;
      bool oldobj;

      letter = fread_letter( fp );
      if( letter != '#' )
      {
         bug( "Load_objects: # not found." );
         if( fBootDb )
         {
            shutdown_mud( "# not found" );
            exit( 1 );
         }
         else
            return;
      }

      vnum = fread_number( fp );
      if( vnum == 0 )
         break;

      tmpBootDb = fBootDb;
      fBootDb = FALSE;
      if( get_obj_index( vnum ) )
      {
         if( tmpBootDb )
         {
            bug( "Load_objects: vnum %d duplicated.", vnum );
            shutdown_mud( "duplicate vnum" );
            exit( 1 );
         }
         else
         {
            pObjIndex = get_obj_index( vnum );
            sprintf( buf, "Cleaning object: %d", vnum );
            log_string_plus( buf, LOG_BUILD, sysdata.log_level );
            clean_obj( pObjIndex );
            oldobj = TRUE;
         }
      }
      else
      {
         oldobj = FALSE;
         CREATE( pObjIndex, OBJ_INDEX_DATA, 1 );
      }
      fBootDb = tmpBootDb;

      pObjIndex->vnum = vnum;
      if( fBootDb )
      {
         if( !tarea->low_o_vnum )
            tarea->low_o_vnum = vnum;
         if( vnum > tarea->hi_o_vnum )
            tarea->hi_o_vnum = vnum;
      }
      pObjIndex->name = fread_string( fp );
      pObjIndex->short_descr = fread_string( fp );
      pObjIndex->description = fread_string( fp );
      pObjIndex->action_desc = fread_string( fp );

      /*
       * Commented out by Narn, Apr/96 to allow item short descs like 
       * Bonecrusher and Oblivion 
       */
      /*
       * pObjIndex->short_descr[0]  = LOWER(pObjIndex->short_descr[0]);
       */
      pObjIndex->description[0] = UPPER( pObjIndex->description[0] );

      pObjIndex->item_type = fread_number( fp );
      pObjIndex->extra_flags = fread_bitvector( fp );
      ln = fread_line( fp );
      x1 = x2 = 0;
      sscanf( ln, "%d %d", &x1, &x2 );
      pObjIndex->wear_flags = x1;
      pObjIndex->layers = x2;

      ln = fread_line( fp );
      x1 = x2 = x3 = x4 = x5 = x6 = 0;
      sscanf( ln, "%d %d %d %d %d %d", &x1, &x2, &x3, &x4, &x5, &x6 );
      pObjIndex->value[0] = x1;
      pObjIndex->value[1] = x2;
      pObjIndex->value[2] = x3;
      pObjIndex->value[3] = x4;
      pObjIndex->value[4] = x5;
      pObjIndex->value[5] = x6;
      pObjIndex->weight = fread_number( fp );
      pObjIndex->weight = UMAX( 1, pObjIndex->weight );
      if( area_version >= 2 )
      {
         pObjIndex->gold_cost = fread_number( fp );
         pObjIndex->silver_cost = fread_number( fp );
         pObjIndex->copper_cost = fread_number( fp );
         pObjIndex->rent = fread_number( fp );  /* unused */
         if( pObjIndex->rent < 0 )
            pObjIndex = 0;
      }
      else
      {
         tempcost = fread_number( fp );
         pObjIndex->gold_cost = 0;
         pObjIndex->silver_cost = 0;
         pObjIndex->copper_cost = 0;
         pObjIndex->rent = fread_number( fp );
         if( pObjIndex->rent < 0 )
            pObjIndex->rent = 0;
         /*
          * Convert to new currency. -Druid
          */
         pObjIndex->gold_cost = tempcost / 10000;
         tempcost = tempcost % 10000;
         pObjIndex->silver_cost = tempcost / 100;
         tempcost = tempcost % 100;
         pObjIndex->copper_cost = tempcost;
      }
      if( area_version >= 1 )
      {
         switch ( pObjIndex->item_type )
         {
            case ITEM_PILL:
            case ITEM_POTION:
            case ITEM_SCROLL:
               pObjIndex->value[1] = skill_lookup( fread_word( fp ) );
               pObjIndex->value[2] = skill_lookup( fread_word( fp ) );
               pObjIndex->value[3] = skill_lookup( fread_word( fp ) );
               break;
            case ITEM_STAFF:
            case ITEM_WAND:
               pObjIndex->value[3] = skill_lookup( fread_word( fp ) );
               break;
            case ITEM_SALVE:
               pObjIndex->value[4] = skill_lookup( fread_word( fp ) );
               pObjIndex->value[5] = skill_lookup( fread_word( fp ) );
               break;
         }
      }
      for( ;; )
      {
         letter = fread_letter( fp );

         if( letter == 'A' )
         {
            AFFECT_DATA *paf;

            CREATE( paf, AFFECT_DATA, 1 );
            paf->type = -1;
            paf->duration = -1;
            paf->location = fread_number( fp );
            if( paf->location == APPLY_WEAPONSPELL
                || paf->location == APPLY_WEARSPELL
                || paf->location == APPLY_REMOVESPELL
                || paf->location == APPLY_STRIPSN || paf->location == APPLY_RECURRINGSPELL )
               paf->modifier = slot_lookup( fread_number( fp ) );
            else
               paf->modifier = fread_number( fp );
            xCLEAR_BITS( paf->bitvector );
            LINK( paf, pObjIndex->first_affect, pObjIndex->last_affect, next, prev );
            top_affect++;
         }

         else if( letter == 'E' )
         {
            EXTRA_DESCR_DATA *ed;

            CREATE( ed, EXTRA_DESCR_DATA, 1 );
            ed->keyword = fread_string( fp );
            ed->description = fread_string( fp );
            LINK( ed, pObjIndex->first_extradesc, pObjIndex->last_extradesc, next, prev );
            top_ed++;
         }
         else if( letter == '>' )
         {
            ungetc( letter, fp );
            oprog_read_programs( fp, pObjIndex );
         }

         else
         {
            ungetc( letter, fp );
            break;
         }
      }

      /*
       * Translate spell "slot numbers" to internal "skill numbers."
       */
      if( area_version == 0 )
         switch ( pObjIndex->item_type )
         {
            case ITEM_PILL:
            case ITEM_POTION:
            case ITEM_SCROLL:
               pObjIndex->value[1] = slot_lookup( pObjIndex->value[1] );
               pObjIndex->value[2] = slot_lookup( pObjIndex->value[2] );
               pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );
               break;

            case ITEM_STAFF:
            case ITEM_WAND:
               pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );
               break;
            case ITEM_SALVE:
               pObjIndex->value[4] = slot_lookup( pObjIndex->value[4] );
               pObjIndex->value[5] = slot_lookup( pObjIndex->value[5] );
               break;
         }

      if( !oldobj )
      {
         iHash = vnum % MAX_KEY_HASH;
         pObjIndex->next = obj_index_hash[iHash];
         obj_index_hash[iHash] = pObjIndex;
         top_obj_index++;
      }
   }

   return;
}

/*
 * Load a reset section.
 */
void load_resets( AREA_DATA * tarea, FILE * fp )
{
   ROOM_INDEX_DATA *pRoomIndex = NULL;
   ROOM_INDEX_DATA *roomlist;
   bool not01 = FALSE;
   int count = 0;

   if( !tarea )
   {
      bug( "%s", "Load_resets: no #AREA seen yet." );
      if( fBootDb )
      {
         shutdown_mud( "No #AREA" );
         exit( 1 );
      }
      else
         return;
   }

   if( !tarea->first_room )
   {
      bug( "%s: No #ROOMS section found. Cannot load resets.", __FUNCTION__ );
      if( fBootDb )
      {
         shutdown_mud( "No #ROOMS" );
         exit( 1 );
      }
      else
         return;
   }

   for( ;; )
   {
      EXIT_DATA *pexit;
      char letter;
      int extra, arg1, arg2, arg3;

      if( ( letter = fread_letter( fp ) ) == 'S' )
         break;

      if( letter == '*' )
      {
         fread_to_eol( fp );
         continue;
      }

      extra = fread_number( fp );
      if( letter == 'M' || letter == 'O' )
         extra = 0;
      arg1 = fread_number( fp );
      arg2 = fread_number( fp );
      arg3 = ( letter == 'G' || letter == 'R' ) ? 0 : fread_number( fp );
      fread_to_eol( fp );
      ++count;

      /*
       * Validate parameters.
       * We're calling the index functions for the side effect.
       */
      switch ( letter )
      {
         default:
            bug( "%s: bad command '%c'.", __FUNCTION__, letter );
            if( fBootDb )
               boot_log( "%s: %s (%d) bad command '%c'.", __FUNCTION__, tarea->filename, count, letter );
            return;

         case 'M':
            if( get_mob_index( arg1 ) == NULL && fBootDb )
               boot_log( "%s: %s (%d) 'M': mobile %d doesn't exist.", __FUNCTION__, tarea->filename, count, arg1 );

            if( ( pRoomIndex = get_room_index( arg3 ) ) == NULL && fBootDb )
               boot_log( "%s: %s (%d) 'M': room %d doesn't exist.", __FUNCTION__, tarea->filename, count, arg3 );
            else
               add_reset( pRoomIndex, letter, extra, arg1, arg2, arg3 );
            break;

         case 'O':
            if( get_obj_index( arg1 ) == NULL && fBootDb )
               boot_log( "%s: %s (%d) '%c': object %d doesn't exist.", __FUNCTION__, tarea->filename, count, letter, arg1 );

            if( ( pRoomIndex = get_room_index( arg3 ) ) == NULL && fBootDb )
               boot_log( "%s: %s (%d) '%c': room %d doesn't exist.", __FUNCTION__, tarea->filename, count, letter, arg3 );
            else
            {
               if( !pRoomIndex )
                  bug( "%s: Unable to add room reset - room not found.", __FUNCTION__ );
               else
                  add_reset( pRoomIndex, letter, extra, arg1, arg2, arg3 );
            }
            break;

         case 'P':
            if( get_obj_index( arg1 ) == NULL && fBootDb )
               boot_log( "%s: %s (%d) '%c': object %d doesn't exist.", __FUNCTION__, tarea->filename, count, letter, arg1 );
            if( arg3 > 0 )
            {
               if( get_obj_index( arg3 ) == NULL && fBootDb )
                  boot_log( "%s: %s (%d) 'P': destination object %d doesn't exist.", __FUNCTION__, tarea->filename, count,
                            arg3 );
               if( extra > 1 )
                  not01 = TRUE;
            }
            if( !pRoomIndex )
               bug( "%s: Unable to add room reset - room not found.", __FUNCTION__ );
            else
            {
               if( arg3 == 0 )
                  arg3 = OBJ_VNUM_GOLD_ONE;  // This may look stupid, but for some reason it works.
               add_reset( pRoomIndex, letter, extra, arg1, arg2, arg3 );
            }
            break;

         case 'G':
         case 'E':
            if( get_obj_index( arg1 ) == NULL && fBootDb )
               boot_log( "%s: %s (%d) '%c': object %d doesn't exist.", __FUNCTION__, tarea->filename, count, letter, arg1 );
            if( !pRoomIndex )
               bug( "%s: Unable to add room reset - room not found.", __FUNCTION__ );
            else
               add_reset( pRoomIndex, letter, extra, arg1, arg2, arg3 );
            break;

         case 'T':
            if( IS_SET( extra, TRAP_OBJ ) )
               bug( "%s: Unable to add legacy object trap reset. Must be converted manually.", __FUNCTION__ );
            else
            {
               if( !( pRoomIndex = get_room_index( arg3 ) ) )
                  bug( "%s: Unable to add trap reset - room not found.", __FUNCTION__ );
               else
                  add_reset( pRoomIndex, letter, extra, arg1, arg2, arg3 );
            }
            break;

         case 'H':
            bug( "%s: Unable to convert legacy hide reset. Must be converted manually.", __FUNCTION__ );
            break;

         case 'D':
            if( !( pRoomIndex = get_room_index( arg1 ) ) )
            {
               bug( "%s: 'D': room %d doesn't exist.", __FUNCTION__, arg1 );
               bug( "Reset: %c %d %d %d %d", letter, extra, arg1, arg2, arg3 );
               if( fBootDb )
                  boot_log( "%s: %s (%d) 'D': room %d doesn't exist.", __FUNCTION__, tarea->filename, count, arg1 );
               break;
            }

            if( arg2 < 0 || arg2 > MAX_DIR + 1
                || !( pexit = get_exit( pRoomIndex, arg2 ) ) || !IS_SET( pexit->exit_info, EX_ISDOOR ) )
            {
               bug( "%s: 'D': exit %d not door.", __FUNCTION__, arg2 );
               bug( "Reset: %c %d %d %d %d", letter, extra, arg1, arg2, arg3 );
               if( fBootDb )
                  boot_log( "%s: %s (%d) 'D': exit %d not door.", __FUNCTION__, tarea->filename, count, arg2 );
            }

            if( arg3 < 0 || arg3 > 2 )
            {
               bug( "%s: 'D': bad 'locks': %d.", __FUNCTION__, arg3 );
               if( fBootDb )
                  boot_log( "%s: %s (%d) 'D': bad 'locks': %d.", __FUNCTION__, tarea->filename, count, arg3 );
            }
            add_reset( pRoomIndex, letter, extra, arg1, arg2, arg3 );
            break;

         case 'R':
            if( !( pRoomIndex = get_room_index( arg1 ) ) && fBootDb )
               boot_log( "%s: %s (%d) 'R': room %d doesn't exist.", __FUNCTION__, tarea->filename, count, arg1 );
            else
               add_reset( pRoomIndex, letter, extra, arg1, arg2, arg3 );
            if( arg2 < 0 || arg2 > 10 )
            {
               bug( "%s: 'R': bad exit %d.", __FUNCTION__, arg2 );
               if( fBootDb )
                  boot_log( "%s: %s (%d) 'R': bad exit %d.", __FUNCTION__, tarea->filename, count, arg2 );
               break;
            }
            break;
      }
   }
   if( !not01 )
   {
      for( roomlist = tarea->first_room; roomlist; roomlist = roomlist->next_aroom )
         renumber_put_resets( roomlist );
   }
   return;
}

#ifdef COMPRESSION_TEST
/*
uLong dictID;
const char dictionary[] = {"the ", "a ", "  "};
*/
/* Compress room desc */
/* Unfinished functions. Some shit needs to be repaired.
 * Kratas
 */
bool compressRoom( ROOM_INDEX_DATA * room )
{
   z_stream *s;
   int length;
   int status;

   if( room->compress )
      return TRUE;

   if( !room->description )
   {
      bug( "compressRoom: No room desc!" );
      return FALSE;
   }

   length = strlen( room->description );
   room->desc_size = length;
//    room->desc_alloc = length*2+15;
   room->desc_alloc = length * 2 + 15;

   CREATE( s, z_stream, 1 );

   CREATE( room->compress_desc, unsigned char, room->desc_alloc );

   s->next_in = room->description;
   s->avail_in = length;
   s->next_out = room->compress_desc;
   s->avail_out = room->desc_alloc;
   s->zalloc = zlib_alloc;
   s->zfree = zlib_free;
   s->opaque = NULL;

/*
    err = deflateSetDictionary(&s,
                               (const Bytef*)dictionary, sizeof(dictionary));

    dictID = s.adler;
*/
   if( deflateInit2( s, 9, Z_DEFLATED, 15, 9, Z_FILTERED ) != Z_OK )
//  if( deflateInit2(s, 9, Z_DEFLATED, 15, 9, Z_DEFAULT_STRATEGY) != Z_OK )
   {
      DISPOSE( room->compress_desc );
      DISPOSE( s );
      return FALSE;
   }

//    compress( room->compress_desc, (length+(length*.5)+24) , room->description, length );
   room->compress = s;
/*
    while( s->avail_in )
    {
        s->avail_out = room->desc_alloc - (s->next_out - room->compress_desc);
        if( s->avail_out )
        {
            int status = deflate(s, Z_SYNC_FLUSH);
            if (status != Z_OK)
                return FALSE;
        }
        s->next_out = room->compress_desc;
        } */
   status = deflate( room->compress, Z_FINISH );

   if( status == Z_STREAM_ERROR )
   {
      //        bug( "Failed compressing of room! %d", room->vnum );
      bug( "stream error: %d", room->vnum );
      DISPOSE( s );
      DISPOSE( room->compress_desc );
      return FALSE;
   }

   if( status == Z_BUF_ERROR )
   {
      bug( "buf error: %d", room->vnum );
      DISPOSE( s );
      DISPOSE( room->compress_desc );
      return FALSE;
   }

   if( status != Z_OK && status != Z_STREAM_END )
   {
      bug( "Failed compressing of room! %d", room->vnum );
      DISPOSE( s );
      DISPOSE( room->compress_desc );
      return FALSE;
   }

   deflateEnd( room->compress );
/*
    room->desc_alloc = strlen( room->compress_desc );
    RECREATE( room->compress_desc, unsigned char, room->desc_alloc );
*/
   /*
    * Get rid of old description! -- Kratas 
    */
   DISPOSE( room->description );
   return TRUE;
}

void addDecompress( ROOM_INDEX_DATA * room )
{
   ROOM_DECOMP *rdecomp;

   CREATE( rdecomp, ROOM_DECOMP, 1 );

   rdecomp->room = room;
   room->rdecomp = rdecomp;
   /*
    * Decompress the room in ~9 minutes 
    */
   rdecomp->ttl_decomp = 8;

   LINK( rdecomp, first_decompress, last_decompress, next, prev );
}

void removeDecompress( ROOM_DECOMP * rdecomp )
{
   /*
    * its expired... destroy the decompressed desc 
    */
   rdecomp->room->rdecomp = NULL;
   UNLINK( rdecomp, first_decompress, last_decompress, next, prev );
   DISPOSE( rdecomp->room->description );
   DISPOSE( rdecomp );
}

void decompressRoom( ROOM_INDEX_DATA * room )
{
   z_stream *s;
   int status;

   if( !room->compress )
   {
      bug( "Room isn't compressed" );
      return;
   }

   /*
    * Room was already decompressed! 
    */
   if( room->description )
   {
      room->rdecomp->ttl_decomp = 8;
      return;
   }

   /*
    * This here will soon be obsolete -- Kratas 
    */
/*
    if( room->description )
    {
        DISPOSE( room->description );
        CREATE( room->description, unsigned char, room->desc_size );
    } */
   if( !room->description )
      CREATE( room->description, unsigned char, room->desc_size );

   s = room->compress;

   s->next_in = room->compress_desc;
   s->next_out = room->description;
   s->avail_in = strlen( room->compress_desc ) + 1;
   s->avail_out = room->desc_size;

   if( inflateInit2( s, 15 ) != Z_OK )
   {
      bug( "decompressRoom: Failed init!" );
      DISPOSE( room->compress_desc );
      DISPOSE( room->description );
      DISPOSE( s );
      return;
   }

   status = inflate( s, Z_PARTIAL_FLUSH );

   room->compress = s;
//    status = uncompress( room->description, room->desc_size,
   if( status == Z_OK || Z_STREAM_END )
   {
      addDecompress( room );
      inflateEnd( s );
      return;
   }
   /*
    * if( status == Z_STREAM_END )
    * {
    * bug( "decompressRoom: Hey! How come the stream ended?" );
    * return;
    * } 
    */
   if( status == Z_BUF_ERROR || status == Z_STREAM_ERROR || status == Z_DATA_ERROR || status == Z_MEM_ERROR )
   {
      bug( "decompressRoom: Failed decompression" );
      return;
   }
}

/* Stop compression of the room desc */
#if 0
bool uncompressRoom( ROOM_INDEX_DATA * room )
{
   unsigned char dummy[1];

   if( !room->compress )
      return TRUE;

   room->compress->avail_in = 0;
   room->compress->next_in = dummy;

   if( deflate( room->compress, Z_FINISH ) != Z_STREAM_END )
      return FALSE;

   if( !processCompressed( d ) ) /* try to send any residual data */
      return FALSE;

   deflateEnd( room->compress );
   DISPOSE( room->compress_desc );
   DISPOSE( room->compress );

   return TRUE;
}
#endif

/*
 * Decompress rooms when the time comes
 * Kratas
 */
void room_update( void )
{
   ROOM_DECOMP *rdecomp;

   if( !first_decompress )
      return;

   for( rdecomp = first_decompress; rdecomp; rdecomp = rdecomp->next )
   {
      if( !rdecomp )
         break;

      if( rdecomp->ttl_decomp <= 0 )
      {
         removeDecompress( rdecomp );
         bug( "room_update: Decompress room: %d.", rdecomp->room->vnum );
         continue;
      }
      else
         rdecomp->ttl_decomp -= 1;
   }
}
#endif

void load_room_reset( ROOM_INDEX_DATA * room, FILE * fp )
{
   EXIT_DATA *pexit;
   char letter;
   int extra, arg1, arg2, arg3;
   bool not01 = FALSE;
   int count = 0;

   letter = fread_letter( fp );
   extra = fread_number( fp );
   if( letter == 'M' || letter == 'O' )
      extra = 0;
   arg1 = fread_number( fp );
   arg2 = fread_number( fp );
   arg3 = ( letter == 'G' || letter == 'R' ) ? 0 : fread_number( fp );
   fread_to_eol( fp );
   ++count;

   /*
    * Validate parameters.
    * We're calling the index functions for the side effect.
    */
   switch ( letter )
   {
      default:
         bug( "%s: bad command '%c'.", __FUNCTION__, letter );
         if( fBootDb )
            boot_log( "%s: %s (%d) bad command '%c'.", __FUNCTION__, room->area->filename, count, letter );
         return;

      case 'M':
         if( get_mob_index( arg1 ) == NULL && fBootDb )
            boot_log( "%s: %s (%d) 'M': mobile %d doesn't exist.", __FUNCTION__, room->area->filename, count, arg1 );
         break;

      case 'O':
         if( get_obj_index( arg1 ) == NULL && fBootDb )
            boot_log( "%s: %s (%d) '%c': object %d doesn't exist.", __FUNCTION__, room->area->filename, count, letter,
                      arg1 );
         break;

      case 'P':
         if( get_obj_index( arg1 ) == NULL && fBootDb )
            boot_log( "%s: %s (%d) '%c': object %d doesn't exist.", __FUNCTION__, room->area->filename, count, letter,
                      arg1 );

         if( arg3 <= 0 )
            arg3 = OBJ_VNUM_GOLD_ONE;  // This may look stupid, but for some reason it works.
         if( get_obj_index( arg3 ) == NULL && fBootDb )
            boot_log( "%s: %s (%d) 'P': destination object %d doesn't exist.", __FUNCTION__, room->area->filename, count,
                      arg3 );
         if( extra > 1 )
            not01 = TRUE;
         break;

      case 'G':
      case 'E':
         if( get_obj_index( arg1 ) == NULL && fBootDb )
            boot_log( "%s: %s (%d) '%c': object %d doesn't exist.", __FUNCTION__, room->area->filename, count, letter,
                      arg1 );
         break;

      case 'T':
      case 'H':
         break;

      case 'D':
         if( arg2 < 0 || arg2 > MAX_DIR + 1
             || !( pexit = get_exit( room, arg2 ) ) || !IS_SET( pexit->exit_info, EX_ISDOOR ) )
         {
            bug( "%s: 'D': exit %d not door.", __FUNCTION__, arg2 );
            bug( "Reset: %c %d %d %d %d", letter, extra, arg1, arg2, arg3 );
            if( fBootDb )
               boot_log( "%s: %s (%d) 'D': exit %d not door.", __FUNCTION__, room->area->filename, count, arg2 );
         }

         if( arg3 < 0 || arg3 > 2 )
         {
            bug( "%s: 'D': bad 'locks': %d.", __FUNCTION__, arg3 );
            if( fBootDb )
               boot_log( "%s: %s (%d) 'D': bad 'locks': %d.", __FUNCTION__, room->area->filename, count, arg3 );
         }
         break;

      case 'R':
         if( arg2 < 0 || arg2 > 10 )
         {
            bug( "%s: 'R': bad exit %d.", __FUNCTION__, arg2 );
            if( fBootDb )
               boot_log( "%s: %s (%d) 'R': bad exit %d.", __FUNCTION__, room->area->filename, count, arg2 );
            break;
         }
         break;
   }
   add_reset( room, letter, extra, arg1, arg2, arg3 );

   if( !not01 )
      renumber_put_resets( room );
   return;
}

/*
 * Load a room section.
 */
void load_rooms( AREA_DATA * tarea, FILE * fp )
{
   ROOM_INDEX_DATA *pRoomIndex;
   char buf[MAX_STRING_LENGTH];
   char *ln;
#ifdef COMPRESSION_TEST
   int compress;
#endif

   if( !tarea )
   {
      bug( "Load_rooms: no #AREA seen yet." );
      shutdown_mud( "No #AREA" );
      exit( 1 );
   }

   tarea->first_room = tarea->last_room = NULL;

   for( ;; )
   {
      int vnum;
      char letter;
      int door;
      int iHash;
      bool tmpBootDb;
      bool oldroom;
      int x1, x2, x3, x4, x5, x6;

      letter = fread_letter( fp );
      if( letter != '#' )
      {
         bug( "Load_rooms: # not found." );
         if( fBootDb )
         {
            shutdown_mud( "# not found" );
            exit( 1 );
         }
         else
            return;
      }

      vnum = fread_number( fp );
      if( vnum == 0 )
         break;

      tmpBootDb = fBootDb;
      fBootDb = FALSE;
      if( get_room_index( vnum ) != NULL )
      {
         if( tmpBootDb )
         {
            bug( "Load_rooms: vnum %d duplicated.", vnum );
            shutdown_mud( "duplicate vnum" );
            exit( 1 );
         }
         else
         {
            pRoomIndex = get_room_index( vnum );
            sprintf( buf, "Cleaning room: %d", vnum );
            log_string_plus( buf, LOG_BUILD, sysdata.log_level );
            clean_room( pRoomIndex );
            oldroom = TRUE;
         }
      }
      else
      {
         oldroom = FALSE;
         CREATE( pRoomIndex, ROOM_INDEX_DATA, 1 );
         pRoomIndex->first_person = NULL;
         pRoomIndex->last_person = NULL;
         pRoomIndex->first_content = NULL;
         pRoomIndex->last_content = NULL;
      }

      fBootDb = tmpBootDb;
      pRoomIndex->area = tarea;
      pRoomIndex->vnum = vnum;
      pRoomIndex->first_extradesc = NULL;
      pRoomIndex->last_extradesc = NULL;

      if( fBootDb )
      {
         if( !tarea->low_r_vnum )
            tarea->low_r_vnum = vnum;
         if( vnum > tarea->hi_r_vnum )
            tarea->hi_r_vnum = vnum;
      }
      pRoomIndex->name = fread_string( fp );
      pRoomIndex->description = fread_string_nohash( fp );
#ifdef COMPRESSION_TEST
      compress = compressRoom( pRoomIndex );
      /*
       * if( compress )
       * DISPOSE( pRoomIndex->description ); 
       */
#endif
      /*
       * Area number         fread_number( fp ); 
       */
      ln = fread_line( fp );
      x1 = x2 = x3 = x4 = x5 = x6 = 0;
      sscanf( ln, "%d %d %d %d %d %d", &x1, &x2, &x3, &x4, &x5, &x6 );

      pRoomIndex->room_flags = x2;
      pRoomIndex->sector_type = x3;
      pRoomIndex->tele_delay = x4;
      pRoomIndex->tele_vnum = x5;
      pRoomIndex->tunnel = x6;

      if( pRoomIndex->sector_type < 0 || pRoomIndex->sector_type >= SECT_MAX )
      {
         bug( "Fread_rooms: vnum %d has bad sector_type %d.", vnum, pRoomIndex->sector_type );
         pRoomIndex->sector_type = 1;
      }
      pRoomIndex->light = 0;
      pRoomIndex->first_exit = NULL;
      pRoomIndex->last_exit = NULL;

      for( ;; )
      {
         letter = fread_letter( fp );

         if( letter == 'S' )
            break;

         if( letter == 'D' )
         {
            EXIT_DATA *pexit;
            int locks;

            door = fread_number( fp );
            if( door < 0 || door > 10 )
            {
               bug( "Fread_rooms: vnum %d has bad door number %d.", vnum, door );
               if( fBootDb )
                  exit( 1 );
            }
            else
            {
               pexit = make_exit( pRoomIndex, NULL, door );
               pexit->description = fread_string( fp );
               pexit->keyword = fread_string( fp );
               pexit->exit_info = 0;
               ln = fread_line( fp );
               x1 = x2 = x3 = x4 = x5 = x6 = 0;
               sscanf( ln, "%d %d %d %d %d %d", &x1, &x2, &x3, &x4, &x5, &x6 );

               locks = x1;
               pexit->key = x2;
               pexit->vnum = x3;
               pexit->vdir = door;
               pexit->distance = x4;
               pexit->pulltype = x5;
               pexit->pull = x6;

               switch ( locks )
               {
                  case 1:
                     pexit->exit_info = EX_ISDOOR;
                     break;
                  case 2:
                     pexit->exit_info = EX_ISDOOR | EX_PICKPROOF;
                     break;
                  default:
                     pexit->exit_info = locks;
               }
            }
         }
         else if( letter == 'E' )
         {
            EXTRA_DESCR_DATA *ed;

            CREATE( ed, EXTRA_DESCR_DATA, 1 );
            ed->keyword = fread_string( fp );
            ed->description = fread_string( fp );
            LINK( ed, pRoomIndex->first_extradesc, pRoomIndex->last_extradesc, next, prev );
            top_ed++;
         }
         else if( letter == 'R' )
            load_room_reset( pRoomIndex, fp );
         else if( letter == 'M' )   /* ignore maps */
            fread_to_eol( fp );
         else if( letter == '>' )
         {
            ungetc( letter, fp );
            rprog_read_programs( fp, pRoomIndex );
         }
         else
         {
            bug( "Load_rooms: vnum %d has flag '%c' not 'DES'.", vnum, letter );
            shutdown_mud( "Room flag not DES" );
            exit( 1 );
         }
      }

      if( !oldroom )
      {
         iHash = vnum % MAX_KEY_HASH;
         pRoomIndex->next = room_index_hash[iHash];
         room_index_hash[iHash] = pRoomIndex;
         LINK( pRoomIndex, tarea->first_room, tarea->last_room, next_aroom, prev_aroom );
         top_room++;
      }
   }

   return;
}

/*
 * Load a shop section.
 */
void load_shops( FILE * fp )
{
   SHOP_DATA *pShop;

   for( ;; )
   {
      MOB_INDEX_DATA *pMobIndex;
      int iTrade;

      CREATE( pShop, SHOP_DATA, 1 );
      pShop->keeper = fread_number( fp );
      if( pShop->keeper == 0 )
      {
         DISPOSE( pShop );
         break;
      }
      for( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
         pShop->buy_type[iTrade] = fread_number( fp );
      pShop->profit_buy = fread_number( fp );
      pShop->profit_sell = fread_number( fp );
      pShop->profit_buy = URANGE( pShop->profit_sell + 5, pShop->profit_buy, 1000 );
      pShop->profit_sell = URANGE( 0, pShop->profit_sell, pShop->profit_buy - 5 );
      pShop->open_hour = fread_number( fp );
      pShop->close_hour = fread_number( fp );
      fread_to_eol( fp );
      pMobIndex = get_mob_index( pShop->keeper );
      pMobIndex->pShop = pShop;

      if( !first_shop )
         first_shop = pShop;
      else
         last_shop->next = pShop;
      pShop->next = NULL;
      pShop->prev = last_shop;
      last_shop = pShop;
      top_shop++;
   }
   return;
}

/*
 * Load a repair shop section.					-Thoric
 */
void load_repairs( FILE * fp )
{
   REPAIR_DATA *rShop;

   for( ;; )
   {
      MOB_INDEX_DATA *pMobIndex;
      int iFix;

      CREATE( rShop, REPAIR_DATA, 1 );
      rShop->keeper = fread_number( fp );
      if( rShop->keeper == 0 )
      {
         DISPOSE( rShop );
         break;
      }
      for( iFix = 0; iFix < MAX_FIX; iFix++ )
         rShop->fix_type[iFix] = fread_number( fp );
      rShop->profit_fix = fread_number( fp );
      rShop->shop_type = fread_number( fp );
      rShop->open_hour = fread_number( fp );
      rShop->close_hour = fread_number( fp );
      fread_to_eol( fp );
      pMobIndex = get_mob_index( rShop->keeper );
      pMobIndex->rShop = rShop;

      if( !first_repair )
         first_repair = rShop;
      else
         last_repair->next = rShop;
      rShop->next = NULL;
      rShop->prev = last_repair;
      last_repair = rShop;
      top_repair++;
   }
   return;
}

/*
 * Load spec proc declarations.
 */
void load_specials( FILE * fp )
{
   for( ;; )
   {
      MOB_INDEX_DATA *pMobIndex;
      char letter;

      switch ( letter = fread_letter( fp ) )
      {
         default:
            bug( "Load_specials: letter '%c' not *MS.", letter );
            exit( 1 );

         case 'S':
            return;

         case '*':
            break;

         case 'M':
            pMobIndex = get_mob_index( fread_number( fp ) );
            pMobIndex->spec_fun = spec_lookup( fread_word( fp ) );
            if( pMobIndex->spec_fun == 0 )
            {
               bug( "Load_specials: 'M': vnum %d.", pMobIndex->vnum );
               exit( 1 );
            }
            break;
      }

      fread_to_eol( fp );
   }
}

/*
 * Load soft / hard area ranges.
 */
void load_ranges( AREA_DATA * tarea, FILE * fp )
{
   int x1, x2, x3, x4;
   char *ln;

   if( !tarea )
   {
      bug( "Load_ranges: no #AREA seen yet." );
      shutdown_mud( "No #AREA" );
      exit( 1 );
   }

   for( ;; )
   {
      ln = fread_line( fp );

      if( ln[0] == '$' )
         break;

      x1 = x2 = x3 = x4 = 0;
      sscanf( ln, "%d %d %d %d", &x1, &x2, &x3, &x4 );

      tarea->low_soft_range = x1;
      tarea->hi_soft_range = x2;
      tarea->low_hard_range = x3;
      tarea->hi_hard_range = x4;
   }
   return;

}

/*
 * Load climate information for the area
 * Last modified: July 13, 1997
 * Fireblade
 */
void load_climate( AREA_DATA * tarea, FILE * fp )
{
   if( !tarea )
   {
      bug( "load_climate: no #AREA seen yet" );
      if( fBootDb )
      {
         shutdown_mud( "No #AREA" );
         exit( 1 );
      }
      else
         return;
   }

   tarea->weather->climate_temp = fread_number( fp );
   tarea->weather->climate_precip = fread_number( fp );
   tarea->weather->climate_wind = fread_number( fp );

   return;
}

/*
 * Load data for a neghboring weather system
 * Last modified: July 13, 1997
 * Fireblade
 */
void load_neighbor( AREA_DATA * tarea, FILE * fp )
{
   NEIGHBOR_DATA *new;

   if( !tarea )
   {
      bug( "load_neighbor: no #AREA seen yet." );
      if( fBootDb )
      {
         shutdown_mud( "No #AREA" );
         exit( 1 );
      }
      else
         return;
   }

   CREATE( new, NEIGHBOR_DATA, 1 );
   new->next = NULL;
   new->prev = NULL;
   new->address = NULL;
   new->name = fread_string( fp );
   LINK( new, tarea->weather->first_neighbor, tarea->weather->last_neighbor, next, prev );

   return;
}


/*
 * Go through all areas, and set up initial economy based on mob
 * levels and gold
 */
void initialize_economy( void )
{
   AREA_DATA *tarea;
   MOB_INDEX_DATA *mob;
   int idx, gold, rng, mobmoney;

   for( tarea = first_area; tarea; tarea = tarea->next )
   {
      /*
       * skip area if they already got some gold 
       */
      if( tarea->high_economy > 0 || tarea->low_economy > 10000 )
         continue;
      rng = tarea->hi_soft_range - tarea->low_soft_range;
      if( rng )
         rng /= 2;
      else
         rng = 25;
      gold = rng * rng * 50000;
      boost_economy( tarea, gold );
      for( idx = tarea->low_m_vnum; idx < tarea->hi_m_vnum; idx++ )
         if( ( mob = get_mob_index( idx ) ) != NULL )
         {
            /*
             * Gold/Silver/copper Support -Druid 
             */
            mobmoney = get_value( mob->gold, mob->silver, mob->copper );
            boost_economy( tarea, mobmoney * 10 );
         }
   }
}

/*
 * Translate all room exits from virtual to real.
 * Has to be done after all rooms are read in.
 * Check for bad reverse exits.
 */
void fix_exits( void )
{
   ROOM_INDEX_DATA *pRoomIndex;
   EXIT_DATA *pexit, *pexit_next, *rv_exit;
   int iHash;

   for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
   {
      for( pRoomIndex = room_index_hash[iHash]; pRoomIndex; pRoomIndex = pRoomIndex->next )
      {
         bool fexit;

         fexit = FALSE;
         for( pexit = pRoomIndex->first_exit; pexit; pexit = pexit_next )
         {
            pexit_next = pexit->next;
            pexit->rvnum = pRoomIndex->vnum;
            if( pexit->vnum <= 0 || ( pexit->to_room = get_room_index( pexit->vnum ) ) == NULL )
            {
               if( fBootDb )
                  boot_log( "Fix_exits: room %d, exit %s leads to bad vnum (%d)",
                            pRoomIndex->vnum, dir_name[pexit->vdir], pexit->vnum );

               bug( "Deleting %s exit in room %d", dir_name[pexit->vdir], pRoomIndex->vnum );
               extract_exit( pRoomIndex, pexit );
            }
            else
               fexit = TRUE;
         }
         if( !fexit )
            SET_BIT( pRoomIndex->room_flags, ROOM_NO_MOB );
      }
   }

   /*
    * Set all the rexit pointers   -Thoric 
    */
   for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
   {
      for( pRoomIndex = room_index_hash[iHash]; pRoomIndex; pRoomIndex = pRoomIndex->next )
      {
         for( pexit = pRoomIndex->first_exit; pexit; pexit = pexit->next )
         {
            if( pexit->to_room && !pexit->rexit )
            {
               rv_exit = get_exit_to( pexit->to_room, rev_dir[pexit->vdir], pRoomIndex->vnum );
               if( rv_exit )
               {
                  pexit->rexit = rv_exit;
                  rv_exit->rexit = pexit;
               }
            }
         }
      }
   }
   return;
}

/*
 * Get diku-compatable exit by number				-Thoric
 */
EXIT_DATA *get_exit_number( ROOM_INDEX_DATA * room, int xit )
{
   EXIT_DATA *pexit;
   int count;

   count = 0;
   for( pexit = room->first_exit; pexit; pexit = pexit->next )
      if( ++count == xit )
         return pexit;
   return NULL;
}

/*
 * (prelude...) This is going to be fun... NOT!
 * (conclusion) QSort is f*cked!
 */
int exit_comp( EXIT_DATA ** xit1, EXIT_DATA ** xit2 )
{
   int d1, d2;

   d1 = ( *xit1 )->vdir;
   d2 = ( *xit2 )->vdir;

   if( d1 < d2 )
      return -1;
   if( d1 > d2 )
      return 1;
   return 0;
}

void sort_exits( ROOM_INDEX_DATA * room )
{
   EXIT_DATA *pexit; /* *texit *//* Unused */
   EXIT_DATA *exits[MAX_REXITS];
   int x, nexits;

   nexits = 0;
   for( pexit = room->first_exit; pexit; pexit = pexit->next )
   {
      exits[nexits++] = pexit;
      if( nexits > MAX_REXITS )
      {
         bug( "sort_exits: more than %d exits in room... fatal", nexits );
         return;
      }
   }
   qsort( &exits[0], nexits, sizeof( EXIT_DATA * ), ( int ( * )( const void *, const void * ) )exit_comp );
   for( x = 0; x < nexits; x++ )
   {
      if( x > 0 )
         exits[x]->prev = exits[x - 1];
      else
      {
         exits[x]->prev = NULL;
         room->first_exit = exits[x];
      }
      if( x >= ( nexits - 1 ) )
      {
         exits[x]->next = NULL;
         room->last_exit = exits[x];
      }
      else
         exits[x]->next = exits[x + 1];
   }
}

void randomize_exits( ROOM_INDEX_DATA * room, short maxdir )
{
   EXIT_DATA *pexit;
   int nexits, /* maxd, */ d0, d1, count, door; /* Maxd unused */
   int vdirs[MAX_REXITS];

   nexits = 0;
   for( pexit = room->first_exit; pexit; pexit = pexit->next )
      vdirs[nexits++] = pexit->vdir;

   for( d0 = 0; d0 < nexits; d0++ )
   {
      if( vdirs[d0] > maxdir )
         continue;
      count = 0;
      while( vdirs[( d1 = number_range( d0, nexits - 1 ) )] > maxdir || ++count > 5 );
      if( vdirs[d1] > maxdir )
         continue;
      door = vdirs[d0];
      vdirs[d0] = vdirs[d1];
      vdirs[d1] = door;
   }
   count = 0;
   for( pexit = room->first_exit; pexit; pexit = pexit->next )
      pexit->vdir = vdirs[count++];

   sort_exits( room );
}

/*
 * Repopulate areas periodically.
 */
void area_update( void )
{
   AREA_DATA *pArea;

   for( pArea = first_area; pArea; pArea = pArea->next )
   {
      CHAR_DATA *pch;   /* Comment out is using load_world */
      int reset_age = pArea->reset_frequency ? pArea->reset_frequency : 15;

      /*
       * Area isn't loaded yet, skip 
       */
      if( !IS_SET( pArea->flags, AFLAG_PLR_LOADED ) )
         continue;

      if( ( reset_age == -1 && pArea->age == -1 ) || ++pArea->age < ( reset_age - 1 ) )
         continue;

      if( pArea->ttl == 0 && pArea->nplayer < 1 )
      {
         ROOM_INDEX_DATA *room;
         CHAR_DATA *victim, *vic_next;
         OBJ_DATA *obj, *obj_next;
         long vnum;

         char buf[MAX_INPUT_LENGTH];

         if( IS_SET( pArea->flags, AFLAG_RESET_ON_BOOT ) )
         {
            pArea->ttl = -1;
            continue;
         }

         sprintf( buf, "Unloading %s.", pArea->filename );
         log_string( buf );

         for( vnum = pArea->low_r_vnum; vnum <= pArea->hi_r_vnum; vnum++ )
         {
            room = get_room_index( vnum );

            if( !room )
               continue;

            if( IS_SET( room->room_flags, ROOM_DONATION ) || IS_SET( room->room_flags, ROOM_CLANSTOREROOM ) )
               continue;

            for( victim = room->first_person; victim; victim = vic_next )
            {
               vic_next = victim->next;

               if( IS_NPC( victim ) && victim != supermob )
                  extract_char( victim, TRUE );
            }

            for( obj = room->first_content; obj; obj = obj_next )
            {
               obj_next = obj->next_content;
               if( obj->item_type == ITEM_CORPSE_PC )
                  continue;
               extract_obj( obj );
            }
         }
         REMOVE_BIT( pArea->flags, AFLAG_PLR_LOADED );
      }
      else if( pArea->nplayer > 0 )
         pArea->ttl = -1;

      if( pArea->ttl > 0 )
         pArea->ttl--;

      /*
       * Check for PC's.
       */
      if( pArea->nplayer > 0 && pArea->age == ( reset_age - 1 ) )
      {
         char buf[MAX_STRING_LENGTH];  /* Comment out if using load_world */

         /*
          * Rennard 
          */
         /*
          * Comment out everything below here to Scion if using load_world 
          */
         if( pArea->resetmsg )
            sprintf( buf, "%s\r\n", pArea->resetmsg );
         else
            strcpy( buf, "You hear some squeaking sounds...\r\n" );
         for( pch = first_char; pch; pch = pch->next )
         {
            if( !IS_NPC( pch )
                && IS_AWAKE( pch )
                && pch->in_room && pch->in_room->area == pArea && ( pch->desc && pch->desc->connected == CON_PLAYING ) )
            {
               set_char_color( AT_RESET, pch );
               send_to_char( buf, pch );
            }
         }

/*	-- Don't reset the area if players are in it. -- Scion */
         continue;
      }

      /*
       * Check age and reset.
       * Note: Mud Academy resets every 3 minutes (not 15).
       */

/* NEVER RESET if the total number of objects is less than the number of areas * 60  -- Scion */
      if( ( pArea->nplayer == 0 || pArea->age >= reset_age ) &&
          ( ( physicalobjects < ( top_area * 90 ) ) || ( nummobsloaded < ( top_area * 90 ) ) ) )
      {
         ROOM_INDEX_DATA *pRoomIndex;  /* objs and mobs top_area usually *90 */

         reset_area( pArea );
         if( reset_age == -1 )
            pArea->age = -1;
         else
            pArea->age = number_range( 0, reset_age / 5 );
         pRoomIndex = get_room_index( ROOM_VNUM_SCHOOL );
         if( pRoomIndex != NULL && pArea == pRoomIndex->area && pArea->reset_frequency == 0 )
            pArea->age = 15 - 3;
      }
   }
   return;
}

/*
 * Create an instance of a mobile.
 */
CHAR_DATA *create_mobile( MOB_INDEX_DATA * pMobIndex )
{
   CHAR_DATA *mob;

   if( !pMobIndex )
   {
      bug( "Create_mobile: NULL pMobIndex." );
      return NULL;
   }

   CREATE( mob, CHAR_DATA, 1 );
   clear_char( mob );
   mob->pIndexData = pMobIndex;

   mob->editor = NULL;
   mob->name = QUICKLINK( pMobIndex->player_name );
   mob->short_descr = QUICKLINK( pMobIndex->short_descr );
   mob->long_descr = QUICKLINK( pMobIndex->long_descr );
   mob->description = QUICKLINK( pMobIndex->description );
   mob->spec_fun = pMobIndex->spec_fun;
   mob->mpscriptpos = 0;
   mob->level = number_fuzzy( pMobIndex->level );
   mob->act = pMobIndex->act;

   if( xIS_SET( mob->act, ACT_MOBINVIS ) )
      mob->mobinvis = mob->level;

   mob->affected_by = pMobIndex->affected_by;
   mob->alignment = pMobIndex->alignment;
   mob->sex = pMobIndex->sex;

   /*
    * Bug fix from mailing list by stu (sprice@@ihug.co.nz)
    * was:  if ( !pMobIndex->ac )
    */
   if( pMobIndex->ac )
      mob->armor = pMobIndex->ac;
   else
      mob->armor = interpolate( mob->level, 100, -100 );

   if( !pMobIndex->hitnodice )
      mob->max_hit = mob->level * 8 + number_range( mob->level * mob->level / 4, mob->level * mob->level );
   else
      mob->max_hit = pMobIndex->hitnodice * number_range( 1, pMobIndex->hitsizedice ) + pMobIndex->hitplus;
   mob->hit = mob->max_hit;
   /*
    * lets put things back the way they used to be! -Thoric 
    */
   mob->gold = pMobIndex->gold;
   mob->silver = pMobIndex->silver;
   mob->copper = pMobIndex->copper;
   mob->exp = pMobIndex->exp;
   mob->position = pMobIndex->position;
   mob->defposition = pMobIndex->defposition;
   mob->barenumdie = pMobIndex->damnodice;
   mob->baresizedie = pMobIndex->damsizedice;
   mob->mobthac0 = pMobIndex->mobthac0;
   mob->hitplus = pMobIndex->hitplus;
   mob->damplus = pMobIndex->damplus;

   mob->perm_str = pMobIndex->perm_str;
   mob->perm_dex = pMobIndex->perm_dex;
   mob->perm_wis = pMobIndex->perm_wis;
   mob->perm_int = pMobIndex->perm_int;
   mob->perm_con = pMobIndex->perm_con;
   mob->perm_cha = pMobIndex->perm_cha;
   mob->perm_lck = pMobIndex->perm_lck;
   mob->hitroll = pMobIndex->hitroll;
   mob->damroll = pMobIndex->damroll;
   mob->race = pMobIndex->race;
   mob->class = pMobIndex->class;
   mob->xflags = pMobIndex->xflags;
   mob->saving_poison_death = pMobIndex->saving_poison_death;
   mob->saving_wand = pMobIndex->saving_wand;
   mob->saving_para_petri = pMobIndex->saving_para_petri;
   mob->saving_breath = pMobIndex->saving_breath;
   mob->saving_spell_staff = pMobIndex->saving_spell_staff;
   mob->height = pMobIndex->height;
   mob->weight = pMobIndex->weight;
   mob->resistant = pMobIndex->resistant;
   mob->immune = pMobIndex->immune;
   mob->susceptible = pMobIndex->susceptible;
   mob->attacks = pMobIndex->attacks;
   mob->defenses = pMobIndex->defenses;
   mob->numattacks = pMobIndex->numattacks;
   mob->speaks = pMobIndex->speaks;
   mob->speaking = pMobIndex->speaking;

   /*
    * Perhaps add this to the index later --Shaddai
    */
   xCLEAR_BITS( mob->no_affected_by );
   mob->no_resistant = 0;
   mob->no_immune = 0;
   mob->no_susceptible = 0;
   /*
    * Insert in list.
    */
   add_char( mob );
   pMobIndex->count++;
   nummobsloaded++;
   return mob;
}



/*
 * Create an instance of an object.
 */
OBJ_DATA *create_object( OBJ_INDEX_DATA * pObjIndex, int level )
{
   OBJ_DATA *obj;

   if( !pObjIndex )
   {
      bug( "Create_object: NULL pObjIndex." );
      exit( 1 );
   }

   CREATE( obj, OBJ_DATA, 1 );

   obj->pIndexData = pObjIndex;
   obj->in_room = NULL;
   obj->level = level;
   obj->wear_loc = -1;
   obj->count = 1;
   cur_obj_serial = UMAX( ( cur_obj_serial + 1 ) & ( BV30 - 1 ), 1 );
   obj->serial = obj->pIndexData->serial = cur_obj_serial;

   obj->name = QUICKLINK( pObjIndex->name );
   obj->short_descr = QUICKLINK( pObjIndex->short_descr );
   obj->description = QUICKLINK( pObjIndex->description );
   obj->action_desc = QUICKLINK( pObjIndex->action_desc );
   obj->item_type = pObjIndex->item_type;
   obj->extra_flags = pObjIndex->extra_flags;
   obj->wear_flags = pObjIndex->wear_flags;
   obj->value[0] = pObjIndex->value[0];
   obj->value[1] = pObjIndex->value[1];
   obj->value[2] = pObjIndex->value[2];
   obj->value[3] = pObjIndex->value[3];
   obj->value[4] = pObjIndex->value[4];
   obj->value[5] = pObjIndex->value[5];
   obj->weight = pObjIndex->weight;
   obj->gold_cost = pObjIndex->gold_cost;
   obj->silver_cost = pObjIndex->silver_cost;
   obj->copper_cost = pObjIndex->copper_cost;

   /*
    * obj->cost     = number_fuzzy( 10 )
    * * number_fuzzy( level ) * number_fuzzy( level );
    */

   /*
    * Mess with object properties.
    */
   switch ( obj->item_type )
   {
      default:
         bug( "Read_object: vnum %d bad type.", pObjIndex->vnum );
         bug( "------------------------>     ", obj->item_type );
         break;

      case ITEM_LIGHT:
      case ITEM_TREASURE:
      case ITEM_FURNITURE:
      case ITEM_TRASH:
      case ITEM_CONTAINER:
      case ITEM_DRINK_CON:
      case ITEM_KEY:
      case ITEM_KEYRING:
      case ITEM_PIECE:
      case ITEM_ODOR:
         break;
      case ITEM_COOK:
      case ITEM_FOOD:
         /*
          * optional food condition (rotting food)    -Thoric
          * value1 is the max condition of the food
          * value4 is the optional initial condition
          */
         if( obj->value[4] )
            obj->timer = obj->value[4];
         else
            obj->timer = obj->value[1];
         break;
      case ITEM_BOAT:
      case ITEM_CORPSE_NPC:
      case ITEM_CORPSE_PC:
      case ITEM_FOUNTAIN:
      case ITEM_BLOOD:
      case ITEM_BLOODSTAIN:
      case ITEM_SCRAPS:
      case ITEM_PIPE:
      case ITEM_HERB_CON:
      case ITEM_HERB:
      case ITEM_INCENSE:
      case ITEM_FIRE:
      case ITEM_BOOK:
      case ITEM_SWITCH:
      case ITEM_LEVER:
      case ITEM_PULLCHAIN:
      case ITEM_BUTTON:
      case ITEM_DIAL:
      case ITEM_RUNE:
      case ITEM_RUNEPOUCH:
      case ITEM_MATCH:
      case ITEM_TRAP:
      case ITEM_MAP:
      case ITEM_PORTAL:
      case ITEM_PAPER:
      case ITEM_PEN:
      case ITEM_TINDER:
      case ITEM_LOCKPICK:
      case ITEM_SPIKE:
      case ITEM_DISEASE:
      case ITEM_OIL:
      case ITEM_FUEL:
      case ITEM_QUIVER:
      case ITEM_SHOVEL:
         break;

      case ITEM_SALVE:
         obj->value[3] = number_fuzzy( obj->value[3] );
         break;

      case ITEM_SCROLL:
         obj->value[0] = number_fuzzy( obj->value[0] );
         break;

      case ITEM_WAND:
      case ITEM_STAFF:
         obj->value[0] = number_fuzzy( obj->value[0] );
         obj->value[1] = number_fuzzy( obj->value[1] );
         obj->value[2] = obj->value[1];
         break;

      case ITEM_WEAPON:
      case ITEM_MISSILE_WEAPON:
      case ITEM_PROJECTILE:
         if( obj->value[1] && obj->value[2] )
            obj->value[2] *= obj->value[1];
         else
         {
            obj->value[1] = number_fuzzy( number_fuzzy( 1 * level / 4 + 2 ) );
            obj->value[2] = number_fuzzy( number_fuzzy( 3 * level / 4 + 6 ) );
         }
         if( obj->value[0] == 0 )
            obj->value[0] = INIT_WEAPON_CONDITION;
         break;

      case ITEM_ARMOR:
         if( obj->value[0] == 0 )
            obj->value[0] = number_fuzzy( level / 4 + 2 );
         if( obj->value[1] == 0 )
            obj->value[1] = obj->value[0];
         break;

      case ITEM_POTION:
      case ITEM_PILL:
         obj->value[0] = number_fuzzy( number_fuzzy( obj->value[0] ) );
         break;

      case ITEM_GOLD:
         obj->value[0] = obj->gold_cost;
         if( obj->value[0] == 0 )
            obj->value[0] = 1;
         break;

      case ITEM_SILVER:
         obj->value[0] = obj->silver_cost;
         if( obj->value[0] == 0 )
            obj->value[0] = 1;
         break;

      case ITEM_COPPER:
         obj->value[0] = obj->copper_cost;
         if( obj->value[0] == 0 )
            obj->value[0] = 1;
         break;
   }

   LINK( obj, first_object, last_object, next, prev );
   ++pObjIndex->count;
   ++numobjsloaded;
   ++physicalobjects;

   return obj;
}


/*
 * Clear a new character.
 */
void clear_char( CHAR_DATA * ch )
{
   ch->editor = NULL;
   ch->hunting = NULL;
   ch->fearing = NULL;
   ch->hating = NULL;
   ch->name = NULL;
   ch->short_descr = NULL;
   ch->long_descr = NULL;
   ch->description = NULL;
   ch->next = NULL;
   ch->prev = NULL;
   ch->reply = NULL;
   ch->retell = NULL;
   ch->first_carrying = NULL;
   ch->last_carrying = NULL;
   ch->next_in_room = NULL;
   ch->prev_in_room = NULL;
   ch->fighting = NULL;
   ch->switched = NULL;
   ch->first_affect = NULL;
   ch->last_affect = NULL;
   ch->prev_cmd = NULL; /* maps */
   ch->last_cmd = NULL;
   ch->dest_buf = NULL;
   ch->alloc_ptr = NULL;
   ch->spare_ptr = NULL;
   ch->mount = NULL;
   ch->morph = NULL;
   xCLEAR_BITS( ch->affected_by );
   ch->logon = current_time;
   ch->armor = 100;
   ch->position = POS_STANDING;
   ch->practice = 0;
   ch->hit = 20;
   ch->max_hit = 20;
   ch->mana = 100;
   ch->max_mana = 100;
   ch->move = 100;
   ch->max_move = 100;
   ch->height = 72;
   ch->weight = 180;
   ch->xflags = 0;
   ch->race = 0;
   ch->class = 3;
   ch->class2 = 0;
   ch->class3 = 0;
   ch->level2 = -1;
   ch->level3 = -1;
   ch->speaking = LANG_COMMON;
   ch->speaks = LANG_COMMON;
   ch->barenumdie = 1;
   ch->baresizedie = 4;
   ch->substate = 0;
   ch->tempnum = 0;
   ch->perm_str = 13;
   ch->perm_dex = 13;
   ch->perm_int = 13;
   ch->perm_wis = 13;
   ch->perm_cha = 13;
   ch->perm_con = 13;
   ch->perm_lck = 13;
   ch->mod_str = 0;
   ch->mod_dex = 0;
   ch->mod_int = 0;
   ch->mod_wis = 0;
   ch->mod_cha = 0;
   ch->mod_con = 0;
   ch->mod_lck = 0;
   return;
}

/*
 * Free a character.
 */
void free_char( CHAR_DATA * ch )
{
   OBJ_DATA *obj;
   AFFECT_DATA *paf;
   TIMER *timer;
   MPROG_ACT_LIST *mpact, *mpact_next;
   NOTE_DATA *comments, *comments_next;
   int x;

   if( !ch )
   {
      bug( "Free_char: null ch!" );
      return;
   }

   if( ch->desc )
      bug( "Free_char: char still has descriptor." );

   if( ch->morph )
      DISPOSE( ch->morph );

   while( ( obj = ch->last_carrying ) != NULL )
      extract_obj( obj );

   while( ( paf = ch->last_affect ) != NULL )
      affect_remove( ch, paf );

   while( ( timer = ch->first_timer ) != NULL )
      extract_timer( ch, timer );

   if( ch->editor )
      stop_editing( ch );

   STRFREE( ch->name );
   STRFREE( ch->short_descr );
   STRFREE( ch->long_descr );
   STRFREE( ch->description );

   stop_hunting( ch );
   stop_hating( ch );
   stop_fearing( ch );
   free_fight( ch );

   if( ch->pnote )
      free_note( ch->pnote );

   if( ch->pcdata )
   {
      IGNORE_DATA *temp, *next;
      ALIAS_DATA *alias, *alias_next;
      int count;

      /*
       * free up memory allocated to stored ignored names 
       */
      for( temp = ch->pcdata->first_ignored; temp; temp = next )
      {
         next = temp->next;
         UNLINK( temp, ch->pcdata->first_ignored, ch->pcdata->last_ignored, next, prev );
         STRFREE( temp->name );
         DISPOSE( temp );
      }

      STRFREE( ch->pcdata->filename );
      STRFREE( ch->pcdata->deity_name );
      STRFREE( ch->pcdata->clan_name );
      STRFREE( ch->pcdata->council_name );
      STRFREE( ch->pcdata->council2_name );
      DISPOSE( ch->pcdata->pwd );   /* no hash */
      DISPOSE( ch->pcdata->bamfin );   /* no hash */
      DISPOSE( ch->pcdata->bamfout );  /* no hash */
      DISPOSE( ch->pcdata->rank );
      STRFREE( ch->pcdata->title );
      STRFREE( ch->pcdata->bio );

      /*
       * Free Character Attributes 
       */
      STRFREE( ch->pcdata->hair_color );
      STRFREE( ch->pcdata->eye_color );
      STRFREE( ch->pcdata->skin_tone );
      STRFREE( ch->pcdata->ch_build );
      STRFREE( ch->pcdata->hair_length );
      STRFREE( ch->pcdata->ch_height );

      if( ch->pcdata->hometown )
         ch->pcdata->hometown = NULL;
      if( ch->pcdata->nation )
         ch->pcdata->nation = NULL;

      if( ch->pcdata->wedto )
         STRFREE( ch->pcdata->wedto );

      if( ch->pcdata->in_progress )
         free_global_note( ch->pcdata->in_progress );

      /*
       * These were missing 
       */
      if( ch->pcdata->extraname )
         STRFREE( ch->pcdata->extraname );
      if( ch->pcdata->lastname )
         STRFREE( ch->pcdata->lastname );

      /*
       * Free Alias data 
       */
      for( count = 0; ch->pcdata->alias[count]; count++ )
      {
         DISPOSE( ch->pcdata->alias[count] );
         DISPOSE( ch->pcdata->alias_sub[count] );
      }

      if( ch->pcdata->outputprefix )
         DISPOSE( ch->pcdata->outputprefix );
      if( ch->pcdata->outputsuffix )
         DISPOSE( ch->pcdata->outputsuffix );

      for( alias = ch->pcdata->first_alias; alias; alias = alias_next )
      {
         alias_next = alias->next;
         if( alias->alias && alias->alias[0] != '\0' )
            STRFREE( alias->alias );
         if( alias->name && alias->name[0] != '\0' )
            STRFREE( alias->name );
         if( alias != NULL )
            DISPOSE( alias );
      }

      /*

       * Free the arena stuff 
       */
      if( ch->pcdata->arena )
         DISPOSE( ch->pcdata->arena );

      /*
       * Free the say history 
       */
      for( x = 0; x < 10; x++ )
         if( ch->pcdata->say_history[x] )
            DISPOSE( ch->pcdata->say_history[x] );

      DISPOSE( ch->pcdata->bestowments ); /* no hash */
      DISPOSE( ch->pcdata->homepage ); /* no hash */
      DISPOSE( ch->pcdata->email ); /* no hash */
      STRFREE( ch->pcdata->authed_by );
      STRFREE( ch->pcdata->prompt );
      STRFREE( ch->pcdata->fprompt );

      /*
       * Remove marriage data, maybe it should remove on other plr too? 
       */

      if( ch->pcdata->helled_by )
         STRFREE( ch->pcdata->helled_by );
      if( ch->pcdata->subprompt )
         STRFREE( ch->pcdata->subprompt );

      if( ch->pcdata->tell_history )
      {
         int i;
         for( i = 0; i < 26; i++ )
         {
            if( ch->pcdata->tell_history[i] )
               STRFREE( ch->pcdata->tell_history[i] );
         }
         DISPOSE( ch->pcdata->tell_history );
      }

#ifdef I3
      free_i3chardata( ch );
#endif

#ifdef IMC
      imc_freechardata( ch );
#endif

      DISPOSE( ch->pcdata );
   }

   for( mpact = ch->mpact; mpact; mpact = mpact_next )
   {
      mpact_next = mpact->next;
      DISPOSE( mpact->buf );
      DISPOSE( mpact );
   }

   for( comments = ch->comments; comments; comments = comments_next )
   {
      comments_next = comments->next;
      STRFREE( comments->text );
      STRFREE( comments->to_list );
      STRFREE( comments->subject );
      STRFREE( comments->sender );
      STRFREE( comments->date );
      DISPOSE( comments );
   }
   DISPOSE( ch );
   return;
}



/*
 * Get an extra description from a list.
 */
char *get_extra_descr( const char *name, EXTRA_DESCR_DATA * ed )
{
   for( ; ed; ed = ed->next )
      if( is_name( name, ed->keyword ) )
         return ed->description;

   return NULL;
}



/*
 * Translates mob virtual number to its mob index struct.
 * Hash table lookup.
 */
MOB_INDEX_DATA *get_mob_index( int vnum )
{
   MOB_INDEX_DATA *pMobIndex;

   if( vnum < 0 )
      vnum = 0;

   for( pMobIndex = mob_index_hash[vnum % MAX_KEY_HASH]; pMobIndex; pMobIndex = pMobIndex->next )
      if( pMobIndex->vnum == vnum )
         return pMobIndex;

   if( fBootDb )
      bug( "Get_mob_index: bad vnum %d.", vnum );

   return NULL;
}



/*
 * Translates obj virtual number to its obj index struct.
 * Hash table lookup.
 */
OBJ_INDEX_DATA *get_obj_index( int vnum )
{
   OBJ_INDEX_DATA *pObjIndex;

   if( vnum < 0 )
      vnum = 0;

   for( pObjIndex = obj_index_hash[vnum % MAX_KEY_HASH]; pObjIndex; pObjIndex = pObjIndex->next )
      if( pObjIndex->vnum == vnum )
         return pObjIndex;

   if( fBootDb )
      bug( "Get_obj_index: bad vnum %d.", vnum );

   return NULL;
}



/*
 * Translates room virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index( int vnum )
{
   ROOM_INDEX_DATA *pRoomIndex;

   if( vnum < 0 )
      vnum = 0;

   for( pRoomIndex = room_index_hash[vnum % MAX_KEY_HASH]; pRoomIndex; pRoomIndex = pRoomIndex->next )
      if( pRoomIndex->vnum == vnum )
         return pRoomIndex;

   if( fBootDb )
      bug( "Get_room_index: bad vnum %d.", vnum );

   return NULL;
}



/*
 * Added lots of EOF checks, as most of the file crashes are based on them.
 * If an area file encounters EOF, the fread_* functions will shutdown the
 * MUD, as all area files should be read in in full or bad things will
 * happen during the game.  Any files loaded in without fBootDb which
 * encounter EOF will return what they have read so far.   These files
 * should include player files, and in-progress areas that are not loaded
 * upon bootup.
 * -- Altrag
 */


/*
 * Read a letter from a file.
 */
char fread_letter( FILE * fp )
{
   char c;

   do
   {
      if( feof( fp ) )
      {
         bug( "fread_letter: EOF encountered on read.\r\n" );
         if( fBootDb )
            exit( 1 );
         return '\0';
      }
      c = getc( fp );
   }
   while( isspace( c ) );

   return c;
}



/*
 * Read a number from a file.
 */
int fread_number( FILE * fp )
{
   int number;
   bool sign;
   char c;

   do
   {
      if( feof( fp ) )
      {
         bug( "fread_number: EOF encountered on read.\r\n" );
         if( fBootDb )
            exit( 1 );
         return 0;
      }
      c = getc( fp );
   }
   while( isspace( c ) );

   number = 0;

   sign = FALSE;
   if( c == '+' )
   {
      c = getc( fp );
   }
   else if( c == '-' )
   {
      sign = TRUE;
      c = getc( fp );
   }

   if( !isdigit( c ) )
   {
      bug( "Fread_number: bad format. (%c)", c );
      if( fBootDb )
         exit( 1 );
      return 0;
   }

   while( isdigit( c ) )
   {
      if( feof( fp ) )
      {
         bug( "fread_number: EOF encountered on read.\r\n" );
         if( fBootDb )
            exit( 1 );
         return number;
      }
      number = number * 10 + c - '0';
      c = getc( fp );
   }

   if( sign )
      number = 0 - number;

   if( c == '|' )
      number += fread_number( fp );
   else if( c != ' ' )
      ungetc( c, fp );

   return number;
}

/*
 * custom str_dup using create					-Thoric
 */
char *str_dup( char const *str )
{
   static char *ret;
   int len;

   if( !str )
      return NULL;

   len = strlen( str ) + 1;

   CREATE( ret, char, len );
   strcpy( ret, str );
   return ret;
}

/* Read a string from file and return it */
char *fread_flagstring( FILE * fp )
{
   static char flagstring[MAX_STRING_LENGTH];
   char *plast;
   char c;
   int ln;

   plast = flagstring;
   flagstring[0] = '\0';
   ln = 0;
   /*
    * Skip blanks. Read first char. 
    */
   do
   {
      if( feof( fp ) )
      {
         bug( "%s: EOF encountered on read.", __FUNCTION__ );
         if( fBootDb )
            exit( 1 );
         return "";
      }
      c = getc( fp );
   }
   while( isspace( c ) );
   if( ( *plast++ = c ) == '~' )
      return "";

   for( ;; )
   {
      if( ln >= ( MAX_STRING_LENGTH - 1 ) )
      {
         bug( "%s: string too long", __FUNCTION__ );
         *plast = '\0';
         return flagstring;
      }
      switch ( *plast = getc( fp ) )
      {
         default:
            plast++;
            ln++;
            break;

         case EOF:
            bug( "%s: EOF", __FUNCTION__ );
            if( fBootDb )
               exit( 1 );
            *plast = '\0';
            return flagstring;
            break;

         case '\n':
            plast++;
            ln++;
            *plast++ = '\r';
            ln++;
            break;

         case '\r':
            break;

         case '~':
            *plast = '\0';
            return flagstring;
      }
   }
}

/*
 * Read a string from file fp
 */
char *fread_string( FILE * fp )
{
   char buf[MAX_STRING_LENGTH];
   char *plast;
   char c;
   int ln;

   plast = buf;
   buf[0] = '\0';
   ln = 0;

   /*
    * Skip blanks.
    * Read first char.
    */
   do
   {
      if( feof( fp ) )
      {
         bug( "fread_string: EOF encountered on read.\r\n" );
         if( fBootDb )
            exit( 1 );
         return STRALLOC( "" );
      }
      c = getc( fp );
   }
   while( isspace( c ) );

   if( ( *plast++ = c ) == '~' )
      return STRALLOC( "" );

   for( ;; )
   {
      if( ln >= ( MAX_STRING_LENGTH - 1 ) )
      {
         bug( "fread_string: string too long" );
         *plast = '\0';
         return STRALLOC( buf );
      }
      switch ( *plast = getc( fp ) )
      {
         default:
            plast++;
            ln++;
            break;

         case EOF:
            bug( "Fread_string: EOF" );
            if( fBootDb )
               exit( 1 );
            *plast = '\0';
            return STRALLOC( buf );
            break;

         case '\n':
            plast++;
            ln++;
            *plast++ = '\r';
            ln++;
            break;

         case '\r':
            break;

         case '~':
            *plast = '\0';
            return STRALLOC( buf );
      }
   }
}

/*
 * Read a string from file fp using str_dup (ie: no string hashing)
 */
char *fread_string_nohash( FILE * fp )
{
   char buf[MAX_STRING_LENGTH];
   char *plast;
   char c;
   int ln;

   plast = buf;
   buf[0] = '\0';
   ln = 0;

   /*
    * Skip blanks.
    * Read first char.
    */
   do
   {
      if( feof( fp ) )
      {
         bug( "fread_string_no_hash: EOF encountered on read.\r\n" );
         if( fBootDb )
            exit( 1 );
         return str_dup( "" );
      }
      c = getc( fp );
   }
   while( isspace( c ) );

   if( ( *plast++ = c ) == '~' )
      return str_dup( "" );

   for( ;; )
   {
      if( ln >= ( MAX_STRING_LENGTH - 1 ) )
      {
         bug( "fread_string_no_hash: string too long" );
         *plast = '\0';
         return str_dup( buf );
      }
      switch ( *plast = getc( fp ) )
      {
         default:
            plast++;
            ln++;
            break;

         case EOF:
            bug( "Fread_string_no_hash: EOF" );
            if( fBootDb )
               exit( 1 );
            *plast = '\0';
            return str_dup( buf );
            break;

         case '\n':
            plast++;
            ln++;
            *plast++ = '\r';
            ln++;
            break;

         case '\r':
            break;

         case '~':
            *plast = '\0';
            return str_dup( buf );
      }
   }
}



/*
 * Read to end of line (for comments).
 */
void fread_to_eol( FILE * fp )
{
   char c;

   do
   {
      if( feof( fp ) )
      {
         bug( "fread_to_eol: EOF encountered on read.\r\n" );
         if( fBootDb )
            exit( 1 );
         return;
      }
      c = getc( fp );
   }
   while( c != '\n' && c != '\r' );

   do
   {
      c = getc( fp );
   }
   while( c == '\n' || c == '\r' );

   ungetc( c, fp );
   return;
}

/*
 * Read to end of line into static buffer			-Thoric
 */
char *fread_line( FILE * fp )
{
   static char line[MAX_STRING_LENGTH];
   char *pline;
   char c;
   int ln;

   pline = line;
   line[0] = '\0';
   ln = 0;

   /*
    * Skip blanks.
    * Read first char.
    */
   do
   {
      if( feof( fp ) )
      {
         bug( "fread_line: EOF encountered on read.\r\n" );
         if( fBootDb )
            exit( 1 );
         strcpy( line, "" );
         return line;
      }
      c = getc( fp );
   }
   while( isspace( c ) );

   ungetc( c, fp );
   do
   {
      if( feof( fp ) )
      {
         bug( "fread_line: EOF encountered on read.\r\n" );
         if( fBootDb )
            exit( 1 );
         *pline = '\0';
         return line;
      }
      c = getc( fp );
      *pline++ = c;
      ln++;
      if( ln >= ( MAX_STRING_LENGTH - 1 ) )
      {
         bug( "fread_line: line too long" );
         break;
      }
   }
   while( c != '\n' && c != '\r' );

   do
   {
      c = getc( fp );
   }
   while( c == '\n' || c == '\r' );

   ungetc( c, fp );
   *pline = '\0';
   return line;
}



/*
 * Read one word (into static buffer).
 */
char *fread_word( FILE * fp )
{
   static char word[MAX_INPUT_LENGTH];
   char *pword;
   char cEnd;

   do
   {
      if( feof( fp ) )
      {
         bug( "fread_word: EOF encountered on read.\r\n" );
         if( fBootDb )
            exit( 1 );
         word[0] = '\0';
         return word;
      }
      cEnd = getc( fp );
   }
   while( isspace( cEnd ) );

   if( cEnd == '\'' || cEnd == '"' )
   {
      pword = word;
   }
   else
   {
      word[0] = cEnd;
      pword = word + 1;
      cEnd = ' ';
   }

   for( ; pword < word + MAX_INPUT_LENGTH; pword++ )
   {
      if( feof( fp ) )
      {
         bug( "fread_word: EOF encountered on read.\r\n" );
         if( fBootDb )
            exit( 1 );
         *pword = '\0';
         return word;
      }
      *pword = getc( fp );
      if( cEnd == ' ' ? isspace( *pword ) : *pword == cEnd )
      {
         if( cEnd == ' ' )
            ungetc( *pword, fp );
         *pword = '\0';
         return word;
      }
   }

   bug( "Fread_word: word too long" );
   exit( 1 );
   return NULL;
}

void do_memory( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int hash;

   set_char_color( AT_PLAIN, ch );
   argument = one_argument( argument, arg );
   send_to_char_color( "\r\n&wSystem Memory [arguments - hash, check, showhigh]\r\n", ch );
   ch_printf_color( ch, "&wAffects: &W%5d\t\t\t&wAreas:   &W%5d\r\n", top_affect, top_area );
   ch_printf_color( ch, "&wExtDes:  &W%5d\t\t\t&wExits:   &W%5d\r\n", top_ed, top_exit );
   ch_printf_color( ch, "&wHelps:   &W%5d\t\t\t&wResets:  &W%5d\r\n", top_help, top_reset );
   ch_printf_color( ch, "&wIdxMobs: &W%5d\t\t\t&wMobiles: &W%5d\r\n", top_mob_index, nummobsloaded );
   ch_printf_color( ch, "&wIdxObjs: &W%5d\t\t\t&wObjs:    &W%5d(%d)\r\n", top_obj_index, numobjsloaded, physicalobjects );
   ch_printf_color( ch, "&wRooms:   &W%5d\t\t\t&wVRooms:  &W%5d\r\n", top_room, top_vroom );
   ch_printf_color( ch, "&wShops:   &W%5d\t\t\t&wRepShps: &W%5d\r\n", top_shop, top_repair );
   ch_printf_color( ch, "&wCurOq's: &W%5d\t\t\t&wCurCq's: &W%5d\r\n", cur_qobjs, cur_qchars );
   ch_printf_color( ch, "&wSocials: &W%5d\t\t\t&wCommands:&W%5d\r\n", sysdata.scount, sysdata.ccount );
   ch_printf_color( ch, "&wPlayers: &W%5d\t\t\t&wMaxplrs: &W%5d\r\n", num_descriptors, sysdata.maxplayers );
   ch_printf_color( ch, "&wTotalplayers: &W%5d\r\n", sysdata.totalplayers );
   ch_printf_color( ch, "&wMaxEver: &W%5d\t\t\t&wTopsn:   &W%5d(%d)\r\n", sysdata.alltimemax, top_sn, MAX_SKILL );
   ch_printf_color( ch, "&wMaxEver was recorded on:  &W%s\r\n\r\n", sysdata.time_of_max );
   ch_printf_color( ch, "&wPotion Val:  &W%-16d   &wScribe/Brew: &W%d/%d\r\n",
                    sysdata.upotion_val, sysdata.scribed_used, sysdata.brewed_used );
   ch_printf_color( ch, "&wPill Val:    &W%-16d   &wGlobal gold loot: &W%ld\r\n",
                    sysdata.upill_val, sysdata.global_gold_looted );
   ch_printf_color( ch, "&wGlobal silver loot: &W%ld    &wGlobal copper loot: &W%ld\r\n",
                    sysdata.global_silver_looted, sysdata.global_copper_looted );
/* Ntanel - Codebase version ID */
   ch_printf_color( ch, "&wVersion ID:  &WResortMUD 6.0\r\n" );
   ch_printf_color( ch, "&wDev. Date:   &WFebruary 5, 2006\r\n\r\n" );

   if( !str_cmp( arg, "check" ) )
   {
#ifdef HASHSTR
      send_to_char( check_hash( argument ), ch );
#else
      send_to_char( "Hash strings not enabled.\r\n", ch );
#endif
      return;
   }
   if( !str_cmp( arg, "showhigh" ) )
   {
#ifdef HASHSTR
      show_high_hash( atoi( argument ) );
#else
      send_to_char( "Hash strings not enabled.\r\n", ch );
#endif
      return;
   }
   if( argument[0] != '\0' )
      hash = atoi( argument );
   else
      hash = -1;
   if( !str_cmp( arg, "hash" ) )
   {
#ifdef HASHSTR
      ch_printf( ch, "Hash statistics:\r\n%s", hash_stats(  ) );
      if( hash != -1 )
         hash_dump( hash );
#else
      send_to_char( "Hash strings not enabled.\r\n", ch );
#endif
   }
   return;
}

/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy( int number )
{
   switch ( number_bits( 2 ) )
   {
      case 0:
         number -= 1;
         break;
      case 3:
         number += 1;
         break;
   }

   return UMAX( 1, number );
}



/*
 * Generate a random number.
 */
int number_range( int from, int to )
{
/*    int power;
    int number;*/

   if( ( to = to - from + 1 ) <= 1 )
      return from;

/*    for ( power = 2; power < to; power <<= 1 )
	;

    while ( ( number = number_mm( ) & (power - 1) ) >= to )
	;

    return from + number;*/
   return ( number_mm(  ) % to ) + from;
}



/*
 * Generate a percentile roll.
 */
int number_percent( void )
{
/*    int percent;

    while ( ( percent = number_mm( ) & (128-1) ) > 99 )
	;

    return 1 + percent;*/
   return number_mm(  ) % 100;
}



/*
 * Generate a random door.
 */
int number_door( void )
{
   int door;

   while( ( door = number_mm(  ) & ( 16 - 1 ) ) > 9 )
      ;

   return door;
/*    return number_mm() & 10; */
}



int number_bits( int width )
{
   return number_mm(  ) & ( ( 1 << width ) - 1 );
}



/*
 * I've gotten too many bad reports on OS-supplied random number generators.
 * This is the Mitchell-Moore algorithm from Knuth Volume II.
 * Best to leave the constants alone unless you've read Knuth.
 * -- Furey
 */
static int rgiState[2 + 55];

void init_mm(  )
{
   int *piState;
   int iState;

   piState = &rgiState[2];

   piState[-2] = 55 - 55;
   piState[-1] = 55 - 24;

   piState[0] = ( ( int )current_time ) & ( ( 1 << 30 ) - 1 );
   piState[1] = 1;
   for( iState = 2; iState < 55; iState++ )
   {
      piState[iState] = ( piState[iState - 1] + piState[iState - 2] ) & ( ( 1 << 30 ) - 1 );
   }
   return;
}



int number_mm( void )
{
   int *piState;
   int iState1;
   int iState2;
   int iRand;

   piState = &rgiState[2];
   iState1 = piState[-2];
   iState2 = piState[-1];
   iRand = ( piState[iState1] + piState[iState2] ) & ( ( 1 << 30 ) - 1 );
   piState[iState1] = iRand;
   if( ++iState1 == 55 )
      iState1 = 0;
   if( ++iState2 == 55 )
      iState2 = 0;
   piState[-2] = iState1;
   piState[-1] = iState2;
   return iRand >> 6;
}



/*
 * Roll some dice.						-Thoric
 */
int dice( int number, int size )
{
   int idice;
   int sum;

   switch ( size )
   {
      case 0:
         return 0;
      case 1:
         return number;
   }

   for( idice = 0, sum = 0; idice < number; idice++ )
      sum += number_range( 1, size );

   return sum;
}



/*
 * Simple linear interpolation.
 */
int interpolate( int level, int value_00, int value_32 )
{
   return value_00 + level * ( value_32 - value_00 ) / 32;
}


/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
void smash_tilde( char *str )
{
   for( ; *str != '\0'; str++ )
      if( *str == '~' )
         *str = '-';

   return;
}

/*
 * Encodes the tildes in a string.				-Thoric
 * Used for player-entered strings that go into disk files.
 */
void hide_tilde( char *str )
{
   for( ; *str != '\0'; str++ )
      if( *str == '~' )
         *str = HIDDEN_TILDE;

   return;
}

char *show_tilde( char *str )
{
   static char buf[MAX_STRING_LENGTH];
   char *bufptr;

   bufptr = buf;
   for( ; *str != '\0'; str++, bufptr++ )
   {
      if( *str == HIDDEN_TILDE )
         *bufptr = '~';
      else
         *bufptr = *str;
   }
   *bufptr = '\0';

   return buf;
}



/*
 * Compare strings, case insensitive.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
bool str_cmp( const char *astr, const char *bstr )
{

   if( !astr )
   {
      bug( "Str_cmp: null astr." );
      if( bstr )
         bug( "str_cmp: astr: (null)  bstr: %s\n", bstr );
      return TRUE;
   }

   if( !bstr )
   {
      bug( "Str_cmp: null bstr." );
      if( astr )
         bug( "str_cmp: astr: %s  bstr: (null)\n", astr );
      return TRUE;
   }

   for( ; *astr || *bstr; astr++, bstr++ )
   {
      if( LOWER( *astr ) != LOWER( *bstr ) )
         return TRUE;
   }

   return FALSE;
}



/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix( const char *astr, const char *bstr )
{
   if( !astr )
   {
      bug( "Strn_cmp: null astr." );
      return TRUE;
   }

   if( !bstr )
   {
      bug( "Strn_cmp: null bstr." );
      return TRUE;
   }

   for( ; *astr; astr++, bstr++ )
   {
      if( LOWER( *astr ) != LOWER( *bstr ) )
         return TRUE;
   }

   return FALSE;
}



/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix( const char *astr, const char *bstr )
{
   int sstr1;
   int sstr2;
   int ichar;
   char c0;

   if( ( c0 = LOWER( astr[0] ) ) == '\0' )
      return FALSE;

   sstr1 = strlen( astr );
   sstr2 = strlen( bstr );

   for( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )
      if( c0 == LOWER( bstr[ichar] ) && !str_prefix( astr, bstr + ichar ) )
         return FALSE;

   return TRUE;
}



/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix( const char *astr, const char *bstr )
{
   int sstr1;
   int sstr2;

   sstr1 = strlen( astr );
   sstr2 = strlen( bstr );
   if( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )
      return FALSE;
   else
      return TRUE;
}



/*
 * Returns an initial-capped string.
 */
char *capitalize( const char *str )
{
   static char strcap[MAX_STRING_LENGTH];
   int i;

   for( i = 0; str[i] != '\0'; i++ )
   {
      if( i && str[i - 1] != '&' && str[i - 1] != '^' )  /* color fix - shogar */
         strcap[i] = LOWER( str[i] );
      else
         strcap[i] = str[i];
   }
   strcap[i] = '\0';
   strcap[0] = UPPER( strcap[0] );
   return strcap;
}


/*
 * Returns a lowercase string.
 */
char *strlower( const char *str )
{
   static char strlow[MAX_STRING_LENGTH];
   int i;

   for( i = 0; str[i] != '\0'; i++ )
      strlow[i] = LOWER( str[i] );
   strlow[i] = '\0';
   return strlow;
}

/*
 * Returns an uppercase string.
 */
char *strupper( const char *str )
{
   static char strup[MAX_STRING_LENGTH];
   int i;

   for( i = 0; str[i] != '\0'; i++ )
      strup[i] = UPPER( str[i] );
   strup[i] = '\0';
   return strup;
}

/*
 * Returns TRUE or FALSE if a letter is a vowel			-Thoric
 */
bool isavowel( char letter )
{
   char c;

   c = LOWER( letter );
   if( c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' )
      return TRUE;
   else
      return FALSE;
}

/*
 * Shove either "a " or "an " onto the beginning of a string	-Thoric
 */
char *aoran( const char *str )
{
   static char temp[MAX_STRING_LENGTH];

   if( !str )
   {
      bug( "Aoran(): NULL str" );
      return "";
   }

   if( isavowel( str[0] ) || ( strlen( str ) > 1 && LOWER( str[0] ) == 'y' && !isavowel( str[1] ) ) )
      strcpy( temp, "an " );
   else
      strcpy( temp, "a " );
   strcat( temp, str );
   return temp;
}


/*
 * Append a string to a file.
 */
void append_file( CHAR_DATA * ch, char *file, char *str )
{
   FILE *fp;

   if( IS_NPC( ch ) || str[0] == '\0' )
      return;

   if( ( fp = fopen( file, "a" ) ) == NULL )
   {
      perror( file );
      send_to_char( "Could not open the file!\r\n", ch );
   }
   else
   {
      fprintf( fp, "[%5d] %-10s: %s\n", ch->in_room ? ch->in_room->vnum : 0, ch->name, str );
      fclose( fp );
      fp = NULL;
   }
   return;
}

/*
 * Append a string to a file.
 */
void append_to_file( char *file, char *str )
{
   FILE *fp;

   if( ( fp = fopen( file, "a" ) ) == NULL )
      perror( file );
   else
   {
      fprintf( fp, "%s\n", str );
      fclose( fp );
   }

   return;
}


/*
 * Reports a bug.
 */
void bug( const char *str, ... )
{
   char buf[MAX_STRING_LENGTH];
   FILE *fp;
   struct stat fst;

   if( fpArea != NULL )
   {
      int iLine;
      int iChar;

      if( fpArea == stdin )
      {
         iLine = 0;
      }
      else
      {
         iChar = ftell( fpArea );
         fseek( fpArea, 0, 0 );
         for( iLine = 0; ftell( fpArea ) < iChar; iLine++ )
         {
            while( getc( fpArea ) != '\n' )
               ;
         }
         fseek( fpArea, iChar, 0 );
      }

      sprintf( buf, "[*****] FILE: %s LINE: %d", strArea, iLine );
      log_string( buf );

      if( stat( SHUTDOWN_FILE, &fst ) != -1 )   /* file exists */
      {
         if( ( fp = fopen( SHUTDOWN_FILE, "a" ) ) != NULL )
         {
            fprintf( fp, "[*****] %s\n", buf );
            fclose( fp );
            fp = NULL;  /* redhat 5.1 fix - shogar */
         }
      }
   }

   strcpy( buf, "[*****] BUG: " );
   {
      va_list param;

      va_start( param, str );
      vsprintf( buf + strlen( buf ), str, param );
      va_end( param );
   }
   log_string( buf );
   return;
}

/*
 * Add a string to the boot-up log				-Thoric
 */
void boot_log( const char *str, ... )
{
   char buf[MAX_STRING_LENGTH];
   FILE *fp;
   va_list param;

   strcpy( buf, "[*****] BOOT: " );
   va_start( param, str );
   vsprintf( buf + strlen( buf ), str, param );
   va_end( param );
   log_string( buf );

   if( ( fp = fopen( BOOTLOG_FILE, "a" ) ) != NULL )
   {
      fprintf( fp, "%s\n", buf );
      fclose( fp );
      fp = NULL;
   }
   return;
}

/*
 * Dump a text file to a player, a line at a time		-Thoric
 */
void show_file( CHAR_DATA * ch, char *filename )
{
   FILE *fp;
   char buf[MAX_STRING_LENGTH];
   int c;
   int num = 0;

   if( ( fp = fopen( filename, "r" ) ) != NULL )
   {
      while( !feof( fp ) )
      {
         while( ( buf[num] = fgetc( fp ) ) != EOF
                && buf[num] != '\n' && buf[num] != '\r' && num < ( MAX_STRING_LENGTH - 2 ) )
            num++;
         c = fgetc( fp );
         if( ( c != '\n' && c != '\r' ) || c == buf[num] )
            ungetc( c, fp );
         buf[num++] = '\n';
         buf[num++] = '\r';
         buf[num] = '\0';
         send_to_pager_color( buf, ch );
         num = 0;
      }
      /*
       * Thanks to stu <sprice@@ihug.co.nz> from the mailing list in pointing
       * *  This out. 
       */
      fclose( fp );
      fp = NULL;
   }
}

/*
 * Show the boot log file					-Thoric
 */
void do_dmesg( CHAR_DATA * ch, char *argument )
{
   set_pager_color( AT_LOG, ch );
   show_file( ch, BOOTLOG_FILE );
}

/*
 * Writes a string to the log, extended version			-Thoric
 */
void log_string_plus( const char *str, short log_type, short level )
{
   char *strtime;
   int offset;
   strtime = ctime( &current_time );
   strtime[strlen( strtime ) - 1] = '\0';

   fprintf( stderr, "%s :: %s\n", strtime, str );
   if( strncmp( str, "Log ", 4 ) == 0 )
      offset = 4;
   else
      offset = 0;
   switch ( log_type )
   {
      default:
         to_channel( str + offset, "Log", level );
         break;
      case LOG_BUILD:
         to_channel( str + offset, "Build", level );
         break;
      case LOG_COMM:
         to_channel( str + offset, "Comm", level );
         break;
      case LOG_WARN:
         to_channel( str + offset, "Warn", level );
         break;
      case LOG_ALL:
         break;
   }
   return;
}

/* Added for below function. Help Centers */
int strlen_color( const char *argument )
{
   const char *str;
   size_t i, length;

   str = argument;
   if( argument[0] == '\0' )
      return 0;

   for( length = i = 0; i < strlen( argument ); ++i )
   {
      if( ( str[i] != '&' ) && ( str[i] != '^' ) )
         ++length;
      if( ( str[i] == '&' ) || ( str[i] == '^' ) )
      {
         if( ( str[i] == '&' ) && ( str[i + 1] == '&' ) )
            length = 2 + length;
         else if( ( str[i] == '^' ) && ( str[i + 1] == '^' ) )
            length = 2 + length;
         else
            --length;
      }
   }

   return length;
}

/*
 * wizlist builder!						-Thoric
 * Modified so seperate function wouldn't be needed for guestlist - Kratas
 */
void towizfile( const char *line, bool wizlist )
{
   int filler, xx;
   char outline[MAX_STRING_LENGTH];
   FILE *wfp;

   outline[0] = '\0';

   if( line && line[0] != '\0' )
   {
      filler = ( 78 - strlen_color( line ) );
      if( filler < 1 )
         filler = 1;
      filler /= 2;
      for( xx = 0; xx < filler; xx++ )
         strcat( outline, " " );
      strcat( outline, line );
   }
   strcat( outline, "\r\n" );
   if( wizlist )
      wfp = fopen( WIZLIST_FILE, "a" );
   else
      wfp = fopen( GUESTLIST_FILE, "a" );
   fputs( outline, wfp );
   fclose( wfp );
}

void towebwiz( const char *line )
{
   char outline[MAX_STRING_LENGTH];
   FILE *wfp;

   outline[0] = '\0';

   strcat( outline, " " );
   strcat( outline, line );

   strcat( outline, "\r\n" );
   wfp = fopen( WEBWIZ_FILE, "a" );
   if( wfp )
   {
      fputs( outline, wfp );
      fclose( wfp );
   }
}

void add_to_wizlist( char *name, int level )
{
   WIZENT *wiz, *tmp;

#ifdef DEBUG
   log_string( "Adding to wizlist..." );
#endif

   CREATE( wiz, WIZENT, 1 );
   wiz->name = str_dup( name );
   wiz->level = level;

   if( !first_wiz )
   {
      wiz->last = NULL;
      wiz->next = NULL;
      first_wiz = wiz;
      last_wiz = wiz;
      return;
   }

   /*
    * insert sort, of sorts 
    */
   for( tmp = first_wiz; tmp; tmp = tmp->next )
      if( ( level > tmp->level ) )
      {
         if( !tmp->last )
            first_wiz = wiz;
         else
            tmp->last->next = wiz;
         wiz->last = tmp->last;
         wiz->next = tmp;
         tmp->last = wiz;
         return;
      }

   wiz->last = last_wiz;
   wiz->next = NULL;
   last_wiz->next = wiz;
   last_wiz = wiz;
   return;
}

/*
 * Wizlist builder						-Thoric
 * Now excludes guests -- Scion
 */
void make_wizlist(  )
{
   DIR *dp;
   struct dirent *dentry;
   FILE *gfp;
   char *word;
   int ilevel, iflags;
   WIZENT *wiz, *wiznext;
   char buf[MAX_STRING_LENGTH];

   first_wiz = NULL;
   last_wiz = NULL;

   dp = opendir( GOD_DIR );

   ilevel = 0;
   dentry = readdir( dp );

   while( dentry )   /* Enhanced By Josh */
   {
      if( dentry->d_name[0] != '.' )
      {
         char *s = strchr( dentry->d_name, '.' );
         if( s )
         {
            dentry = readdir( dp );
            continue;
         }
         sprintf( buf, "%s%s", GOD_DIR, dentry->d_name );
         gfp = fopen( buf, "r" );
         /*
          * log_string(buf);  *scream* We have too many imms/guests for this! -- Scion 
          */
         if( gfp )
         {
            word = feof( gfp ) ? "End" : fread_word( gfp );
            ilevel = fread_number( gfp );
            fread_to_eol( gfp );
            word = feof( gfp ) ? "End" : fread_word( gfp );
            if( !str_cmp( word, "Pcflags" ) )
               iflags = fread_number( gfp );
            else
               iflags = 0;
            fclose( gfp );

            if( ( ( ilevel >= 101 && ilevel <= 103 ) || ilevel == 110 ) )
            {
               dentry = readdir( dp );
               continue;
            }

            if( IS_SET( iflags, PCFLAG_RETIRED ) )
               ilevel = MAX_LEVEL - 16;
            if( IS_SET( iflags, PCFLAG_GUEST ) )
               ilevel = MAX_LEVEL - 17;

            add_to_wizlist( dentry->d_name, ilevel );
         }
      }
      dentry = readdir( dp );
   }
   closedir( dp );

   unlink( WIZLIST_FILE );

   sprintf( buf, "Immortals - Staff of %s!", sysdata.mud_name );
   towizfile( buf, TRUE );

   /*
    * towizfile( " Masters of the Realms of Despair!" );
    */
   buf[0] = '\0';
   ilevel = 65535;
   for( wiz = first_wiz; wiz; wiz = wiz->next )
   {
      if( wiz->level < ilevel )
      {
         if( buf[0] )
         {
            towizfile( buf, TRUE );
            buf[0] = '\0';
         }
         towizfile( "", TRUE );
         ilevel = wiz->level;

         switch ( ilevel )
         {
            case MAX_LEVEL - 0:
               towizfile( " &ROmnipotent &w(Level 116) &ROwner&W", TRUE );
               break;
            case MAX_LEVEL - 1:
               towizfile( " &RSupreme Entity &w(Level 115) &RHead Implementors&W", TRUE );
               break;
            case MAX_LEVEL - 2:
               towizfile( " &RInfinite &w(Level 114) &RImplementors&W", TRUE );
               break;
            case MAX_LEVEL - 3:
               towizfile( " &REternal &w(Level 113) &RAssistant Implementors&W", TRUE );
               break;
            case MAX_LEVEL - 4:
               towizfile( " &RAncient &w(Level 112) &RAdministrators&W", TRUE );
               break;
            case MAX_LEVEL - 5:
               towizfile( " &cExalted &w(Level 111) &cCoders&W", TRUE );
               break;
            case MAX_LEVEL - 6:
               towizfile( " &cAscendant &w(Level 110) &cRepresentatives&W", TRUE );
               break;
            case MAX_LEVEL - 7:
               towizfile( " &cMaster Gods &w(Level 109) &cMaster Builders&W", TRUE );
               break;
            case MAX_LEVEL - 8:
               towizfile( " &cGreater Gods &w(Level 108) &cGreater Builders&W", TRUE );
               break;
            case MAX_LEVEL - 9:
               towizfile( " &cGods &w(Level 107) &cBuilders&W", TRUE );
               break;
            case MAX_LEVEL - 10:
               towizfile( " &cLesser Gods &w(Level 106) &cLesser Builders&W", TRUE );
               break;
            case MAX_LEVEL - 11:
               towizfile( " &cNew Gods &w(Level 105) &cNew Builders&W", TRUE );
               break;
            case MAX_LEVEL - 12:
               towizfile( " &cSaviors &w(Level 104) &cGeneral Staff&W", TRUE );
               break;
            case MAX_LEVEL - 13:
               towizfile( " &BCreators &w(Level 103) &BImmortal Family&W", TRUE );
               break;
            case MAX_LEVEL - 14:
               towizfile( " &BAcolytes &w(Level 102) &BInter-MUD Guest&W", TRUE );
               break;
            case MAX_LEVEL - 15:
               towizfile( " &BNeophytes &w(Level 101) &BAmbassador&W", TRUE );
               break;
            case MAX_LEVEL - 16:
               towizfile( " &BRetired&W", TRUE );
               break;
            case MAX_LEVEL - 17:
               towizfile( " &BGuests&W", TRUE );
               break;
            default:
               towizfile( " &BServants&W", TRUE );
               break;
         }
      }

      if( strlen( buf ) + strlen( wiz->name ) > 76 )
      {
         towizfile( buf, TRUE );
         buf[0] = '\0';
      }
      strcat( buf, " " );
      strcat( buf, wiz->name );
      if( strlen( buf ) > 70 )
      {
         towizfile( buf, TRUE );
         buf[0] = '\0';
      }
   }

   if( buf[0] )
      towizfile( buf, TRUE ); /* Would it be otherwise? */

   for( wiz = first_wiz; wiz; wiz = wiznext )
   {
      wiznext = wiz->next;
      DISPOSE( wiz->name );
      DISPOSE( wiz );
   }

   towizfile( "&w", TRUE );

   first_wiz = NULL;
   last_wiz = NULL;
}

/*
 * Rip of make_wizlist(). Creates guestlist for mud, ignores other imms
 * -- Kratas
 */
void make_guestlist(  )
{
   DIR *dp;
   struct dirent *dentry;
   FILE *gfp;
   char *word;
   int ilevel, iflags;
   WIZENT *wiz, *wiznext;
   char buf[MAX_STRING_LENGTH];

   first_wiz = NULL;
   last_wiz = NULL;

   dp = opendir( GOD_DIR );

   ilevel = 0;
   dentry = readdir( dp );

   while( dentry )
   {
      if( dentry->d_name[0] != '.' )
      {
         if( strstr( dentry->d_name, "immlist" ) )
         {
            dentry = readdir( dp );
            continue;
         }
         sprintf( buf, "%s%s", GOD_DIR, dentry->d_name );
         gfp = fopen( buf, "r" );
         if( gfp )
         {
            word = feof( gfp ) ? "End" : fread_word( gfp );
            ilevel = fread_number( gfp );
            fread_to_eol( gfp );
            word = feof( gfp ) ? "End" : fread_word( gfp );
            if( !str_cmp( word, "Pcflags" ) )
               iflags = fread_number( gfp );
            else
               iflags = 0;
            fclose( gfp );

            if( ( ilevel >= 104 && ilevel <= 109 ) || ( ilevel >= 111 && ilevel <= 116 ) )
            {
               dentry = readdir( dp );
               continue;
            }

            if( IS_SET( iflags, PCFLAG_RETIRED ) )
               ilevel = MAX_LEVEL - 16;
            if( IS_SET( iflags, PCFLAG_GUEST ) )
               ilevel = MAX_LEVEL - 17;
            add_to_wizlist( dentry->d_name, ilevel );
         }
      }
      dentry = readdir( dp );
   }
   closedir( dp );

   unlink( GUESTLIST_FILE );

   sprintf( buf, "Immortals - Guests of %s!", sysdata.mud_name );
   towizfile( buf, FALSE );

   buf[0] = '\0';
   ilevel = 65535;
   for( wiz = first_wiz; wiz; wiz = wiz->next )
   {
      if( wiz->level < ilevel )
      {
         if( buf[0] )
         {
            towizfile( buf, FALSE );
            buf[0] = '\0';
         }
         towizfile( "", FALSE );
         ilevel = wiz->level;

         switch ( ilevel )
         {
            case MAX_LEVEL - 0:
               towizfile( " &ROmnipotent &w(Level 116) &ROwner&W", FALSE );
               break;
            case MAX_LEVEL - 1:
               towizfile( " &RSupreme Entity &w(Level 115) &RHead Implementors&W", FALSE );
               break;
            case MAX_LEVEL - 2:
               towizfile( " &RInfinite &w(Level 114) &RImplementors&W", FALSE );
               break;
            case MAX_LEVEL - 3:
               towizfile( " &REternal &w(Level 113) &RAssistant Implementors&W", FALSE );
               break;
            case MAX_LEVEL - 4:
               towizfile( " &RAncient &w(Level 112) &RAdministrators&W", FALSE );
               break;
            case MAX_LEVEL - 5:
               towizfile( " &cExalted &w(Level 111) &cCoders&W", FALSE );
               break;
            case MAX_LEVEL - 6:
               towizfile( " &cAscendant &w(Level 110) &cRepresentatives&W", FALSE );
               break;
            case MAX_LEVEL - 7:
               towizfile( " &cMaster Gods &w(Level 109) &cMaster Builders&W", FALSE );
               break;
            case MAX_LEVEL - 8:
               towizfile( " &cGreater Gods &w(Level 108) &cGreater Builders&W", FALSE );
               break;
            case MAX_LEVEL - 9:
               towizfile( " &cGods &w(Level 107) &cBuilders&W", FALSE );
               break;
            case MAX_LEVEL - 10:
               towizfile( " &cLesser Gods &w(Level 106) &cLesser Builders&W", FALSE );
               break;
            case MAX_LEVEL - 11:
               towizfile( " &cNew Gods &w(Level 105) &cNew Builders&W", FALSE );
               break;
            case MAX_LEVEL - 12:
               towizfile( " &cSaviors &w(Level 104) &cGeneral Staff&W", FALSE );
               break;
            case MAX_LEVEL - 13:
               towizfile( " &BCreators &w(Level 103) &BImmortal Family&W", FALSE );
               break;
            case MAX_LEVEL - 14:
               towizfile( " &BAcolytes &w(Level 102) &BInter-MUD Guest&W", FALSE );
               break;
            case MAX_LEVEL - 15:
               towizfile( " &BNeophytes &w(Level 101) &BAmbassador&W", FALSE );
               break;
            case MAX_LEVEL - 16:
               towizfile( " &BRetired&W", FALSE );
               break;
            case MAX_LEVEL - 17:
               towizfile( " &BGuests&W", FALSE );
               break;
            default:
               towizfile( " &BServants&W", FALSE );
               break;
         }
      }

      if( strlen( buf ) + strlen( wiz->name ) > 76 )
      {
         towizfile( buf, FALSE );
         buf[0] = '\0';
      }
      strcat( buf, wiz->name );
      strcat( buf, " " );
      if( strlen( buf ) > 70 )
      {
         towizfile( buf, FALSE );
         buf[0] = '\0';
      }
   }

   if( buf[0] )
      towizfile( buf, FALSE );   /* Would it be otherwise? */

   for( wiz = first_wiz; wiz; wiz = wiznext )
   {
      wiznext = wiz->next;
      DISPOSE( wiz->name );
      DISPOSE( wiz );
   }

   towizfile( "&w", FALSE );

   first_wiz = NULL;
   last_wiz = NULL;
}

/*
 *	Makes a wizlist for showing on the Telnet Interface WWW Site -- KCAH
 */
void make_webwiz(  )
{
   DIR *dp;
   struct dirent *dentry;
   FILE *gfp;
   char *word;
   int ilevel, iflags;
   WIZENT *wiz, *wiznext;
   char buf[MAX_STRING_LENGTH];

   first_wiz = NULL;
   last_wiz = NULL;

   dp = opendir( GOD_DIR );

   ilevel = 0;
   dentry = readdir( dp );
   while( dentry )
   {
      if( dentry->d_name[0] != '.' )
      {
         if( strstr( dentry->d_name, "immlist" ) )
         {
            dentry = readdir( dp );
            continue;
         }

         sprintf( buf, "%s%s", GOD_DIR, dentry->d_name );
         gfp = fopen( buf, "r" );
         if( gfp )
         {
            word = feof( gfp ) ? "End" : fread_word( gfp );
            ilevel = fread_number( gfp );
            fread_to_eol( gfp );
            word = feof( gfp ) ? "End" : fread_word( gfp );
            if( !str_cmp( word, "Pcflags" ) )
               iflags = fread_number( gfp );
            else
               iflags = 0;
            fclose( gfp );
            if( IS_SET( iflags, PCFLAG_RETIRED ) )
               ilevel = MAX_LEVEL - 15;
            if( IS_SET( iflags, PCFLAG_GUEST ) )
               ilevel = MAX_LEVEL - 16;
            add_to_wizlist( dentry->d_name, ilevel );
         }
      }
      dentry = readdir( dp );
   }
   closedir( dp );

   unlink( WEBWIZ_FILE );

   sprintf( buf, "<BR>&YImmortals - Staff of %s!", sysdata.mud_name );
   towebwiz( buf );
   buf[0] = '\0';
   ilevel = 65535;
   for( wiz = first_wiz; wiz; wiz = wiz->next )
   {
      if( wiz->level < ilevel )
      {
         if( buf[0] )
         {
            towebwiz( buf );
            buf[0] = '\0';
         }
         towebwiz( "" );
         ilevel = wiz->level;
         switch ( ilevel )
         {
            case MAX_LEVEL - 0:
               towebwiz( "<P>&ROmnipotent &w(Level 116) &ROwner<BR>&W" );
               break;
            case MAX_LEVEL - 1:
               towebwiz( "<P>&RSupreme Entity &w(Level 115) &RHead Implementors<BR>&W" );
               break;
            case MAX_LEVEL - 2:
               towebwiz( "<P>&RInfinite &w(Level 114) &RImplementors<BR>&W" );
               break;
            case MAX_LEVEL - 3:
               towebwiz( "<P>&REternal &w(Level 113) &RAssistant Implementors<BR>&W" );
               break;
            case MAX_LEVEL - 4:
               towebwiz( "<P>&RAncient &w(Level 112) &RAdministrators<BR>&W" );
               break;
            case MAX_LEVEL - 5:
               towebwiz( "<P>&cExalted &w(Level 111) &cCoders<BR>&W" );
               break;
            case MAX_LEVEL - 6:
               towebwiz( "<P>&cAscendant &w(Level 110) &cRepresentatives<BR>&W" );
               break;
            case MAX_LEVEL - 7:
               towebwiz( "<P>&cMaster Gods &w(Level 109) &cMaster Builders<BR>&W" );
               break;
            case MAX_LEVEL - 8:
               towebwiz( "<P>&cGreater Gods &w(Level 108) &cGreater Builders<BR>&W" );
               break;
            case MAX_LEVEL - 9:
               towebwiz( "<P>&cGods &w(Level 107) &cBuilders<BR>&W" );
               break;
            case MAX_LEVEL - 10:
               towebwiz( "<P>&cLesser Gods &w(Level 106) &cLesser Builders<BR>&W" );
               break;
            case MAX_LEVEL - 11:
               towebwiz( "<P>&cNew Gods &w(Level 105) &cNew Builders<BR>&W" );
               break;
            case MAX_LEVEL - 12:
               towebwiz( "<P>&cSaviors &w(Level 104) &cGeneral Staff<BR>&W" );
               break;
            case MAX_LEVEL - 13:
               towebwiz( "<P>&BCreators &w(Level 103) &BImmortal Family<BR>&W" );
               break;
            case MAX_LEVEL - 14:
               towebwiz( "<P>&BAcolytes &w(Level 102) &BInter-MUD Guest<BR>&W" );
               break;
            case MAX_LEVEL - 15:
               towebwiz( "<P>&BNeophytes &w(Level 101) &BAmbassador<BR>&W" );
               break;
            case MAX_LEVEL - 16:
               towebwiz( "<P>&BRetired<BR>&W" );
               break;
            case MAX_LEVEL - 17:
               towebwiz( "<P>&BGuests<BR>&W" );
               break;
            default:
               towebwiz( "<P>&BServants<BR>&W" );
               break;
         }
      }

      if( strlen( buf ) + strlen( wiz->name ) > 999 )
      {
         towebwiz( buf );
         buf[0] = '\0';
      }

      strcat( buf, " " );
      strcat( buf, wiz->name );

      if( strlen( buf ) > 999 )
      {
         towebwiz( buf );
         buf[0] = '\0';
      }

   }

   if( buf[0] )
      towebwiz( buf );

   for( wiz = first_wiz; wiz; wiz = wiznext )
   {
      wiznext = wiz->next;
      DISPOSE( wiz->name );
      DISPOSE( wiz );
   }
   first_wiz = NULL;
   last_wiz = NULL;
}

void do_makewizlist( CHAR_DATA * ch, char *argument )
{
   WAIT_STATE( ch, 8 );
   make_wizlist(  );
   make_webwiz(  );
   make_guestlist(  );
   make_immlist(  );
   make_immweb(  );
}

/* mud prog functions */
/* This routine reads in scripts of MUDprograms from a file */
int mprog_name_to_type( char *name )
{
   if( !str_cmp( name, "in_file_prog" ) )
      return IN_FILE_PROG;
   if( !str_cmp( name, "act_prog" ) )
      return ACT_PROG;
   if( !str_cmp( name, "speech_prog" ) )
      return SPEECH_PROG;
   if( !str_cmp( name, "rand_prog" ) )
      return RAND_PROG;
   if( !str_cmp( name, "fight_prog" ) )
      return FIGHT_PROG;
   if( !str_cmp( name, "hitprcnt_prog" ) )
      return HITPRCNT_PROG;
   if( !str_cmp( name, "death_prog" ) )
      return DEATH_PROG;
   if( !str_cmp( name, "entry_prog" ) )
      return ENTRY_PROG;
   if( !str_cmp( name, "greet_prog" ) )
      return GREET_PROG;
   if( !str_cmp( name, "all_greet_prog" ) )
      return ALL_GREET_PROG;
   if( !str_cmp( name, "give_prog" ) )
      return GIVE_PROG;
   if( !str_cmp( name, "bribe_prog" ) )
      return BRIBE_PROG;
   if( !str_cmp( name, "time_prog" ) )
      return TIME_PROG;
   if( !str_cmp( name, "hour_prog" ) )
      return HOUR_PROG;
   if( !str_cmp( name, "wear_prog" ) )
      return WEAR_PROG;
   if( !str_cmp( name, "remove_prog" ) )
      return REMOVE_PROG;
   if( !str_cmp( name, "sac_prog" ) )
      return SAC_PROG;
   if( !str_cmp( name, "look_prog" ) )
      return LOOK_PROG;
   if( !str_cmp( name, "exa_prog" ) )
      return EXA_PROG;
   if( !str_cmp( name, "zap_prog" ) )
      return ZAP_PROG;
   if( !str_cmp( name, "get_prog" ) )
      return GET_PROG;
   if( !str_cmp( name, "drop_prog" ) )
      return DROP_PROG;
   if( !str_cmp( name, "damage_prog" ) )
      return DAMAGE_PROG;
   if( !str_cmp( name, "repair_prog" ) )
      return REPAIR_PROG;
   if( !str_cmp( name, "greet_prog" ) )
      return GREET_PROG;
   if( !str_cmp( name, "randiw_prog" ) )
      return RANDIW_PROG;
   if( !str_cmp( name, "speechiw_prog" ) )
      return SPEECHIW_PROG;
   if( !str_cmp( name, "pull_prog" ) )
      return PULL_PROG;
   if( !str_cmp( name, "push_prog" ) )
      return PUSH_PROG;
   if( !str_cmp( name, "sleep_prog" ) )
      return SLEEP_PROG;
   if( !str_cmp( name, "rest_prog" ) )
      return REST_PROG;
   if( !str_cmp( name, "rfight_prog" ) )
      return FIGHT_PROG;
   if( !str_cmp( name, "enter_prog" ) )
      return ENTRY_PROG;
   if( !str_cmp( name, "leave_prog" ) )
      return LEAVE_PROG;
   if( !str_cmp( name, "rdeath_prog" ) )
      return DEATH_PROG;
   if( !str_cmp( name, "script_prog" ) )
      return SCRIPT_PROG;
   if( !str_cmp( name, "use_prog" ) )
      return USE_PROG;
   if( !str_cmp( name, "bribe_silver_prog" ) )
      return BRIBE_SILVER_PROG;
   if( !str_cmp( name, "bribe_copper_prog" ) )
      return BRIBE_COPPER_PROG;

   return ( ERROR_PROG );
}

void mobprog_file_read( MOB_INDEX_DATA * mob, char *f )
{
   MPROG_DATA *mprg = NULL;
   char MUDProgfile[256];
   FILE *progfile;
   char letter;

   snprintf( MUDProgfile, 256, "%s%s", PROG_DIR, f );

   if( !( progfile = fopen( MUDProgfile, "r" ) ) )
   {
      bug( "%s: couldn't open mudprog file", __FUNCTION__ );
      return;
   }

   for( ;; )
   {
      letter = fread_letter( progfile );

      if( letter == '|' )
         break;

      if( letter != '>' )
      {
         bug( "%s: MUDPROG char", __FUNCTION__ );
         break;
      }

      CREATE( mprg, MPROG_DATA, 1 );
      mprg->type = mprog_name_to_type( fread_word( progfile ) );
      switch ( mprg->type )
      {
         case ERROR_PROG:
            bug( "%s: mudprog file type error", __FUNCTION__ );
            DISPOSE( mprg );
            continue;

         case IN_FILE_PROG:
            bug( "%s: Nested file programs are not allowed.", __FUNCTION__ );
            DISPOSE( mprg );
            continue;

         default:
            mprg->arglist = fread_string( progfile );
            mprg->comlist = fread_string( progfile );
            mprg->fileprog = TRUE;
            xSET_BIT( mob->progtypes, mprg->type );
            mprg->next = mob->mudprogs;
            mob->mudprogs = mprg;
            break;
      }
   }
   fclose( progfile );
   progfile = NULL;
   return;
}

/* This procedure is responsible for reading any in_file MUDprograms.
 */

void mprog_read_programs( FILE * fp, MOB_INDEX_DATA * mob )
{
   MPROG_DATA *mprg;
   char letter;
   char *word;

   for( ;; )
   {
      letter = fread_letter( fp );

      if( letter == '|' )
         return;

      if( letter != '>' )
      {
         bug( "%s: vnum %d MUDPROG char", __FUNCTION__, mob->vnum );
         exit( 1 );
      }
      CREATE( mprg, MPROG_DATA, 1 );
      mprg->next = mob->mudprogs;
      mob->mudprogs = mprg;

      word = fread_word( fp );
      mprg->type = mprog_name_to_type( word );

      switch ( mprg->type )
      {
         case ERROR_PROG:
            bug( "%s: vnum %d MUDPROG type.", __FUNCTION__, mob->vnum );
            exit( 1 );

         case IN_FILE_PROG:
            mprg->arglist = fread_string( fp );
            mprg->fileprog = FALSE;
            mobprog_file_read( mob, mprg->arglist );
            break;

         default:
            xSET_BIT( mob->progtypes, mprg->type );
            mprg->fileprog = FALSE;
            mprg->arglist = fread_string( fp );
            mprg->comlist = fread_string( fp );
            break;
      }
   }
   return;
}

void objprog_file_read( OBJ_INDEX_DATA * obj, char *f )
{
   MPROG_DATA *mprg = NULL;
   char MUDProgfile[256];
   FILE *progfile;
   char letter;

   snprintf( MUDProgfile, 256, "%s%s", PROG_DIR, f );

   if( !( progfile = fopen( MUDProgfile, "r" ) ) )
   {
      bug( "%s: couldn't open mudprog file", __FUNCTION__ );
      return;
   }

   for( ;; )
   {
      letter = fread_letter( progfile );

      if( letter == '|' )
         break;

      if( letter != '>' )
      {
         bug( "%s: MUDPROG char", __FUNCTION__ );
         break;
      }

      CREATE( mprg, MPROG_DATA, 1 );
      mprg->type = mprog_name_to_type( fread_word( progfile ) );
      switch ( mprg->type )
      {
         case ERROR_PROG:
            bug( "%s: mudprog file type error", __FUNCTION__ );
            DISPOSE( mprg );
            continue;

         case IN_FILE_PROG:
            bug( "%s: Nested file programs are not allowed.", __FUNCTION__ );
            DISPOSE( mprg );
            continue;

         default:
            mprg->arglist = fread_string( progfile );
            mprg->comlist = fread_string( progfile );
            mprg->fileprog = TRUE;
            xSET_BIT( obj->progtypes, mprg->type );
            mprg->next = obj->mudprogs;
            obj->mudprogs = mprg;
            break;
      }
   }
   fclose( progfile );
   progfile = NULL;
   return;
}

/* This procedure is responsible for reading any in_file OBJprograms.
 */

void oprog_read_programs( FILE * fp, OBJ_INDEX_DATA * obj )
{
   MPROG_DATA *mprg;
   char letter;
   char *word;

   for( ;; )
   {
      letter = fread_letter( fp );

      if( letter == '|' )
         return;

      if( letter != '>' )
      {
         bug( "%s: vnum %d MUDPROG char", __FUNCTION__, obj->vnum );
         exit( 1 );
      }
      CREATE( mprg, MPROG_DATA, 1 );
      mprg->next = obj->mudprogs;
      obj->mudprogs = mprg;

      word = fread_word( fp );
      mprg->type = mprog_name_to_type( word );

      switch ( mprg->type )
      {
         case ERROR_PROG:
            bug( "%s: vnum %d MUDPROG type.", __FUNCTION__, obj->vnum );
            exit( 1 );

         case IN_FILE_PROG:
            mprg->arglist = fread_string( fp );
            mprg->fileprog = FALSE;
            objprog_file_read( obj, mprg->arglist );
            break;

         default:
            xSET_BIT( obj->progtypes, mprg->type );
            mprg->fileprog = FALSE;
            mprg->arglist = fread_string( fp );
            mprg->comlist = fread_string( fp );
            break;
      }
   }
   return;
}

void roomprog_file_read( ROOM_INDEX_DATA * room, char *f )
{
   MPROG_DATA *mprg = NULL;
   char MUDProgfile[256];
   FILE *progfile;
   char letter;

   snprintf( MUDProgfile, 256, "%s%s", PROG_DIR, f );

   if( !( progfile = fopen( MUDProgfile, "r" ) ) )
   {
      bug( "%s: couldn't open mudprog file", __FUNCTION__ );
      return;
   }

   for( ;; )
   {
      letter = fread_letter( progfile );

      if( letter == '|' )
         break;

      if( letter != '>' )
      {
         bug( "%s: MUDPROG char", __FUNCTION__ );
         break;
      }

      CREATE( mprg, MPROG_DATA, 1 );
      mprg->type = mprog_name_to_type( fread_word( progfile ) );
      switch ( mprg->type )
      {
         case ERROR_PROG:
            bug( "%s: mudprog file type error", __FUNCTION__ );
            DISPOSE( mprg );
            continue;

         case IN_FILE_PROG:
            bug( "%s: Nested file programs are not allowed.", __FUNCTION__ );
            DISPOSE( mprg );
            continue;

         default:
            mprg->arglist = fread_string( progfile );
            mprg->comlist = fread_string( progfile );
            mprg->fileprog = TRUE;
            xSET_BIT( room->progtypes, mprg->type );
            mprg->next = room->mudprogs;
            room->mudprogs = mprg;
            break;
      }
   }
   fclose( progfile );
   progfile = NULL;
   return;
}

/* This procedure is responsible for reading any in_file ROOMprograms.
 */

void rprog_read_programs( FILE * fp, ROOM_INDEX_DATA * room )
{
   MPROG_DATA *mprg;
   char letter;
   char *word;

   for( ;; )
   {
      letter = fread_letter( fp );

      if( letter == '|' )
         return;

      if( letter != '>' )
      {
         bug( "%s: vnum %d MUDPROG char", __FUNCTION__, room->vnum );
         exit( 1 );
      }
      CREATE( mprg, MPROG_DATA, 1 );
      mprg->next = room->mudprogs;
      room->mudprogs = mprg;

      word = fread_word( fp );
      mprg->type = mprog_name_to_type( word );

      switch ( mprg->type )
      {
         case ERROR_PROG:
            bug( "%s: vnum %d MUDPROG type.", __FUNCTION__, room->vnum );
            exit( 1 );

         case IN_FILE_PROG:
            mprg->arglist = fread_string( fp );
            mprg->fileprog = FALSE;
            roomprog_file_read( room, mprg->arglist );
            break;

         default:
            xSET_BIT( room->progtypes, mprg->type );
            mprg->fileprog = FALSE;
            mprg->arglist = fread_string( fp );
            mprg->comlist = fread_string( fp );
            break;
      }
   }
   return;
}

/*************************************************************/
/* Function to delete a room index.  Called from do_rdelete in build.c
   Narn, May/96
   Don't ask me why they return bool.. :).. oh well.. -- Alty
   Don't ask me either, so I changed it to void. - Samson
*/
void delete_room( ROOM_INDEX_DATA * room )
{
   int hash;
   ROOM_INDEX_DATA *prev, *limbo = get_room_index( ROOM_VNUM_LIMBO );
   OBJ_DATA *o;
   CHAR_DATA *ch;
   EXTRA_DESCR_DATA *ed;
   EXIT_DATA *ex;
   MPROG_ACT_LIST *mpact;
   MPROG_DATA *mp;

   UNLINK( room, room->area->first_room, room->area->last_room, next_aroom, prev_aroom );

   while( ( ch = room->first_person ) != NULL )
   {
      if( !IS_NPC( ch ) )
      {
         char_from_room( ch );
         char_to_room( ch, limbo );
      }
      else
         extract_char( ch, TRUE );
   }

   while( ( o = room->first_content ) != NULL )
      extract_obj( o );

   wipe_resets( room );

   while( ( ed = room->first_extradesc ) != NULL )
   {
      room->first_extradesc = ed->next;
      STRFREE( ed->keyword );
      STRFREE( ed->description );
      DISPOSE( ed );
      --top_ed;
   }

   while( ( ex = room->first_exit ) != NULL )
      extract_exit( room, ex );

   while( ( mpact = room->mpact ) != NULL )
   {
      room->mpact = mpact->next;
      DISPOSE( mpact->buf );
      DISPOSE( mpact );
   }

   while( ( mp = room->mudprogs ) != NULL )
   {
      room->mudprogs = mp->next;
      STRFREE( mp->arglist );
      STRFREE( mp->comlist );
      DISPOSE( mp );
   }

   STRFREE( room->name );
   STRFREE( room->description );

   hash = room->vnum % MAX_KEY_HASH;
   if( room == room_index_hash[hash] )
      room_index_hash[hash] = room->next;
   else
   {
      for( prev = room_index_hash[hash]; prev; prev = prev->next )
         if( prev->next == room )
            break;
      if( prev )
         prev->next = room->next;
      else
         bug( "delete_room: room %d not in hash bucket %d.", room->vnum, hash );
   }
   DISPOSE( room );
   --top_room;
   return;
}

/* See comment on delete_room. */
void delete_obj( OBJ_INDEX_DATA * obj )
{
   int hash;
   OBJ_INDEX_DATA *prev;
   OBJ_DATA *o, *o_next;
   EXTRA_DESCR_DATA *ed;
   AFFECT_DATA *af;
   MPROG_DATA *mp;

   /*
    * Remove references to object index 
    */
   for( o = first_object; o; o = o_next )
   {
      o_next = o->next;
      if( o->pIndexData == obj )
         extract_obj( o );
   }
   while( ( ed = obj->first_extradesc ) != NULL )
   {
      obj->first_extradesc = ed->next;
      STRFREE( ed->keyword );
      STRFREE( ed->description );
      DISPOSE( ed );
      --top_ed;
   }
   while( ( af = obj->first_affect ) != NULL )
   {
      obj->first_affect = af->next;
      DISPOSE( af );
      --top_affect;
   }
   while( ( mp = obj->mudprogs ) != NULL )
   {
      obj->mudprogs = mp->next;
      STRFREE( mp->arglist );
      STRFREE( mp->comlist );
      DISPOSE( mp );
   }
   STRFREE( obj->name );
   STRFREE( obj->short_descr );
   STRFREE( obj->description );
   STRFREE( obj->action_desc );
   hash = obj->vnum % MAX_KEY_HASH;
   if( obj == obj_index_hash[hash] )
      obj_index_hash[hash] = obj->next;
   else
   {
      for( prev = obj_index_hash[hash]; prev; prev = prev->next )
         if( prev->next == obj )
            break;
      if( prev )
         prev->next = obj->next;
      else
         bug( "delete_obj: object %d not in hash bucket %d.", obj->vnum, hash );
   }
   DISPOSE( obj );
   --top_obj_index;
   return;
}

/* See comment on delete_room. */
void delete_mob( MOB_INDEX_DATA * mob )
{
   int hash;
   MOB_INDEX_DATA *prev;
   CHAR_DATA *ch, *ch_next;
   MPROG_DATA *mp;
   for( ch = first_char; ch; ch = ch_next )
   {
      ch_next = ch->next;
      if( ch->pIndexData == mob )
         extract_char( ch, TRUE );
   }
   while( ( mp = mob->mudprogs ) != NULL )
   {
      mob->mudprogs = mp->next;
      STRFREE( mp->arglist );
      STRFREE( mp->comlist );
      DISPOSE( mp );
   }
   if( mob->pShop )
   {
      UNLINK( mob->pShop, first_shop, last_shop, next, prev );
      DISPOSE( mob->pShop );
      --top_shop;
   }
   if( mob->rShop )
   {
      UNLINK( mob->rShop, first_repair, last_repair, next, prev );
      DISPOSE( mob->rShop );
      --top_repair;
   }
   STRFREE( mob->player_name );
   STRFREE( mob->short_descr );
   STRFREE( mob->long_descr );
   STRFREE( mob->description );
   hash = mob->vnum % MAX_KEY_HASH;
   if( mob == mob_index_hash[hash] )
      mob_index_hash[hash] = mob->next;
   else
   {
      for( prev = mob_index_hash[hash]; prev; prev = prev->next )
         if( prev->next == mob )
            break;
      if( prev )
         prev->next = mob->next;
      else
         bug( "delete_mob: mobile %d not in hash bucket %d.", mob->vnum, hash );
   }
   DISPOSE( mob );
   --top_mob_index;
   return;
}

/*
 * Creat a new room (for online building)			-Thoric
 */
ROOM_INDEX_DATA *make_room( int vnum, AREA_DATA * area )
{
   ROOM_INDEX_DATA *pRoomIndex;
   int iHash;

   CREATE( pRoomIndex, ROOM_INDEX_DATA, 1 );
   pRoomIndex->first_person = NULL;
   pRoomIndex->last_person = NULL;
   pRoomIndex->first_content = NULL;
   pRoomIndex->last_content = NULL;
   pRoomIndex->first_reset = pRoomIndex->last_reset = NULL;
   pRoomIndex->first_extradesc = NULL;
   pRoomIndex->last_extradesc = NULL;
   pRoomIndex->area = area;
   pRoomIndex->vnum = vnum;
   pRoomIndex->name = STRALLOC( "Floating in a void" );
   pRoomIndex->description = STRALLOC( "" );
   pRoomIndex->room_flags = ROOM_PROTOTYPE;
   pRoomIndex->sector_type = 1;
   pRoomIndex->light = 0;
   pRoomIndex->first_exit = NULL;
   pRoomIndex->last_exit = NULL;
   LINK( pRoomIndex, area->first_room, area->last_room, next_aroom, prev_aroom );

   iHash = vnum % MAX_KEY_HASH;
   pRoomIndex->next = room_index_hash[iHash];
   room_index_hash[iHash] = pRoomIndex;
   top_room++;

   return pRoomIndex;
}

/*
 * Create a new INDEX object (for online building)		-Thoric
 * Option to clone an existing index object.
 */
OBJ_INDEX_DATA *make_object( int vnum, int cvnum, char *name )
{
   OBJ_INDEX_DATA *pObjIndex, *cObjIndex;
   char buf[MAX_STRING_LENGTH];
   int iHash;

   if( cvnum > 0 )
      cObjIndex = get_obj_index( cvnum );
   else
      cObjIndex = NULL;
   CREATE( pObjIndex, OBJ_INDEX_DATA, 1 );
   pObjIndex->vnum = vnum;
   pObjIndex->name = STRALLOC( name );
   pObjIndex->first_affect = NULL;
   pObjIndex->last_affect = NULL;
   pObjIndex->first_extradesc = NULL;
   pObjIndex->last_extradesc = NULL;
   if( !cObjIndex )
   {
      sprintf( buf, "A newly created %s", name );
      pObjIndex->short_descr = STRALLOC( buf );
      sprintf( buf, "Some god dropped a newly created %s here.", name );
      pObjIndex->description = STRALLOC( buf );
      pObjIndex->action_desc = STRALLOC( "" );
      pObjIndex->short_descr[0] = LOWER( pObjIndex->short_descr[0] );
      pObjIndex->description[0] = UPPER( pObjIndex->description[0] );
      pObjIndex->item_type = ITEM_TRASH;
      xCLEAR_BITS( pObjIndex->extra_flags );
      xSET_BIT( pObjIndex->extra_flags, ITEM_PROTOTYPE );
      pObjIndex->wear_flags = 0;
      pObjIndex->value[0] = 0;
      pObjIndex->value[1] = 0;
      pObjIndex->value[2] = 0;
      pObjIndex->value[3] = 0;
      pObjIndex->value[4] = 0;
      pObjIndex->value[5] = 0;
      pObjIndex->weight = 1;
      pObjIndex->gold_cost = 0;
      pObjIndex->silver_cost = 0;
      pObjIndex->copper_cost = 0;
   }
   else
   {
      EXTRA_DESCR_DATA *ed, *ced;
      AFFECT_DATA *paf, *cpaf;

      pObjIndex->short_descr = QUICKLINK( cObjIndex->short_descr );
      pObjIndex->description = QUICKLINK( cObjIndex->description );
      pObjIndex->action_desc = QUICKLINK( cObjIndex->action_desc );
      pObjIndex->item_type = cObjIndex->item_type;
      pObjIndex->extra_flags = cObjIndex->extra_flags;
      xSET_BIT( pObjIndex->extra_flags, ITEM_PROTOTYPE );
      pObjIndex->wear_flags = cObjIndex->wear_flags;
      pObjIndex->value[0] = cObjIndex->value[0];
      pObjIndex->value[1] = cObjIndex->value[1];
      pObjIndex->value[2] = cObjIndex->value[2];
      pObjIndex->value[3] = cObjIndex->value[3];
      pObjIndex->value[4] = cObjIndex->value[4];
      pObjIndex->value[5] = cObjIndex->value[5];
      pObjIndex->weight = cObjIndex->weight;
      pObjIndex->gold_cost = cObjIndex->gold_cost;
      pObjIndex->silver_cost = cObjIndex->silver_cost;
      pObjIndex->copper_cost = cObjIndex->copper_cost;
      for( ced = cObjIndex->first_extradesc; ced; ced = ced->next )
      {
         CREATE( ed, EXTRA_DESCR_DATA, 1 );
         ed->keyword = QUICKLINK( ced->keyword );
         ed->description = QUICKLINK( ced->description );
         LINK( ed, pObjIndex->first_extradesc, pObjIndex->last_extradesc, next, prev );
         top_ed++;
      }
      for( cpaf = cObjIndex->first_affect; cpaf; cpaf = cpaf->next )
      {
         CREATE( paf, AFFECT_DATA, 1 );
         paf->type = cpaf->type;
         paf->duration = cpaf->duration;
         paf->location = cpaf->location;
         paf->modifier = cpaf->modifier;
         paf->bitvector = cpaf->bitvector;
         LINK( paf, pObjIndex->first_affect, pObjIndex->last_affect, next, prev );
         top_affect++;
      }
   }
   pObjIndex->count = 0;
   iHash = vnum % MAX_KEY_HASH;
   pObjIndex->next = obj_index_hash[iHash];
   obj_index_hash[iHash] = pObjIndex;
   top_obj_index++;

   return pObjIndex;
}

/*
 * Create a new INDEX mobile (for online building)		-Thoric
 * Option to clone an existing index mobile.
 */
MOB_INDEX_DATA *make_mobile( int vnum, int cvnum, char *name )
{
   MOB_INDEX_DATA *pMobIndex, *cMobIndex;
   char buf[MAX_STRING_LENGTH];
   int iHash;

   if( cvnum > 0 )
      cMobIndex = get_mob_index( cvnum );
   else
      cMobIndex = NULL;
   CREATE( pMobIndex, MOB_INDEX_DATA, 1 );
   pMobIndex->vnum = vnum;
   pMobIndex->count = 0;
   pMobIndex->killed = 0;
   pMobIndex->player_name = STRALLOC( name );
   if( !cMobIndex )
   {
      sprintf( buf, "A newly created %s", name );
      pMobIndex->short_descr = STRALLOC( buf );
      sprintf( buf, "Some god abandoned a newly created %s here.\r\n", name );
      pMobIndex->long_descr = STRALLOC( buf );
      pMobIndex->description = STRALLOC( "" );
      pMobIndex->short_descr[0] = LOWER( pMobIndex->short_descr[0] );
      pMobIndex->long_descr[0] = UPPER( pMobIndex->long_descr[0] );
      pMobIndex->description[0] = UPPER( pMobIndex->description[0] );
      xCLEAR_BITS( pMobIndex->act );
      xSET_BIT( pMobIndex->act, ACT_IS_NPC );
      xSET_BIT( pMobIndex->act, ACT_PROTOTYPE );
      xCLEAR_BITS( pMobIndex->affected_by );
      pMobIndex->pShop = NULL;
      pMobIndex->rShop = NULL;
      pMobIndex->spec_fun = NULL;
      pMobIndex->mudprogs = NULL;
      xCLEAR_BITS( pMobIndex->progtypes );
      pMobIndex->alignment = 0;
      pMobIndex->level = 1;
      pMobIndex->mobthac0 = 0;
      pMobIndex->ac = 0;
      pMobIndex->hitnodice = 0;
      pMobIndex->hitsizedice = 0;
      pMobIndex->hitplus = 0;
      pMobIndex->damnodice = 0;
      pMobIndex->damsizedice = 0;
      pMobIndex->damplus = 0;
      pMobIndex->gold = 0;
      pMobIndex->silver = 0;
      pMobIndex->copper = 0;
      pMobIndex->exp = 0;
/* changed this to reflect the new positions, position 8 is now sitting
- shogar
	  pMobIndex->position		= 12;
	  pMobIndex->defposition	= 12;
*/
      pMobIndex->position = POS_STANDING;
      pMobIndex->defposition = POS_STANDING;
      pMobIndex->sex = 0;
      pMobIndex->perm_str = 13;
      pMobIndex->perm_dex = 13;
      pMobIndex->perm_int = 13;
      pMobIndex->perm_wis = 13;
      pMobIndex->perm_cha = 13;
      pMobIndex->perm_con = 13;
      pMobIndex->perm_lck = 13;
      pMobIndex->race = 0;
      pMobIndex->class = 3;
      pMobIndex->xflags = 0;
      pMobIndex->resistant = 0;
      pMobIndex->immune = 0;
      pMobIndex->susceptible = 0;
      pMobIndex->numattacks = 0;
      pMobIndex->speaks = LANG_COMMON;
      pMobIndex->speaking = LANG_COMMON;

      xCLEAR_BITS( pMobIndex->attacks );
      xCLEAR_BITS( pMobIndex->defenses );
   }
   else
   {
      pMobIndex->short_descr = QUICKLINK( cMobIndex->short_descr );
      pMobIndex->long_descr = QUICKLINK( cMobIndex->long_descr );
      pMobIndex->description = QUICKLINK( cMobIndex->description );
      pMobIndex->act = cMobIndex->act;
      xSET_BIT( pMobIndex->act, ACT_PROTOTYPE );
      pMobIndex->affected_by = cMobIndex->affected_by;
      pMobIndex->pShop = NULL;
      pMobIndex->rShop = NULL;
      pMobIndex->spec_fun = cMobIndex->spec_fun;
      pMobIndex->mudprogs = NULL;
      xCLEAR_BITS( pMobIndex->progtypes );
      pMobIndex->alignment = cMobIndex->alignment;
      pMobIndex->level = cMobIndex->level;
      pMobIndex->mobthac0 = cMobIndex->mobthac0;
      pMobIndex->ac = cMobIndex->ac;
      pMobIndex->hitnodice = cMobIndex->hitnodice;
      pMobIndex->hitsizedice = cMobIndex->hitsizedice;
      pMobIndex->hitplus = cMobIndex->hitplus;
      pMobIndex->damnodice = cMobIndex->damnodice;
      pMobIndex->damsizedice = cMobIndex->damsizedice;
      pMobIndex->damplus = cMobIndex->damplus;
      pMobIndex->gold = cMobIndex->gold;
      pMobIndex->silver = cMobIndex->silver;
      pMobIndex->copper = cMobIndex->copper;
      pMobIndex->exp = cMobIndex->exp;
      pMobIndex->position = cMobIndex->position;
      pMobIndex->defposition = cMobIndex->defposition;
      pMobIndex->sex = cMobIndex->sex;
      pMobIndex->perm_str = cMobIndex->perm_str;
      pMobIndex->perm_dex = cMobIndex->perm_dex;
      pMobIndex->perm_int = cMobIndex->perm_int;
      pMobIndex->perm_wis = cMobIndex->perm_wis;
      pMobIndex->perm_cha = cMobIndex->perm_cha;
      pMobIndex->perm_con = cMobIndex->perm_con;
      pMobIndex->perm_lck = cMobIndex->perm_lck;
      pMobIndex->race = cMobIndex->race;
      pMobIndex->class = cMobIndex->class;
      pMobIndex->xflags = cMobIndex->xflags;
      pMobIndex->resistant = cMobIndex->resistant;
      pMobIndex->immune = cMobIndex->immune;
      pMobIndex->susceptible = cMobIndex->susceptible;
      pMobIndex->numattacks = cMobIndex->numattacks;
      pMobIndex->attacks = cMobIndex->attacks;
      pMobIndex->defenses = cMobIndex->defenses;
   }
   iHash = vnum % MAX_KEY_HASH;
   pMobIndex->next = mob_index_hash[iHash];
   mob_index_hash[iHash] = pMobIndex;
   top_mob_index++;

   return pMobIndex;
}

/*
 * Creates a simple exit with no fields filled but rvnum and optionally
 * to_room and vnum.						-Thoric
 * Exits are inserted into the linked list based on vdir.
 */
EXIT_DATA *make_exit( ROOM_INDEX_DATA * pRoomIndex, ROOM_INDEX_DATA * to_room, short door )
{
   EXIT_DATA *pexit, *texit;
   bool broke;

   CREATE( pexit, EXIT_DATA, 1 );
   pexit->vdir = door;
   pexit->rvnum = pRoomIndex->vnum;
   pexit->to_room = to_room;
   pexit->distance = 1;
   if( to_room )
   {
      pexit->vnum = to_room->vnum;
      texit = get_exit_to( to_room, rev_dir[door], pRoomIndex->vnum );
      if( texit ) /* assign reverse exit pointers */
      {
         texit->rexit = pexit;
         pexit->rexit = texit;
      }
   }
   broke = FALSE;
   for( texit = pRoomIndex->first_exit; texit; texit = texit->next )
      if( door < texit->vdir )
      {
         broke = TRUE;
         break;
      }
   if( !pRoomIndex->first_exit )
      pRoomIndex->first_exit = pexit;
   else
   {
      /*
       * keep exits in incremental order - insert exit into list 
       */
      if( broke && texit )
      {
         if( !texit->prev )
            pRoomIndex->first_exit = pexit;
         else
            texit->prev->next = pexit;
         pexit->prev = texit->prev;
         pexit->next = texit;
         texit->prev = pexit;
         top_exit++;
         return pexit;
      }
      pRoomIndex->last_exit->next = pexit;
   }
   pexit->next = NULL;
   pexit->prev = pRoomIndex->last_exit;
   pRoomIndex->last_exit = pexit;
   top_exit++;
   return pexit;
}

void fix_area_exits( AREA_DATA * tarea )
{
   ROOM_INDEX_DATA *pRoomIndex;
   EXIT_DATA *pexit, *rv_exit;
   int rnum;
   bool fexit;

   for( rnum = tarea->low_r_vnum; rnum <= tarea->hi_r_vnum; rnum++ )
   {
      if( ( pRoomIndex = get_room_index( rnum ) ) == NULL )
         continue;

      fexit = FALSE;
      for( pexit = pRoomIndex->first_exit; pexit; pexit = pexit->next )
      {
         fexit = TRUE;
         pexit->rvnum = pRoomIndex->vnum;
         if( pexit->vnum <= 0 )
            pexit->to_room = NULL;
         else
            pexit->to_room = get_room_index( pexit->vnum );
      }
      if( !fexit )
         SET_BIT( pRoomIndex->room_flags, ROOM_NO_MOB );
   }

   for( rnum = tarea->low_r_vnum; rnum <= tarea->hi_r_vnum; rnum++ )
   {
      if( ( pRoomIndex = get_room_index( rnum ) ) == NULL )
         continue;

      for( pexit = pRoomIndex->first_exit; pexit; pexit = pexit->next )
      {
         if( pexit->to_room && !pexit->rexit )
         {
            rv_exit = get_exit_to( pexit->to_room, rev_dir[pexit->vdir], pRoomIndex->vnum );
            if( rv_exit )
            {
               pexit->rexit = rv_exit;
               rv_exit->rexit = pexit;
            }
         }
      }
   }
}

void load_area_file( AREA_DATA * tarea, char *filename )
{
   if( fBootDb )
      tarea = last_area;
   if( !fBootDb && !tarea )
   {
      bug( "Load_area: null area!" );
      return;
   }

   if( ( fpArea = fopen( filename, "r" ) ) == NULL )
   {
      perror( filename );
      bug( "load_area: error loading file (can't open)" );
      bug( filename );
      return;
   }
   area_version = 0;
   for( ;; )
   {
      char *word;

      if( fread_letter( fpArea ) != '#' )
      {
         bug( tarea->filename );
         bug( "load_area: # not found." );
         exit( 1 );
      }

      word = fread_word( fpArea );

      if( word[0] == '$' )
         break;
      else if( !str_cmp( word, "AREA" ) )
      {
         if( fBootDb )
         {
            load_area( fpArea );
            tarea = last_area;
         }
         else
         {
            DISPOSE( tarea->name );
            tarea->name = fread_string_nohash( fpArea );
         }
      }
      else if( !str_cmp( word, "AUTHOR" ) )
         load_author( tarea, fpArea );
      else if( !str_cmp( word, "FLAGS" ) )
         load_flags( tarea, fpArea );
      else if( !str_cmp( word, "RANGES" ) )
         load_ranges( tarea, fpArea );
      else if( !str_cmp( word, "ECONOMY" ) )
         load_economy( tarea, fpArea );
      else if( !str_cmp( word, "RESETMSG" ) )
         load_resetmsg( tarea, fpArea );
      /*
       * Rennard 
       */
      else if( !str_cmp( word, "HELPS" ) )
         load_helps( fpArea );
      else if( !str_cmp( word, "MOBILES" ) )
         load_mobiles( tarea, fpArea );
      else if( !str_cmp( word, "OBJECTS" ) )
         load_objects( tarea, fpArea );
      else if( !str_cmp( word, "RESETS" ) )
         load_resets( tarea, fpArea );
      else if( !str_cmp( word, "ROOMS" ) )
         load_rooms( tarea, fpArea );
      else if( !str_cmp( word, "SHOPS" ) )
         load_shops(fpArea );
      else if( !str_cmp( word, "REPAIRS" ) )
         load_repairs( fpArea );
      else if( !str_cmp( word, "SPECIALS" ) )
         load_specials( fpArea );
      else if( !str_cmp( word, "CLIMATE" ) )
         load_climate( tarea, fpArea );
      else if( !str_cmp( word, "NEIGHBOR" ) )
         load_neighbor( tarea, fpArea );
      else if( !str_cmp( word, "VERSION" ) )
         load_version( tarea, fpArea );
      else
      {
         bug( tarea->filename );
         bug( "load_area: bad section name." );
         if( fBootDb )
            exit( 1 );
         else
         {
            fclose( fpArea );
            return;
         }
      }
   }
   fclose( fpArea );
   /*
    * redhat 5.1 fix, appears fclose does not null the file pointer , but
    * needs it null if you reuse it. - shogar 
    */
   fpArea = NULL;

   if( tarea )
   {
      if( fBootDb )
      {
         sort_area_by_name( tarea );   /* 4/27/97 */
         sort_area( tarea, FALSE );
         if( IS_SET( tarea->flags, AFLAG_RESET_ON_BOOT ) )
         {
            tarea->ttl = -1;
            SET_BIT( tarea->flags, AFLAG_PLR_LOADED );
         }
      }
      fprintf( stderr, "%-14s: Rooms: %5d - %-5d Objs: %5d - %-5d Mobs: %5d - %d\n",
               tarea->filename,
               tarea->low_r_vnum, tarea->hi_r_vnum,
               tarea->low_o_vnum, tarea->hi_o_vnum, tarea->low_m_vnum, tarea->hi_m_vnum );
      if( !tarea->author )
         tarea->author = STRALLOC( "" );
      SET_BIT( tarea->status, AREA_LOADED );
      /*
       * Make sure this isn't set! 
       */
      if( !IS_SET( tarea->flags, AFLAG_RESET_ON_BOOT ) )
         REMOVE_BIT( tarea->flags, AFLAG_PLR_LOADED );
   }
   else
      fprintf( stderr, "(%s)\n", filename );
}

void load_reserved( void )
{
   RESERVE_DATA *res;
   FILE *fp;

   if( !( fp = fopen( SYSTEM_DIR RESERVED_LIST, "r" ) ) )
      return;

   for( ;; )
   {
      if( feof( fp ) )
      {
         bug( "Load_reserved: no $ found." );
         fclose( fp );
         return;
      }
      CREATE( res, RESERVE_DATA, 1 );
      res->name = fread_string_nohash( fp );
      if( *res->name == '$' )
         break;
      sort_reserved( res );
   }
   DISPOSE( res->name );
   DISPOSE( res );
   fclose( fp );
   return;
}

void load_filtered( void )
{
   FILTER_DATA *flt;
   FILE *fp;

   if( !( fp = fopen( SYSTEM_DIR FILTERED_LIST, "r" ) ) )
      return;

   for( ;; )
   {
      if( feof( fp ) )
      {
         bug( "Load_filtered: no $ found." );
         fclose( fp );
         return;
      }
      CREATE( flt, FILTER_DATA, 1 );
      flt->name = fread_string_nohash( fp );
      if( *flt->name == '$' )
         break;
      sort_filtered( flt );
   }
   DISPOSE( flt->name );
   DISPOSE( flt );
   fclose( fp );
   return;
}

/* Build list of in_progress areas.  Do not load areas.
 * define AREA_READ if you want it to build area names rather than reading
 * them out of the area files. -- Altrag */
void load_buildlist( void )
{
   DIR *dp;
   struct dirent *dentry;
   FILE *fp;
   char buf[MAX_STRING_LENGTH];
   AREA_DATA *pArea;
   char line[81];
   char word[81];
   int low, hi;
   int mlow, mhi, olow, ohi, rlow, rhi;
   bool badfile = FALSE;
   char temp;

   dp = opendir( GOD_DIR );
   dentry = readdir( dp );
   while( dentry )
   {
      if( dentry->d_name[0] != '.' )
      {
         sprintf( buf, "%s%s", GOD_DIR, dentry->d_name );
         if( !( fp = fopen( buf, "r" ) ) )
         {
            bug( "Load_buildlist: invalid file" );
            perror( buf );
            dentry = readdir( dp );
            continue;
         }
         log_string( buf );
         badfile = FALSE;
         rlow = rhi = olow = ohi = mlow = mhi = 0;
         while( !feof( fp ) && !ferror( fp ) )
         {
            low = 0;
            hi = 0;
            word[0] = 0;
            line[0] = 0;
            if( ( temp = fgetc( fp ) ) != EOF )
               ungetc( temp, fp );
            else
               break;

            fgets( line, 80, fp );
            sscanf( line, "%s %d %d", word, &low, &hi );
            if( !strcmp( word, "Level" ) )
            {
               if( low < LEVEL_IMMORTAL )
               {
                  sprintf( buf, "%s: God file with level %d < %d", dentry->d_name, low, LEVEL_IMMORTAL );
                  badfile = TRUE;
               }
            }
            if( !strcmp( word, "RoomRange" ) )
               rlow = low, rhi = hi;
            else if( !strcmp( word, "MobRange" ) )
               mlow = low, mhi = hi;
            else if( !strcmp( word, "ObjRange" ) )
               olow = low, ohi = hi;
         }
         fclose( fp );
         if( rlow && rhi && !badfile )
         {
            sprintf( buf, "%s%s.are", BUILD_DIR, dentry->d_name );
            if( !( fp = fopen( buf, "r" ) ) )
            {
               bug( "Load_buildlist: cannot open area file for read" );
               perror( buf );
               dentry = readdir( dp );
               continue;
            }
#if !defined(READ_AREA) /* Dont always want to read stuff.. dunno.. shrug */
            strcpy( word, fread_word( fp ) );
            if( word[0] != '#' || strcmp( &word[1], "AREA" ) )
            {
               sprintf( buf, "Make_buildlist: %s.are: no #AREA found.", dentry->d_name );
               fclose( fp );
               dentry = readdir( dp );
               continue;
            }
#endif
            CREATE( pArea, AREA_DATA, 1 );
            sprintf( buf, "%s.are", dentry->d_name );
            pArea->author = STRALLOC( dentry->d_name );
            pArea->filename = str_dup( buf );
#if !defined(READ_AREA)
            pArea->name = fread_string_nohash( fp );
#else
            sprintf( buf, "{PROTO} %s's area in progress", dentry->d_name );
            pArea->name = str_dup( buf );
#endif
            fclose( fp );
            pArea->low_r_vnum = rlow;
            pArea->hi_r_vnum = rhi;
            pArea->low_m_vnum = mlow;
            pArea->hi_m_vnum = mhi;
            pArea->low_o_vnum = olow;
            pArea->hi_o_vnum = ohi;
            pArea->low_soft_range = -1;
            pArea->hi_soft_range = -1;
            pArea->low_hard_range = -1;
            pArea->hi_hard_range = -1;

            CREATE( pArea->weather, WEATHER_DATA, 1 );   /* FB */
            pArea->weather->temp = 0;
            pArea->weather->precip = 0;
            pArea->weather->wind = 0;
            pArea->weather->temp_vector = 0;
            pArea->weather->precip_vector = 0;
            pArea->weather->wind_vector = 0;
            pArea->weather->climate_temp = 2;
            pArea->weather->climate_precip = 2;
            pArea->weather->climate_wind = 2;
            pArea->weather->first_neighbor = NULL;
            pArea->weather->last_neighbor = NULL;
            pArea->weather->echo = NULL;
            pArea->weather->echo_color = AT_GREY;
            pArea->first_room = pArea->last_room = NULL;
            SET_BIT( pArea->flags, AFLAG_PROTOTYPE );

            LINK( pArea, first_build, last_build, next, prev );
            fprintf( stderr, "%-14s: Rooms: %5d - %-5d Objs: %5d - %-5d "
                     "Mobs: %5d - %-5d\n",
                     pArea->filename,
                     pArea->low_r_vnum, pArea->hi_r_vnum,
                     pArea->low_o_vnum, pArea->hi_o_vnum, pArea->low_m_vnum, pArea->hi_m_vnum );
            sort_area( pArea, TRUE );
         }
      }
      dentry = readdir( dp );
   }
   closedir( dp );
}

/* Rebuilt from broken copy, but bugged - commented out for now - Blod */
void sort_reserved( RESERVE_DATA * pRes )
{
   RESERVE_DATA *res = NULL;

   if( !pRes )
   {
      bug( "Sort_reserved: NULL pRes" );
      return;
   }

   pRes->next = NULL;
   pRes->prev = NULL;

   for( res = first_reserved; res; res = res->next )
   {
      if( strcasecmp( pRes->name, res->name ) > 0 )
      {
         INSERT( pRes, res, first_reserved, next, prev );
         break;
      }
   }

   if( !res )
   {
      LINK( pRes, first_reserved, last_reserved, next, prev );
   }

   return;
}

void sort_filtered( FILTER_DATA * pFlt )
{
   FILTER_DATA *flt = NULL;

   if( !pFlt )
   {
      bug( "Sort_filtered: NULL pFlt" );
      return;
   }

   pFlt->next = NULL;
   pFlt->prev = NULL;

   for( flt = first_filtered; flt; flt = flt->next )
   {
      if( strcasecmp( pFlt->name, flt->name ) > 0 )
      {
         INSERT( pFlt, flt, first_filtered, next, prev );
         break;
      }
   }

   if( !flt )
   {
      LINK( pFlt, first_filtered, last_filtered, next, prev );
   }

   return;
}

/*
 * Sort areas by name alphanumercially
 *      - 4/27/97, Fireblade
 */
void sort_area_by_name( AREA_DATA * pArea )
{
   AREA_DATA *temp_area;

   if( !pArea )
   {
      bug( "Sort_area_by_name: NULL pArea" );
      return;
   }
   for( temp_area = first_area_name; temp_area; temp_area = temp_area->next_sort_name )
   {
      if( strcmp( pArea->name, temp_area->name ) < 0 )
      {
         INSERT( pArea, temp_area, first_area_name, next_sort_name, prev_sort_name );
         break;
      }
   }
   if( !temp_area )
   {
      LINK( pArea, first_area_name, last_area_name, next_sort_name, prev_sort_name );
   }
   return;
}

/*
 * Sort by room vnums					-Altrag & Thoric
 */
void sort_area( AREA_DATA * pArea, bool proto )
{
   AREA_DATA *area = NULL;
   AREA_DATA *first_sort, *last_sort;
   bool found;

   if( !pArea )
   {
      bug( "Sort_area: NULL pArea" );
      return;
   }

   if( proto )
   {
      first_sort = first_bsort;
      last_sort = last_bsort;
   }
   else
   {
      first_sort = first_asort;
      last_sort = last_asort;
   }

   found = FALSE;
   pArea->next_sort = NULL;
   pArea->prev_sort = NULL;

   if( !first_sort )
   {
      pArea->prev_sort = NULL;
      pArea->next_sort = NULL;
      first_sort = pArea;
      last_sort = pArea;
      found = TRUE;
   }
   else
      for( area = first_sort; area; area = area->next_sort )
         if( pArea->low_r_vnum < area->low_r_vnum )
         {
            if( !area->prev_sort )
               first_sort = pArea;
            else
               area->prev_sort->next_sort = pArea;
            pArea->prev_sort = area->prev_sort;
            pArea->next_sort = area;
            area->prev_sort = pArea;
            found = TRUE;
            break;
         }

   if( !found )
   {
      pArea->prev_sort = last_sort;
      pArea->next_sort = NULL;
      last_sort->next_sort = pArea;
      last_sort = pArea;
   }

   if( proto )
   {
      first_bsort = first_sort;
      last_bsort = last_sort;
   }
   else
   {
      first_asort = first_sort;
      last_asort = last_sort;
   }
}


/*
 * Display vnums currently assigned to areas		-Altrag & Thoric
 * Sorted, and flagged if loaded.
 */
void show_vnums( CHAR_DATA * ch, int low, int high, bool proto, bool shownl, char *loadst, char *notloadst )
{
   AREA_DATA *pArea, *first_sort;
   int count, loaded;

   count = 0;
   loaded = 0;
   set_pager_color( AT_PLAIN, ch );
   if( proto )
      first_sort = first_bsort;
   else
      first_sort = first_asort;
   for( pArea = first_sort; pArea; pArea = pArea->next_sort )
   {
      if( IS_SET( pArea->status, AREA_DELETED ) )
         continue;
      if( pArea->low_r_vnum < low )
         continue;
      if( pArea->hi_r_vnum > high )
         break;
      if( IS_SET( pArea->status, AREA_LOADED ) )
         loaded++;
      else if( !shownl )
         continue;
      pager_printf( ch, "&W%-15s&w| &cRooms: %5d - %-5d"
                    " &CObjs: %5d - %-5d &BMobs: %5d - %-5d%s&w\r\n",
                    ( pArea->filename ? pArea->filename : "(invalid)" ),
                    pArea->low_r_vnum, pArea->hi_r_vnum,
                    pArea->low_o_vnum, pArea->hi_o_vnum,
                    pArea->low_m_vnum, pArea->hi_m_vnum, IS_SET( pArea->status, AREA_LOADED ) ? loadst : notloadst );
      count++;
   }
   pager_printf( ch, "&YAreas listed: %d  &RLoaded: %d&w\r\n", count, loaded );
   return;
}

/*
 * Shows prototype vnums ranges, and if loaded
 */
void do_vnums( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   int low, high;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   low = 0;
   high = 1048575999;
   if( arg1[0] != '\0' )
   {
      low = atoi( arg1 );
      if( arg2[0] != '\0' )
         high = atoi( arg2 );
   }
   show_vnums( ch, low, high, TRUE, TRUE, " *", "" );
}

/*
 * Shows installed areas, sorted.  Mark unloaded areas with an X
 */
void do_zones( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   int low, high;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   low = 0;
   high = 1048575999;
   if( arg1[0] != '\0' )
   {
      low = atoi( arg1 );
      if( arg2[0] != '\0' )
         high = atoi( arg2 );
   }
   show_vnums( ch, low, high, FALSE, TRUE, "", " X" );
}

/*
 * Show prototype areas, sorted.  Only show loaded areas
 */
void do_newzones( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   int low, high;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   low = 0;
   high = 1048575999;
   if( arg1[0] != '\0' )
   {
      low = atoi( arg1 );
      if( arg2[0] != '\0' )
         high = atoi( arg2 );
   }
   show_vnums( ch, low, high, TRUE, FALSE, "", " X" );
}

/*
 * Save system info to data file
 */
void save_sysdata( SYSTEM_DATA sys )
{
   FILE *fp;
   char filename[MAX_INPUT_LENGTH];

   sprintf( filename, "%ssysdata.dat", SYSTEM_DIR );

   if( ( fp = fopen( filename, "w" ) ) == NULL )
   {
      bug( "save_sysdata: fopen" );
      perror( filename );
   }
   else
   {
      fprintf( fp, "#SYSTEM\n" );
      fprintf( fp, "MudName	     %s~\n", sys.mud_name );
      fprintf( fp, "Highplayers    %d\n", sys.alltimemax );
      fprintf( fp, "Highplayertime %s~\n", sys.time_of_max );
      fprintf( fp, "CheckImmHost   %d\n", sys.check_imm_host );
      fprintf( fp, "Nameresolving  %d\n", sys.NO_NAME_RESOLVING );
      fprintf( fp, "Waitforauth    %d\n", sys.WAIT_FOR_AUTH );
      fprintf( fp, "Readallmail    %d\n", sys.read_all_mail );
      fprintf( fp, "Readmailfree   %d\n", sys.read_mail_free );
      fprintf( fp, "Writemailfree  %d\n", sys.write_mail_free );
      fprintf( fp, "Takeothersmail %d\n", sys.take_others_mail );
      fprintf( fp, "Muse           %d\n", sys.muse_level );
      fprintf( fp, "Think          %d\n", sys.think_level );
      fprintf( fp, "Build          %d\n", sys.build_level );
      fprintf( fp, "Log            %d\n", sys.log_level );
      fprintf( fp, "Protoflag      %d\n", sys.level_modify_proto );
      fprintf( fp, "Overridepriv   %d\n", sys.level_override_private );
      fprintf( fp, "Msetplayer     %d\n", sys.level_mset_player );
      fprintf( fp, "Stunplrvsplr   %d\n", sys.stun_plr_vs_plr );
      fprintf( fp, "Stunregular    %d\n", sys.stun_regular );
      fprintf( fp, "Gougepvp       %d\n", sys.gouge_plr_vs_plr );
      fprintf( fp, "Gougenontank   %d\n", sys.gouge_nontank );
      fprintf( fp, "Bashpvp        %d\n", sys.bash_plr_vs_plr );
      fprintf( fp, "Bashnontank    %d\n", sys.bash_nontank );
      fprintf( fp, "Dodgemod       %d\n", sys.dodge_mod );
      fprintf( fp, "Parrymod       %d\n", sys.parry_mod );
      fprintf( fp, "Tumblemod      %d\n", sys.tumble_mod );
      fprintf( fp, "Damplrvsplr    %d\n", sys.dam_plr_vs_plr );
      fprintf( fp, "Damplrvsmob    %d\n", sys.dam_plr_vs_mob );
      fprintf( fp, "Dammobvsplr    %d\n", sys.dam_mob_vs_plr );
      fprintf( fp, "Webport	     %d\n", sys.webport );
      fprintf( fp, "Homepage	     %s~\n", sys.homepage );
      fprintf( fp, "Dammobvsmob    %d\n", sys.dam_mob_vs_mob );
      fprintf( fp, "Forcepc        %d\n", sys.level_forcepc );
      fprintf( fp, "Guildoverseer  %s~\n", sys.guild_overseer );
      fprintf( fp, "Guildadvisor   %s~\n", sys.guild_advisor );
      fprintf( fp, "Saveflags      %d\n", sys.save_flags );
      fprintf( fp, "Savefreq       %d\n", sys.save_frequency );
      fprintf( fp, "Bestowdif      %d\n", sys.bestow_dif );
      fprintf( fp, "BanSiteLevel   %d\n", sys.ban_site_level );
      fprintf( fp, "BanRaceLevel   %d\n", sys.ban_race_level );
      fprintf( fp, "BanClassLevel  %d\n", sys.ban_class_level );
      fprintf( fp, "MorphOpt       %d\n", sys.morph_opt );
      fprintf( fp, "PetSave	     %d\n", sys.save_pets );
      fprintf( fp, "IdentTries     %d\n", sys.ident_retries );
/* Scion arenacode */
      fprintf( fp, "Imm_arena	     %d\n", sys.imm_arena );
      fprintf( fp, "WebServer	     %d\n", sys.webtoggle );
      fprintf( fp, "WebCounter     %d\n", sys.webcounter );
      fprintf( fp, "DoubleEXP      %d\n", sys.double_exp );
      fprintf( fp, "Wizlock        %d\n", sys.wizlock );
      fprintf( fp, "NeverEmpty     %d\n", sys.neverempty ); /* Altrag */
      fprintf( fp, "NoTake         %d\n", sys.level_getobjnotake );
      fprintf( fp, "Newbie_purge   %d\n", sys.newbie_purge );
      fprintf( fp, "Regular_purge  %d\n", sys.regular_purge );
      fprintf( fp, "Autopurge	     %d\n", sys.CLEANPFILES );
      fprintf( fp, "MPLimit        %d\n", sys.mplimit );
      fprintf( fp, "PKLimit        %d\n", sys.pklimit );
      fprintf( fp, "End\n\n" );
      fprintf( fp, "#END\n" );
      fclose( fp );
      fp = NULL;
   }
   return;
}

void fread_sysdata( SYSTEM_DATA * sys, FILE * fp )
{
   char *word = NULL;
   bool fMatch = FALSE;

   sys->time_of_max = NULL;
   sys->mud_name = NULL;
   for( ;; )
   {
      word = feof( fp ) ? "End" : fread_word( fp );
      fMatch = FALSE;

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;
         case 'A':
            KEY( "Autopurge", sys->CLEANPFILES, fread_number( fp ) );
            break;
         case 'B':
            KEY( "Bashpvp", sys->bash_plr_vs_plr, fread_number( fp ) );
            KEY( "Bashnontank", sys->bash_nontank, fread_number( fp ) );
            KEY( "Bestowdif", sys->bestow_dif, fread_number( fp ) );
            KEY( "Build", sys->build_level, fread_number( fp ) );
            KEY( "BanSiteLevel", sys->ban_site_level, fread_number( fp ) );
            KEY( "BanClassLevel", sys->ban_class_level, fread_number( fp ) );
            KEY( "BanRaceLevel", sys->ban_race_level, fread_number( fp ) );
            break;

         case 'C':
            KEY( "CheckImmHost", sys->check_imm_host, fread_number( fp ) );
            break;

         case 'D':
            KEY( "Damplrvsplr", sys->dam_plr_vs_plr, fread_number( fp ) );
            KEY( "Damplrvsmob", sys->dam_plr_vs_mob, fread_number( fp ) );
            KEY( "Dammobvsplr", sys->dam_mob_vs_plr, fread_number( fp ) );
            KEY( "Dammobvsmob", sys->dam_mob_vs_mob, fread_number( fp ) );
            KEY( "Dodgemod", sys->dodge_mod, fread_number( fp ) );
            KEY( "DoubleEXP", sys->double_exp, fread_number( fp ) );
            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
            {
               if( !sys->time_of_max )
                  sys->time_of_max = str_dup( "(not recorded)" );
               if( !sys->mud_name )
                  sys->mud_name = str_dup( "(Name Not Set)" );
               return;
            }
            break;

         case 'F':
            KEY( "Forcepc", sys->level_forcepc, fread_number( fp ) );
            break;

         case 'G':
            KEY( "Gougepvp", sys->gouge_plr_vs_plr, fread_number( fp ) );
            KEY( "Gougenontank", sys->gouge_nontank, fread_number( fp ) );
            KEY( "Guildoverseer", sys->guild_overseer, fread_string( fp ) );
            KEY( "Guildadvisor", sys->guild_advisor, fread_string( fp ) );
            break;

         case 'H':
            KEY( "Homepage", sys->homepage, fread_string_nohash( fp ) );
            KEY( "Highplayers", sys->alltimemax, fread_number( fp ) );
            KEY( "Highplayertime", sys->time_of_max, fread_string_nohash( fp ) );
            break;

         case 'I':
            KEY( "IdentTries", sys->ident_retries, fread_number( fp ) );
/* Scion arenacode */
            KEY( "Imm_arena", sys->imm_arena, fread_number( fp ) );
            break;

         case 'L':
            KEY( "Log", sys->log_level, fread_number( fp ) );
            break;

         case 'M':
            KEY( "MorphOpt", sys->morph_opt, fread_number( fp ) );
            KEY( "Msetplayer", sys->level_mset_player, fread_number( fp ) );
            KEY( "MudName", sys->mud_name, fread_string_nohash( fp ) );
            KEY( "Muse", sys->muse_level, fread_number( fp ) );
            KEY( "MPLimit", sys->mplimit, fread_number( fp ) );
            break;

         case 'N':
            KEY( "Nameresolving", sys->NO_NAME_RESOLVING, fread_number( fp ) );
            KEY( "Newbie_purge", sys->newbie_purge, fread_number( fp ) );
            KEY( "NeverEmpty", sys->neverempty, fread_number( fp ) );   /* Altrag */
            KEY( "NoTake", sys->level_getobjnotake, fread_number( fp ) );
            break;

         case 'O':
            KEY( "Overridepriv", sys->level_override_private, fread_number( fp ) );
            break;

         case 'P':
            KEY( "Parrymod", sys->parry_mod, fread_number( fp ) );
            KEY( "PetSave", sys->save_pets, fread_number( fp ) );
            KEY( "Protoflag", sys->level_modify_proto, fread_number( fp ) );
            KEY( "PKLimit", sys->pklimit, fread_number( fp ) );
            break;

         case 'R':
            KEY( "Readallmail", sys->read_all_mail, fread_number( fp ) );
            KEY( "Readmailfree", sys->read_mail_free, fread_number( fp ) );
            KEY( "Regular_purge", sys->regular_purge, fread_number( fp ) );
            break;

         case 'S':
            KEY( "Stunplrvsplr", sys->stun_plr_vs_plr, fread_number( fp ) );
            KEY( "Stunregular", sys->stun_regular, fread_number( fp ) );
            KEY( "Saveflags", sys->save_flags, fread_number( fp ) );
            KEY( "Savefreq", sys->save_frequency, fread_number( fp ) );
            KEY( "Staff", sys->muse_level, fread_number( fp ) );
            break;

         case 'T':
            KEY( "Takeothersmail", sys->take_others_mail, fread_number( fp ) );
            KEY( "Think", sys->think_level, fread_number( fp ) );
            KEY( "Tumblemod", sys->tumble_mod, fread_number( fp ) );
            break;


         case 'W':
            KEY( "WebCounter", sys->webcounter, fread_number( fp ) );
            KEY( "WebServer", sys->webtoggle, fread_number( fp ) );
            KEY( "Webport", sys->webport, fread_number( fp ) );
            KEY( "Wizlock", sys->wizlock, fread_number( fp ) );
            KEY( "Waitforauth", sys->WAIT_FOR_AUTH, fread_number( fp ) );
            KEY( "Writemailfree", sys->write_mail_free, fread_number( fp ) );
            break;
      }


      if( !fMatch )
      {
         bug( "Fread_sysdata: no match: %s", word );
      }
   }
}

/*
 * Load the sysdata file
 */
bool load_systemdata( SYSTEM_DATA * sys )
{
   char filename[MAX_INPUT_LENGTH];
   FILE *fp;
   bool found;

   found = FALSE;
   sprintf( filename, "%ssysdata.dat", SYSTEM_DIR );

   if( ( fp = fopen( filename, "r" ) ) != NULL )
   {

      found = TRUE;
      for( ;; )
      {
         char letter;
         char *word;

         letter = fread_letter( fp );
         if( letter == '*' )
         {
            fread_to_eol( fp );
            continue;
         }

         if( letter != '#' )
         {
            bug( "Load_sysdata_file: # not found." );
            break;
         }

         word = fread_word( fp );
         if( !str_cmp( word, "SYSTEM" ) )
         {
            fread_sysdata( sys, fp );
            break;
         }
         else if( !str_cmp( word, "END" ) )
            break;
         else
         {
            bug( "Load_sysdata_file: bad section." );
            break;
         }
      }
      fclose( fp );
   }

   if( !sysdata.guild_overseer )
      sysdata.guild_overseer = STRALLOC( "" );
   if( !sysdata.guild_advisor )
      sysdata.guild_advisor = STRALLOC( "" );
   return found;
}

void load_watchlist( void )
{
   WATCH_DATA *pwatch;
   FILE *fp;
   int number;
   CMDTYPE *cmd;

   if( !( fp = fopen( SYSTEM_DIR WATCH_LIST, "r" ) ) )
      return;

   for( ;; )
   {
      if( feof( fp ) )
      {
         bug( "Load_watchlist: no -1 found." );
         fclose( fp );
         return;
      }
      number = fread_number( fp );
      if( number == -1 )
      {
         fclose( fp );
         return;
      }

      CREATE( pwatch, WATCH_DATA, 1 );
      pwatch->imm_level = number;
      pwatch->imm_name = fread_string_nohash( fp );
      pwatch->target_name = fread_string_nohash( fp );
      if( strlen( pwatch->target_name ) < 2 )
         DISPOSE( pwatch->target_name );
      pwatch->player_site = fread_string_nohash( fp );
      if( strlen( pwatch->player_site ) < 2 )
         DISPOSE( pwatch->player_site );

      /*
       * Check for command watches 
       */
      if( pwatch->target_name )
         for( cmd = command_hash[( int )pwatch->target_name[0]]; cmd; cmd = cmd->next )
         {
            if( !str_cmp( pwatch->target_name, cmd->name ) )
            {
               SET_BIT( cmd->flags, CMD_WATCH );
               break;
            }
         }

      LINK( pwatch, first_watch, last_watch, next, prev );
   }
}


/* Check to make sure range of vnums is free - Scryn 2/27/96 */

void do_check_vnums( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   AREA_DATA *pArea;
   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   bool room, mob, obj, all, area_conflict;
   int low_range, high_range;

   room = FALSE;
   mob = FALSE;
   obj = FALSE;
   all = FALSE;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Please specify room, mob, object, or all as your first argument.\r\n", ch );
      return;
   }

   if( !str_cmp( arg1, "room" ) )
      room = TRUE;

   else if( !str_cmp( arg1, "mob" ) )
      mob = TRUE;

   else if( !str_cmp( arg1, "object" ) )
      obj = TRUE;

   else if( !str_cmp( arg1, "all" ) )
      all = TRUE;
   else
   {
      send_to_char( "Please specify room, mob, or object as your first argument.\r\n", ch );
      return;
   }

   if( arg2[0] == '\0' )
   {
      send_to_char( "Please specify the low end of the range to be searched.\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "Please specify the high end of the range to be searched.\r\n", ch );
      return;
   }

   low_range = atoi( arg2 );
   high_range = atoi( argument );

   if( low_range < 1 || low_range > 1048576000 )
   {
      send_to_char( "Invalid argument for bottom of range.\r\n", ch );
      return;
   }

   if( high_range < 1 || high_range > 1048576000 )
   {
      send_to_char( "Invalid argument for top of range.\r\n", ch );
      return;
   }

   if( high_range < low_range )
   {
      send_to_char( "Bottom of range must be below top of range.\r\n", ch );
      return;
   }

   if( all )
   {
      sprintf( buf, "room %d %d", low_range, high_range );
      do_check_vnums( ch, buf );
      sprintf( buf, "mob %d %d", low_range, high_range );
      do_check_vnums( ch, buf );
      sprintf( buf, "object %d %d", low_range, high_range );
      do_check_vnums( ch, buf );
      return;
   }
   set_char_color( AT_PLAIN, ch );

   for( pArea = first_asort; pArea; pArea = pArea->next_sort )
   {
      area_conflict = FALSE;
      if( IS_SET( pArea->status, AREA_DELETED ) )
         continue;
      else if( room )
      {
         if( low_range < pArea->low_r_vnum && pArea->low_r_vnum < high_range )
            area_conflict = TRUE;

         if( low_range < pArea->hi_r_vnum && pArea->hi_r_vnum < high_range )
            area_conflict = TRUE;

         if( ( low_range >= pArea->low_r_vnum ) && ( low_range <= pArea->hi_r_vnum ) )
            area_conflict = TRUE;

         if( ( high_range <= pArea->hi_r_vnum ) && ( high_range >= pArea->low_r_vnum ) )
            area_conflict = TRUE;
      }

      if( mob )
      {
         if( low_range < pArea->low_m_vnum && pArea->low_m_vnum < high_range )
            area_conflict = TRUE;

         if( low_range < pArea->hi_m_vnum && pArea->hi_m_vnum < high_range )
            area_conflict = TRUE;
         if( ( low_range >= pArea->low_m_vnum ) && ( low_range <= pArea->hi_m_vnum ) )
            area_conflict = TRUE;

         if( ( high_range <= pArea->hi_m_vnum ) && ( high_range >= pArea->low_m_vnum ) )
            area_conflict = TRUE;
      }

      if( obj )
      {
         if( low_range < pArea->low_o_vnum && pArea->low_o_vnum < high_range )
            area_conflict = TRUE;

         if( low_range < pArea->hi_o_vnum && pArea->hi_o_vnum < high_range )
            area_conflict = TRUE;

         if( ( low_range >= pArea->low_o_vnum ) && ( low_range <= pArea->hi_o_vnum ) )
            area_conflict = TRUE;

         if( ( high_range <= pArea->hi_o_vnum ) && ( high_range >= pArea->low_o_vnum ) )
            area_conflict = TRUE;
      }

      if( area_conflict )
      {
         sprintf( buf, "Conflict:%-15s| ", ( pArea->filename ? pArea->filename : "(invalid)" ) );
         if( room )
            sprintf( buf2, "Rooms: %5d - %-5d\r\n", pArea->low_r_vnum, pArea->hi_r_vnum );
         if( mob )
            sprintf( buf2, "Mobs: %5d - %-5d\r\n", pArea->low_m_vnum, pArea->hi_m_vnum );
         if( obj )
            sprintf( buf2, "Objects: %5d - %-5d\r\n", pArea->low_o_vnum, pArea->hi_o_vnum );

         strcat( buf, buf2 );
         send_to_char( buf, ch );
      }
   }
   for( pArea = first_bsort; pArea; pArea = pArea->next_sort )
   {
      area_conflict = FALSE;
      if( IS_SET( pArea->status, AREA_DELETED ) )
         continue;
      else if( room )
      {
         if( low_range < pArea->low_r_vnum && pArea->low_r_vnum < high_range )
            area_conflict = TRUE;

         if( low_range < pArea->hi_r_vnum && pArea->hi_r_vnum < high_range )
            area_conflict = TRUE;

         if( ( low_range >= pArea->low_r_vnum ) && ( low_range <= pArea->hi_r_vnum ) )
            area_conflict = TRUE;

         if( ( high_range <= pArea->hi_r_vnum ) && ( high_range >= pArea->low_r_vnum ) )
            area_conflict = TRUE;
      }

      if( mob )
      {
         if( low_range < pArea->low_m_vnum && pArea->low_m_vnum < high_range )
            area_conflict = TRUE;

         if( low_range < pArea->hi_m_vnum && pArea->hi_m_vnum < high_range )
            area_conflict = TRUE;
         if( ( low_range >= pArea->low_m_vnum ) && ( low_range <= pArea->hi_m_vnum ) )
            area_conflict = TRUE;

         if( ( high_range <= pArea->hi_m_vnum ) && ( high_range >= pArea->low_m_vnum ) )
            area_conflict = TRUE;
      }

      if( obj )
      {
         if( low_range < pArea->low_o_vnum && pArea->low_o_vnum < high_range )
            area_conflict = TRUE;

         if( low_range < pArea->hi_o_vnum && pArea->hi_o_vnum < high_range )
            area_conflict = TRUE;

         if( ( low_range >= pArea->low_o_vnum ) && ( low_range <= pArea->hi_o_vnum ) )
            area_conflict = TRUE;

         if( ( high_range <= pArea->hi_o_vnum ) && ( high_range >= pArea->low_o_vnum ) )
            area_conflict = TRUE;
      }

      if( area_conflict )
      {
         sprintf( buf, "Conflict:%-15s| ", ( pArea->filename ? pArea->filename : "(invalid)" ) );
         if( room )
            sprintf( buf2, "Rooms: %5d - %-5d\r\n", pArea->low_r_vnum, pArea->hi_r_vnum );
         if( mob )
            sprintf( buf2, "Mobs: %5d - %-5d\r\n", pArea->low_m_vnum, pArea->hi_m_vnum );
         if( obj )
            sprintf( buf2, "Objects: %5d - %-5d\r\n", pArea->low_o_vnum, pArea->hi_o_vnum );

         strcat( buf, buf2 );
         send_to_char( buf, ch );
      }
   }

/*
    for ( pArea = first_asort; pArea; pArea = pArea->next_sort )
    {
        area_conflict = FALSE;
	if ( IS_SET( pArea->status, AREA_DELETED ) )
	   continue;
	else
	if (room)
	  if((pArea->low_r_vnum >= low_range) 
	  && (pArea->hi_r_vnum <= high_range))
	    area_conflict = TRUE;

	if (mob)
	  if((pArea->low_m_vnum >= low_range) 
	  && (pArea->hi_m_vnum <= high_range))
	    area_conflict = TRUE;

	if (obj)
	  if((pArea->low_o_vnum >= low_range) 
	  && (pArea->hi_o_vnum <= high_range))
	    area_conflict = TRUE;

	if (area_conflict)
	  ch_printf(ch, "Conflict:%-15s| Rooms: %5d - %-5d"
		     " Objs: %5d - %-5d Mobs: %5d - %-5d\r\n",
		(pArea->filename ? pArea->filename : "(invalid)"),
		pArea->low_r_vnum, pArea->hi_r_vnum,
		pArea->low_o_vnum, pArea->hi_o_vnum,
		pArea->low_m_vnum, pArea->hi_m_vnum );
    }

    for ( pArea = first_bsort; pArea; pArea = pArea->next_sort )
    {
        area_conflict = FALSE;
	if ( IS_SET( pArea->status, AREA_DELETED ) )
	   continue;
	else
	if (room)
	  if((pArea->low_r_vnum >= low_range) 
	  && (pArea->hi_r_vnum <= high_range))
	    area_conflict = TRUE;

	if (mob)
	  if((pArea->low_m_vnum >= low_range) 
	  && (pArea->hi_m_vnum <= high_range))
	    area_conflict = TRUE;

	if (obj)
	  if((pArea->low_o_vnum >= low_range) 
	  && (pArea->hi_o_vnum <= high_range))
	    area_conflict = TRUE;

	if (area_conflict)
	  sprintf(ch, "Conflict:%-15s| Rooms: %5d - %-5d"
		     " Objs: %5d - %-5d Mobs: %5d - %-5d\r\n",
		(pArea->filename ? pArea->filename : "(invalid)"),
		pArea->low_r_vnum, pArea->hi_r_vnum,
		pArea->low_o_vnum, pArea->hi_o_vnum,
		pArea->low_m_vnum, pArea->hi_m_vnum );
    }
*/
   return;
}

/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */
void tail_chain( void )
{
   return;
}


/*
 * Initialize the weather for all the areas
 * Last Modified: July 21, 1997
 * Fireblade
 */
void init_area_weather(  )
{
   AREA_DATA *pArea;
   NEIGHBOR_DATA *neigh;
   NEIGHBOR_DATA *next_neigh;

   for( pArea = first_area; pArea; pArea = pArea->next )
   {
      int cf;

      /*
       * init temp and temp vector 
       */
      cf = pArea->weather->climate_temp - 2;
      pArea->weather->temp = number_range( -weath_unit, weath_unit ) + cf * number_range( 0, weath_unit );
      pArea->weather->temp_vector = cf + number_range( -rand_factor, rand_factor );

      /*
       * init precip and precip vector 
       */
      cf = pArea->weather->climate_precip - 2;
      pArea->weather->precip = number_range( -weath_unit, weath_unit ) + cf * number_range( 0, weath_unit );
      pArea->weather->precip_vector = cf + number_range( -rand_factor, rand_factor );

      /*
       * init wind and wind vector 
       */
      cf = pArea->weather->climate_wind - 2;
      pArea->weather->wind = number_range( -weath_unit, weath_unit ) + cf * number_range( 0, weath_unit );
      pArea->weather->wind_vector = cf + number_range( -rand_factor, rand_factor );

      /*
       * check connections between neighbors 
       */
      for( neigh = pArea->weather->first_neighbor; neigh; neigh = next_neigh )
      {
         AREA_DATA *tarea;
         NEIGHBOR_DATA *tneigh;

         /*
          * get the address if needed 
          */
         if( !neigh->address )
            neigh->address = get_area( neigh->name );

         /*
          * area does not exist 
          */
         if( !neigh->address )
         {
            tneigh = neigh;
            next_neigh = tneigh->next;
            UNLINK( tneigh, pArea->weather->first_neighbor, pArea->weather->last_neighbor, next, prev );
            STRFREE( tneigh->name );
            DISPOSE( tneigh );
            fold_area( pArea, pArea->filename, FALSE );
            continue;
         }

         /*
          * make sure neighbors both point to each other 
          */
         tarea = neigh->address;
         for( tneigh = tarea->weather->first_neighbor; tneigh; tneigh = tneigh->next )
         {
            if( !strcmp( pArea->name, tneigh->name ) )
               break;
         }

         if( !tneigh )
         {
            CREATE( tneigh, NEIGHBOR_DATA, 1 );
            tneigh->name = STRALLOC( pArea->name );
            LINK( tneigh, tarea->weather->first_neighbor, tarea->weather->last_neighbor, next, prev );
            fold_area( tarea, tarea->filename, FALSE );
         }

         tneigh->address = pArea;

         next_neigh = neigh->next;
      }
   }

   return;
}

/*
 * Load weather data from appropriate file in system dir
 * Last Modified: July 24, 1997
 * Fireblade
 */
void load_weatherdata(  )
{
   char filename[MAX_INPUT_LENGTH];
   FILE *fp;

   sprintf( filename, "%sweather.dat", SYSTEM_DIR );

   if( ( fp = fopen( filename, "r" ) ) != NULL )
   {
      for( ;; )
      {
         char letter;
         char *word;

         letter = fread_letter( fp );

         if( letter != '#' )
         {
            bug( "load_weatherdata: # not found" );
            return;
         }

         word = fread_word( fp );

         if( !str_cmp( word, "RANDOM" ) )
            rand_factor = fread_number( fp );
         else if( !str_cmp( word, "CLIMATE" ) )
            climate_factor = fread_number( fp );
         else if( !str_cmp( word, "NEIGHBOR" ) )
            neigh_factor = fread_number( fp );
         else if( !str_cmp( word, "UNIT" ) )
            weath_unit = fread_number( fp );
         else if( !str_cmp( word, "MAXVECTOR" ) )
            max_vector = fread_number( fp );
         else if( !str_cmp( word, "END" ) )
         {
            fclose( fp );
            break;
         }
         else
         {
            bug( "load_weatherdata: unknown field" );
            fclose( fp );
            break;
         }
      }
   }

   return;
}

/*
 * Write data for global weather parameters
 * Last Modified: July 24, 1997
 * Fireblade
 */
void save_weatherdata(  )
{
   char filename[MAX_INPUT_LENGTH];
   FILE *fp;

   sprintf( filename, "%sweather.dat", SYSTEM_DIR );

   if( ( fp = fopen( filename, "w" ) ) != NULL )
   {
      fprintf( fp, "#RANDOM %d\n", rand_factor );
      fprintf( fp, "#CLIMATE %d\n", climate_factor );
      fprintf( fp, "#NEIGHBOR %d\n", neigh_factor );
      fprintf( fp, "#UNIT %d\n", weath_unit );
      fprintf( fp, "#MAXVECTOR %d\n", max_vector );
      fprintf( fp, "#END\n" );
      fclose( fp );
   }
   else
   {
      bug( "save_weatherdata: could not open file" );
   }

   return;
}

void load_projects( void ) /* Copied load_boards structure for simplicity */
{
   char filename[MAX_INPUT_LENGTH];
   FILE *fp;
   PROJECT_DATA *project;

   first_project = NULL;
   last_project = NULL;
   sprintf( filename, "%s", PROJECTS_FILE );
   if( ( fp = fopen( filename, "r" ) ) == NULL )
      return;

   while( ( project = read_project( fp ) ) != NULL )
      LINK( project, first_project, last_project, next, prev );

   return;
}

PROJECT_DATA *read_project( FILE * fp )
{
   PROJECT_DATA *project;
   NOTE_DATA *n_log, *tlog;
   char *word = NULL;
   char buf[MAX_STRING_LENGTH];
   bool fMatch = FALSE;
   char letter;

   do
   {
      letter = getc( fp );
      if( feof( fp ) )
      {
         fclose( fp );
         return NULL;
      }
   }
   while( isspace( letter ) );
   ungetc( letter, fp );

   CREATE( project, PROJECT_DATA, 1 );

   project->first_log = NULL;
   project->last_log = NULL;
   project->next = NULL;
   project->prev = NULL;
   project->coder = NULL;
   project->description = STRALLOC( "" );
   project->name = NULL;
   project->owner = STRALLOC( "" );
   project->date = STRALLOC( "Not Set?!" );
   project->status = STRALLOC( "No update." );

   for( ;; )
   {
      word = feof( fp ) ? "End" : fread_word( fp );
      fMatch = FALSE;

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;
         case 'C':
            KEY( "Coder", project->coder, fread_string_nohash( fp ) );
            break;
         case 'D':
            if( !str_cmp( word, "Date" ) )
               STRFREE( project->date );
            else if( !str_cmp( word, "Description" ) )
               STRFREE( project->description );
            KEY( "Date", project->date, fread_string( fp ) );
            KEY( "Description", project->description, fread_string( fp ) );
            break;
         case 'E':
            if( !str_cmp( word, "End" ) )
            {
               if( !project->description )
                  project->description = STRALLOC( "" );
               if( !project->name )
                  project->name = STRALLOC( "" );
               if( !project->owner )
                  project->owner = STRALLOC( "" );
               if( !project->date )
                  project->date = STRALLOC( "Not Set?!" );
               if( !project->status )
                  project->status = STRALLOC( "No update." );
               if( str_cmp( project->owner, "None" ) )
                  project->taken = TRUE;
               return project;
            }
            break;
         case 'L':
            if( !str_cmp( word, "Log" ) )
            {
               fread_to_eol( fp );
               n_log = read_log( fp );
               if( !n_log )
               {
                  sprintf( buf, "read_project: couldn't read log, aborting" );
                  bug( buf, 0 );
                  exit( 1 );
               }
               if( !n_log->sender )
                  n_log->sender = STRALLOC( "" );
               if( !n_log->date )
                  n_log->date = STRALLOC( "" );
               if( !n_log->subject )
                  n_log->subject = STRALLOC( "None" );
               n_log->to_list = STRALLOC( "" );
               LINK( n_log, project->first_log, project->last_log, next, prev );
               fMatch = TRUE;
               break;
            }
            break;
         case 'N':
            if( !str_cmp( word, "Name" ) )
               if( project->name )
                  DISPOSE( project->name );
            KEY( "Name", project->name, fread_string_nohash( fp ) );
            break;
         case 'O':
            if( !str_cmp( word, "Owner" ) )
               STRFREE( project->owner );
            KEY( "Owner", project->owner, fread_string( fp ) );
            break;
         case 'S':
            if( !str_cmp( word, "Status" ) )
               STRFREE( project->status );
            KEY( "Status", project->status, fread_string( fp ) );
            break;
      }
      if( !fMatch )
      {
         sprintf( buf, "read_project: no match: %s", word );
         bug( buf, 0 );
      }
   }
   n_log = project->last_log;
   while( n_log )
   {
      UNLINK( n_log, project->first_log, project->last_log, next, prev );
      tlog = n_log->prev;
      free_note( n_log );
      n_log = tlog;
   }
   if( project->coder )
      DISPOSE( project->coder );
   if( project->description )
      STRFREE( project->description );
   if( project->name )
      DISPOSE( project->name );
   if( project->owner )
      STRFREE( project->owner );
   if( project->date )
      STRFREE( project->date );
   if( project->status )
      STRFREE( project->status );
   DISPOSE( project );
   return project;
}

NOTE_DATA *read_log( FILE * fp )
{
   NOTE_DATA *n_log;
   char *word;
   CREATE( n_log, NOTE_DATA, 1 );

   for( ;; )
   {
      word = fread_word( fp );

      if( !str_cmp( word, "Sender" ) )
         n_log->sender = fread_string( fp );
      else if( !str_cmp( word, "Date" ) )
         n_log->date = fread_string( fp );
      else if( !str_cmp( word, "Subject" ) )
         n_log->subject = fread_string( fp );
      else if( !str_cmp( word, "Text" ) )
         n_log->text = fread_string( fp );
      else if( !str_cmp( word, "Endlog" ) )
      {
         fread_to_eol( fp );
         n_log->next = NULL;
         n_log->prev = NULL;
         return n_log;
      }
      else
      {
         DISPOSE( n_log );
         bug( "read_log: bad key word.", 0 );
         return NULL;
      }
   }
}


void write_projects(  )
{
   PROJECT_DATA *project;
   NOTE_DATA *n_log;
   FILE *fpout;
   char filename[MAX_INPUT_LENGTH];

   sprintf( filename, "%s", PROJECTS_FILE );
   fpout = fopen( filename, "w" );
   if( !fpout )
   {
      bug( "FATAL: cannot open projects.txt for writing!\r\n", 0 );
      return;
   }
   for( project = first_project; project; project = project->next )
   {
      fprintf( fpout, "Name		   %s~\n", project->name );
      fprintf( fpout, "Owner		   %s~\n", ( project->owner ) ? project->owner : "None" );
      if( project->coder )
         fprintf( fpout, "Coder		    %s~\n", project->coder );
      fprintf( fpout, "Status		   %s~\n", ( project->status ) ? project->status : "No update." );
      fprintf( fpout, "Date		   %s~\n", ( project->date ) ? project->date : "Not Set?!?" );
      if( project->description )
         fprintf( fpout, "Description         %s~\n", project->description );
      for( n_log = project->first_log; n_log; n_log = n_log->next )
         fprintf( fpout, "Log\nSender %s~\nDate %s~\nSubject %s~\nText %s~\nEndlog\n",
                  n_log->sender, n_log->date, n_log->subject, n_log->text );

      fprintf( fpout, "End\n" );
   }
   fclose( fpout );
}