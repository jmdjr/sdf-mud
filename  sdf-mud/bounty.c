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
 *                           Bounty Add-On                                  *
 ****************************************************************************/
/****************************************************************************
 * Bounty for ResortMUD 5.0 by Garinan                                      *        
 *                                                                          *
 * A simple code which allows players to place a gold bounty on the head of *
 * a deadly player.                                                         *
 *                                                                          *
 * If you have any questions, comments, concerns or bugs to report which    *
 * are not resolved within the included readme.txt, please email me at:     *
 * garinan@hotmail.com, your support is greatly appreciated.                * 
 *                                                                          *
 *                                                  - Garinan               *
 ****************************************************************************/
/****************************************************************************
 * Copyright (c) Andrew Brunelle 2000, All Rights Reserved                  *
 * This code, as well as this document is distributed to be used freely     *
 * provided that credit be given to myself, its author Andrew Brunelle      *
 * (Garinan Templar). If you wish to distribute this snippet via your own   *
 * website or any other means, please contact me first at the email address *
 * below, I will likely only ask that you add a link to UCMM in exchange,   *
 * thank you and good luck!                                                 *
 ****************************************************************************/
/****************************************************************************
 *********************************DISCLAIMER*********************************
 ****************************************************************************
 * By adding this code to your MUD you are doing so at your own risk.       *
 * Neither I (Andrew Brunelle), nor any organization with which i am        *
 * affiliated, will be held responsible for any damages to your MUD by      *
 * adding this code.                                                        *
 ****************************************************************************/

#include "mud.h"

CHAR_DATA *find_bounty( CHAR_DATA * ch )
{
   CHAR_DATA *bounty;

   for( bounty = ch->in_room->first_person; bounty; bounty = bounty->next_in_room )
      if( IS_NPC( bounty ) && xIS_SET( bounty->act, ACT_BOUNTY ) )
         break;
   return bounty;
}

void do_bounty( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *bounty;
   CHAR_DATA *victim;
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   int amount, cost, bountymax;

/* cost is the cost for a peaceful to become ineligable to be bountied *
 * bountymax is the maximum amount to be put on a player's head        *
 * alter below to suit your needs.                           - Garinan */

   cost = 100000000;
   bountymax = 2000000000;

   if( !( bounty = find_bounty( ch ) ) )
   {

/* Why should players need to be at a bounty officer to check *
 * the value of their bounty? They shouldn't :P - Garinan     */

      if( !str_cmp( argument, "check" ) )
      {
         if( IS_NPC( ch ) )
            ch_printf( ch, "&YMobiles can not have bounties.\r\n" );
         else if( ch->bounty > 0 )
            ch_printf( ch, "&YYou have a bounty worth %d gold on your head.\r\n", ch->bounty );
         else
            ch_printf( ch, "&YYou have no bounty on your head.\r\n" );
         if( IS_NPC( ch ) )
            ch_printf( ch, "&YMobiles can not collect bounties.\r\n" );
         else if( ch->bowed > 0 )
            ch_printf( ch, "&YYou are owed %d gold in bounty earnings.\r\n", ch->bowed );
         else
            ch_printf( ch, "&YYou have not claimed any bounties.\r\n" );
         return;
      }

      send_to_char( "There is no bounty officer here!\r\n", ch );
      return;
   }

   act( AT_ACTION, "$n and $N whisper quietly to each other.", ch, ch, bounty, TO_ROOM );
   act( AT_ACTION, "You whisper your request to $N.", ch, bounty, bounty, TO_CHAR );

   if( IS_NPC( ch ) )
   {
      sprintf( buf, "We don't deal with your kind %s.", ch->short_descr );
      do_say( bounty, buf );
      return;
   }

   if( argument[0] == '\0' )
   {
      sprintf( buf, "If you need help, see HELP BOUNTY." );
      do_say( bounty, buf );
      return;
   }

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1 == '\0' )
   {
      sprintf( buf, "How much gold do you want to place, and on who?" );
      do_say( bounty, buf );
      return;
   }

/* Bounty check, players can check if they have a bounty on them *
 * and if so, how much its value is.                             *
 *                                         - Garinan             */

   if( !str_cmp( arg1, "check" ) )
   {

      if( arg2[0] == '\0' )
      {
         if( IS_NPC( ch ) )
            ch_printf( ch, "&YMobiles can not have bounties.\r\n" );
         else if( ch->bounty > 0 )
            ch_printf( ch, "&YYou have a bounty worth %d gold on your head.\r\n", ch->bounty );
         else
            ch_printf( ch, "&YYou have no bounty on your head.\r\n" );
         if( IS_NPC( ch ) )
            ch_printf( ch, "&YMobiles can not collect bounties.\r\n" );
         else if( ch->bowed > 0 )
            ch_printf( ch, "&YYou are owed %d gold in bounty earnings.\r\n", ch->bowed );
         else
            ch_printf( ch, "&YYou have not collected any bounties.\r\n" );
         return;
      }

      if( !( victim = get_char_world( ch, arg2 ) ) )
      {
         do_say( bounty, "I don't know anyone by that name." );
         return;
      }

      if( IS_NPC( victim ) )
         do_say( bounty, "Mobiles cannot have bounties!" );
      else if( ( victim = get_char_world( ch, arg2 ) ) )
      {

         if( victim->bounty > 0 )
         {
            sprintf( buf, "%s %s's bounty is worth %d gold.", ch->name, victim->name, victim->bounty );
            do_tell( bounty, buf );
            return;
         }

         sprintf( buf, "%s That player has no bounty.", ch->name );
         do_tell( bounty, buf );
         return;
      }
      return;

   }

   if( !str_cmp( arg1, "buy" ) )
   {

      if( !( bounty = find_bounty( ch ) ) )
      {
         ch_printf( ch, "There is no bounty officer here!\r\n" );
         return;
      }

      if( ch->belig == 0 )
      {
         sprintf( buf, "%s You are already uneligible to have a bounty on your head!", ch->name );
         do_tell( bounty, buf );
         return;
      }

      if( ch->gold < cost )
      {
         sprintf( buf, "%s I'm sorry, you can't afford this.", ch->name );
         do_tell( bounty, buf );
         return;
      }

      ch->gold -= cost;
      ch->belig = 0;
      sprintf( buf, "%s You are no longer eligible to have a bounty on your head.", ch->name );
      do_tell( bounty, buf );
      return;
   }

   if( !str_cmp( arg1, "collect" ) )
   {
      if( ch->bowed <= 0 )
      {
         sprintf( buf, "%s I owe you nothing %s!", ch->name, ch->name );
         do_tell( bounty, buf );
         return;
      }

      ch->gold += ch->bowed;
      sprintf( buf, "&YYou collect %d gold worth of bounties.\r\n", ch->bowed );
      send_to_char( buf, ch );
      ch->bowed = 0;
      sprintf( buf, "%s A pleasure doing business with you.", ch->name );
      do_tell( bounty, buf );
      act( AT_ACTION, "$N hands a small sack of gold to $n.", ch, ch, bounty, TO_ROOM );
      return;
   }

   if( !( victim = get_char_world( ch, arg2 ) ) )
   {
      do_say( bounty, "That player is not online." );
      return;
   }

   if( IS_NPC( victim ) )
   {
      do_say( bounty, "A mobile? Kill it yourself!" );
      return;
   }

   amount = 0;

   if( !str_cmp( arg1, "all" ) )
   {
      amount = ch->gold;
   }

   if( amount == 0 )
   {
      amount = atoi( arg1 );
   }

   if( amount > ch->gold )
   {
      sprintf( buf, "%s Check your pockets and try again, you don't have that much gold!", ch->name );
      do_tell( bounty, buf );
      return;
   }

   if( amount <= 0 )
   {
      sprintf( buf, "%s That amount is too frugal, try again!", ch->name );
      do_tell( bounty, buf );
      return;
   }

   if( amount < 1000 )
   {
      sprintf( buf, "%s How do you expect me to stay in business at those prices!?", ch->name );
      do_tell( bounty, buf );
      return;
   }

   if( ( amount + victim->bounty ) > bountymax )
   {
      sprintf( buf, "%s That would exceed the maximum allowable bounty of %d gold!", ch->name, bountymax );
      do_tell( bounty, buf );
      return;
   }

   if( victim->level >= LEVEL_IMMORTAL && ch != victim )
   {
      sprintf( buf, "%s You may not place a bounty on the head of an immortal!", ch->name );
      do_tell( bounty, buf );
      return;
   }

   if( !IS_SET( victim->pcdata->flags, PCFLAG_DEADLY ) && victim->belig == 0 && ch != victim )
   {
      sprintf( buf, "%s You may only place a bounty on the head of a pkiller, or an eligible peaceful.", ch->name );
      do_tell( bounty, buf );
      return;
   }

   if( ch == victim )
   {
      sprintf( buf, "%s Place a bounty on yourself? Ever considered suicide?", ch->name );
      do_tell( bounty, buf );
      return;
   }

   if( !IS_SET( ch->pcdata->flags, PCFLAG_DEADLY ) && ch->belig == 0 )
   {
      sprintf( buf, "%s By doing this you are now eligible to have bounties set on your own head!", ch->name );
      do_tell( bounty, buf );
      sprintf( buf, "%s You can become uneligible again by typing bounty buy, at a cost of %d gold", ch->name, cost );
      do_tell( bounty, buf );
      ch->belig = 1;
   }

   if( victim->bounty < 0 )
   {
      victim->bounty = 0;
   }
   ch->gold -= amount;
   act( AT_ACTION, "$n hands a small sack of gold to $N.", ch, ch, bounty, TO_ROOM );
   if( victim->bounty > 0 )
   {
      sprintf( buf, "%s The bounty on %s has been increased by %d gold!", ch->name, victim->name, amount );
      do_tell( bounty, buf );
      sprintf( buf, "&YThe bounty on your head has been increased by %d gold!\r\n", amount );
      send_to_char( buf, victim );
      victim->bounty += amount;
      return;
   }
   victim->bounty += amount;
   sprintf( buf, "A bounty worth %d gold has been placed on the head of %s!", amount, victim->name );
   do_info( bounty, buf );
}

void do_unbounty( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;

   if( !( victim = get_char_world( ch, argument ) ) )
   {
      ch_printf( ch, "&YNo such player online.\r\n" );
      return;
   }

   if( victim->bounty > 0 )
   {
      ch_printf( ch, "&pRemoving Bounty...\r\nDone.\r\n" );
      victim->bounty = 0;
      return;
   }

   ch_printf( ch, "&YThat player has no bounty.\r\n" );
}
