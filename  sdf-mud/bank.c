/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/****************************************************************************
 *                   ^     +----- |  / ^     ^ |     | +-\                  *
 *                  / \    |      | /  |\   /| |     | |  \                 *
 *                 /   \   +---   |<   | \ / | |     | |  |                 *
 *                /-----\  |      | \  |  v  | |     | |  /                 *
 *               /       \ |      |  \ |     | +-----+ +-/                  *
 ****************************************************************************
 * AFKMud Copyright 1997-2003 by Roger Libiez (Samson),                     *
 * Levi Beckerson (Whir), Michael Ward (Tarl), Erik Wolfe (Dwip),           *
 * Cameron Carroll (Cam), Cyberfox, Karangi, Rathian, Raine, and Adjani.    *
 * All Rights Reserved.                                                     *
 *                                                                          *
 * Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,        *
 * Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,        *
 * Grishnakh, Fireblade, and Nivek.                                         *
 *                                                                          *
 * Original MERC 2.1 code by Hatchet, Furey, and Kahn.                      *
 *                                                                          *
 * Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,      *
 * Michael Seifert, and Sebastian Hammer.                                   *
 ****************************************************************************
 *                              Bank module                                 *
 ****************************************************************************/

/***************************************************************************  
 *                          SMAUG Banking Support Code                     *
 ***************************************************************************
 *                                                                         *
 * This code may be used freely, as long as credit is given in the help    *
 * file. Thanks.                                                           *
 *								                           *
 *                                        -= Minas Ravenblood =-           *
 *                                 Implementor of The Apocalypse Theatre   *
 *                                      (email: krisco7@hotmail.com)       *
 *									                     *
 ***************************************************************************/

/* Modifications to original source by Samson */
#include "mud.h"

/* You can add this or just put it in the do_bank code. I don't really know
   why I made a seperate function for this, but I did. If you do add it,
   don't forget to declare it - Minas */
/* Finds banker mobs in a room. Installed by Samson on unknown date */
/* NOTE: Smaug 1.02a Users - Your compiler probably died on this
   function - if so, remove the x in front of IS_SET and recompile */

CHAR_DATA *find_banker( CHAR_DATA * ch )
{
   CHAR_DATA *banker;

   for( banker = ch->in_room->first_person; banker; banker = banker->next_in_room )
      if( IS_NPC( banker ) && xIS_SET( banker->act, ACT_BANKER ) )
         break;

   return banker;
}

/* SMAUG Bank Support
 * Coded by Minas Ravenblood for The Apocalypse Theatre
 * (email: krisco7@hotmail.com)
 */
/* Installed by Samson on unknown date */
/* Deposit, withdraw, balance and transfer commands */
void do_deposit( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *banker;
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   bool isgold = FALSE;
   bool issilver = FALSE;
   bool iscopper = FALSE;
   long int amount;

   if( !( banker = find_banker( ch ) ) )
   {
      send_to_char( "&WYou're not in a bank!&w\r\n", ch );
      return;
   }

   if( IS_NPC( ch ) )
   {
      sprintf( buf, "Sorry, %s, we don't do business with mobs.", ch->short_descr );
      do_say( banker, buf );
      return;
   }

   if( argument[0] == '\0' )
   {
      do_say( banker, "If you need help, see HELP BANK." );
      return;
   }

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1 == '\0' )
   {
      sprintf( buf, "%s How much do you wish to deposit?", ch->name );
      do_tell( banker, buf );
      return;
   }

   if( str_cmp( arg1, "all" ) && !is_number( arg1 ) )
   {
      sprintf( buf, "%s How much do you wish to deposit?", ch->name );
      do_tell( banker, buf );
      return;
   }

   /*
    * New Money System -Garinan 
    */
   if( arg2 == '\0' )
   {
      sprintf( buf, "%s You must specify gold, silver, or copper.", ch->name );
      do_tell( banker, buf );
      return;
   }

   if( !str_cmp( arg2, "gold" ) )
      isgold = TRUE;
   else if( !str_cmp( arg2, "silver" ) )
      issilver = TRUE;
   else if( !str_cmp( arg2, "copper" ) )
      iscopper = TRUE;
   else
   {
      sprintf( buf, "%s I don't trade in that currency.", ch->name );
      do_tell( banker, buf );
      return;
   }

   if( !str_cmp( arg1, "all" ) )
      isgold ? ( amount = ch->gold ) : issilver ? ( amount = ch->silver ) : ( amount = ch->copper );
   else
      amount = atoi( arg1 );

   if( ( amount > ch->gold && isgold ) || ( amount > ch->silver && issilver ) || ( amount > ch->copper && iscopper ) )
   {
      sprintf( buf, "%s Sorry, but you don't have that much %s to deposit.", ch->name,
               isgold ? "gold" : issilver ? "silver" : "copper" );
      do_tell( banker, buf );
      return;
   }

   if( amount <= 0 )
   {
      sprintf( buf, "%s Oh, I see.. I didn't know i was doing business with a comedian.", ch->name );
      do_tell( banker, buf );
      return;
   }

 /*  if( !IS_IMMORTAL( ch ) )
   {
  if( isgold )
      {
         if( ch->pcdata->gbalance == 500 )
         {
            do_tell( banker, "You can't deposit more than 500 gold coins!" );
            return;
         }

         if( ch->pcdata->gbalance + amount > 500 )
            amount = 500 - ch->pcdata->gbalance;
      }
      if( issilver )
      {
         if( ch->pcdata->sbalance == 50000 )
         {
            do_tell( banker, "You can't deposit more than 50,000 silver coins!" );
            return;
         }

         if( ch->pcdata->sbalance + amount > 50000 )
            amount = 50000 - ch->pcdata->sbalance;
      }
      if( iscopper )
      {
         if( ch->pcdata->balance == 5000000 )
         {
            do_tell( banker, "You can't deposit more than 5,000,000 copper coins!" );
            return;
         }

         if( ch->pcdata->balance + amount > 5000000 )
            amount = 5000000 - ch->pcdata->balance;
      }
   } */
   if( isgold )
   {
      ch->gold -= amount;
      ch->pcdata->gbalance += amount;
   }
   else if( issilver )
   {
      ch->silver -= amount;
      ch->pcdata->sbalance += amount;
   }
   else if( iscopper )
   {
      ch->copper -= amount;
      ch->pcdata->balance += amount;
   }
   else
   {
      bug( "Invalid money type in banks. Error code: Er21301 Please report this code to Garinan." );
      return;
   }
   sprintf( buf, "You deposit %ld %s coin%s.\r\n", amount, isgold ? "gold" : issilver ? "silver" : "copper", ( amount != 1 ) ?
            "s" : "" );
   set_char_color( AT_WHITE, ch );
   send_to_char( buf, ch );
   sprintf( buf, "$n deposits %ld %s coin%s.\r\n", amount, isgold ? "gold" : issilver ? "silver" : "copper", ( amount != 1 ) ?
            "s" : "" );
   act( AT_WHITE, buf, ch, NULL, NULL, TO_ROOM );
   return;
}

void do_withdraw( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *banker;
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   bool isgold = FALSE;
   bool issilver = FALSE;
   bool iscopper = FALSE;
   long int amount;
   long int thebalance;

   if( !( banker = find_banker( ch ) ) )
   {
      send_to_char( "&WYou're not in a bank!&w\r\n", ch );
      return;
   }

   if( IS_NPC( ch ) )
   {
      sprintf( buf, "Sorry, %s, we don't do business with mobs.", ch->short_descr );
      do_say( banker, buf );
      return;
   }

   if( argument[0] == '\0' )
   {
      do_say( banker, "If you need help, see HELP BANK." );
      return;
   }

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1 == '\0' )
   {
      sprintf( buf, "%s How much do you wish to withdraw?", ch->name );
      do_tell( banker, buf );
      return;
   }
   if( str_cmp( arg1, "all" ) && !is_number( arg1 ) )
   {
      sprintf( buf, "%s How much do you wish to withdraw?", ch->name );
      do_tell( banker, buf );
      return;
   }

   if( arg2 == '\0' || ( str_cmp( arg2, "gold" ) && str_cmp( arg2, "silver" ) && str_cmp( arg2, "copper" ) ) )
   {
      sprintf( buf, "%s You must specify gold, silver, or copper.", ch->name );
      do_tell( banker, buf );
      return;
   }

   if( !str_cmp( arg2, "gold" ) )
   {
      isgold = TRUE;
      thebalance = ch->pcdata->gbalance;
   }
   else if( !str_cmp( arg2, "silver" ) )
   {
      issilver = TRUE;
      thebalance = ch->pcdata->sbalance;
   }
   else if( !str_cmp( arg2, "copper" ) )
   {
      iscopper = TRUE;
      thebalance = ch->pcdata->balance;
   }
   else
   {
      bug( "Invalid money type in banks. Error code: Er21302 Please report this code to Garinan." );
      return;
   }

   if( !str_cmp( arg1, "all" ) )
      isgold ? ( amount = ch->pcdata->gbalance ) : issilver ? ( amount = ch->pcdata->sbalance ) : ( amount =
                                                                                                    ch->pcdata->balance );
   else
      amount = atoi( arg1 );

   if( amount > thebalance )
   {
      sprintf( buf, "%s But you do not have that much %s in your account!", ch->name, isgold ? "gold" : issilver ? "silver" :
               "copper" );
      do_tell( banker, buf );
      return;
   }

   if( amount <= 0 )
   {
      sprintf( buf, "%s Oh I see.. I didn't know i was doing business with a comedian.", ch->name );
      do_tell( banker, buf );
      return;
   }
   if( iscopper )
   {
      ch->pcdata->balance -= amount;
      ch->copper += amount;
   }
   else if( isgold )
   {
      ch->pcdata->gbalance -= amount;
      ch->gold += amount;
   }
   else if( issilver )
   {
      ch->pcdata->sbalance -= amount;
      ch->silver += amount;
   }
   else
   {
      bug( "Invalid money type in banks. Error code: Er21303 Please report this code to Garinan." );
      return;
   }
   sprintf( buf, "You withdraw %ld %s coin%s.\r\n", amount, isgold ? "gold" : issilver ? "silver" : "copper",
            ( amount != 1 ) ? "s" : "" );
   set_char_color( AT_WHITE, ch );
   send_to_char( buf, ch );
   sprintf( buf, "$n withdraws %ld %s coin%s.\r\n", amount, isgold ? "gold" : issilver ? "silver" : "copper",
            ( amount != 1 ) ? "s" : "" );
   act( AT_WHITE, buf, ch, NULL, NULL, TO_ROOM );
   return;
}

void do_balance( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *banker;
   char buf[MAX_STRING_LENGTH];

   if( !( banker = find_banker( ch ) ) )
   {
      send_to_char( "&WYou're not in a bank!&w\r\n", ch );
      return;
   }

   if( IS_NPC( ch ) )
   {
      sprintf( buf, "Sorry, %s, we don't do business with mobs.", ch->short_descr );
      do_say( banker, buf );
      return;
   }

   set_char_color( AT_WHITE, ch );
   send_to_char( "Your account value is:\r\n", ch );
   sprintf( buf, "&YGold: %ld.\r\n&WSilver: %ld.\r\n&RCopper: %ld.\r\n\r\n", ch->pcdata->gbalance, ch->pcdata->sbalance,
            ch->pcdata->balance );
   send_to_char( buf, ch );
   return;
}

void do_transfer( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *banker;
   CHAR_DATA *victim;
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   bool isgold = FALSE;
   bool issilver = FALSE;
   bool iscopper = FALSE;

   char buf[MAX_STRING_LENGTH];
   long int amount;
   long int thebalance;

   if( !( banker = find_banker( ch ) ) )
   {
      send_to_char( "&WYou're not in a bank!&w\r\n", ch );
      return;
   }

   if( IS_NPC( ch ) )
   {
      sprintf( buf, "Sorry, %s, we don't do business with mobs.", ch->short_descr );
      do_say( banker, buf );
      return;
   }

   if( argument[0] == '\0' )
   {
      do_say( banker, "If you need help, see HELP BANK." );
      return;
   }


   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   argument = one_argument( argument, arg3 );

   if( arg1 == '\0' || arg2 == '\0' || arg3 == '\0' )
   {
      sprintf( buf, "%s How much do you wish to send to who?", ch->name );
      do_tell( banker, buf );
      return;
   }
   if( str_cmp( arg1, "all" ) && !is_number( arg1 ) )
   {
      sprintf( buf, "%s How much do you wish to send to who?", ch->name );
      do_tell( banker, buf );
      return;
   }
   if( !str_cmp( arg2, "gold" ) )
   {
      isgold = TRUE;
      thebalance = ch->pcdata->gbalance;
   }
   else if( !str_cmp( arg2, "silver" ) )
   {
      issilver = TRUE;
      thebalance = ch->pcdata->sbalance;
   }
   else if( !str_cmp( arg2, "copper" ) )
   {
      iscopper = TRUE;
      thebalance = ch->pcdata->balance;
   }
   else
   {
      sprintf( buf, "%s I don't trade in that currency.", ch->name );
      do_tell( banker, buf );
      return;
   }

   if( !( victim = get_char_world( ch, arg3 ) ) )
   {
      sprintf( buf, "%s %s could not be located.", ch->name, capitalize( arg2 ) );
      do_tell( banker, buf );
      return;
   }

   if( IS_NPC( victim ) )
   {
      sprintf( buf, "%s We do not do business with mobiles...", ch->name );
      do_tell( banker, buf );
      return;
   }

   if( !str_cmp( arg1, "all" ) )
      isgold ? ( amount = ch->pcdata->gbalance ) : issilver ? ( amount = ch->pcdata->sbalance ) : ( amount =
                                                                                                    ch->pcdata->balance );
   else
      amount = atoi( arg1 );

   if( amount > thebalance )
   {
      sprintf( buf, "%s You are very generous, but you don't have that much %s!", ch->name, isgold ? "gold" : issilver ?
               "silver" : "copper" );
      do_tell( banker, buf );
      return;
   }

   if( amount <= 0 )
   {
      sprintf( buf, "%s Oh I see.. I didn't know I was doing business with a comedian.", ch->name );
      do_tell( banker, buf );
      return;
   }
   if( isgold )
   {
      ch->pcdata->gbalance -= amount;
      victim->pcdata->gbalance += amount;
   }
   else if( issilver )
   {
      ch->pcdata->sbalance -= amount;
      victim->pcdata->sbalance += amount;
   }
   else if( iscopper )
   {
      ch->pcdata->balance -= amount;
      victim->pcdata->balance += amount;
   }
   else
   {
      bug( "Invalid money type in banks. Error code: Er21303 Please report this code to Garinan." );
      return;
   }

   sprintf( buf, "You transfer %ld %s coin%s to %s's bank account.\r\n",
            amount, isgold ? "gold" : issilver ? "silver" : "copper", ( amount != 1 ) ? "s" : "", victim->name );
   set_char_color( AT_WHITE, ch );
   send_to_char( buf, ch );
   sprintf( buf, "%s just transferred %ld %s coin%s to your bank account.\r\n",
            ch->name, amount, isgold ? "gold" : issilver ? "silver" : "copper", ( amount != 1 ) ? "s" : "" );
   set_char_color( AT_WHITE, victim );
   send_to_char( buf, victim );
   return;
}

/* End of new bank support */
