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
 *                           Trivia Add-On                                  *
 ****************************************************************************/

#include <string.h>
#ifndef MUD_HFILE
#include "mud.h"
#endif

void trivia_update( void );
void load_trivia( void );
void sort_questions( TRIVIA_DATA * pTriv );

int triviacycle = 0;
int topic = 0;
int onquestion = 0;
int maxquestions = 0;
bool TRIVIA_ON = FALSE;

void do_trivia( CHAR_DATA * ch, char *argument )
{
   char arg1[MIL];
   char arg2[MIL];
   char arg3[MIL];
   char buf[MSL];

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   argument = one_argument( argument, arg3 );

   if( IS_NPC( ch ) )
      return;

   if( !( IS_IMMORTAL( ch ) ) )
      return;

   if( !arg1 || arg1[0] == '\0' )
   {
      send_to_char( "&WSyntax: Trivia <start/stop> <Topic #> <Rounds>\r\n", ch );
      send_to_char( "Topics: 1. General 2. MUDding\r\n\r\n", ch );
      send_to_char( "&CCurrent Status:\r\n", ch );
      if( TRIVIA_ON )
      {
         sprintf( buf, "&wCurrently on round %d of %d\r\nTrivia Cycle is %d\r\n", onquestion, maxquestions, triviacycle );
         send_to_char( buf, ch );
      }
      else
         send_to_char( "Trivia is not currently running.\r\n", ch );

      return;
   }

   if( !str_cmp( arg1, "start" ) )
   {
      send_to_char( "Starting Trivia.\r\n", ch );
      TRIVIA_ON = TRUE;
   }
   else if( !str_cmp( arg1, "stop" ) )
   {
      send_to_char( "Stopping Trivia.\r\n", ch );
/*	close_trivia();*/
      TRIVIA_ON = FALSE;
      sprintf( buf, "%s has stopped the trivia", ch->name );
      do_info( NULL, buf );
      return;
   }
   else
   {
      send_to_char( "Invalid Argument\r\n", ch );
      return;
   }

   if( !arg2 || arg2[0] == '\0' || !is_number( arg2 ) )
   {
      send_to_char( "Setting topic to default: General\r\n", ch );
      topic = 1;
   }
   else
      topic = atoi( arg2 );

   if( !arg3 || arg3[0] == '\0' || !is_number( arg3 ) )
   {
      send_to_char( "Setting number of questions to default: 20\r\n", ch );
      maxquestions = 20;
   }
   else
      maxquestions = atoi( arg3 );

   sprintf( buf, "%s has started trivia, with %d rounds and a topic of %s", ch->name, maxquestions,
            topic == 1 ? "General" : "MUDding" );

   do_info( NULL, buf );
   return;
}

void trivia_update( void )
{
   char buf[MSL];

   if( TRIVIA_ON )
   {
      switch ( triviacycle )
      {
         case 0:
            onquestion++;
/*	    new_question();
	    display_question();
*/ triviacycle++;
            sprintf( buf, "Simulated Question %d of %d", onquestion, maxquestions );
            do_info( NULL, buf );
            break;
         case 1:
/*	    display_question();
	    display_hint();
*/ triviacycle++;
            do_info( NULL, "Hint Time!" );
            break;
         case 2:
/*	    display_answer();
*/ triviacycle++;
            do_info( NULL, "And the answer is..." );
            break;
         case 3:
            if( onquestion < maxquestions )
            {
               do_info( NULL, "Get ready for the next round of trivia!" );
               triviacycle = 0;
            }
            else
            {
/*		close_trivia();
*/ }
      }
   }
   return;
}

void load_trivia( void )
{
   TRIVIA_DATA *trivia;
   FILE *fp;

   if( !( fp = fopen( SYSTEM_DIR TRIVIA_FILE, "r" ) ) )
      return;

   for( ;; )
   {
      if( feof( fp ) )
      {
         bug( "Load_trivia: no $ found." );
         fclose( fp );
         return;
      }
      CREATE( trivia, TRIVIA_DATA, 1 );
      trivia->question = fread_string_nohash( fp );
      if( *trivia->question == '$' )
         break;
      sort_questions( trivia );
   }
   DISPOSE( trivia->question );
   DISPOSE( trivia );
   fclose( fp );
   return;
}

TRIVIA_DATA *first_question;
TRIVIA_DATA *last_question;
void sort_questions( TRIVIA_DATA * pTriv )
{
   TRIVIA_DATA *trivia = NULL;

   if( !pTriv )
   {
      bug( "Sort_questions: NULL pTriv" );
      return;
   }

   pTriv->next = NULL;
   pTriv->prev = NULL;

   for( trivia = first_question; trivia; trivia = trivia->next )
   {
      if( strcasecmp( pTriv->question, trivia->question ) > 0 )
      {
         INSERT( pTriv, trivia, first_question, next, prev );
         break;
      }
   }

   if( !trivia )
   {
      LINK( pTriv, first_question, last_question, next, prev );
   }

   return;
}
