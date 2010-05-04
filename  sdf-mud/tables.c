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
 * SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider      |    ).:.(    *
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
 * 			Table load/save Module				    *
 ****************************************************************************/

#include <string.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>
#include "mud.h"

bool load_race_file( char *fname );
void write_race_file( int ra );

/* global variables */
int top_sn;
int top_herb;

SKILLTYPE *skill_table[MAX_SKILL];
struct class_type *class_table[MAX_CLASS];
RACE_TYPE *race_table[MAX_RACE];
char *title_table[MAX_CLASS][MAX_LEVEL + 1][2];
SKILLTYPE *herb_table[MAX_HERB];
SKILLTYPE *disease_table[MAX_DISEASE];

LANG_DATA *first_lang;
LANG_DATA *last_lang;

char *const skill_tname[] = { "unknown", "Spell", "Skill", "Weapon", "Tongue", "Herb", "Racial", "Disease" };

SPELL_FUN *spell_function( char *name )
{
   if( !str_cmp( name, "spell_smaug" ) )
      return spell_smaug;
   if( !str_cmp( name, "spell_acid_blast" ) )
      return spell_acid_blast;
   if( !str_cmp( name, "spell_animate_dead" ) )
      return spell_animate_dead;
   if( !str_cmp( name, "spell_astral_walk" ) )
      return spell_astral_walk;
   if( !str_cmp( name, "spell_blindness" ) )
      return spell_blindness;
   if( !str_cmp( name, "spell_burning_hands" ) )
      return spell_burning_hands;
   if( !str_cmp( name, "spell_call_lightning" ) )
      return spell_call_lightning;
   if( !str_cmp( name, "spell_custos_angelus" ) )
      return spell_custos_angelus;
   if( !str_cmp( name, "spell_cause_critical" ) )
      return spell_cause_critical;
   if( !str_cmp( name, "spell_cause_light" ) )
      return spell_cause_light;
   if( !str_cmp( name, "spell_cause_serious" ) )
      return spell_cause_serious;
   if( !str_cmp( name, "spell_change_sex" ) )
      return spell_change_sex;
   if( !str_cmp( name, "spell_charm_person" ) )
      return spell_charm_person;
   if( !str_cmp( name, "spell_chill_touch" ) )
      return spell_chill_touch;
   if( !str_cmp( name, "spell_colour_spray" ) )
      return spell_colour_spray;
   if( !str_cmp( name, "spell_control_weather" ) )
      return spell_control_weather;
   if( !str_cmp( name, "spell_create_food" ) )
      return spell_create_food;
   if( !str_cmp( name, "spell_create_water" ) )
      return spell_create_water;
   if( !str_cmp( name, "spell_cure_blindness" ) )
      return spell_cure_blindness;
   if( !str_cmp( name, "spell_cure_poison" ) )
      return spell_cure_poison;
   if( !str_cmp( name, "spell_curse" ) )
      return spell_curse;
   if( !str_cmp( name, "spell_detect_poison" ) )
      return spell_detect_poison;
   if( !str_cmp( name, "spell_disenchant_weapon" ) )
      return spell_disenchant_weapon;
   if( !str_cmp( name, "spell_dispel_evil" ) )
      return spell_dispel_evil;
   if( !str_cmp( name, "spell_dispel_magic" ) )
      return spell_dispel_magic;
   if( !str_cmp( name, "spell_dream" ) )
      return spell_dream;
   if( !str_cmp( name, "spell_earthquake" ) )
      return spell_earthquake;
   if( !str_cmp( name, "spell_enchant_armor" ) )
      return spell_enchant_armor;
   if( !str_cmp( name, "spell_enchant_weapon" ) )
      return spell_enchant_weapon;
   if( !str_cmp( name, "spell_energy_drain" ) )
      return spell_energy_drain;
   if( !str_cmp( name, "spell_familiar" ) )
      return spell_familiar;
   if( !str_cmp( name, "spell_faerie_fire" ) )
      return spell_faerie_fire;
   if( !str_cmp( name, "spell_faerie_fog" ) )
      return spell_faerie_fog;
   if( !str_cmp( name, "spell_farsight" ) )
      return spell_farsight;
   if( !str_cmp( name, "spell_fear" ) )
      return spell_fear;   /* fear.c */
   if( !str_cmp( name, "spell_fireball" ) )
      return spell_fireball;
   if( !str_cmp( name, "spell_flamestrike" ) )
      return spell_flamestrike;
   if( !str_cmp( name, "spell_gate" ) )
      return spell_gate;
   if( !str_cmp( name, "spell_knock" ) )
      return spell_knock;
   if( !str_cmp( name, "spell_harm" ) )
      return spell_harm;
   if( !str_cmp( name, "spell_identify" ) )
      return spell_identify;
   if( !str_cmp( name, "spell_inferno" ) )
      return spell_inferno;
   if( !str_cmp( name, "spell_invis" ) )
      return spell_invis;
   if( !str_cmp( name, "spell_know_alignment" ) )
      return spell_know_alignment;
   if( !str_cmp( name, "spell_lightning_bolt" ) )
      return spell_lightning_bolt;
   if( !str_cmp( name, "spell_locate_object" ) )
      return spell_locate_object;
   if( !str_cmp( name, "spell_magic_missile" ) )
      return spell_magic_missile;
   if( !str_cmp( name, "spell_mist_walk" ) )
      return spell_mist_walk;
   if( !str_cmp( name, "spell_pass_door" ) )
      return spell_pass_door;
   if( !str_cmp( name, "spell_plant_pass" ) )
      return spell_plant_pass;
   if( !str_cmp( name, "spell_poison" ) )
      return spell_poison;
   if( !str_cmp( name, "spell_polymorph" ) )
      return spell_polymorph;
   if( !str_cmp( name, "spell_possess" ) )
      return spell_possess;
   if( !str_cmp( name, "spell_recharge" ) )
      return spell_recharge;
   if( !str_cmp( name, "spell_remove_curse" ) )
      return spell_remove_curse;
   if( !str_cmp( name, "spell_remove_invis" ) )
      return spell_remove_invis;
   if( !str_cmp( name, "spell_remove_trap" ) )
      return spell_remove_trap;
   if( !str_cmp( name, "spell_shocking_grasp" ) )
      return spell_shocking_grasp;
   if( !str_cmp( name, "spell_sleep" ) )
      return spell_sleep;
   if( !str_cmp( name, "spell_solar_flight" ) )
      return spell_solar_flight;
   if( !str_cmp( name, "spell_summon" ) )
      return spell_summon;
   if( !str_cmp( name, "spell_teleport" ) )
      return spell_teleport;
   if( !str_cmp( name, "spell_teleport_superior" ) )
      return spell_teleport_superior;
   if( !str_cmp( name, "spell_ventriloquate" ) )
      return spell_ventriloquate;
   if( !str_cmp( name, "spell_weaken" ) )
      return spell_weaken;
   if( !str_cmp( name, "spell_word_of_recall" ) )
      return spell_word_of_recall;
   if( !str_cmp( name, "spell_acid_breath" ) )
      return spell_acid_breath;
   if( !str_cmp( name, "spell_fire_breath" ) )
      return spell_fire_breath;
   if( !str_cmp( name, "spell_frost_breath" ) )
      return spell_frost_breath;
   if( !str_cmp( name, "spell_gas_breath" ) )
      return spell_gas_breath;
   if( !str_cmp( name, "spell_lightning_breath" ) )
      return spell_lightning_breath;
   if( !str_cmp( name, "spell_spiral_blast" ) )
      return spell_spiral_blast;
   if( !str_cmp( name, "spell_scorching_surge" ) )
      return spell_scorching_surge;
   if( !str_cmp( name, "spell_helical_flow" ) )
      return spell_helical_flow;
   if( !str_cmp( name, "spell_transport" ) )
      return spell_transport;
   if( !str_cmp( name, "spell_portal" ) )
      return spell_portal;

   if( !str_cmp( name, "spell_ethereal_fist" ) )
      return spell_ethereal_fist;
   if( !str_cmp( name, "spell_spectral_furor" ) )
      return spell_spectral_furor;
   if( !str_cmp( name, "spell_hand_of_chaos" ) )
      return spell_hand_of_chaos;
   if( !str_cmp( name, "spell_disruption" ) )
      return spell_disruption;
   if( !str_cmp( name, "spell_sonic_resonance" ) )
      return spell_sonic_resonance;
   if( !str_cmp( name, "spell_mind_wrack" ) )
      return spell_mind_wrack;
   if( !str_cmp( name, "spell_mind_wrench" ) )
      return spell_mind_wrench;
   if( !str_cmp( name, "spell_revive" ) )
      return spell_revive;
   if( !str_cmp( name, "spell_sulfurous_spray" ) )
      return spell_sulfurous_spray;
   if( !str_cmp( name, "spell_caustic_fount" ) )
      return spell_caustic_fount;
   if( !str_cmp( name, "spell_acetum_primus" ) )
      return spell_acetum_primus;
   if( !str_cmp( name, "spell_galvanic_whip" ) )
      return spell_galvanic_whip;
   if( !str_cmp( name, "spell_magnetic_thrust" ) )
      return spell_magnetic_thrust;
   if( !str_cmp( name, "spell_quantum_spike" ) )
      return spell_quantum_spike;
   if( !str_cmp( name, "spell_black_hand" ) )
      return spell_black_hand;
   if( !str_cmp( name, "spell_black_fist" ) )
      return spell_black_fist;
   if( !str_cmp( name, "spell_black_lightning" ) )
      return spell_black_lightning;
   if( !str_cmp( name, "spell_midas_touch" ) )
      return spell_midas_touch;
   if( !str_cmp( name, "spell_bethsaidean_touch" ) )
      return spell_bethsaidean_touch;
   if( !str_cmp( name, "spell_expurgation" ) )
      return spell_expurgation;
   if( !str_cmp( name, "spell_sacral_divinity" ) )
      return spell_sacral_divinity;
   if( !str_cmp( name, "spell_life_transfer" ) )
      return spell_life_transfer;

   if( !str_cmp( name, "reserved" ) )
      return NULL;
   if( !str_cmp( name, "spell_null" ) )
      return spell_null;
   return spell_notfound;
}


char *spell_name( SPELL_FUN * spell )
{
   if( spell == spell_smaug )
      return "spell_smaug";
   if( spell == spell_acid_blast )
      return "spell_acid_blast";
   if( spell == spell_animate_dead )
      return "spell_animate_dead";
   if( spell == spell_astral_walk )
      return "spell_astral_walk";
   if( spell == spell_blindness )
      return "spell_blindness";
   if( spell == spell_burning_hands )
      return "spell_burning_hands";
   if( spell == spell_call_lightning )
      return "spell_call_lightning";   
   if( spell == spell_custos_angelus )
      return "spell_custos_angelus";
   if( spell == spell_cause_critical )
      return "spell_cause_critical";
   if( spell == spell_cause_light )
      return "spell_cause_light";
   if( spell == spell_cause_serious )
      return "spell_cause_serious";
   if( spell == spell_change_sex )
      return "spell_change_sex";
   if( spell == spell_charm_person )
      return "spell_charm_person";
   if( spell == spell_chill_touch )
      return "spell_chill_touch";
   if( spell == spell_colour_spray )
      return "spell_colour_spray";
   if( spell == spell_control_weather )
      return "spell_control_weather";
   if( spell == spell_create_food )
      return "spell_create_food";
   if( spell == spell_create_water )
      return "spell_create_water";
   if( spell == spell_cure_blindness )
      return "spell_cure_blindness";
   if( spell == spell_cure_poison )
      return "spell_cure_poison";
   if( spell == spell_curse )
      return "spell_curse";
   if( spell == spell_detect_poison )
      return "spell_detect_poison";
   if( spell == spell_disenchant_weapon )
      return "spell_disenchant_weapon";
   if( spell == spell_dispel_evil )
      return "spell_dispel_evil";
   if( spell == spell_dispel_magic )
      return "spell_dispel_magic";
   if( spell == spell_dream )
      return "spell_dream";
   if( spell == spell_earthquake )
      return "spell_earthquake";
   if( spell == spell_enchant_armor )
      return "spell_enchant_armor";
   if( spell == spell_enchant_weapon )
      return "spell_enchant_weapon";
   if( spell == spell_energy_drain )
      return "spell_energy_drain";
   if( spell == spell_fear )
      return "spell_fear"; /* fear.c */
   if( spell == spell_familiar )
      return "spell_familiar";
   if( spell == spell_faerie_fire )
      return "spell_faerie_fire";
   if( spell == spell_faerie_fog )
      return "spell_faerie_fog";
   if( spell == spell_farsight )
      return "spell_farsight";
   if( spell == spell_fireball )
      return "spell_fireball";
   if( spell == spell_flamestrike )
      return "spell_flamestrike";
   if( spell == spell_gate )
      return "spell_gate";
   if( spell == spell_knock )
      return "spell_knock";
   if( spell == spell_harm )
      return "spell_harm";
   if( spell == spell_identify )
      return "spell_identify";
   if( spell == spell_inferno )
      return "spell_inferno";
   if( spell == spell_invis )
      return "spell_invis";
   if( spell == spell_know_alignment )
      return "spell_know_alignment";
   if( spell == spell_lightning_bolt )
      return "spell_lightning_bolt";
   if( spell == spell_locate_object )
      return "spell_locate_object";
   if( spell == spell_magic_missile )
      return "spell_magic_missile";
   if( spell == spell_mist_walk )
      return "spell_mist_walk";
   if( spell == spell_pass_door )
      return "spell_pass_door";
   if( spell == spell_plant_pass )
      return "spell_plant_pass";
   if( spell == spell_poison )
      return "spell_poison";
   if( spell == spell_polymorph )
      return "spell_polymorph";
   if( spell == spell_possess )
      return "spell_possess";
   if( spell == spell_recharge )
      return "spell_recharge";
   if( spell == spell_remove_curse )
      return "spell_remove_curse";
   if( spell == spell_remove_invis )
      return "spell_remove_invis";
   if( spell == spell_remove_trap )
      return "spell_remove_trap";
   if( spell == spell_shocking_grasp )
      return "spell_shocking_grasp";
   if( spell == spell_sleep )
      return "spell_sleep";
   if( spell == spell_solar_flight )
      return "spell_solar_flight";
   if( spell == spell_summon )
      return "spell_summon";
   if( spell == spell_teleport )
      return "spell_teleport";
   if( spell == spell_teleport_superior )
      return "spell_teleport_superior";
   if( spell == spell_ventriloquate )
      return "spell_ventriloquate";
   if( spell == spell_weaken )
      return "spell_weaken";
   if( spell == spell_word_of_recall )
      return "spell_word_of_recall";
   if( spell == spell_acid_breath )
      return "spell_acid_breath";
   if( spell == spell_fire_breath )
      return "spell_fire_breath";
   if( spell == spell_frost_breath )
      return "spell_frost_breath";
   if( spell == spell_gas_breath )
      return "spell_gas_breath";
   if( spell == spell_lightning_breath )
      return "spell_lightning_breath";
   if( spell == spell_spiral_blast )
      return "spell_spiral_blast";
   if( spell == spell_scorching_surge )
      return "spell_scorching_surge";
   if( spell == spell_helical_flow )
      return "spell_helical_flow";
   if( spell == spell_transport )
      return "spell_transport";
   if( spell == spell_portal )
      return "spell_portal";

   if( spell == spell_ethereal_fist )
      return "spell_ethereal_fist";
   if( spell == spell_spectral_furor )
      return "spell_spectral_furor";
   if( spell == spell_hand_of_chaos )
      return "spell_hand_of_chaos";
   if( spell == spell_disruption )
      return "spell_disruption";
   if( spell == spell_sonic_resonance )
      return "spell_sonic_resonance";
   if( spell == spell_mind_wrack )
      return "spell_mind_wrack";
   if( spell == spell_mind_wrench )
      return "spell_mind_wrench";
   if( spell == spell_revive )
      return "spell_revive";
   if( spell == spell_sulfurous_spray )
      return "spell_sulfurous_spray";
   if( spell == spell_caustic_fount )
      return "spell_caustic_fount";
   if( spell == spell_acetum_primus )
      return "spell_acetum_primus";
   if( spell == spell_galvanic_whip )
      return "spell_galvanic_whip";
   if( spell == spell_magnetic_thrust )
      return "spell_magnetic_thrust";
   if( spell == spell_quantum_spike )
      return "spell_quantum_spike";
   if( spell == spell_black_hand )
      return "spell_black_hand";
   if( spell == spell_black_fist )
      return "spell_black_fist";
   if( spell == spell_black_lightning )
      return "spell_black_lightning";
   if( spell == spell_midas_touch )
      return "spell_midas_touch";
   if( spell == spell_bethsaidean_touch )
      return "spell_bethsaidean_touch";
   if( spell == spell_expurgation )
      return "spell_expurgation";
   if( spell == spell_sacral_divinity )
      return "spell_sacral_divinity";
   if( spell == spell_life_transfer )
      return "spell_life_transfer";


   if( spell == spell_null )
      return "spell_null";
   return "reserved";
}

bool load_class_file( char *fname )
{
   char buf[MAX_STRING_LENGTH];
   char *word;
   bool fMatch;
   struct class_type *class;
   int cl = -1;
   int tlev = 0;
   FILE *fp;

   sprintf( buf, "%s%s", CLASS_DIR, fname );
   if( ( fp = fopen( buf, "r" ) ) == NULL )
   {
      perror( buf );
      return FALSE;
   }

   CREATE( class, struct class_type, 1 );

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
            KEY( "AttrPrime", class->attr_prime, fread_number( fp ) );
            KEY( "Alignment", class->alignment, fread_number( fp ) );
            break;

         case 'C':
            KEY( "Class", cl, fread_number( fp ) );
            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
            {
               fclose( fp );
               if( cl < 0 || cl >= MAX_CLASS )
               {
                  sprintf( buf, "Load_class_file: Class (%s) bad/not found (%d)",
                           class->who_name ? class->who_name : "name not found", cl );
                  bug( buf, 0 );
                  if( class->who_name )
                     STRFREE( class->who_name );
                  DISPOSE( class );
                  return FALSE;
               }
               class_table[cl] = class;
               return TRUE;
            }
            KEY( "ExpBase", class->exp_base, fread_number( fp ) );
            break;

         case 'G':
            KEY( "Guild", class->guild, fread_number( fp ) );
            break;

         case 'H':
            KEY( "HpMax", class->hp_max, fread_number( fp ) );
            KEY( "HpMin", class->hp_min, fread_number( fp ) );
            break;

         case 'M':
            KEY( "Mana", class->fMana, fread_number( fp ) );
            break;

         case 'N':
            KEY( "Name", class->who_name, fread_string( fp ) );
            break;

         case 'S':
            if( !str_cmp( word, "Skill" ) )
            {
               int sn, lev, adp;

               word = fread_word( fp );
               lev = fread_number( fp );
               adp = fread_number( fp );
               sn = skill_lookup( word );
               if( cl < 0 || cl >= MAX_CLASS )
               {
                  sprintf( buf, "load_class_file: Skill %s -- class bad/not found (%d)", word, cl );
                  bug( buf, 0 );
               }
               else if( !IS_VALID_SN( sn ) )
               {
                  sprintf( buf, "load_class_file: Skill %s unknown", word );
                  bug( buf, 0 );
               }
               else
               {
                  skill_table[sn]->skill_level[cl] = lev;
                  skill_table[sn]->skill_adept[cl] = adp;
               }
               fMatch = TRUE;
               break;
            }
            KEY( "Skilladept", class->skill_adept, fread_number( fp ) );
            break;

         case 'T':
            if( !str_cmp( word, "Title" ) )
            {
               if( cl < 0 || cl >= MAX_CLASS )
               {
                  char *tmp;

                  sprintf( buf, "load_class_file: Title -- class bad/not found (%d)", cl );
                  bug( buf, 0 );
                  tmp = fread_string_nohash( fp );
                  DISPOSE( tmp );
                  tmp = fread_string_nohash( fp );
                  DISPOSE( tmp );
               }
               else if( tlev < MAX_LEVEL + 1 )
               {
                  title_table[cl][tlev][0] = fread_string_nohash( fp );
                  title_table[cl][tlev][1] = fread_string_nohash( fp );
                  ++tlev;
               }
               else
                  bug( "load_class_file: Too many titles" );
               fMatch = TRUE;
               break;
            }
            KEY( "Thac0", class->thac0_00, fread_number( fp ) );
            KEY( "Thac32", class->thac0_32, fread_number( fp ) );
            break;

         case 'W':
            KEY( "Weapon", class->weapon, fread_number( fp ) );
            break;
      }
      if( !fMatch )
      {
         sprintf( buf, "load_class_file: no match: %s", word );
         bug( buf, 0 );
      }
   }
   return FALSE;
}

/*
 * Load in all the class files.
 */
void load_classes(  )
{
   FILE *fpList;
   char *filename;
   char classlist[256];
   char buf[MAX_STRING_LENGTH];

   sprintf( classlist, "%s%s", CLASS_DIR, CLASS_LIST );
   if( ( fpList = fopen( classlist, "r" ) ) == NULL )
   {
      perror( classlist );
      exit( 1 );
   }

   for( ;; )
   {
      filename = feof( fpList ) ? "$" : fread_word( fpList );
      if( filename[0] == '$' )
         break;

      if( !load_class_file( filename ) )
      {
         sprintf( buf, "Cannot load class file: %s", filename );
         bug( buf, 0 );
      }
   }
   fclose( fpList );
   return;
}


void write_class_file( int cl )
{
   FILE *fpout;
   char buf[MAX_STRING_LENGTH];
   char filename[MAX_INPUT_LENGTH];
   struct class_type *class = class_table[cl];
   int x, y;

   sprintf( filename, "%s%s.class", CLASSDIR, class->who_name );
   if( ( fpout = fopen( filename, "w" ) ) == NULL )
   {
      sprintf( buf, "Cannot open: %s for writing", filename );
      bug( buf, 0 );
      return;
   }
   fprintf( fpout, "Name        %s~\n", class->who_name );
   fprintf( fpout, "Class       %d\n", cl );
   fprintf( fpout, "Attrprime   %d\n", class->attr_prime );
   fprintf( fpout, "Alignment   %d\n", class->alignment );
   fprintf( fpout, "Weapon      %d\n", class->weapon );
   fprintf( fpout, "Guild       %d\n", class->guild );
   fprintf( fpout, "Skilladept  %d\n", class->skill_adept );
   fprintf( fpout, "Thac0       %d\n", class->thac0_00 );
   fprintf( fpout, "Thac32      %d\n", class->thac0_32 );
   fprintf( fpout, "Hpmin       %d\n", class->hp_min );
   fprintf( fpout, "Hpmax       %d\n", class->hp_max );
   fprintf( fpout, "Mana        %d\n", class->fMana );
   fprintf( fpout, "Expbase     %d\n", class->exp_base );

   for( x = 0; x < top_sn; x++ )
   {
      if( !skill_table[x]->name || skill_table[x]->name[0] == '\0' )
         break;
      if( ( y = skill_table[x]->skill_level[cl] ) < LEVEL_IMMORTAL )
         fprintf( fpout, "Skill '%s' %d %d\n", skill_table[x]->name, y, skill_table[x]->skill_adept[cl] );
   }
   for( x = 0; x <= MAX_LEVEL; x++ )
      fprintf( fpout, "Title\n%s~\n%s~\n", title_table[cl][x][0], title_table[cl][x][1] );
   fprintf( fpout, "End\n" );
   fclose( fpout );
}


/*
 * Load in all the race files.
 */
void load_races(  )
{
   FILE *fpList;
   char *filename;
   char racelist[256];
   char buf[MAX_STRING_LENGTH];
   int i;
   /*
    * Pre-init the race_table with blank races
    */
   for( i = 0; i < MAX_RACE; i++ )
      race_table[i] = NULL;

   sprintf( racelist, "%s%s", RACEDIR, RACE_LIST );
   if( ( fpList = fopen( racelist, "r" ) ) == NULL )
   {
      perror( racelist );
      exit( 1 );
   }

   for( ;; )
   {
      filename = feof( fpList ) ? "$" : fread_word( fpList );
      if( filename[0] == '$' )
         break;

      if( !load_race_file( filename ) )
      {
         sprintf( buf, "Cannot load race file: %s", filename );
         bug( buf, 0 );
      }
   }
   for( i = 0; i < MAX_RACE; i++ )
   {
      if( race_table[i] == NULL )
      {
         CREATE( race_table[i], struct race_type, 1 );
         sprintf( race_table[i]->race_name, "%s", "unused" );
      }
   }
   fclose( fpList );
   return;
}

void write_race_file( int ra )
{
   FILE *fpout;
   char buf[MAX_STRING_LENGTH];
   char filename[MAX_INPUT_LENGTH];
   struct race_type *race = race_table[ra];
   int i;
   int x, y;

   if( !race->race_name )
   {
      sprintf( buf, "Race %d has null name, not writing .race file.", ra );
      bug( buf, 0 );
      return;
   }

   sprintf( filename, "%s%s.race", RACEDIR, race->race_name );
   if( ( fpout = fopen( filename, "w+" ) ) == NULL )
   {
      sprintf( buf, "Cannot open: %s for writing", filename );
      bug( buf, 0 );
      return;
   }

   fprintf( fpout, "Name        %s~\n", race->race_name );
   fprintf( fpout, "Race        %d\n", ra );
   fprintf( fpout, "Classes     %d\n", race->class_restriction );
   fprintf( fpout, "Str_Plus    %d\n", race->str_plus );
   fprintf( fpout, "Dex_Plus    %d\n", race->dex_plus );
   fprintf( fpout, "Wis_Plus    %d\n", race->wis_plus );
   fprintf( fpout, "Int_Plus    %d\n", race->int_plus );
   fprintf( fpout, "Con_Plus    %d\n", race->con_plus );
   fprintf( fpout, "Cha_Plus    %d\n", race->cha_plus );
   fprintf( fpout, "Lck_Plus    %d\n", race->lck_plus );
   fprintf( fpout, "Hit         %d\n", race->hit );
   fprintf( fpout, "Mana        %d\n", race->mana );
   fprintf( fpout, "Affected    %s\n", print_bitvector( &race->affected ) );
   fprintf( fpout, "Resist      %d\n", race->resist );
   fprintf( fpout, "Suscept     %d\n", race->suscept );
   fprintf( fpout, "Language    %d\n", race->language );
   fprintf( fpout, "Align       %d\n", race->alignment );
   fprintf( fpout, "Min_Align  %d\n", race->minalign );
   fprintf( fpout, "Max_Align	%d\n", race->maxalign );
   fprintf( fpout, "AC_Plus    %d\n", race->ac_plus );
   fprintf( fpout, "Exp_Mult   %d\n", race->exp_multiplier );
   fprintf( fpout, "Attacks    %s\n", print_bitvector( &race->attacks ) );
   fprintf( fpout, "Defenses   %s\n", print_bitvector( &race->defenses ) );
   fprintf( fpout, "Height     %d\n", race->height );
   fprintf( fpout, "Weight     %d\n", race->weight );
   fprintf( fpout, "Hunger_Mod  %d\n", race->hunger_mod );
   fprintf( fpout, "Thirst_mod  %d\n", race->thirst_mod );
   fprintf( fpout, "Mana_Regen  %d\n", race->mana_regen );
   fprintf( fpout, "HP_Regen    %d\n", race->hp_regen );
   fprintf( fpout, "Race_Recall %d\n", race->race_recall );
   fprintf( fpout, "Base_age	%d\n", race->base_age );
   for( i = 0; i < MAX_WHERE_NAME; i++ )
      fprintf( fpout, "WhereName  %s~\n", race->where_name[i] );

/* Save these for later */
   fprintf( fpout, "HairColor  %d\n", race->attribs[HAIR_COLOR] );
   fprintf( fpout, "HairLength %d\n", race->attribs[HAIR_LENGTH] );
   fprintf( fpout, "EyeColor   %d\n", race->attribs[EYE_COLOR] );
   fprintf( fpout, "SkinTone   %d\n", race->attribs[SKIN_TONE] );
   fprintf( fpout, "Build      %d\n", race->attribs[RACE_BUILD] );
   fprintf( fpout, "RaceHeight %d\n", race->attribs[RACE_HEIGHT] );

   for( x = 0; x < top_sn; x++ )
   {
      if( !skill_table[x]->name || skill_table[x]->name[0] == '\0' )
         break;
      if( ( y = skill_table[x]->race_level[ra] ) < LEVEL_IMMORTAL )
         fprintf( fpout, "Skill '%s' %d %d\n", skill_table[x]->name, y, skill_table[x]->race_adept[ra] );
   }
   fprintf( fpout, "End\n" );
   fclose( fpout );
}

bool load_race_file( char *fname )
{
   char buf[MAX_STRING_LENGTH];
   char *word;
   char *race_name = NULL;
   bool fMatch;
   struct race_type *race;
   int ra = -1;
   FILE *fp;
   int i, wear = 0;

   sprintf( buf, "%s%s", RACEDIR, fname );
   if( ( fp = fopen( buf, "r" ) ) == NULL )
   {
      perror( buf );
      return FALSE;
   }

   CREATE( race, struct race_type, 1 );
   for( i = 0; i < MAX_WHERE_NAME; i++ )
      race->where_name[i] = str_dup( where_name[i] );

   race->attribs[HAIR_COLOR] = -1;
   race->attribs[HAIR_LENGTH] = -1;
   race->attribs[EYE_COLOR] = -1;
   race->attribs[SKIN_TONE] = -1;
   race->attribs[RACE_BUILD] = -1;
   race->attribs[RACE_HEIGHT] = -1;

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
            KEY( "Align", race->alignment, fread_number( fp ) );
            KEY( "AC_Plus", race->ac_plus, fread_number( fp ) );
            KEY( "Affected", race->affected, fread_bitvector( fp ) );
            KEY( "Attacks", race->attacks, fread_bitvector( fp ) );
            break;

         case 'B':
            KEY( "Base_age", race->base_age, fread_number( fp ) );
/* Save these for later */
            KEY( "Build", race->attribs[RACE_HEIGHT], fread_number( fp ) );
            break;

         case 'C':
            KEY( "Con_Plus", race->con_plus, fread_number( fp ) );
            KEY( "Cha_Plus", race->cha_plus, fread_number( fp ) );
            KEY( "Classes", race->class_restriction, fread_number( fp ) );
            break;


         case 'D':
            KEY( "Dex_Plus", race->dex_plus, fread_number( fp ) );
            KEY( "Defenses", race->defenses, fread_bitvector( fp ) );
            break;

         case 'E':
/* Save these for later */
            KEY( "EyeColor", race->attribs[EYE_COLOR], fread_number( fp ) );

            if( !str_cmp( word, "End" ) )
            {
               fclose( fp );
               fp = NULL;
               if( ra < 0 || ra >= MAX_RACE )
               {
                  bug( "Load_race_file: Race (%s) bad/not found (%d)",
                       race->race_name ? race->race_name : "name not found", ra );
                  STRFREE( race_name );
                  for( i = 0; i < MAX_WHERE_NAME; ++i )
                     DISPOSE( race->where_name[i] );
                  DISPOSE( race );
                  return FALSE;
               }

               if( race->attribs[HAIR_COLOR] == -1 )
               {
                  race->attribs[HAIR_COLOR] = 0;
                  for( i = 0; i < 18; i++ )
                     SET_BIT( race->attribs[HAIR_COLOR], 1 << i );
               }

               if( race->attribs[HAIR_LENGTH] == -1 )
               {
                  race->attribs[HAIR_LENGTH] = 0;
                  for( i = 0; i < 7; i++ )
                     SET_BIT( race->attribs[HAIR_LENGTH], 1 << i );
               }

               if( race->attribs[EYE_COLOR] == -1 )
               {
                  race->attribs[EYE_COLOR] = 0;
                  for( i = 0; i < 15; i++ )
                     SET_BIT( race->attribs[EYE_COLOR], 1 << i );
               }

               if( race->attribs[SKIN_TONE] == -1 )
               {
                  race->attribs[SKIN_TONE] = 0;
                  for( i = 0; i < 7; i++ )
                     SET_BIT( race->attribs[SKIN_TONE], 1 << i );
               }

               if( race->attribs[RACE_BUILD] == -1 )
               {
                  race->attribs[RACE_BUILD] = 0;
                  for( i = 0; i < 5; i++ )
                     SET_BIT( race->attribs[RACE_BUILD], 1 << i );
               }

               if( race->attribs[RACE_HEIGHT] == -1 )
               {
                  race->attribs[RACE_HEIGHT] = 0;
                  for( i = 0; i < 5; i++ )
                     SET_BIT( race->attribs[RACE_HEIGHT], 1 << i );
               }

               /*
                * for( i = 0 ; i < MAX_TYPES ; i++ )
                * {
                * if( race->attribs[i] == -1 )
                * {
                * SET_BIT( race->attribs, HAIR_COLORS );
                * } 
                */
               race_table[ra] = race;
               return TRUE;
            }

            KEY( "Exp_Mult", race->exp_multiplier, fread_number( fp ) );

            break;


         case 'I':
            KEY( "Int_Plus", race->int_plus, fread_number( fp ) );
            break;

         case 'H':
/* Save these for later */
            KEY( "HairColor", race->attribs[HAIR_COLOR], fread_number( fp ) );
            KEY( "HairLength", race->attribs[HAIR_LENGTH], fread_number( fp ) );
            KEY( "Height", race->height, fread_number( fp ) );
            KEY( "Hit", race->hit, fread_number( fp ) );
            KEY( "HP_Regen", race->hp_regen, fread_number( fp ) );
            KEY( "Hunger_Mod", race->hunger_mod, fread_number( fp ) );
            break;

         case 'L':
            KEY( "Language", race->language, fread_number( fp ) );
            KEY( "Lck_Plus", race->lck_plus, fread_number( fp ) );
            break;


         case 'M':
            KEY( "Mana", race->mana, fread_number( fp ) );
            KEY( "Mana_Regen", race->mana_regen, fread_number( fp ) );
            KEY( "Min_Align", race->minalign, fread_number( fp ) );
            KEY( "Max_Align", race->maxalign, fread_number( fp ) );
            break;

         case 'N':
            KEY( "Name", race_name, fread_string( fp ) );
            break;

         case 'R':
            KEY( "Race", ra, fread_number( fp ) );
            KEY( "RaceHeight", race->attribs[RACE_BUILD], fread_number( fp ) );
            KEY( "Race_Recall", race->race_recall, fread_number( fp ) );
            KEY( "Resist", race->resist, fread_number( fp ) );
            break;

         case 'S':
            KEY( "Str_Plus", race->str_plus, fread_number( fp ) );
/* Save these for later */
            KEY( "SkinTone", race->attribs[SKIN_TONE], fread_number( fp ) );

            KEY( "Suscept", race->suscept, fread_number( fp ) );
            if( !str_cmp( word, "Skill" ) )
            {
               int sn, lev, adp;

               word = fread_word( fp );
               lev = fread_number( fp );
               adp = fread_number( fp );
               sn = skill_lookup( word );
               if( ra < 0 || ra >= MAX_RACE )
               {
                  sprintf( buf, "load_race_file: Skill %s -- race bad/not found (%d)", word, ra );
                  bug( buf, 0 );
               }
               else if( !IS_VALID_SN( sn ) )
               {
                  sprintf( buf, "load_race_file: Skill %s unknown", word );
                  bug( buf, 0 );
               }
               else
               {
                  skill_table[sn]->race_level[ra] = lev;
                  skill_table[sn]->race_adept[ra] = adp;
               }
               fMatch = TRUE;
               break;
            }
            break;

         case 'T':
            KEY( "Thirst_Mod", race->thirst_mod, fread_number( fp ) );
            break;

         case 'W':
            KEY( "Weight", race->weight, fread_number( fp ) );
            KEY( "Wis_Plus", race->wis_plus, fread_number( fp ) );
            if( !str_cmp( word, "WhereName" ) )
            {
               if( ra < 0 || ra >= MAX_RACE )
               {
                  char *tmp;

                  bug( "load_race_file: Title -- race bad/not found (%d)", ra );
                  tmp = fread_string_nohash( fp );
                  DISPOSE( tmp );
                  tmp = fread_string_nohash( fp );
                  DISPOSE( tmp );
               }
               else if( wear < MAX_WHERE_NAME )
               {
                  DISPOSE( race->where_name[wear] );
                  race->where_name[wear] = fread_string_nohash( fp );
                  ++wear;
               }
               else
                  bug( "load_race_file: Too many where_names" );
               fMatch = TRUE;
               break;
            }
            break;
      }

      if( race_name != NULL )
         sprintf( race->race_name, "%-.16s", race_name );

      if( !fMatch )
      {
         sprintf( buf, "load_race_file: no match: %s", word );
         bug( buf, 0 );
      }
   }
   return FALSE;
}

/*
 * Function used by qsort to sort skills
 */
int skill_comp( SKILLTYPE ** sk1, SKILLTYPE ** sk2 )
{
   SKILLTYPE *skill1 = ( *sk1 );
   SKILLTYPE *skill2 = ( *sk2 );

   if( !skill1 && skill2 )
      return 1;
   if( skill1 && !skill2 )
      return -1;
   if( !skill1 && !skill2 )
      return 0;
   if( skill1->type < skill2->type )
      return -1;
   if( skill1->type > skill2->type )
      return 1;
   return strcmp( skill1->name, skill2->name );
}

/*
 * Sort the skill table with qsort
 */
void sort_skill_table(  )
{
   log_string( "Sorting skill table..." );
   qsort( &skill_table[1], top_sn - 1, sizeof( SKILLTYPE * ), ( int ( * )( const void *, const void * ) )skill_comp );
}


/*
 * Remap slot numbers to sn values
 */
void remap_slot_numbers(  )
{
   SKILLTYPE *skill;
   SMAUG_AFF *aff;
   char tmp[32];
   int sn;

   log_string( "Remapping slots to sns" );

   for( sn = 0; sn <= top_sn; sn++ )
   {
      if( ( skill = skill_table[sn] ) != NULL )
      {
         for( aff = skill->affects; aff; aff = aff->next )
            if( aff->location == APPLY_WEAPONSPELL
                || aff->location == APPLY_WEARSPELL
                || aff->location == APPLY_REMOVESPELL
                || aff->location == APPLY_STRIPSN || aff->location == APPLY_RECURRINGSPELL )
            {
               sprintf( tmp, "%d", slot_lookup( atoi( aff->modifier ) ) );
               DISPOSE( aff->modifier );
               aff->modifier = str_dup( tmp );
            }
      }
   }
}

/*
 * Write skill data to a file
 */
void fwrite_skill( FILE * fpout, SKILLTYPE * skill )
{
   SMAUG_AFF *aff;
   int modifier;

   fprintf( fpout, "Name         %s~\n", skill->name );
   fprintf( fpout, "Type         %s\n", skill_tname[skill->type] );
   fprintf( fpout, "Info         %d\n", skill->info );
   fprintf( fpout, "Flags        %d\n", skill->flags );
   if( skill->target )
      fprintf( fpout, "Target       %d\n", skill->target );
   /*
    * store as new minpos (minpos>=100 flags new style in character loading)
    */
   if( skill->minimum_position )
      fprintf( fpout, "Minpos       %d\n", skill->minimum_position + 100 );
   if( skill->saves )
      fprintf( fpout, "Saves        %d\n", skill->saves );
   if( skill->slot )
      fprintf( fpout, "Slot         %d\n", skill->slot );
   if( skill->min_mana )
      fprintf( fpout, "Mana         %d\n", skill->min_mana );
   if( skill->beats )
      fprintf( fpout, "Rounds       %d\n", skill->beats );
   if( skill->range )
      fprintf( fpout, "Range        %d\n", skill->range );
   if( skill->guild != -1 )
      fprintf( fpout, "Guild        %d\n", skill->guild );
   if( skill->skill_fun )
      fprintf( fpout, "Code         %s\n", skill_name( skill->skill_fun ) );
   else if( skill->spell_fun )
      fprintf( fpout, "Code         %s\n", spell_name( skill->spell_fun ) );
   fprintf( fpout, "Dammsg       %s~\n", skill->noun_damage );
   if( skill->msg_off && skill->msg_off[0] != '\0' )
      fprintf( fpout, "Wearoff      %s~\n", skill->msg_off );

   if( skill->hit_char && skill->hit_char[0] != '\0' )
      fprintf( fpout, "Hitchar      %s~\n", skill->hit_char );
   if( skill->hit_vict && skill->hit_vict[0] != '\0' )
      fprintf( fpout, "Hitvict      %s~\n", skill->hit_vict );
   if( skill->hit_room && skill->hit_room[0] != '\0' )
      fprintf( fpout, "Hitroom      %s~\n", skill->hit_room );
   if( skill->hit_dest && skill->hit_dest[0] != '\0' )
      fprintf( fpout, "Hitdest      %s~\n", skill->hit_dest );

   if( skill->miss_char && skill->miss_char[0] != '\0' )
      fprintf( fpout, "Misschar     %s~\n", skill->miss_char );
   if( skill->miss_vict && skill->miss_vict[0] != '\0' )
      fprintf( fpout, "Missvict     %s~\n", skill->miss_vict );
   if( skill->miss_room && skill->miss_room[0] != '\0' )
      fprintf( fpout, "Missroom     %s~\n", skill->miss_room );

   if( skill->die_char && skill->die_char[0] != '\0' )
      fprintf( fpout, "Diechar      %s~\n", skill->die_char );
   if( skill->die_vict && skill->die_vict[0] != '\0' )
      fprintf( fpout, "Dievict      %s~\n", skill->die_vict );
   if( skill->die_room && skill->die_room[0] != '\0' )
      fprintf( fpout, "Dieroom      %s~\n", skill->die_room );

   if( skill->imm_char && skill->imm_char[0] != '\0' )
      fprintf( fpout, "Immchar      %s~\n", skill->imm_char );
   if( skill->imm_vict && skill->imm_vict[0] != '\0' )
      fprintf( fpout, "Immvict      %s~\n", skill->imm_vict );
   if( skill->imm_room && skill->imm_room[0] != '\0' )
      fprintf( fpout, "Immroom      %s~\n", skill->imm_room );

   if( skill->dice && skill->dice[0] != '\0' )
      fprintf( fpout, "Dice         %s~\n", skill->dice );
   if( skill->value )
      fprintf( fpout, "Value        %d\n", skill->value );
   if( skill->difficulty )
      fprintf( fpout, "Difficulty   %d\n", skill->difficulty );
   if( skill->participants )
      fprintf( fpout, "Participants %d\n", skill->participants );
   if( skill->components && skill->components[0] != '\0' )
      fprintf( fpout, "Components   %s~\n", skill->components );
   if( skill->teachers && skill->teachers[0] != '\0' )
      fprintf( fpout, "Teachers     %s~\n", skill->teachers );
   for( aff = skill->affects; aff; aff = aff->next )
   {
      fprintf( fpout, "Affect       '%s' %d ", aff->duration, aff->location );
      modifier = atoi( aff->modifier );
      if( ( aff->location == APPLY_WEAPONSPELL
            || aff->location == APPLY_WEARSPELL
            || aff->location == APPLY_REMOVESPELL
            || aff->location == APPLY_STRIPSN || aff->location == APPLY_RECURRINGSPELL ) && IS_VALID_SN( modifier ) )
         fprintf( fpout, "'%d' ", skill_table[modifier]->slot );
      else
         fprintf( fpout, "'%s' ", aff->modifier );
      fprintf( fpout, "%d\n", aff->bitvector );
   }

   if( skill->type != SKILL_HERB )
   {
      int y;
      int min = 1000;
      for( y = 0; y < MAX_CLASS; y++ )
         if( skill->skill_level[y] < min )
            min = skill->skill_level[y];

      fprintf( fpout, "Minlevel     %d\n", min );

      min = 1000;
      for( y = 0; y < MAX_RACE; y++ )
         if( skill->race_level[y] < min )
            min = skill->race_level[y];

   }
   fprintf( fpout, "End\n\n" );
}

/*
 * Save the skill table to disk
 */
void save_skill_table(  )
{
   int x;
   FILE *fpout;

   if( ( fpout = fopen( SKILL_FILE, "w" ) ) == NULL )
   {
      bug( "Cannot open skills.dat for writting", 0 );
      perror( SKILL_FILE );
      return;
   }

   for( x = 0; x < top_sn; x++ )
   {
      if( !skill_table[x]->name || skill_table[x]->name[0] == '\0' )
         break;
      fprintf( fpout, "#SKILL\n" );
      fwrite_skill( fpout, skill_table[x] );
   }
   fprintf( fpout, "#END\n" );
   fclose( fpout );
}

/*
 * Save the herb table to disk
 */
void save_herb_table(  )
{
   int x;
   FILE *fpout;

   if( ( fpout = fopen( HERB_FILE, "w" ) ) == NULL )
   {
      bug( "Cannot open herbs.dat for writting", 0 );
      perror( HERB_FILE );
      return;
   }

   for( x = 0; x < top_herb; x++ )
   {
      if( !herb_table[x]->name || herb_table[x]->name[0] == '\0' )
         break;
      fprintf( fpout, "#HERB\n" );
      fwrite_skill( fpout, herb_table[x] );
   }
   fprintf( fpout, "#END\n" );
   fclose( fpout );
}

/*
 * Save the socials to disk
 */
void save_socials(  )
{
   FILE *fpout;
   SOCIALTYPE *social;
   int x;

   if( ( fpout = fopen( SOCIAL_FILE, "w" ) ) == NULL )
   {
      bug( "Cannot open socials.dat for writting", 0 );
      perror( SOCIAL_FILE );
      return;
   }

   for( x = 0; x < 27; x++ )
   {
      for( social = social_index[x]; social; social = social->next )
      {
         if( !social->name || social->name[0] == '\0' )
         {
            bug( "Save_socials: blank social in hash bucket %d", x );
            continue;
         }
         fprintf( fpout, "#SOCIAL\n" );
         fprintf( fpout, "Name        %s~\n", social->name );
         if( social->char_no_arg )
            fprintf( fpout, "CharNoArg   %s~\n", social->char_no_arg );
         else
            bug( "Save_socials: NULL char_no_arg in hash bucket %d", x );
         if( social->others_no_arg )
            fprintf( fpout, "OthersNoArg %s~\n", social->others_no_arg );
         if( social->char_found )
            fprintf( fpout, "CharFound   %s~\n", social->char_found );
         if( social->others_found )
            fprintf( fpout, "OthersFound %s~\n", social->others_found );
         if( social->vict_found )
            fprintf( fpout, "VictFound   %s~\n", social->vict_found );
         if( social->char_auto )
            fprintf( fpout, "CharAuto    %s~\n", social->char_auto );
         if( social->others_auto )
            fprintf( fpout, "OthersAuto  %s~\n", social->others_auto );
         fprintf( fpout, "End\n\n" );
      }
   }
   fprintf( fpout, "#END\n" );
   fclose( fpout );
}

int get_skill( char *skilltype )
{
   if( !str_cmp( skilltype, "Race" ) )
      return SKILL_RACIAL;
   if( !str_cmp( skilltype, "Spell" ) )
      return SKILL_SPELL;
   if( !str_cmp( skilltype, "Skill" ) )
      return SKILL_SKILL;
   if( !str_cmp( skilltype, "Weapon" ) )
      return SKILL_WEAPON;
   if( !str_cmp( skilltype, "Tongue" ) )
      return SKILL_TONGUE;
   if( !str_cmp( skilltype, "Herb" ) )
      return SKILL_HERB;
   return SKILL_UNKNOWN;
}

/*
 * Save the commands to disk
 * Added flags Aug 25, 1997 --Shaddai
 */
void save_commands(  )
{
   FILE *fpout;
   CMDTYPE *command;
   int x;

   if( ( fpout = fopen( COMMAND_FILE, "w" ) ) == NULL )
   {
      bug( "Cannot open commands.dat for writing", 0 );
      perror( COMMAND_FILE );
      return;
   }

   for( x = 0; x < 126; x++ )
   {
      for( command = command_hash[x]; command; command = command->next )
      {
         if( !command->name || command->name[0] == '\0' )
         {
            bug( "Save_commands: blank command in hash bucket %d", x );
            continue;
         }
         fprintf( fpout, "#COMMAND\n" );
         fprintf( fpout, "Name        %s~\n", command->name );
         fprintf( fpout, "Code        %s\n", skill_name( command->do_fun ) );
/* Oops I think this may be a bad thing so I changed it -- Shaddai */
         if( command->position < 100 )
            fprintf( fpout, "Position    %d\n", command->position + 100 );
         else
            fprintf( fpout, "Position    %d\n", command->position );
         fprintf( fpout, "Level       %d\n", command->level );
         fprintf( fpout, "CShow       %d\n", command->cshow );
         fprintf( fpout, "Log         %d\n", command->log );
         if( command->flags )
            fprintf( fpout, "Flags       %d\n", command->flags );
         fprintf( fpout, "End\n\n" );
      }
   }
   fprintf( fpout, "#END\n" );
   fclose( fpout );
}

SKILLTYPE *fread_skill( FILE * fp )
{
   char buf[MAX_STRING_LENGTH];
   char *word;
   bool fMatch;
   bool got_info = FALSE;
   SKILLTYPE *skill;
   int x;

   CREATE( skill, SKILLTYPE, 1 );
   skill->slot = 0;
   skill->min_mana = 0;
   for( x = 0; x < MAX_CLASS; x++ )
   {
      skill->skill_level[x] = LEVEL_IMMORTAL;
      skill->skill_adept[x] = 95;
   }
   for( x = 0; x < MAX_RACE; x++ )
   {
      skill->race_level[x] = LEVEL_IMMORTAL;
      skill->race_adept[x] = 95;
   }
   skill->guild = -1;
   skill->target = 0;
   skill->skill_fun = NULL;
   skill->spell_fun = NULL;

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
            if( !str_cmp( word, "Affect" ) )
            {
               SMAUG_AFF *aff;

               CREATE( aff, SMAUG_AFF, 1 );
               aff->duration = str_dup( fread_word( fp ) );
               aff->location = fread_number( fp );
               aff->modifier = str_dup( fread_word( fp ) );
               aff->bitvector = fread_number( fp );
               if( !got_info )
               {
                  for( x = 0; x < 32; x++ )
                  {
                     if( IS_SET( aff->bitvector, 1 << x ) )
                     {
                        aff->bitvector = x;
                        break;
                     }
                  }
                  if( x == 32 )
                     aff->bitvector = -1;
               }
               aff->next = skill->affects;
               skill->affects = aff;
               fMatch = TRUE;
               break;
            }
            break;

         case 'C':
            if( !str_cmp( word, "Class" ) )
            {
               int class = fread_number( fp );

               skill->skill_level[class] = fread_number( fp );
               skill->skill_adept[class] = fread_number( fp );
               fMatch = TRUE;
               break;
            }
            if( !str_cmp( word, "Code" ) )
            {
               SPELL_FUN *spellfun;
               DO_FUN *dofun;
               char *w = fread_word( fp );

               fMatch = TRUE;
               if( ( spellfun = spell_function( w ) ) != spell_notfound )
               {
                  skill->spell_fun = spellfun;
                  skill->skill_fun = NULL;
               }
               else if( ( dofun = skill_function( w ) ) != skill_notfound )
               {
                  skill->skill_fun = dofun;
                  skill->spell_fun = NULL;
               }
               else
               {
                  bug( "fread_skill: unknown skill/spell %s", w );
                  skill->spell_fun = spell_null;
               }
               break;
            }
            KEY( "Code", skill->spell_fun, spell_function( fread_word( fp ) ) );
            KEY( "Components", skill->components, fread_string_nohash( fp ) );
            break;

         case 'D':
            KEY( "Dammsg", skill->noun_damage, fread_string_nohash( fp ) );
            KEY( "Dice", skill->dice, fread_string_nohash( fp ) );
            KEY( "Diechar", skill->die_char, fread_string_nohash( fp ) );
            KEY( "Dieroom", skill->die_room, fread_string_nohash( fp ) );
            KEY( "Dievict", skill->die_vict, fread_string_nohash( fp ) );
            KEY( "Difficulty", skill->difficulty, fread_number( fp ) );
            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
            {
               if( skill->saves != 0 && SPELL_SAVE( skill ) == SE_NONE )
               {
                  bug( "fread_skill(%s):  Has saving throw (%d) with no saving effect.", skill->name, skill->saves );
                  SET_SSAV( skill, SE_NEGATE );
               }
               return skill;
            }
            break;

         case 'F':
            if( !str_cmp( word, "Flags" ) )
            {
               skill->flags = fread_number( fp );
               /*
                * convert to new style       -Thoric
                */
               if( !got_info )
               {
                  skill->info = skill->flags & ( BV11 - 1 );
                  if( IS_SET( skill->flags, OLD_SF_SAVE_NEGATES ) )
                  {
                     if( IS_SET( skill->flags, OLD_SF_SAVE_HALF_DAMAGE ) )
                     {
                        SET_SSAV( skill, SE_QUARTERDAM );
                        REMOVE_BIT( skill->flags, OLD_SF_SAVE_HALF_DAMAGE );
                     }
                     else
                        SET_SSAV( skill, SE_NEGATE );
                     REMOVE_BIT( skill->flags, OLD_SF_SAVE_NEGATES );
                  }
                  else if( IS_SET( skill->flags, OLD_SF_SAVE_HALF_DAMAGE ) )
                  {
                     SET_SSAV( skill, SE_HALFDAM );
                     REMOVE_BIT( skill->flags, OLD_SF_SAVE_HALF_DAMAGE );
                  }
                  skill->flags >>= 11;
               }
               fMatch = TRUE;
               break;
            }
            break;

         case 'G':
            KEY( "Guild", skill->guild, fread_number( fp ) );
            break;

         case 'H':
            KEY( "Hitchar", skill->hit_char, fread_string_nohash( fp ) );
            KEY( "Hitdest", skill->hit_dest, fread_string_nohash( fp ) );
            KEY( "Hitroom", skill->hit_room, fread_string_nohash( fp ) );
            KEY( "Hitvict", skill->hit_vict, fread_string_nohash( fp ) );
            break;

         case 'I':
            KEY( "Immchar", skill->imm_char, fread_string_nohash( fp ) );
            KEY( "Immroom", skill->imm_room, fread_string_nohash( fp ) );
            KEY( "Immvict", skill->imm_vict, fread_string_nohash( fp ) );
            if( !str_cmp( word, "Info" ) )
            {
               skill->info = fread_number( fp );
               got_info = TRUE;
               fMatch = TRUE;
               break;
            }
            break;

         case 'M':
            KEY( "Mana", skill->min_mana, fread_number( fp ) );
            if( !str_cmp( word, "Minlevel" ) )
            {
               fread_to_eol( fp );
               fMatch = TRUE;
               break;
            }
            /*
             * KEY( "Minpos",   skill->minimum_position, fread_number( fp ) ); 
             */
            /*
             * 
             */
            if( !str_cmp( word, "Minpos" ) )
            {
               fMatch = TRUE;
               skill->minimum_position = fread_number( fp );
               if( skill->minimum_position < 100 )
               {
                  switch ( skill->minimum_position )
                  {
                     default:
                     case 0:
                     case 1:
                     case 2:
                     case 3:
                     case 4:
                        break;
                     case 5:
                        skill->minimum_position = 6;
                        break;
                     case 6:
                        skill->minimum_position = 8;
                        break;
                     case 7:
                        skill->minimum_position = 9;
                        break;
                     case 8:
                        skill->minimum_position = 12;
                        break;
                     case 9:
                        skill->minimum_position = 13;
                        break;
                     case 10:
                        skill->minimum_position = 14;
                        break;
                     case 11:
                        skill->minimum_position = 15;
                        break;
                  }
               }
               else
                  skill->minimum_position -= 100;
               break;
            }

            KEY( "Misschar", skill->miss_char, fread_string_nohash( fp ) );
            KEY( "Missroom", skill->miss_room, fread_string_nohash( fp ) );
            KEY( "Missvict", skill->miss_vict, fread_string_nohash( fp ) );
            break;

         case 'N':
            KEY( "Name", skill->name, fread_string_nohash( fp ) );
            break;

         case 'P':
            KEY( "Participants", skill->participants, fread_number( fp ) );
            break;

         case 'R':
            KEY( "Range", skill->range, fread_number( fp ) );
            KEY( "Rounds", skill->beats, fread_number( fp ) );
            if( !str_cmp( word, "Race" ) )
            {
               int race = fread_number( fp );

               skill->race_level[race] = fread_number( fp );
               skill->race_adept[race] = fread_number( fp );
               fMatch = TRUE;
               break;
            }
            break;

         case 'S':
            KEY( "Slot", skill->slot, fread_number( fp ) );
            KEY( "Saves", skill->saves, fread_number( fp ) );
            break;

         case 'T':
            KEY( "Target", skill->target, fread_number( fp ) );
            KEY( "Teachers", skill->teachers, fread_string_nohash( fp ) );
            KEY( "Type", skill->type, get_skill( fread_word( fp ) ) );
            break;

         case 'V':
            KEY( "Value", skill->value, fread_number( fp ) );
            break;

         case 'W':
            KEY( "Wearoff", skill->msg_off, fread_string_nohash( fp ) );
            break;
      }

      if( !fMatch )
      {
         sprintf( buf, "Fread_skill: no match: %s", word );
         bug( buf, 0 );
      }
   }
}

void load_skill_table(  )
{
   FILE *fp;

   if( ( fp = fopen( SKILL_FILE, "r" ) ) != NULL )
   {
      top_sn = 0;
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
            bug( "Load_skill_table: # not found.", 0 );
            break;
         }

         word = fread_word( fp );
         if( !str_cmp( word, "SKILL" ) )
         {
            if( top_sn >= MAX_SKILL )
            {
               bug( "load_skill_table: more skills than MAX_SKILL %d", MAX_SKILL );
               fclose( fp );
               return;
            }
            skill_table[top_sn++] = fread_skill( fp );
            continue;
         }
         else if( !str_cmp( word, "END" ) )
            break;
         else
         {
            bug( "Load_skill_table: bad section.", 0 );
            continue;
         }
      }
      fclose( fp );
   }
   else
   {
      bug( "Cannot open skills.dat", 0 );
      exit( 0 );
   }
}

void load_herb_table(  )
{
   FILE *fp;

   if( ( fp = fopen( HERB_FILE, "r" ) ) != NULL )
   {
      top_herb = 0;
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
            bug( "Load_herb_table: # not found.", 0 );
            break;
         }

         word = fread_word( fp );
         if( !str_cmp( word, "HERB" ) )
         {
            if( top_herb >= MAX_HERB )
            {
               bug( "load_herb_table: more herbs than MAX_HERB %d", MAX_HERB );
               fclose( fp );
               return;
            }
            herb_table[top_herb++] = fread_skill( fp );
            if( herb_table[top_herb - 1]->slot == 0 )
               herb_table[top_herb - 1]->slot = top_herb - 1;
            continue;
         }
         else if( !str_cmp( word, "END" ) )
            break;
         else
         {
            bug( "Load_herb_table: bad section.", 0 );
            continue;
         }
      }
      fclose( fp );
   }
   else
   {
      bug( "Cannot open herbs.dat", 0 );
      exit( 0 );
   }
}

void fread_social( FILE * fp )
{
   char buf[MAX_STRING_LENGTH];
   char *word;
   bool fMatch;
   SOCIALTYPE *social;
   CREATE( social, SOCIALTYPE, 1 );

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
            KEY( "CharNoArg", social->char_no_arg, fread_string_nohash( fp ) );
            KEY( "CharFound", social->char_found, fread_string_nohash( fp ) );
            KEY( "CharAuto", social->char_auto, fread_string_nohash( fp ) );
            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
            {
               sysdata.scount += 1;
               if( !social->name )
               {
                  bug( "Fread_social: Name not found", 0 );
                  free_social( social );
                  return;
               }
               if( !social->char_no_arg )
               {
                  bug( "Fread_social: CharNoArg not found", 0 );
                  free_social( social );
                  return;
               }
               add_social( social );
               return;
            }
            break;

         case 'N':
            KEY( "Name", social->name, fread_string_nohash( fp ) );
            break;

         case 'O':
            KEY( "OthersNoArg", social->others_no_arg, fread_string_nohash( fp ) );
            KEY( "OthersFound", social->others_found, fread_string_nohash( fp ) );
            KEY( "OthersAuto", social->others_auto, fread_string_nohash( fp ) );
            break;

         case 'V':
            KEY( "VictFound", social->vict_found, fread_string_nohash( fp ) );
            break;
      }

      if( !fMatch )
      {
         sprintf( buf, "Fread_social: no match: %s", word );
         bug( buf, 0 );
      }
   }
}

void load_socials(  )
{
   FILE *fp;

   if( ( fp = fopen( SOCIAL_FILE, "r" ) ) != NULL )
   {
      top_sn = 0;
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
            bug( "Load_socials: # not found.", 0 );
            break;
         }

         word = fread_word( fp );
         if( !str_cmp( word, "SOCIAL" ) )
         {
            fread_social( fp );
            continue;
         }
         else if( !str_cmp( word, "END" ) )
            break;
         else
         {
            bug( "Load_socials: bad section.", 0 );
            continue;
         }
      }
      fclose( fp );
   }
   else
   {
      bug( "Cannot open socials.dat", 0 );
      exit( 0 );
   }
}

/*
 *  Added the flags Aug 25, 1997 --Shaddai
 */

void fread_command( FILE * fp )
{
   char buf[MAX_STRING_LENGTH];
   char *word;
   bool fMatch;
   CMDTYPE *command;
   CREATE( command, CMDTYPE, 1 );
   command->lag_count = 0; /* can't have caused lag yet... FB */
   command->flags = 0;  /* Default to no flags set */
   command->cshow = 1;  /* Default to command shown - Zarius */

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
            KEY( "Code", command->do_fun, skill_function( fread_word( fp ) ) );
            KEY( "CShow", command->cshow, fread_number( fp ) );
            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
            {
               sysdata.ccount += 1;
               if( !command->name )
               {
                  bug( "Fread_command: Name not found", 0 );
                  free_command( command );
                  return;
               }
               if( !command->do_fun )
               {
                  bug( "Fread_command: Function not found", 0 );
                  free_command( command );
                  return;
               }
               add_command( command );
               return;
            }
            break;

         case 'F':
            KEY( "Flags", command->flags, fread_number( fp ) );
            break;

         case 'L':
            KEY( "Level", command->level, fread_number( fp ) );
            KEY( "Log", command->log, fread_number( fp ) );
            break;

         case 'N':
            KEY( "Name", command->name, fread_string_nohash( fp ) );
            break;

         case 'P':
            /*
             * KEY( "Position", command->position,   fread_number(fp) ); 
             */
            if( !str_cmp( word, "Position" ) )
            {
               fMatch = TRUE;
               command->position = fread_number( fp );
               if( command->position < 100 )
               {
                  switch ( command->position )
                  {
                     default:
                     case 0:
                     case 1:
                     case 2:
                     case 3:
                     case 4:
                        break;
                     case 5:
                        command->position = 6;
                        break;
                     case 6:
                        command->position = 8;
                        break;
                     case 7:
                        command->position = 9;
                        break;
                     case 8:
                        command->position = 12;
                        break;
                     case 9:
                        command->position = 13;
                        break;
                     case 10:
                        command->position = 14;
                        break;
                     case 11:
                        command->position = 15;
                        break;
                  }
               }
               else
                  command->position -= 100;
               break;
            }
            break;
      }

      if( !fMatch )
      {
         sprintf( buf, "Fread_command: no match: %s", word );
         bug( buf, 0 );
      }
   }
}

void load_commands(  )
{
   FILE *fp;

   if( ( fp = fopen( COMMAND_FILE, "r" ) ) != NULL )
   {
      top_sn = 0;
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
            bug( "Load_commands: # not found.", 0 );
            break;
         }

         word = fread_word( fp );
         if( !str_cmp( word, "COMMAND" ) )
         {
            fread_command( fp );
            continue;
         }
         else if( !str_cmp( word, "END" ) )
            break;
         else
         {
            bug( "Load_commands: bad section.", 0 );
            continue;
         }
      }
      fclose( fp );
   }
   else
   {
      bug( "Cannot open commands.dat", 0 );
      exit( 0 );
   }

}

void save_classes(  )
{
   int x;

   for( x = 0; x < MAX_CLASS; x++ )
      write_class_file( x );
}

/*
 * Tongues / Languages loading/saving functions			-Altrag
 */
void fread_cnv( FILE * fp, LCNV_DATA ** first_cnv, LCNV_DATA ** last_cnv )
{
   LCNV_DATA *cnv;
   char letter;

   for( ;; )
   {
      letter = fread_letter( fp );
      if( letter == '~' || letter == EOF )
         break;
      ungetc( letter, fp );
      CREATE( cnv, LCNV_DATA, 1 );

      cnv->old = str_dup( fread_word( fp ) );
      cnv->olen = strlen( cnv->old );
      cnv->new = str_dup( fread_word( fp ) );
      cnv->nlen = strlen( cnv->new );
      fread_to_eol( fp );
      LINK( cnv, *first_cnv, *last_cnv, next, prev );
   }
}

void load_tongues(  )
{
   FILE *fp;
   LANG_DATA *lng;
   char *word;
   char letter;

   if( !( fp = fopen( TONGUE_FILE, "r" ) ) )
   {
      perror( "Load_tongues" );
      return;
   }
   for( ;; )
   {
      letter = fread_letter( fp );
      if( letter == EOF )
         return;
      else if( letter == '*' )
      {
         fread_to_eol( fp );
         continue;
      }
      else if( letter != '#' )
      {
         bug( "Letter '%c' not #.", letter );
         exit( 0 );
      }
      word = fread_word( fp );
      if( !str_cmp( word, "end" ) )
         break;
      fread_to_eol( fp );
      CREATE( lng, LANG_DATA, 1 );
      lng->name = STRALLOC( word );
      fread_cnv( fp, &lng->first_precnv, &lng->last_precnv );
      lng->alphabet = fread_string( fp );
      fread_cnv( fp, &lng->first_cnv, &lng->last_cnv );
      fread_to_eol( fp );
      LINK( lng, first_lang, last_lang, next, prev );
   }
   fclose( fp );
   fp = NULL;
   return;
}

void fwrite_langs( void )
{
   FILE *fp;
   LANG_DATA *lng;
   LCNV_DATA *cnv;

   if( !( fp = fopen( TONGUE_FILE, "w" ) ) )
   {
      perror( "fwrite_langs" );
      return;
   }
   for( lng = first_lang; lng; lng = lng->next )
   {
      fprintf( fp, "#%s\n", lng->name );
      for( cnv = lng->first_precnv; cnv; cnv = cnv->next )
         fprintf( fp, "'%s' '%s'\n", cnv->old, cnv->new );
      fprintf( fp, "~\n%s~\n", lng->alphabet );
      for( cnv = lng->first_cnv; cnv; cnv = cnv->next )
         fprintf( fp, "'%s' '%s'\n", cnv->old, cnv->new );
      fprintf( fp, "\n" );
   }
   fprintf( fp, "#end\n\n" );
   fclose( fp );
   return;
}

void copy_files_contents( FILE * fsource, FILE * fdestination )
{
   int ch;
   int cnt = 1;

   for( ;; )
   {
      ch = fgetc( fsource );
      if( !feof( fsource ) )
      {
         fputc( ch, fdestination );
         if( ch == '\n' )
         {
            cnt++;
            if( cnt >= LAST_FILE_SIZE )   //limit size of this file please :-)
               break;
         }
      }
      else
         break;
   }
}

void write_last_file( char *entry )
{
   FILE *fpout;
   char filename[MAX_INPUT_LENGTH];

   sprintf( filename, "%s", LAST_LIST );
   if( ( fpout = fopen( filename, "a" ) ) == NULL )
   {
      bug( "Cannot open: %s for append", filename );
      return;
   }
   fprintf( fpout, "%s\n", entry ); //adds new entry to top of the file
   fclose( fpout );
   fpout = NULL;
   return;
}

void read_last_file( CHAR_DATA * ch, int count, char *name )
{
   FILE *fpout;
   char filename[MAX_INPUT_LENGTH];
   char charname[100];
   int cnt = 0;
   int letter = 0;
   char *ln;
   char *c;
   char d, e;
   struct tm *tme;
   time_t now;
   char day[MAX_INPUT_LENGTH];
   char sday[5];
   int fnd = 0;

   sprintf( filename, "%s", LAST_LIST );
   if( ( fpout = fopen( filename, "r" ) ) == NULL )
   {
      send_to_char( "There is no last file to look at.\r\n", ch );
      return;
   }

   for( ;; )
   {
      if( feof( fpout ) )
      {
         fclose( fpout );
         ch_printf( ch,
                    "---------------------------------------------------------------------------\r\n%d Entries Listed.\r\n",
                    cnt );
         return;
      }
      else
      {
         if( count == -2 || ++cnt <= count || count == -1 )
         {
            ln = fread_line( fpout );
            strcpy( charname, "" );
            if( name )  //looking for a certain name
            {
               c = ln;
               for( ;; )
               {
                  if( isalpha( *c ) && !isspace( *c ) )
                  {
                     charname[letter] = *c;
                     letter++;
                     c++;
                  }
                  else
                  {
                     charname[letter] = '\0';
                     if( !str_cmp( charname, name ) )
                     {
                        ch_printf( ch, "%s", ln );
                        letter = 0;
                        strcpy( charname, "" );
                        break;
                     }
                     else
                     {
                        if( !feof( fpout ) )
                        {
                           fread_line( fpout );
                           c = ln;
                           letter = 0;
                           strcpy( charname, "" );
                           continue;
                        }
                        else
                        {
                           cnt--;
                           break;
                        }
                     }
                  }
               }
            }
            else if( count == -2 )  //only today's entries
            {
               c = ln;
               now = time( 0 );
               tme = localtime( &now );
               strftime( day, 10, "%d", tme );
               for( ;; )
               {
                  if( !isdigit( *c ) )
                  {
                     c++;
                  }
                  else
                  {
                     d = *c;
                     c++;
                     e = *c;
                     sprintf( sday, "%c%c", d, e );
                     if( !str_cmp( sday, day ) )
                     {
                        fnd = 1;
                        cnt++;
                        ch_printf( ch, "%s", ln );
                        break;
                     }
                     else
                     {
                        if( fnd == 1 )
                        {
                           fclose( fpout );
                           ch_printf( ch,
                                      "---------------------------------------------------------------------------\r\n%d Entries Listed.\r\n",
                                      cnt );
                           return;
                        }
                        else
                           break;
                     }
                  }
               }
            }
            else
            {
               ch_printf( ch, "%s", ln );
            }

         }
         else
         {
            fclose( fpout );
            ch_printf( ch,
                       "--------------------------------------------------------------------------\r\n%d Entries Listed.\r\n",
                       count );
            return;
         }
      }
   }
}
