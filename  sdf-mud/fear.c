/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*
 *  Fear skill
 *  Author: Cronel (cronel_kal@hotmail.com)
 *  of FrozenMUD (empire.digiunix.net 4000)
 *
 *  Permission to use and distribute this code is granted provided
 *  this header is retained and unaltered, and the distribution
 *  package contains all the original files unmodified.
 *  If you modify this code and use/distribute modified versions
 *  you must give credit to the original author(s).
 */

/* You will only need these if you don't include this code into magic.c */

#include "mud.h"

bool check_save( int sn, int level, CHAR_DATA * ch, CHAR_DATA * victim );
void failed_casting( SKILLTYPE * skill, CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj );
void immune_casting( SKILLTYPE * skill, CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj );
void successful_casting( SKILLTYPE * skill, CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj );

ch_ret spell_fear( int sn, int level, CHAR_DATA * ch, void *vo )
{
   CHAR_DATA *victim;
   SKILLTYPE *skill = get_skilltype( sn );

   if( !ch->fighting )
   {
      send_to_char( "You're not fighting anyone.\r\n", ch );
      return rSPELL_FAILED;
   }

   victim = ch->fighting->who;

   if( ( !IS_NPC( ch ) && !IS_NPC( victim ) )
       || ( SPELL_SAVE( skill ) == SE_NEGATE && check_save( sn, level, ch, victim ) ) )
   {
      failed_casting( skill, ch, victim, NULL );
      return rSPELL_FAILED;
   }

   if( ( IS_NPC( victim ) && xIS_SET( victim->act, ACT_SENTINEL ) ) || ( get_timer( ch, TIMER_FEARSPELL ) != 0 ) )
   {
      immune_casting( skill, ch, victim, NULL );
      return rSPELL_FAILED;
   }

   successful_casting( skill, ch, victim, NULL );
   do_flee( victim, "" );
   if( IS_NPC( victim ) )
      start_fearing( victim, ch );

   add_timer( ch, TIMER_FEARSPELL, 50, NULL, 0 );
   return rNONE;
}
