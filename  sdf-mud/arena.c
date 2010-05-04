/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*
 *  ResortMUD Arena Code
 *  Author: Scion (Scion@usa.net)
 *  of ResortMUD (ucmm.dhs.org:4000)
 *
 *  Permission to use and distribute this code is granted provided
 *  this header is retained and unaltered, and the distribution
 *  package contains all the original files unmodified.
 *  If you modify this code and use/distribute modified versions
 *  you must give credit to the original author(s).
 */
#include "mud.h"

void arena_add( int number, bool money );
int arena_death( CHAR_DATA * ch, CHAR_DATA * victim );
void arena_update( void ); /* called by comm.c */

extern void save_sysdata args( ( SYSTEM_DATA sys ) ); /* act_wiz.c in ShogTag */

const int MAX_ARENA_FEE = 3000000;  /* Set this to something reasonable */
const int ARENA_DEFAULT_PRIZE = 10000;
const int MIN_BET = 100;

int arena_players = 0;  /* This keeps track of the number of players left in the arena */
int arena_total_players = 0;
int arena_prize;  /* This is always 500 coins, set it to be balanced on your MUD */
int arena_fee = 0;   /* This is the entrance fee to get in, which can be set with arena start */
int low_level = 0;   /* Lowest level allowed in arena */
int hi_level = 0; /* Highest level allowes in arena */
int arena_total_bets = 0;  /* Total number of bets. Individual amounts are stored on the players */
int arena_bet_time = 1; /* Time for betting in 30 second increments */
int arena_challenge_wait_time = 2;
int arena_update_mode = 0; /* Update is called once per minute, but we don't want to do something
                            * every minute. */
CHAR_DATA *arena_winner;

void do_arena( CHAR_DATA * ch, char *argument )
{
   char cmd[MIL];
   char cmd2[MIL];
   char cmd3[MIL];
   char buf[MSL];
   CHAR_DATA *victim;
   DESCRIPTOR_DATA *d;
   AREA_DATA *arenaarea;
   int hi_vnum = 0;
   int lo_vnum = 0;
   int real_vnum;
   int i;

   argument = one_argument( argument, cmd );

   set_char_color( AT_IMMORT, ch );

   if( IS_NPC( ch ) )
   {
      send_to_char( "You can't participate in the arena!\r\n", ch );
      return;
   }

//   if( IS_IMMORTAL( ch ) && get_trust( ch ) > 111 )
   if(get_trust( ch ) > 1)
   {
      if( !str_cmp( cmd, "start" ) )
      {
         if( sysdata.arena >= 1 )
         {
            send_to_char( "There is already a battle in the arena!\r\n", ch );
            return;
         }
         argument = one_argument( argument, cmd ); /* entrance fee */
         argument = one_argument( argument, cmd2 );   /* low level */
         argument = one_argument( argument, cmd3 );   /* high level */

         arena_prize = ARENA_DEFAULT_PRIZE;

         if( cmd[0] == '\0' )
            arena_fee = 0;
         else
            arena_fee = atoi( cmd );

         if(arena_fee > 0 && get_trust( ch ) < 101)
         {
            send_to_char("Mortals can only start free arenas. Sorry.", ch);
            return;
         }
         if( arena_fee < 0 )  /* Free matches are, of course, allowed and encouraged! */
            arena_fee = 0;
         if( arena_fee >= MAX_ARENA_FEE )
            arena_fee = MAX_ARENA_FEE;

         if( cmd2[0] == '\0' )
            low_level = -1;   /* Ignore level settings if not specified */
         else
         {
            low_level = atoi( cmd2 );
            if( low_level > MAX_LEVEL )
               low_level = MAX_LEVEL;
            if( low_level <= 0 )
               low_level = -1;
         }

         if( cmd3[0] == '\0' )
            hi_level = -1; /* Ignore level settings if not specified */
         else
         {
            hi_level = atoi( cmd3 );
            if( hi_level > MAX_LEVEL )
               hi_level = MAX_LEVEL;
            if( hi_level <= 0 )
               hi_level = -1;
         }

         if( hi_level < low_level )
         {  /* If they're backward, reverse them */
            i = hi_level;
            hi_level = low_level;
            low_level = i;
         }

         sysdata.arena = 1;   /* move on to betting stage */
         save_sysdata( sysdata );

         /*
          * Clear everyone's bets 
          */
         arena_total_bets = 0;
         for( d = first_descriptor; d; d = d->next )
         {
            if( !d->character )
               continue;

            victim = d->character;

            /*
             * It shouldn't exist yet... 
             */
            if( !victim->pcdata->arena )
               continue;
            else
               DISPOSE( victim->pcdata->arena );
         }

         arena_players = 0;
         arena_total_players = 0;

         /*
          * Display the appropriate start messages 
          */
         if( hi_level > 0 && low_level == -1 )
            sprintf( buf, "%s has ^zOPENED^x the arena with a MAX level of %d!", ch->name, hi_level );
         else if( low_level > 0 && hi_level == -1 )
            sprintf( buf, "%s has ^zOPENED^x the arena with a MIN level of %d!", ch->name, low_level );
         else if( low_level == -1 && hi_level == -1 )
            sprintf( buf, "%s has ^zOPENED^x the arena!", ch->name );
         else
            sprintf( buf, "%s has ^zOPENED^x the arena for levels %d through %d!", ch->name, low_level, hi_level );
         do_info( ch, buf );
         if( arena_fee )
         {
            sprintf( buf, "The entrance fee is %d coins!", arena_fee );
            do_info( ch, buf );
         }
         do_info( ch, "Type \'arena join\' to join, or \'arena bet <amount> <player>\' to bet!" );

         set_char_color( AT_IMMORT, ch );
         send_to_char( "Ready.\r\n", ch );
         return;
      }
      else if( !str_cmp( cmd, "end" ) )
      {
         if( sysdata.arena == 0 )
         {
            send_to_char( "There is not currently an arena battle to end.\r\n", ch );
            return;
         }

         if( sysdata.arena == 1 )
         {
            sprintf( buf, "%s has closed betting.  Let the games begin!", ch->name );
            do_info( ch, buf );
            sysdata.arena = 2;
            return;
         }
         sprintf( buf, "%s has canceled the arena match!", ch->name );
         do_info( ch, buf );
         do_info( ch, "The arena is ^zCLOSED^x." );

         for( d = first_descriptor; d; d = d->next )
         {
            if( !d->character )
               continue;

            victim = d->character;

            if( !victim->pcdata->arena )
               continue;

            if( victim->pcdata->arena->status == 1 )
            {
               send_to_char( "Your bet has been returned to you.\r\n", victim );
               victim->gold += victim->pcdata->arena->bet;
               DISPOSE( victim->pcdata->arena );
               continue;
            }

            if( victim->pcdata->arena->status != 2 )
               continue;

            /*
             * remove recent fight timer 
             */
            if( get_timer( victim, TIMER_RECENTFIGHT ) )
               remove_timer( victim, TIMER_RECENTFIGHT );

            /*
             * make everyone STANDING (for all peace, etc) 
             */
            stop_fighting( victim, TRUE );
            victim->position = POS_STANDING;

            if( arena_fee )
            {
               send_to_char( "Your entrance fee has been returned to you.\r\n", victim );
               victim->gold += arena_fee;
            }

            act( AT_MAGIC, "$n disappears in blinding flash of light.", victim, NULL, NULL, TO_ROOM );
            char_from_room( victim );
            char_to_room( victim, get_room_index( victim->retran ) );
            act( AT_MAGIC, "$n arrives in a blinding flash of light.", victim, NULL, NULL, TO_ROOM );
            do_look( victim, "auto" );
            victim->pcdata->arena->status = 0;
            if( victim->pcdata->arena )
               DISPOSE( victim->pcdata->arena );
         }

         sysdata.arena = 0;
         save_sysdata( sysdata );

         set_char_color( AT_IMMORT, ch );
         send_to_char( "Done.\r\n", ch );
         return;
      }
      else if( !str_cmp( cmd, "who" ) )
      {
         send_to_char( "\r\nWho is in the arena:\r\n", ch );
         for( d = first_descriptor; d; d = d->next )
         {
            if( !d->character )
               continue;

            victim = d->character;

            if( !victim->pcdata->arena )
               continue;

            if( victim->pcdata->arena->status == 0 )
            {
               DISPOSE( victim->pcdata->arena );
               continue;
            }

            switch ( victim->pcdata->arena->status )
            {
               case 1:
                  sprintf( cmd, "Betting (%-12s -- %d)", victim->pcdata->arena->bet_on->name, victim->pcdata->arena->bet );
                  break;
               case 2:
                  sprintf( cmd, "Fighting" );
                  break;
               case 3:
                  sprintf( cmd, "Defeated" );
                  break;
               case 4:
                  sprintf( cmd, "Spectator" );
                  break;
               default:
                  sprintf( cmd, "Unknown" );
                  break;
            }
            sprintf( buf, "%15s: %s\r\n", victim->name, cmd );
            send_to_char( buf, ch );
         }
         return;
      }
   }  /* if IS_IMMORTAL(ch) */

   if( ( !IS_IMMORTAL( ch ) ) || ( sysdata.imm_arena == TRUE ) )
   {
        /******* JOIN *******/
      if( !str_cmp( cmd, "join" ) )
      {
         if( ch->pcdata->arena )
         {
            /*
             * Made a bit more readable -- Kratas 
             */
            switch ( ch->pcdata->arena->status )
            {
               case 1:
                  send_to_char( "You have bet on this round, you cannot participate!\r\n", ch );
                  return;
               case 2:
                  send_to_char( "You are already in the arena!\r\n", ch );
                  return;
               case 3:
                  send_to_char( "You were already defeated this round!\r\n", ch );
                  return;
               case 4:
                  send_to_char( "You are watching a player in this round, you cannot participate.\r\n", ch );
                  return;
            }
         }

         switch ( sysdata.arena )
         {
            case 0:
               send_to_char( "The arena is currently closed. Ask an immortal to open it!\r\n", ch );
               return;

            case 2:
               send_to_char( "You're too late! The betting stage is over!\r\n", ch );
               return;
         }

         if( ( ch->gold < arena_fee ) && !IS_IMMORTAL( ch ) )
         {
            sprintf( buf, "You do not have the %d coins to join this arena match!\r\n", arena_fee );
            send_to_char( buf, ch );
            return;
         }
         if( ch->level < low_level && low_level != -1 )
         {
            send_to_char( "You are not of a high enough level to compete in this match!\r\n", ch );
            return;
         }
         if( ch->level > hi_level && hi_level != -1 )
         {
            send_to_char( "Leave those low level players to bicker amongst themselves.\r\n", ch );
            return;
         }
         if( ch->fighting )
         {
            send_to_char( "You had better stop fighting before entering the arena!\r\n", ch );
            return;
         }

         /*
          * Put them in a random room inside arena.are 
          */
         for( arenaarea = first_area; arenaarea; arenaarea = arenaarea->next )
         {
            if( !str_cmp( arenaarea->filename, "arena.are" ) )
            {
               lo_vnum = arenaarea->low_r_vnum;
               hi_vnum = arenaarea->hi_r_vnum;
               break;
            }
            else
            {
               lo_vnum = 2;
               hi_vnum = 2;
            }
         }

         if( ( lo_vnum == 2 ) && ( hi_vnum == 2 ) )
         {
            bug( "Could not find arena.are!", 0 );
            send_to_char
               ( "An error ocurred while moving you to the arena.\r\nThe administrators have been notified of the error.\r\n",
                 ch );
            return;
         }

         i = 0;
         while( !get_room_index( real_vnum = number_range( lo_vnum, hi_vnum ) ) || real_vnum == 399 )
         {
            if( i++ >= 50 )
            {
               bug( "Cannot find a room in the arena for %s!", ch->name );
               send_to_char
                  ( "An error ocurred while moving you to the arena.\r\nThe administrators have been notified of the error.\r\n",
                    ch );
               return;
            }
         }

         stop_fighting( ch, TRUE );
         ch->position = POS_STANDING;
         act( AT_MAGIC, "$n disappears in a blinding flash of light.", ch, NULL, NULL, TO_ROOM );
         ch->retran = ch->in_room->vnum;
         char_from_room( ch );

         char_to_room( ch, get_room_index( real_vnum ) );

         /*
          * Take their money and put it in the pot 
          */
         if( arena_fee )
         {
            if( IS_IMMORTAL( ch ) )
            {
               sprintf( buf, "Bill Gates donates %d coins for %s\'s entrance fee!", arena_fee, ch->name );
               do_info( ch, buf );
            }
            else
               ch->gold -= arena_fee;
            arena_prize += arena_fee;
         }

         act( AT_MAGIC, "$n arrives in a blinding flash of light.", ch, NULL, NULL, TO_ROOM );
         do_look( ch, "auto" );
         send_to_char( "You have entered the arena!\r\n", ch );
         sprintf( buf, "%s has joined the arena match!", ch->name );
         do_info( ch, buf );

         CREATE( ch->pcdata->arena, ARENA_DATA, 1 );
         ch->pcdata->arena->status = 2;
         arena_add( 1, FALSE );
         return;
      }

        /******** WIMPOUT ********/
      else if( !str_cmp( cmd, "wimpout" ) )
      {
         if( !ch->pcdata->arena )
         {
            send_to_char( "You are not in the arena!\r\n", ch );
            return;
         }
         if( ch->pcdata->arena->status != 2 )
         {
            send_to_char( "You are not in the arena!\r\n", ch );
            return;
         }

         if( ch->fighting )
         {
            send_to_char( "You had better stop fighting before you do that!\r\n", ch );
            return;
         }
         if( arena_fee > 0 )
            send_to_char( "By wimping out, you forfeit your entrance fee!\r\n", ch );
         stop_fighting( ch, TRUE );
         ch->position = POS_STANDING;
         act( AT_MAGIC, "$n disappears in a blinding flash of light.", ch, NULL, NULL, TO_ROOM );
         char_from_room( ch );
         char_to_room( ch, get_room_index( ch->retran ) );
         act( AT_MAGIC, "$n arrives in a blinding flash of light.", ch, NULL, NULL, TO_ROOM );
         do_look( ch, "auto" );
         sprintf( buf, "%s has wimped out of the arena match!", ch->name );
         do_info( ch, buf );
         DISPOSE( ch->pcdata->arena );
         arena_add( -1, FALSE );
         return;
      }
        /******** BET ********/
      else if( !str_cmp( cmd, "bet" ) )
      {
         int betamt;

         argument = one_argument( argument, cmd ); /* amount */
         argument = one_argument( argument, cmd2 );   /* character */

         if( IS_IMMORTAL( ch ) && sysdata.arena == 0 && !str_cmp( cmd, "time" ) )
         {
            arena_bet_time = atoi( cmd2 );
            sprintf( buf, "The arena will now allow %d minutes for betting.\r\n", arena_bet_time );
            send_to_char( buf, ch );
            return;
         }
         else if( IS_IMMORTAL( ch ) && sysdata.arena != 0 && !str_cmp( cmd, "time" ) )
         {
            send_to_char( "You may only set the bet time when the arena is not in use.\r\n", ch );
            send_to_char( "Betting may be ended using \'arena end\' while betting is in progress.\r\n", ch );
            return;
         }

         if( sysdata.arena != 1 )
         {
            send_to_char( "The arena is not accepting bets at this time.\r\n", ch );
            return;
         }
         if( ch->pcdata->arena )
         {
            send_to_char( "You cannot bet on this match.\r\n", ch );
            return;
         }

         if( cmd[0] == '\0' )
         {
            send_to_char( "You must specify an amount to bid, as well as a player to bid on.\r\n", ch );
            return;
         }
         if( cmd2[0] == '\0' )
         {
            send_to_char( "You must specify a player to bid on, after the amount you wish to bid.\r\n", ch );
            return;
         }

         betamt = atoi( cmd );

         if( ch->gold < betamt )
         {
            send_to_char( "You do not have that much money!\r\n", ch );
            return;
         }
         if( betamt < MIN_BET )
         {
            sprintf( buf, "You must bid more than %d.\r\n", MIN_BET );
            send_to_char( buf, ch );
            return;
         }
         if( !( victim = get_char_world( ch, cmd2 ) ) )
         {
            send_to_char( "There is no such player available to bid on.\r\n", ch );
            return;
         }
         if( victim == ch )
         {
            send_to_char( "You cannot bet on yourself!\r\n", ch );
            return;
         }
         if( victim->pcdata->arena && victim->pcdata->arena->status != 2 )
         {
            send_to_char( "That player is not fighting in the arena at this time.\r\n", ch );
            return;
         }
         /*
          * Ok, now we know ch has enough money and has picked a valid victim 
          */
         sprintf( buf, "A bid of %d coins has been placed on %s to win!", betamt, victim->name );
         do_info( victim, buf ); /* in case these infos are logged */
         CREATE( ch->pcdata->arena, ARENA_DATA, 1 );
         ch->pcdata->arena->bet = betamt;
         ch->gold -= ch->pcdata->arena->bet;
         ch->pcdata->arena->bet_on = victim;
         if( !IS_NPC( victim ) && victim->pcdata->arena != NULL )
            ch->pcdata->arena->bet_on->pcdata->arena->num_bets++;
         arena_total_bets += ch->pcdata->arena->bet;
         ch->pcdata->arena->status = 1;
         return;
      }
        /****** NO VALID COMMAND ******/
      else
      {
         /*
          * If we got no valid arguments 
          */
         send_to_char( "Syntax:\r\n", ch );
         if( !IS_IMMORTAL( ch ) || ( sysdata.imm_arena == TRUE ) )
         {
            if( sysdata.arena == 0 )
               send_to_char( "arena challenge\r\n", ch );
            if( sysdata.arena > 0 )
               send_to_char( "arena join\r\n", ch );
            if( ch->pcdata->arena && ch->pcdata->arena->status == 2 )
               send_to_char( "arena wimpout\r\n", ch );
            if( sysdata.arena == 1 && ( ch->pcdata->arena && ch->pcdata->arena->status == 0 ) )
               send_to_char( "arena bet <amount> <player>\r\n", ch );
         }
         if( IS_IMMORTAL( ch ) )
         {
            if( sysdata.arena == 0 )
            {
               send_to_char( "\r\narena start [fee] [low] [high]\r\n", ch );
               send_to_char( "arena bet time [minutes]\r\n", ch );
            }
            if( sysdata.arena > 0 )
               send_to_char( "arena end\r\n", ch );
            send_to_char( "arena who\r\n", ch );
         }
         switch ( sysdata.arena )
         {
            case 0:
               send_to_char( "\r\nThe arena is closed.\r\n", ch );
               break;
            case 1:
               sprintf( buf, "\r\nThe arena has %d minutes left in the betting stage.\r\n",
                        ( arena_bet_time - arena_update_mode ) );
               send_to_char( "\r\nThe arena is in the betting stage.\r\n", ch );
               break;
            case 2:
               sprintf( buf, "\r\nThere are %d combatants in the arena.\r\n", arena_players );
               send_to_char( buf, ch );
               break;
            case 3:
               send_to_char( "\r\nThere is currently a challenge being issued.\r\n", ch );
               break;
            case 4:
               send_to_char( "\r\nThere is currently a challenge in place.\r\n", ch );
               break;
            default:
               sysdata.arena = 0;
               bug( "do_arena: reached default case in last switch" );
               break;
         }  /* switch */
      }  /* else */
   }  /* if !IS_IMMORTAL(ch) */
}  /* function */

void arena_update( void )
{
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];

   switch ( sysdata.arena )
   {
      case 0: /* arena is off */
         arena_update_mode = 0;
         return;
         break;

      case 1: /* arena is in betting mode */
         if( arena_update_mode >= arena_bet_time )
         {
            if( arena_players <= 1 )
            {
               do_info( NULL, "The arena has been closed due to lack of players!" );
               for( d = first_descriptor; d; d = d->next )
               {
                  if( !d->character )
                     continue;

                  victim = d->character;

                  if( !victim->pcdata->arena )
                     continue;

                  if( victim->pcdata->arena->status == 1 )
                  {
                     send_to_char( "Your bet has been returned to you.\r\n", victim );
                     victim->gold += victim->pcdata->arena->bet;
                     DISPOSE( victim->pcdata->arena );
                     continue;
                  }

                  if( victim->pcdata->arena->status != 2 )
                     continue;

                  /*
                   * remove recent fight timer 
                   */
                  if( get_timer( victim, TIMER_RECENTFIGHT ) )
                     remove_timer( victim, TIMER_RECENTFIGHT );

                  /*
                   * make everyone STANDING (for all peace, etc) 
                   */
                  stop_fighting( victim, TRUE );
                  victim->position = POS_STANDING;

                  if( arena_fee )
                  {
                     send_to_char( "Your entrance fee has been returned to you.\r\n", victim );
                     victim->gold += arena_fee;
                  }

                  act( AT_MAGIC, "$n disappears in blinding flash of light.", victim, NULL, NULL, TO_ROOM );
                  char_from_room( victim );
                  char_to_room( victim, get_room_index( victim->retran ) );
                  act( AT_MAGIC, "$n arrives in a blinding flash of light.", victim, NULL, NULL, TO_ROOM );
                  do_look( victim, "auto" );
                  DISPOSE( victim->pcdata->arena );
                  return;
               }
               sysdata.arena = 0;
               save_sysdata( sysdata );
               return;
            }
            do_info( first_char, "All betting has come to an end!  Let the games begin!" );
            sysdata.arena = 2;
            return;
         }
         do_info( first_char, "Place your bets!  Use \'arena bet <amount> <player>\' to bet on your favorite combatant!" );
         arena_update_mode++;
         break;

      case 2: /* arena is in free for all mode */
         if( arena_players <= 1 )
         {
            do_info( NULL, "The arena has been closed due to lack of players!" );
            for( d = first_descriptor; d; d = d->next )
            {
               if( !d->character )
                  continue;

               victim = d->character;

               if( !victim->pcdata->arena )
                  continue;

               if( victim->pcdata->arena->status == 1 )
               {
                  send_to_char( "Your bet has been returned to you.\r\n", victim );
                  victim->gold += victim->pcdata->arena->bet;
                  DISPOSE( victim->pcdata->arena );
                  continue;
               }

               if( victim->pcdata->arena->status != 2 )
                  continue;

               /*
                * remove recent fight timer 
                */
               if( get_timer( victim, TIMER_RECENTFIGHT ) )
                  remove_timer( victim, TIMER_RECENTFIGHT );

               /*
                * make everyone STANDING (for all peace, etc) 
                */
               stop_fighting( victim, TRUE );
               victim->position = POS_STANDING;

               if( arena_fee )
               {
                  send_to_char( "Your entrance fee has been returned to you.\r\n", victim );
                  victim->gold += arena_fee;
               }

               act( AT_MAGIC, "$n disappears in blinding flash of light.", victim, NULL, NULL, TO_ROOM );
               char_from_room( victim );
               char_to_room( victim, get_room_index( victim->retran ) );
               act( AT_MAGIC, "$n arrives in a blinding flash of light.", victim, NULL, NULL, TO_ROOM );
               do_look( victim, "auto" );
               DISPOSE( victim->pcdata->arena );
            }
            sysdata.arena = 0;
            save_sysdata( sysdata );
            return;
         }

         if( arena_fee > 0 )
            sprintf( buf, "Now in the arena: levels %d to %d, entry fee only %d coins!",
                     ( ( low_level > 0 ) ? low_level : 1 ), ( ( hi_level > 0 ) ? hi_level : MAX_LEVEL ), arena_fee );
         else
            sprintf( buf, "Now in the arena: Free for All, levels %d to %d!", ( ( low_level > 0 ) ? low_level : 1 ),
                     ( ( hi_level > 0 ) ? hi_level : MAX_LEVEL ) );
         do_info( first_char, buf );
         break;
      case 3:
         if( arena_update_mode >= arena_challenge_wait_time )
         {
            if( arena_players <= 1 )
            {
               do_info( NULL, "The challenge was never accepted!" );
               for( d = first_descriptor; d; d = d->next )
               {
                  if( !d->character )
                     continue;

                  victim = d->character;

                  if( !victim->pcdata->arena )
                     continue;

                  if( victim->pcdata->arena->challenged_by != NULL )
                     DISPOSE( victim->pcdata->arena );
               }
               sysdata.arena = 0;
               save_sysdata( sysdata );
               return;
            }
            do_info( NULL, "The challenge has begun!" );
            sysdata.arena = 4;
            return;
         }
         arena_update_mode++;
         break;
      case 4:
         break;
      default:
         sysdata.arena = 0;
         bug( "Default case reached in arena_update!", 0 );
         break;
   }
}

void arena_add( int number, bool money )
{
   /*
    * This adds "number" players to arena_players and checks win conditions. 
    */
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *ch;
   DESCRIPTOR_DATA *d;
   double percent = 0;
   int gold = 0;

   arena_players += number;
   if( arena_players > arena_total_players )
      arena_total_players = arena_players;

   if( ( sysdata.arena == 2 ) && ( arena_players <= 1 ) && ( number <= 0 ) )
   {  /* someone won */
      arena_winner = NULL;
      /*
       * Grab the winner 
       */
      for( d = first_descriptor; d; d = d->next )
      {
         if( !d->character )
            continue;

         ch = d->character;

         if( ch->pcdata->arena && ch->pcdata->arena->status == 2 )
         {
            arena_winner = ch;
            break;
         }
      }

      for( d = first_descriptor; d; d = d->next )  /* if number is positive, we've just added */
      {
         if( !d->character )
            continue;

         ch = d->character;

         if( !ch->pcdata->arena )
            continue;

         switch ( ch->pcdata->arena->status )
         {
            case 0: /* Character's not in, and hasn't been in the arena */
               continue;
               break;

            case 1: /* Character has bet on a participant */
               if( ch->pcdata->arena->bet_on != arena_winner )
                  continue;

               if( ch->pcdata->arena->bet_on->pcdata->arena->num_bets <= 0 )
                  ch->pcdata->arena->bet_on->pcdata->arena->num_bets = 1;

               /*
                * percent = ch->pcdata->arenabet/arena_total_bets;
                * percent = (arena_total_bets/ch->pcdata->arenabet);
                * percent = 1/arena_total_bets; 
                */
               {
                  int amount;
                  percent = 1 / ch->pcdata->arena->bet_on->pcdata->arena->num_bets;
                  amount = ch->pcdata->arena->bet * percent;
                  gold = ch->pcdata->arena->bet + amount;
               }
               ch->gold += gold;
               sprintf( buf, "You won %d coins!\r\n", gold );
               send_to_char( buf, ch );
               DISPOSE( ch->pcdata->arena );
               break;

            case 2: /* Character has won, being the last one left alive in arena */
               if( money == TRUE )
               {
                  sprintf( buf, "%s has ^zWON^x the arena match, and ^z%d^x coins!", ch->name, arena_prize );
                  do_info( ch, buf );
                  ch->gold += arena_prize;
                  ch->pcdata->arena_wins++;
                  adjust_hiscore( "arena_pkill", ch, ch->pcdata->arena_wins );
               }
               else
                  do_info( ch, "The arena match has been closed without a winner." );
               stop_fighting( ch, TRUE );
               ch->position = POS_STANDING;
               act( AT_MAGIC, "$n disappears in a golden beam of light.", ch, NULL, NULL, TO_ROOM );

               ch->hit = ch->max_hit;
               ch->mana = ch->max_mana;
               ch->move = ch->max_move;
               ch->pcdata->condition[COND_BLOODTHIRST] = ( 10 + ch->level );
               update_pos( ch );
               send_to_char( "You have been fully restored!\r\n", ch );

               char_from_room( ch );
               char_to_room( ch, get_room_index( ch->retran ) );
               act( AT_MAGIC, "$n appears in a golden beam of light.", ch, NULL, NULL, TO_ROOM );
               do_look( ch, "auto" );
               DISPOSE( ch->pcdata->arena );
               break;
            case 3: /* Character died in arena */
               DISPOSE( ch->pcdata->arena );
               break;
            case 4: /* Character is watching a participant */
               DISPOSE( ch->pcdata->arena );
               break;
         }
      }
      arena_prize = ARENA_DEFAULT_PRIZE;
      sysdata.arena = 0;
      save_sysdata( sysdata );
      arena_players = 0;
   }
}

int arena_death( CHAR_DATA * ch, CHAR_DATA * victim )
{
   char buf[MAX_STRING_LENGTH];

   /*
    * If they were in the arena, override death! 
    */
   if( victim->pcdata->arena->status == 2 )
   {
      act( AT_DEAD, "$n has been DEFEATED!!", victim, 0, 0, TO_ROOM );
      act( AT_DEAD, "You have been DEFEATED!", victim, 0, 0, TO_CHAR );
      sprintf( buf, "%s has been defeated by %s in the arena!", victim->name, ch->name );
      do_info( victim, buf );
      sprintf( buf, "%s (%d) killed by %s in arena at %d",
               victim->name, victim->level, ( IS_NPC( ch ) ? ch->short_descr : ch->name ), victim->in_room->vnum );
      log_string( buf );
      to_channel( buf, "Monitor", LEVEL_IMMORTAL );
      ch->pcdata->arena_kills++;
      victim->pcdata->arena_deaths++;

      /*
       * Now that that fiasco is over, retran and restore the victim 
       */
      stop_fighting( victim, TRUE );
      victim->position = POS_STANDING;
      act( AT_MAGIC, "$n disappears in a beam of holy light.", victim, NULL, NULL, TO_ROOM );
      victim->hit = victim->max_hit;
      victim->mana = victim->max_mana;
      victim->move = victim->max_move;
      victim->pcdata->condition[COND_BLOODTHIRST] = ( 10 + victim->level );
      update_pos( victim );
      send_to_char( "You have been fully restored!\r\n", victim );

      char_from_room( victim );
      char_to_room( victim, get_room_index( victim->retran ) );
      act( AT_MAGIC, "$n falls delicately out of the sky.", victim, NULL, NULL, TO_ROOM );
      do_look( victim, "auto" );
      victim->pcdata->arena->status = 3;  /* 0==no arena, 1==betting, 2==fighting, 3==dead, 4==watching */
      arena_add( -1, TRUE );  /* Reduce the player count by one */
      return 0;
   }
   return 1;
}
