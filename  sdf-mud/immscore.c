/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*
 * Installation Instructions.
 *
 * Add immscore.o to Makefile. (immscore.c if you feel like being that
 * organized)
 * Add do_iscore, do_incognito, and do_ghost into mud.h and tables.c in
 * their respective places. (If you know nothing of how to do this,
 * consult your doc dir where it will give you specific instructions.)
 * Next, add this to mud.h under char_data:
 * short incog_level; * Incog -Froboz *
 * short ghost_level; * Ghost -Froboz *
 * NOTE: Remember to put a / at those "*"'s
 *
 * --Kratas
 *
 * This code was distributed by UCMM / Haslage Net Electronics (C) 1999
 */

#include <time.h>
#include "mud.h"

void do_iscore( CHAR_DATA * ch, char *argument )
{
   if( IS_NPC( ch ) )
   {
      do_oldscore( ch, argument );
      return;
   }

   set_pager_color( AT_SCORE, ch );
   pager_printf_color( ch, "\r\n&cImmortal Data: &W%s&c.\r\n", ch->name );

   if( get_trust( ch ) != ch->level )
      pager_printf( ch, "&cYou are trusted at level &W%d&c.\r\n", get_trust( ch ) );

   send_to_pager_color( "&B----------------------------------------------------------------------------\r\n", ch );

   if( IS_IMMORTAL( ch ) )
   {
      pager_printf_color( ch, "&cLevel : &W%-6d      &cClass : &W%-10.10s      &cPlayed: &W%d hours\r\n",
                          ch->level, capitalize( get_class( ch ) ), ( get_age( ch ) - 17 ) * 2 );
      if( ch->level2 != -1 )
         pager_printf_color( ch, "&cLevel2: &W%-6d      &cClass2: &W%-10.10s\r\n",
                             ch->level2, capitalize( get_class2( ch ) ) );
      if( ch->level3 != -1 )
         pager_printf_color( ch, "&cLevel3: &W%-6d      &cClass3: &W%-10.10s\r\n",
                             ch->level3, capitalize( get_class3( ch ) ) );

      pager_printf_color( ch, "&cYears : &W%-6d      &cRace  : &W%-10.10s      &cLog In: &W%s\r\n",
                          get_age( ch ), capitalize( get_race( ch ) ), ctime( &( ch->logon ) ) );

      send_to_pager_color( "&B----------------------------------------------------------------------------\r\n", ch );

      pager_printf_color( ch, "&RBamfin : &p%s\r\n", ( ch->pcdata->bamfin[0] != '\0' )
                          ? ch->pcdata->bamfin : "Not changed/Switched." );
      pager_printf( ch, "&RBamfout: &p%s\r\n", ( ch->pcdata->bamfout[0] != '\0' )
                    ? ch->pcdata->bamfout : "Not changed/Switched." );

      if( ch->pcdata->area )
      {
         send_to_pager_color( "&B----------------------------------------------------------------------------\r\n", ch );
         pager_printf( ch,
                       "&RVnums  : &pRoom &W(&R%-5.5d &Y- &R%-5.5d&W)   &pObject &W(&R%-5.5d &Y- &R%-5.5d&W)   &pMob &W(&R%-5.5d &Y- &R%-5.5d&W)\r\n",
                       ch->pcdata->area->low_r_vnum, ch->pcdata->area->hi_r_vnum, ch->pcdata->area->low_o_vnum,
                       ch->pcdata->area->hi_o_vnum, ch->pcdata->area->low_m_vnum, ch->pcdata->area->hi_m_vnum );
         pager_printf( ch, "&p         Area Loaded &W[%s&W]\r\n",
                       ( IS_SET( ch->pcdata->area->status, AREA_LOADED ) ) ? "&GYES" : "&RNO" );
      }
      send_to_pager_color( "&B----------------------------------------------------------------------------\r\n", ch );

      pager_printf( ch,
                    "&cHolylight: %3s    &cInvis    : %3s &c[&W%2d&c]    &cIncog: %3s &c[&W%2d&c]    &cGhost: %3s &c[&W%2d&c]\r\n",
                    xIS_SET( ch->act, PLR_HOLYLIGHT ) ? "&GON&c " : "&ROFF&c", xIS_SET( ch->act,
                                                                                        PLR_WIZINVIS ) ? "&GON&c " :
                    "&ROFF&c", ch->pcdata->wizinvis, ch->incog_level > 0 ? "&GON&c " : "&ROFF&c", ch->incog_level,
                    ch->ghost_level > 0 ? "&GON&c " : "&ROFF&c", ch->ghost_level );

      pager_printf( ch, "&cWizlock  : %s    &cDoubleEXP: %s     &cNeverEmpty: %s\r\n",
                    sysdata.wizlock ? "&GON&c " : "&ROFF&c",
                    sysdata.double_exp ? "&GON&c " : "&ROFF", sysdata.neverempty ? "&GON&c " : "&ROFF" );
   }
   send_to_pager( "\r\n", ch );
   return;
}



/* Incognito is a level based command. Once cloaked,
 * all players up to the level set will not be able
 * to see you, unless they are in the same room.
 * -Froboz
 */
void do_incognito( CHAR_DATA * ch, char *argument )
{
   int level;
   char arg[MAX_STRING_LENGTH];

   one_argument( argument, arg );

   /*
    * Defaults to Players max_level 
    */
   if( arg[0] == '\0' )
      if( ch->incog_level )
      {
         ch->incog_level = 0;
         act( AT_BLUE, "$n is no longer cloaked.", ch, NULL, NULL, TO_ROOM );
         send_to_char( "You are no longer cloaked.\r\n", ch );
      }
      else
      {
         ch->incog_level = get_trust( ch );
         ch->ghost_level = 0;
         act( AT_BLUE, "$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
         send_to_char( "You cloak your presence.\r\n", ch );
      }
   /*
    * do the level thing 
    */
   else
   {
      level = atoi( arg );
      if( level < 2 || level > get_trust( ch ) )
      {
         send_to_char( "Incog level must be between 2 and your level.\r\n", ch );
         return;
      }
      else
      {
         ch->reply = NULL;
         ch->incog_level = level;
         ch->ghost_level = 0;
         act( AT_BLUE, "$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
         send_to_char( "You cloak your presence.\r\n", ch );
      }
   }

   return;
}

/* Ghost hides the immortal in the room they are standing.
 * They still show up on who.   -Froboz was here :)
 */
void do_ghost( CHAR_DATA * ch, char *argument )
{
   int level;
   char arg[MAX_STRING_LENGTH];

   one_argument( argument, arg );

   if( arg[0] == '\0' )
      /*
       * take the default path 
       */
      if( ch->ghost_level )
      {
         ch->ghost_level = 0;
         act( AT_WHITE, "$n steps out from the mist.", ch, NULL, NULL, TO_ROOM );
         send_to_char( "You step out from the mist.\r\n", ch );
      }
      else
      {
         ch->ghost_level = get_trust( ch );
         ch->incog_level = 0;
         act( AT_WHITE, "$n vanishes into a mist.", ch, NULL, NULL, TO_ROOM );
         send_to_char( "You vanish into a mist.\r\n", ch );
      }
   else
      /*
       * do the level thing 
       */
   {
      level = atoi( arg );
      if( level < 2 || level > get_trust( ch ) )
      {
         send_to_char( "Ghost level must be between 2 and your level.\r\n", ch );
         return;
      }
      else
      {
         ch->reply = NULL;
         ch->ghost_level = level;
         ch->incog_level = 0;
         act( AT_WHITE, "$n vanishes into a mist.", ch, NULL, NULL, TO_ROOM );
         send_to_char( "You vanish into a mist.\r\n", ch );
      }
   }

   return;
}
