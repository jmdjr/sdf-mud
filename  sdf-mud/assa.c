/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*
 *  Assassinate skill
 *  Author: Unknown (Tsunami?)
 *  This version of the original by Cronel (cronel_kal@hotmail.com)
 */
#include "mud.h"

/* from fight.c */
int xp_compute( CHAR_DATA * ch, CHAR_DATA * victim );
ch_ret one_hit( CHAR_DATA * ch, CHAR_DATA * victim, int dt );

void do_assassinate( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   short percent;
   int xp_gain;

   argument = one_argument( argument, arg1 );
   if( arg1[0] == '\0' )
   {
      send_to_char( "Who do you want to assassinate?\r\n", ch );
      return;
   }

   victim = get_char_room( ch, arg1 );
   if( victim == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   /*
    * Tek fixed 6/20/98 
    */
   if( ( IS_NPC( ch ) && ( ch->desc->original ) ) || ( IS_AFFECTED( ch, AFF_POSSESS ) ) )
   {
      send_to_char( "You can't do that in this state of mind.\r\n", ch );
      return;
   }

   if( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) )
   {
      send_to_char( "You can't do that right now.\r\n", ch );
      return;
   }

   if( IS_IMMORTAL( victim ) )
   {
      send_to_char( "I don't think so...\r\n", ch );
      return;
   }

   if( ch->mount )
   {
      send_to_char( "You can't get close enough while mounted.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "How can you sneak up on yourself?\r\n", ch );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   /*
    * Added Stabbing Weapon -Tsunami 
    */
   if( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || ( obj->value[3] != 11 && obj->value[3] != 2 ) )
   {
      send_to_char( "You need to wield a piercing or stabbing weapon.\r\n", ch );
      return;
   }


   if( victim->fighting )
   {
      send_to_char( "You can't assassinate someone who is in combat.\r\n", ch );
      return;
   }

   /*
    * Can assassinate a char even if it's hurt as long as it's sleeping. -Tsunami 
    */
   if( victim->hit < victim->max_hit && IS_AWAKE( victim ) )
   {
      act( AT_PLAIN, "$N is hurt and suspicious ... you can't sneak up.", ch, NULL, victim, TO_CHAR );
      return;
   }

   WAIT_STATE( ch, skill_table[gsn_assassinate]->beats );
   percent = number_percent(  ) + UMAX( 0, ( victim->level - ch->level ) * 2 );
   if( IS_NPC( ch ) || percent < ch->pcdata->learned[gsn_assassinate] )
   {
      act( AT_MAGIC, "You assassinate $N!", ch, NULL, victim, TO_CHAR );
      act( AT_MAGIC, "$n assassinates you!", ch, NULL, victim, TO_VICT );
      act( AT_MAGIC, "$n assassinates $N.", ch, NULL, victim, TO_NOTVICT );
      learn_from_success( ch, gsn_assassinate );
      check_killer( ch, victim );
      raw_kill( ch, victim );
      xp_gain = xp_compute( ch, victim );
      sprintf( buf, "You recieve %d experience points.\r\n", xp_gain );
      send_to_char( buf, ch );
      gain_exp( ch, xp_gain );
   }
   else
   {
      act( AT_MAGIC, "You fail to assassinate $N.", ch, NULL, victim, TO_CHAR );
      act( AT_MAGIC, "$n tried to assassinate you, but luckily $e failed...", ch, NULL, victim, TO_VICT );
      act( AT_MAGIC, "$n tries to assassinate $N but fails.", ch, NULL, victim, TO_NOTVICT );
      learn_from_failure( ch, gsn_assassinate );
      global_retcode = one_hit( ch, victim, TYPE_UNDEFINED );
   }
   return;
}
