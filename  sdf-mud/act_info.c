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
 *			     Informational module			    *
 ****************************************************************************/

#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#ifndef MUD_HFILE
#include "mud.h"
#include<stdlib.h>
#include<stdio.h>
#endif

char *const where_name[] = {
   "&B<&cused as light&B>&G     ",  /* 0 */
   "&B<&cworn on finger&B>&G    ",
   "&B<&cworn on finger&B>&G    ",
   "&B<&cworn around neck&B>&G  ",
   "&B<&cworn around neck&B>&G  ",
   "&B<&cworn on body&B>&G      ",
   "&B<&cworn on head&B>&G      ",
   "&B<&cworn on legs&B>&G      ",
   "&B<&cworn on feet&B>&G      ",
   "&B<&cworn on hands&B>&G     ",
   "&B<&cworn on arms&B>&G      ",  /* 10 */
   "&B<&cworn as shield&B>&G    ",
   "&B<&cworn about body&B>&G   ",
   "&B<&cworn about waist&B>&G  ",
   "&B<&cworn around wrist&B>&G ",
   "&B<&cworn around wrist&B>&G ",
   "&B<&cwielded&B>&G           ",
   "&B<&cheld&B>&G              ",
   "&B<&cdual wielded&B>&G      ",
   "&B<&cworn on ears&B>&G      ",
   "&B<&cworn on eyes&B>&G      ",  /* 20 */
   "&B<&cmissile wielded&B>&G   ",
   "&B<&cworn on back&B>&G      ",
   "&B<&cworn on face&B>&G      ",
   "&B<&cworn on ankle&B>&G     ",
   "&B<&cworn on ankle&B>&G     ",
   "&B<&cfloating nearby&B>&G   "
};

/*
 * Local functions.
 */
void show_char_to_char_0 args( ( CHAR_DATA * victim, CHAR_DATA * ch ) );
void show_char_to_char_1 args( ( CHAR_DATA * victim, CHAR_DATA * ch ) );
void show_char_to_char args( ( CHAR_DATA * list, CHAR_DATA * ch ) );
bool check_blind args( ( CHAR_DATA * ch ) );
void show_condition args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
#ifdef COMPRESSION_TEST
void decompressRoom args( ( ROOM_INDEX_DATA * room ) );
#endif
/* External Functions */
int strlen_color args( ( const char *argument ) );
char *SetFill( char *string );
void talk_channel args( ( CHAR_DATA *, char *, int, const char * ) );

void do_illegal_pk( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   IPK_DATA *illegal_pkill;

   if( ( victim = get_char_world( ch, argument ) ) == NULL && str_cmp( argument, "all" ) )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   if( !victim || IS_NPC( victim ) )
      ch_printf( ch, "&C> &wFor All:\r\n&C> &w%-12s %-12s %-12s\r\n", "Offender", "Victim", "Date" );
   else
      ch_printf( ch, "&C> &wFor %s:\r\n&C> &w%-12s %-12s\r\n", victim->name, "Victim", "Date" );
   for( illegal_pkill = first_pk; illegal_pkill; illegal_pkill = illegal_pkill->next )
   {
      if( !victim || IS_NPC( victim ) )
         ch_printf( ch, "&C> &z&R%-12s &Y%-12s &G%-12s&w", illegal_pkill->name, illegal_pkill->vict, illegal_pkill->date );
      else if( victim->name == illegal_pkill->name && victim )
         ch_printf( ch, "&C> &z&Y%-12s &G%-12s&w", illegal_pkill->vict, illegal_pkill->date );

   }
   send_to_char( "\r\n", ch );
   return;
}

char *num_punct( int foo )
{
   int index_new, rest, x;
   unsigned int nindex;
   char buf[16];
   static char buf_new[16];

   sprintf( buf, "%d", foo );
   rest = strlen( buf ) % 3;

   for( nindex = index_new = 0; nindex < strlen( buf ); nindex++, index_new++ )
   {
      x = nindex - rest;
      if( nindex != 0 && ( x % 3 ) == 0 )
      {
         buf_new[index_new] = ',';
         index_new++;
         buf_new[index_new] = buf[nindex];
      }
      else
         buf_new[index_new] = buf[nindex];
   }
   buf_new[index_new] = '\0';
   return buf_new;
}
char *format_obj_to_char( OBJ_DATA * obj, CHAR_DATA * ch, bool fShort )
{
   static char buf[MAX_STRING_LENGTH];

   buf[0] = '\0';

   /*
    * Sharpen skill --Cronel 
    */
   if( obj->item_type == ITEM_WEAPON && obj->value[5] == 1 )
      strcat( buf, "(Sharp) " );

   if( IS_OBJ_STAT( obj, ITEM_INVIS ) )
      strcat( buf, "(Invis) " );
   if( ( IS_AFFECTED( ch, AFF_DETECT_EVIL ) || is_class( ch, CLASS_PALADIN ) ) && IS_OBJ_STAT( obj, ITEM_EVIL ) )
      strcat( buf, "(Red Aura) " );

   if( is_class( ch, CLASS_PALADIN )
       && ( IS_OBJ_STAT( obj, ITEM_ANTI_EVIL ) && !IS_OBJ_STAT( obj, ITEM_ANTI_NEUTRAL )
            && !IS_OBJ_STAT( obj, ITEM_ANTI_GOOD ) ) )
      strcat( buf, "(Flaming Red) " );
   if( is_class( ch, CLASS_PALADIN )
       && ( !IS_OBJ_STAT( obj, ITEM_ANTI_EVIL ) && IS_OBJ_STAT( obj, ITEM_ANTI_NEUTRAL )
            && !IS_OBJ_STAT( obj, ITEM_ANTI_GOOD ) ) )
      strcat( buf, "(Flaming Grey) " );
   if( is_class( ch, CLASS_PALADIN )
       && ( !IS_OBJ_STAT( obj, ITEM_ANTI_EVIL ) && !IS_OBJ_STAT( obj, ITEM_ANTI_NEUTRAL )
            && IS_OBJ_STAT( obj, ITEM_ANTI_GOOD ) ) )
      strcat( buf, "(Flaming White) " );


   if( is_class( ch, CLASS_PALADIN )
       && ( IS_OBJ_STAT( obj, ITEM_ANTI_EVIL ) && IS_OBJ_STAT( obj, ITEM_ANTI_NEUTRAL )
            && !IS_OBJ_STAT( obj, ITEM_ANTI_GOOD ) ) )
      strcat( buf, "(Smouldering Red-Grey) " );
   if( is_class( ch, CLASS_PALADIN )
       && ( IS_OBJ_STAT( obj, ITEM_ANTI_EVIL ) && !IS_OBJ_STAT( obj, ITEM_ANTI_NEUTRAL )
            && IS_OBJ_STAT( obj, ITEM_ANTI_GOOD ) ) )
      strcat( buf, "(Smouldering Red-White) " );
   if( is_class( ch, CLASS_PALADIN )
       && ( !IS_OBJ_STAT( obj, ITEM_ANTI_EVIL ) && IS_OBJ_STAT( obj, ITEM_ANTI_NEUTRAL )
            && IS_OBJ_STAT( obj, ITEM_ANTI_GOOD ) ) )
      strcat( buf, "(Smouldering Grey-White) " );

   if( IS_AFFECTED( ch, AFF_DETECT_MAGIC ) && IS_OBJ_STAT( obj, ITEM_MAGIC ) )
      strcat( buf, "(Magical) " );
   if( IS_OBJ_STAT( obj, ITEM_GLOW ) )
      strcat( buf, "(Glowing) " );
   if( IS_OBJ_STAT( obj, ITEM_HUM ) )
      strcat( buf, "(Humming) " );
   if( IS_OBJ_STAT( obj, ITEM_HIDDEN ) )
      strcat( buf, "(Hidden) " );
   if( IS_OBJ_STAT( obj, ITEM_BURIED ) )
      strcat( buf, "(Buried) " );
   if( IS_IMMORTAL( ch ) && IS_OBJ_STAT( obj, ITEM_PROTOTYPE ) )
      strcat( buf, "(PROTO) " );
   if((IS_IMMORTAL( ch )  || IS_AFFECTED( ch, AFF_DETECTTRAPS )) && is_trapped( obj ) )
      strcat( buf, "(Trap) " );

   if( fShort )
   {
      if( obj->short_descr )
         strcat( buf, obj->short_descr );
   }
   else
   {
      if( obj->description )
         strcat( buf, obj->description );
   }

   return buf;
}

/*
 * Some increasingly freaky hallucinated objects		-Thoric
 * (Hats off to Albert Hoffman's "problem child")
 */
char *hallucinated_object( int ms, bool fShort )
{
   int sms = URANGE( 1, ( ms + 10 ) / 5, 20 );

   if( fShort )
      switch ( number_range( 6 - URANGE( 1, sms / 2, 5 ), sms ) )
      {
         case 1:
            return "a sword";
         case 2:
            return "a stick";
         case 3:
            return "something shiny";
         case 4:
            return "something";
         case 5:
            return "something interesting";
         case 6:
            return "something colorful";
         case 7:
            return "something that looks cool";
         case 8:
            return "a nifty thing";
         case 9:
            return "a cloak of flowing colors";
         case 10:
            return "a mystical flaming sword";
         case 11:
            return "a swarm of insects";
         case 12:
            return "a deathbane";
         case 13:
            return "a figment of your imagination";
         case 14:
            return "your gravestone";
         case 15:
            return "the long lost boots of Ranger Thoric";
         case 16:
            return "a glowing tome of arcane knowledge";
         case 17:
            return "a long sought secret";
         case 18:
            return "the meaning of it all";
         case 19:
            return "the answer";
         case 20:
            return "the key to life, the universe and everything";
      }
   switch ( number_range( 6 - URANGE( 1, sms / 2, 5 ), sms ) )
   {
      case 1:
         return "A nice looking sword catches your eye.";
      case 2:
         return "The ground is covered in small sticks.";
      case 3:
         return "Something shiny catches your eye.";
      case 4:
         return "Something catches your attention.";
      case 5:
         return "Something interesting catches your eye.";
      case 6:
         return "Something colorful flows by.";
      case 7:
         return "Something that looks cool calls out to you.";
      case 8:
         return "A nifty thing of great importance stands here.";
      case 9:
         return "A cloak of flowing colors asks you to wear it.";
      case 10:
         return "A mystical flaming sword awaits your grasp.";
      case 11:
         return "A swarm of insects buzzes in your face!";
      case 12:
         return "The extremely rare Deathbane lies at your feet.";
      case 13:
         return "A figment of your imagination is at your command.";
      case 14:
         return "You notice a gravestone here... upon closer examination, it reads your name.";
      case 15:
         return "The long lost boots of Ranger Thoric lie off to the side.";
      case 16:
         return "A glowing tome of arcane knowledge hovers in the air before you.";
      case 17:
         return "A long sought secret of all mankind is now clear to you.";
      case 18:
         return "The meaning of it all, so simple, so clear... of course!";
      case 19:
         return "The answer.  One.  It's always been One.";
      case 20:
         return "The key to life, the universe and everything awaits your hand.";
   }
   return "Whoa!!!";
}


/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char_type( OBJ_DATA * list, CHAR_DATA * ch, bool fShort, bool fShowNothing, int itemtype )
{
   char **prgpstrShow;
   int *prgnShow;
   int *pitShow;
   char *pstrShow;
   OBJ_DATA *obj;
   int nShow;
   int iShow;
   int type = 0;
   int count, offcount, tmp, ms, cnt;
   bool fCombine;

   if( itemtype > 0 )
      type = 1;

   /*
    * if there's no list... then don't do all this crap!  -Thoric
    */
   if( !list )
   {
      if( fShowNothing )
      {
         if( IS_NPC( ch ) || xIS_SET( ch->act, PLR_COMBINE ) )
            send_to_char( "     ", ch );
         set_char_color( AT_OBJECT, ch );
         send_to_char( "Nothing.\r\n", ch );
      }
      return;
   }
   /*
    * Alloc space for output lines.
    */
   count = 0;
   for( obj = list; obj; obj = obj->next_content )
      count++;

   ms = ( ch->mental_state ? ch->mental_state : 1 )
      * ( IS_NPC( ch ) ? 1 : ( ch->pcdata->condition[COND_DRUNK] ? ( ch->pcdata->condition[COND_DRUNK] / 12 ) : 1 ) );

   /*
    * If not mentally stable...
    */
   if( abs( ms ) > 40 )
   {
      offcount = URANGE( -( count ), ( count * ms ) / 100, count * 2 );
      if( offcount < 0 )
         offcount += number_range( 0, abs( offcount ) );
      else if( offcount > 0 )
         offcount -= number_range( 0, offcount );
   }
   else
      offcount = 0;

   if( count + offcount <= 0 )
   {
      if( fShowNothing )
      {
         if( IS_NPC( ch ) || xIS_SET( ch->act, PLR_COMBINE ) )
            send_to_char( "     ", ch );
         set_char_color( AT_OBJECT, ch );
         send_to_char( "Nothing.\r\n", ch );
      }
      return;
   }

   CREATE( prgpstrShow, char *, count + ( ( offcount > 0 ) ? offcount : 0 ) );
   CREATE( prgnShow, int, count + ( ( offcount > 0 ) ? offcount : 0 ) );
   CREATE( pitShow, int, count + ( ( offcount > 0 ) ? offcount : 0 ) );
   nShow = 0;
   tmp = ( offcount > 0 ) ? offcount : 0;
   cnt = 0;

   /*
    * Format the list of objects.
    */
   for( obj = list; obj; obj = obj->next_content )
   {
      if( offcount < 0 && ++cnt > ( count + offcount ) )
         break;
      if( type == 1 )   // For Examine, will only show a certain itemtype if prompted
      {
         if( obj->item_type != itemtype )
            continue;
      }
      if( tmp > 0 && number_bits( 1 ) == 0 )
      {
         prgpstrShow[nShow] = str_dup( hallucinated_object( ms, fShort ) );
         prgnShow[nShow] = 1;
         pitShow[nShow] = number_range( ITEM_LIGHT, ITEM_BOOK );
         nShow++;
         --tmp;
      }
      if( obj->wear_loc == WEAR_NONE
          && can_see_obj( ch, obj ) && ( obj->item_type != ITEM_TRAP || IS_AFFECTED( ch, AFF_DETECTTRAPS ) ) )
      {
         pstrShow = format_obj_to_char( obj, ch, fShort );
         fCombine = FALSE;

         if( IS_NPC( ch ) || xIS_SET( ch->act, PLR_COMBINE ) )
         {
            /*
             * Look for duplicates, case sensitive.
             * Matches tend to be near end so run loop backwords.
             */
            for( iShow = nShow - 1; iShow >= 0; iShow-- )
            {
               if( !strcmp( prgpstrShow[iShow], pstrShow ) )
               {
                  prgnShow[iShow] += obj->count;
                  fCombine = TRUE;
                  break;
               }
            }
         }

         pitShow[nShow] = obj->item_type;
         /*
          * Couldn't combine, or didn't want to.
          */
         if( !fCombine )
         {
            prgpstrShow[nShow] = str_dup( pstrShow );
            prgnShow[nShow] = obj->count;
            nShow++;
         }
      }
   }
   if( tmp > 0 )
   {
      int x;
      for( x = 0; x < tmp; x++ )
      {
         prgpstrShow[nShow] = str_dup( hallucinated_object( ms, fShort ) );
         prgnShow[nShow] = 1;
         pitShow[nShow] = number_range( ITEM_LIGHT, ITEM_BOOK );
         nShow++;
      }
   }

   /*
    * Output the formatted list.      -Color support by Thoric
    */
   for( iShow = 0; iShow < nShow; iShow++ )
   {
      switch ( pitShow[iShow] )
      {
         default:
            set_char_color( AT_OBJECT, ch );
            break;
         case ITEM_BLOOD:
            set_char_color( AT_BLOOD, ch );
            break;
         case ITEM_GOLD:
         case ITEM_TREASURE:
            set_char_color( AT_YELLOW, ch );
            break;
         case ITEM_SILVER:
            set_char_color( AT_WHITE, ch );
            break;
         case ITEM_COOK:
         case ITEM_FOOD:
            set_char_color( AT_HUNGRY, ch );
            break;
         case ITEM_DRINK_CON:
         case ITEM_FOUNTAIN:
            set_char_color( AT_THIRSTY, ch );
            break;
         case ITEM_COPPER:
         case ITEM_FIRE:
            set_char_color( AT_FIRE, ch );
            break;
         case ITEM_SCROLL:
         case ITEM_WAND:
         case ITEM_STAFF:
            set_char_color( AT_MAGIC, ch );
            break;
      }
      if( fShowNothing )
         send_to_char( "     ", ch );
      send_to_char( prgpstrShow[iShow], ch );
/*	if ( IS_NPC(ch) || xIS_SET(ch->act, PLR_COMBINE) ) */
      {
         if( prgnShow[iShow] != 1 )
            ch_printf( ch, " (%d)", prgnShow[iShow] );
      }

      send_to_char( "\r\n", ch );
      DISPOSE( prgpstrShow[iShow] );
   }

   if( fShowNothing && nShow == 0 )
   {
      if( IS_NPC( ch ) || xIS_SET( ch->act, PLR_COMBINE ) )
         send_to_char( "     ", ch );
      set_char_color( AT_OBJECT, ch );
      send_to_char( "Nothing.\r\n", ch );
   }

   /*
    * Clean up.
    */
   DISPOSE( prgpstrShow );
   DISPOSE( prgnShow );
   DISPOSE( pitShow );
   return;
}

    // So we do not have to declare show_list_to_char_type in mud.h
void show_list_to_char( OBJ_DATA * list, CHAR_DATA * ch, bool fShort, bool fShowNothing )
{
   show_list_to_char_type( list, ch, fShort, fShowNothing, 0 );
   return;
}


/*
 * Show fancy descriptions for certain spell affects		-Thoric
 */
void show_visible_affects_to_char( CHAR_DATA * victim, CHAR_DATA * ch )
{
   char buf[MAX_STRING_LENGTH];
   char name[MAX_STRING_LENGTH];

   if( IS_NPC( victim ) )
      strcpy( name, victim->short_descr );
   else
      strcpy( name, victim->name );
   name[0] = toupper( name[0] );

   if( IS_AFFECTED( victim, AFF_SANCTUARY ) )
   {
      set_char_color( AT_WHITE, ch );
      if( IS_GOOD( victim ) )
         ch_printf( ch, "%s glows with an aura of divine radiance.\r\n", name );
      else if( IS_EVIL( victim ) )
         ch_printf( ch, "%s shimmers beneath an aura of dark energy.\r\n", name );
      else
         ch_printf( ch, "%s is shrouded in flowing shadow and light.\r\n", name );
   }
   if( IS_AFFECTED( victim, AFF_FIRESHIELD ) )
   {
      set_char_color( AT_FIRE, ch );
      ch_printf( ch, "%s is engulfed within a blaze of mystical flame.\r\n", name );
   }
   if( IS_AFFECTED( victim, AFF_SHOCKSHIELD ) )
   {
      set_char_color( AT_BLUE, ch );
      ch_printf( ch, "%s is surrounded by cascading torrents of energy.\r\n", name );
   }
/*Scryn 8/13*/
   if( IS_AFFECTED( victim, AFF_ICESHIELD ) )
   {
      set_char_color( AT_LBLUE, ch );
      ch_printf( ch, "%s is ensphered by shards of glistening ice.\r\n", name );
   }
   if( IS_AFFECTED( victim, AFF_CHARM ) )
   {
      set_char_color( AT_MAGIC, ch );
      ch_printf( ch, "%s wanders in a dazed, zombie-like state.\r\n", name );
   }
   if( !IS_NPC( victim ) && !victim->desc && victim->switched && IS_AFFECTED( victim->switched, AFF_POSSESS ) )
   {
      set_char_color( AT_MAGIC, ch );
      strcpy( buf, PERS( victim, ch ) );
      strcat( buf, " appears to be in a deep trance...\r\n" );
   }
}

void show_char_to_char_0( CHAR_DATA * victim, CHAR_DATA * ch )
{
   char buf[MAX_STRING_LENGTH];
   char name[50];
   char buf1[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];

   buf[0] = '\0';

   set_char_color( AT_PERSON, ch );

   if( IS_NPC( victim ) && ch->questmob > 0 && victim->pIndexData->vnum == ch->questmob )
      strcat( buf, "&R[&WTARGET&R]&P " );

   if( !IS_NPC( victim ) && victim->bounty > 0 )
      strcat( buf, "&W(&RBOUNTY&W)&P " );
   if( !IS_NPC( victim ) && !victim->desc )
   {
      if( !victim->switched )
         send_to_char_color( "&P[(Link Dead)] ", ch );
      else if( !IS_AFFECTED( victim, AFF_POSSESS ) )
         strcat( buf, "(Switched) " );
   }
   if( IS_NPC( victim ) && IS_AFFECTED( victim, AFF_POSSESS ) && IS_IMMORTAL( ch ) && victim->desc )
   {
      sprintf( buf1, "(%s)", victim->desc->original->name );
      strcat( buf, buf1 );
   }
   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_AFK ) )
      strcat( buf, "[AWAY] " );

   if( ( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_WIZINVIS ) )
       || ( IS_NPC( victim ) && xIS_SET( victim->act, ACT_MOBINVIS ) && get_trust( ch ) >= LEVEL_IMMORTAL ) )
   {
      if( !IS_NPC( victim ) )
         sprintf( buf1, "(Invis %d) ", victim->pcdata->wizinvis );
      else
         sprintf( buf1, "(Mobinvis %d) ", victim->mobinvis );
      strcat( buf, buf1 );
   }

   if( !IS_NPC( victim ) && victim->pcdata->clan && victim->pcdata->clan->badge )
      ch_printf_color( ch, "%s ", victim->pcdata->clan->badge );

   set_char_color( AT_PERSON, ch );

   if( !IS_NPC( ch ) && xIS_SET( ch->act, PLR_HOLYLIGHT ) )
      if( IS_NPC( victim ) )
         sprintf( buf + strlen( buf ), "[%d] ", victim->pIndexData->vnum );
   if( IS_AFFECTED( victim, AFF_INVISIBLE ) )
      strcat( buf, "(Invis) " );
   if( IS_AFFECTED( victim, AFF_HIDE ) )
      strcat( buf, "(Hide) " );
   if( IS_AFFECTED( victim, AFF_PASS_DOOR ) )
      strcat( buf, "(Translucent) " );
   if( IS_AFFECTED( victim, AFF_FAERIE_FIRE ) )
      strcat( buf, "(Pink Aura) " );
   if( IS_EVIL( victim ) && ( IS_AFFECTED( ch, AFF_DETECT_EVIL ) || is_class( ch, CLASS_PALADIN ) ) )
      strcat( buf, "(Red Aura) " );
   if( IS_NEUTRAL( victim ) && is_class( ch, CLASS_PALADIN ) )
      strcat( buf, "(Grey Aura) " );
   if( IS_GOOD( victim ) && is_class( ch, CLASS_PALADIN ) )
      strcat( buf, "(White Aura) " );

   if( IS_AFFECTED( victim, AFF_BERSERK ) )
      strcat( buf, "(Wild-eyed) " );
   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_ATTACKER ) )
      strcat( buf, "(ATTACKER) " );
   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_COWARD ) )
      strcat( buf, "(COWARD) " );
   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_KILLER ) )
      strcat( buf, "(KILLER) " );
   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_THIEF ) )
      strcat( buf, "(THIEF) " );
   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_LITTERBUG ) )
      strcat( buf, "(LITTERBUG) " );
   if( IS_NPC( victim ) && IS_IMMORTAL( ch ) && xIS_SET( victim->act, ACT_PROTOTYPE ) )
      strcat( buf, "(PROTO) " );
   if( IS_NPC( victim ) && ch->mount && ch->mount == victim && ch->in_room == ch->mount->in_room )
      strcat( buf, "(Mount) " );
   if( victim->desc
       && ( victim->desc->connected == CON_EDITING
            || ( victim->desc->connected >= CON_NOTE_TO && victim->desc->connected <= CON_NOTE_FINISH ) ) )
      strcat( buf, "(Writing) " );

   if( victim->morph != NULL )
      strcat( buf, "(Morphed) " );

   set_char_color( AT_PERSON, ch );
   if( ( victim->position == victim->defposition && victim->long_descr[0] != '\0' )
       || ( victim->morph && victim->morph->morph && victim->morph->morph->defpos == victim->position ) )
   {
      if( victim->morph != NULL )
      {
         if( !IS_IMMORTAL( ch ) )
         {
            if( victim->morph->morph != NULL )
               strcat( buf, victim->morph->morph->long_desc );
            else
               strcat( buf, victim->long_descr );
         }
         else
         {
            strcat( buf, PERS( victim, ch ) );
            if( !IS_NPC( victim ) && !xIS_SET( ch->act, PLR_BRIEF ) )
               strcat( buf, victim->pcdata->title );
            strcat( buf, ".\r\n" );
         }
      }
      else
         strcat( buf, victim->long_descr );
      send_to_char( buf, ch );
      show_visible_affects_to_char( victim, ch );
      return;
   }
   else
   {
      if( victim->morph != NULL && victim->morph->morph != NULL && !IS_IMMORTAL( ch ) )
         strcpy( name, MORPHPERS( victim, ch ) );
      else
         strcpy( name, PERS( victim, ch ) );
   }

   if( !IS_NPC( victim ) && !xIS_SET( ch->act, PLR_BRIEF ) )
   {
      strcat( name, !IS_NPC( victim ) && victim->pcdata->lastname ? victim->pcdata->lastname : "" );
      /*
       * strcat( buf, victim->pcdata->title ); 
       */
   }

   buf2[0] = '\0';
   switch ( victim->position )
   {
      case POS_DEAD:
         strcat( buf2, " is DEAD!!" );
         break;
      case POS_MORTAL:
         strcat( buf2, " is mortally wounded." );
         break;
      case POS_INCAP:
         strcat( buf2, " is incapacitated." );
         break;
      case POS_STUNNED:
         strcat( buf2, " is lying here stunned." );
         break;
      case POS_SLEEPING:
         if( ch->position == POS_SITTING || ch->position == POS_RESTING || 
             ch->position == POS_RECOVER )
            strcat( buf2, " is sleeping nearby." );
         else
            strcat( buf2, " is deep in slumber here." );
         break;
      case POS_RESTING:
      case POS_RECOVER:
         if( ch->position == POS_RESTING )
            strcat( buf2, " is sprawled out alongside you." );
         else if( ch->position == POS_MOUNTED )
            strcat( buf2, " is sprawled out at the foot of your mount." );
         else
            strcat( buf2, " is sprawled out here." );
         break;
      case POS_SITTING:
         if( ch->position == POS_SITTING )
            strcat( buf2, " sits here with you." );
         else if( ch->position == POS_RESTING )
            strcat( buf2, " sits nearby as you lie around." );
         else
            strcat( buf2, " sits upright here." );
         break;
      case POS_STANDING:
         if( IS_IMMORTAL( victim ) )
            strcat( buf2, " is here before you." );
         else if( ( victim->in_room->sector_type == SECT_UNDERWATER )
                  && !IS_AFFECTED( victim, AFF_AQUA_BREATH ) && !IS_NPC( victim ) )
            strcat( buf2, " is drowning here." );
         else if( victim->in_room->sector_type == SECT_UNDERWATER )
            strcat( buf2, " is here in the water." );
         else if( ( victim->in_room->sector_type == SECT_OCEANFLOOR )
                  && !IS_AFFECTED( victim, AFF_AQUA_BREATH ) && !IS_NPC( victim ) )
            strcat( buf2, " is drowning here." );
         else if( victim->in_room->sector_type == SECT_OCEANFLOOR )
            strcat( buf2, " is standing here in the water." );
         else if( IS_AFFECTED( victim, AFF_FLOATING ) || IS_AFFECTED( victim, AFF_FLYING ) )
            strcat( buf2, " is hovering here." );
         else if( IS_AFFECTED( victim, AFF_HASTE ) )
            strcat( buf2, " is moving very rapidly!" );
         else if( IS_AFFECTED( victim, AFF_SLOW ) )
            strcat( buf2, " is moving very slowly!" );
         else
            strcat( buf2, " is standing here." );
         break;
      case POS_SHOVE:
         strcat( buf2, " is being shoved around." );
         break;
      case POS_DRAG:
         strcat( buf2, " is being dragged around." );
         break;
      case POS_MOUNTED:
         strcat( buf2, " is here, upon " );
         if( !victim->mount )
            strcat( buf2, "thin air???" );
         else if( victim->mount == ch )
            strcat( buf2, "your back." );
         else if( victim->in_room == victim->mount->in_room )
         {
            strcat( buf2, PERS( victim->mount, ch ) );
            strcat( buf2, "." );
         }
         else
            strcat( buf2, "someone who left??" );
         break;
      case POS_FIGHTING:
      case POS_EVASIVE:
      case POS_DEFENSIVE:
      case POS_AGGRESSIVE:
      case POS_BERSERK:
         strcat( buf2, " is here, fighting " );
         if( !victim->fighting )
         {
            strcat( buf2, "thin air???" );

            /*
             * some bug somewhere.... kinda hackey fix -h 
             */
            if( !victim->mount )
               victim->position = POS_STANDING;
            else
               victim->position = POS_MOUNTED;
         }
         else if( who_fighting( victim ) == ch )
            strcat( buf2, "YOU!" );
         else if( victim->in_room == victim->fighting->who->in_room )
         {
            strcat( buf2, PERS( victim->fighting->who, ch ) );
            strcat( buf2, "." );
         }
         else
            strcat( buf2, "someone who left??" );
         break;
   }
   if( victim->fur_pos > FURNITURE_NONE )
   {
      char buf3[30];
      sprintf( buf3, "%s", ( !IS_NPC( victim ) && victim->pcdata->lastname ? victim->pcdata->lastname : "" ) );

      if( victim->on )
         sprintf( buf2, "%s%s %s %s %s.",
                  PERS( victim, ch ),
                  ( !IS_NPC( victim ) && victim->pcdata->lastname ? buf3 : "" ),
                  pos_string( victim ), fur_pos_string( victim->fur_pos ), victim->on->short_descr );
      strcat( buf, buf2 );
   }
   else
      sprintf( buf + strlen( buf ), "%s%s", name, buf2 );

   strcat( buf, "\r\n" );
   buf[0] = UPPER( buf[0] );
   send_to_char( buf, ch );
   show_visible_affects_to_char( victim, ch );
   return;
}


void do_observe( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      set_char_color( AT_WHITE, ch );
      send_to_char( "What a cheap command.\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      set_char_color( AT_WHITE, ch );
      send_to_char( "They're not here.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      set_char_color( AT_WHITE, ch );
      send_to_char( "Having fun?\r\n", ch );
      return;
   }

   set_char_color( AT_WHITE, ch );
   act( AT_ACTION, "You demonstrate observe for $N.\r\n", ch, NULL, victim, TO_CHAR );
   act( AT_ACTION, "$n demonstrates observe for $N.\r\n", ch, NULL, victim, TO_NOTVICT );
   act( AT_ACTION, "$n demonstrates observe for you.\r\n", ch, NULL, victim, TO_VICT );
   return;
}



void show_char_to_char_1( CHAR_DATA * victim, CHAR_DATA * ch )
{
   OBJ_DATA *obj;
   int iWear;
   bool found;
   char buf2[MAX_STRING_LENGTH];

   if( can_see( victim, ch ) && !IS_NPC( ch ) && !xIS_SET( ch->act, PLR_WIZINVIS ) )
   {
      act( AT_ACTION, "$n looks at you.", ch, NULL, victim, TO_VICT );
      if( victim != ch )
         act( AT_ACTION, "$n looks at $N.", ch, NULL, victim, TO_NOTVICT );
      else
         act( AT_ACTION, "$n looks at $mself.", ch, NULL, victim, TO_NOTVICT );
   }

   if( victim->description != NULL )
   {
      if( victim->morph != NULL && victim->morph->morph != NULL )
         send_to_char( victim->morph->morph->description, ch );
      else
         send_to_char( victim->description, ch );
   }
   else
   {
      if( victim->morph != NULL && victim->morph->morph != NULL )
         send_to_char( victim->morph->morph->description, ch );
      else if( IS_NPC( victim ) )
         act( AT_PLAIN, "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
      else if( ch != victim )
         act( AT_PLAIN, "$E isn't much to look at...", ch, NULL, victim, TO_CHAR );
      else
         act( AT_PLAIN, "You're not much to look at...", ch, NULL, NULL, TO_CHAR );
   }

   if( victim->fur_pos > FURNITURE_NONE )
   {
      if( victim->on )
         sprintf( buf2, "%s %s %s %s.\n",
                  victim->name, pos_string( victim ), fur_pos_string( victim->fur_pos ), victim->on->short_descr );
      send_to_char( buf2, ch );
   }
   show_race_line( ch, victim );
   show_condition( ch, victim );

   found = FALSE;
   for( iWear = 0; iWear < MAX_WEAR; iWear++ )
   {
      if( ( obj = get_eq_char( victim, iWear ) ) != NULL && can_see_obj( ch, obj ) )
      {
         if( !found )
         {
            send_to_char( "\r\n", ch );
            if( victim != ch )
               act( AT_PLAIN, "$N is using:", ch, NULL, victim, TO_CHAR );
            else
               act( AT_PLAIN, "You are using:", ch, NULL, NULL, TO_CHAR );
            found = TRUE;
         }
         if( ( !IS_NPC( victim ) ) && ( victim->race > 0 ) && ( victim->race < MAX_RACE ) )
            send_to_char( race_table[victim->race]->where_name[iWear], ch );
         else
            send_to_char( where_name[iWear], ch );
         send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
         send_to_char( "\r\n", ch );
      }
   }

   /*
    * Crash fix here by Thoric
    */
   if( IS_NPC( ch ) || victim == ch )
      return;

   if( IS_IMMORTAL( ch ) )
   {
      if( IS_NPC( victim ) )
         ch_printf( ch, "\r\nMobile #%d '%s' ", victim->pIndexData->vnum, victim->name );
      else
         ch_printf( ch, "\r\n%s ", victim->name );
      ch_printf( ch, "is a level %d %s %s, a level %d %s and a level %d %s.\r\n",
                 victim->level,
                 victim->race < MAX_NPC_RACE && victim->race >= 0 ?
                 npc_race[victim->race] : "unknown",
                 victim->class < MAX_NPC_CLASS && victim->class >= 0 ?
                 npc_class[victim->class] : "unknown",
                 victim->level2,
                 victim->class2 < MAX_NPC_CLASS && victim->class2 >= 0 ?
                 npc_class[victim->class2] : "unknown",
                 victim->level3,
                 victim->class3 < MAX_NPC_CLASS && victim->class3 >= 0 ? npc_class[victim->class3] : "unknown" );
   }

   if( number_percent(  ) < LEARNED( ch, gsn_peek ) )
   {
      ch_printf( ch, "\r\nYou peek at %s inventory:\r\n", victim->sex == 1 ? "his" : victim->sex == 2 ? "her" : "its" );
      show_list_to_char( victim->first_carrying, ch, TRUE, TRUE );
      learn_from_success( ch, gsn_peek );
   }
   else if( ch->pcdata->learned[gsn_peek] > 0 )
      learn_from_failure( ch, gsn_peek );

   return;
}


void show_char_to_char( CHAR_DATA * list, CHAR_DATA * ch )
{
   CHAR_DATA *rch;

   for( rch = list; rch; rch = rch->next_in_room )
   {
      if( rch == ch )
         continue;

      if( rch == supermob && !IS_IMMORTAL( ch ) )
         continue;

      if( can_see( ch, rch ) )
      {
         show_char_to_char_0( rch, ch );
      }
      else if( room_is_dark( ch->in_room ) && IS_AFFECTED( rch, AFF_INFRARED ) && !( !IS_NPC( rch ) && IS_IMMORTAL( rch ) ) )
      {
         set_char_color( AT_BLOOD, ch );
         send_to_char( "The red form of a living creature is here.\r\n", ch );
      }
   }

   return;
}



bool check_blind( CHAR_DATA * ch )
{
   if( !IS_NPC( ch ) && xIS_SET( ch->act, PLR_HOLYLIGHT ) )
      return TRUE;

   if( IS_AFFECTED( ch, AFF_TRUESIGHT ) )
      return TRUE;

   if( IS_AFFECTED( ch, AFF_BLIND ) )
   {
      send_to_char( "You can't see a thing!\r\n", ch );
      return FALSE;
   }

   return TRUE;
}

/*
 * Returns classical DIKU door direction based on text in arg	-Thoric
 */
int get_door( char *arg )
{
   int door;

   if( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) )
      door = 0;
   else if( !str_cmp( arg, "e" ) || !str_cmp( arg, "east" ) )
      door = 1;
   else if( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) )
      door = 2;
   else if( !str_cmp( arg, "w" ) || !str_cmp( arg, "west" ) )
      door = 3;
   else if( !str_cmp( arg, "u" ) || !str_cmp( arg, "up" ) )
      door = 4;
   else if( !str_cmp( arg, "d" ) || !str_cmp( arg, "down" ) )
      door = 5;
   else if( !str_cmp( arg, "ne" ) || !str_cmp( arg, "northeast" ) )
      door = 6;
   else if( !str_cmp( arg, "nw" ) || !str_cmp( arg, "northwest" ) )
      door = 7;
   else if( !str_cmp( arg, "se" ) || !str_cmp( arg, "southeast" ) )
      door = 8;
   else if( !str_cmp( arg, "sw" ) || !str_cmp( arg, "southwest" ) )
      door = 9;
   else
      door = -1;
   return door;
}

void showRoom( CHAR_DATA * ch, ROOM_INDEX_DATA * room )
{
#ifdef COMPRESSION_TEST
   if( room->compress )
      decompressRoom( room );
#endif

   if( IS_SET( room->room_flags, ROOM_WILDERNESS ) )
   {
      ch_printf( ch, "%s", room->description );
      STRFREE( ch->in_room->name );
      DISPOSE( ch->in_room->description );
      return;
   }

   ch_printf( ch, "%s", room->description );

   /*
    * if( !room->compress )
    * ch_printf( ch, "%s", room->description );
    * else if (xIS_SET(ch->act, PLR_HOLYLIGHT))
    * {
    * decompressRoom( room );
    * ch_printf( ch, "&z&WCompressed&w -\r\n%s", room->uncomp_desc );
    * DISPOSE( room->uncomp_desc );
    * }
    * else
    * {
    * decompressRoom( room );
    * ch_printf( ch, "%s", room->uncomp_desc );
    * DISPOSE( room->uncomp_desc );
    * } 
    */
}

void do_look( CHAR_DATA * ch, char *argument )
{
   AREA_DATA *tarea;
   ROOM_INDEX_DATA *troom;
   char *kwd;  /* 3/17/99 Scion for hiscore */
   char arg[MAX_INPUT_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   char *sect;
   EXIT_DATA *pexit;
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   ROOM_INDEX_DATA *original;
   char *pdesc;
   bool doexaprog;
   short door;
   int number, cnt;

   if( !ch->desc )
      return;

   if( ch->position < POS_SLEEPING )
   {
      send_to_char( "You can't see anything but stars!\r\n", ch );
      return;
   }

   if( ch->position == POS_SLEEPING )
   {
      send_to_char( "You can't see anything, you're sleeping!\r\n", ch );
      return;
   }

   if( !check_blind( ch ) )
      return;

   if( !IS_NPC( ch )
       && !xIS_SET( ch->act, PLR_HOLYLIGHT ) && !IS_AFFECTED( ch, AFF_TRUESIGHT ) && 
room_is_dark( ch->in_room ) && !is_class(ch, CLASS_VAMPIRE) && ch->race != RACE_VAMPIRE )
   {
      set_char_color( AT_DGREY, ch );
      send_to_char( "It is pitch black ... \r\n", ch );
      show_char_to_char( ch->in_room->first_person, ch );
      return;
   }

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   argument = one_argument( argument, arg3 );

   doexaprog = str_cmp( "noprog", arg2 ) && str_cmp( "noprog", arg3 );

   if( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
   {
      /*
       * 'look' or 'look auto' 
       */
      set_char_color( AT_RMNAME, ch );
      /*
       * make life easier - shogar 
       */
      switch ( ch->in_room->sector_type )
      {
         default:
            sect = "?!";
            break;
         case SECT_INSIDE:
            sect = "Inside";
            break;
         case SECT_CITY:
            sect = "City";
            break;
         case SECT_FIELD:
            sect = "Field";
            break;
         case SECT_FOREST:
            sect = "Forest";
            break;
         case SECT_HILLS:
            sect = "Hills";
            break;
         case SECT_MOUNTAIN:
            sect = "Mountains";
            break;
         case SECT_WATER_SWIM:
            sect = "Swim";
            break;
         case SECT_WATER_NOSWIM:
            sect = "Noswim";
            break;
         case SECT_UNDERWATER:
            sect = "Underwater";
            break;
         case SECT_AIR:
            sect = "Air";
            break;
         case SECT_DESERT:
            sect = "Desert";
            break;
         case SECT_OCEANFLOOR:
            sect = "Oceanfloor";
            break;
         case SECT_UNDERGROUND:
            sect = "Underground";
            break;
      }
      troom = ch->in_room;
      tarea = troom->area;

      if( !IS_NPC( ch ) && xIS_SET( ch->act, PLR_HOLYLIGHT ) )
      {
         /*
          * ch_printf(ch,"&Y[&W %d",troom->vnum);
          * ch_printf(ch,"&Y |&W %s",tarea->filename);
          * if (str_cmp(flag_string(tarea->flags, area_flags), ""))
          * ch_printf(ch,"&Y |&G %s",flag_string(tarea->flags, area_flags));
          * ch_printf(ch,"&Y |&R %s",sect);
          * if (str_cmp(flag_string(troom->room_flags,r_flags), ""))
          * ch_printf(ch,"&Y |&G %s&Y ]&W\r\n",flag_string(troom->room_flags,r_flags));
          * else
          * ch_printf(ch,"&Y ]&W\r\n");
          * }
          * send_to_char( ch->in_room->name, ch );
          * send_to_char( "\r\n", ch ); 
          */
         ch_printf( ch, "&c[ &W%s ", tarea->filename );
         if( str_cmp( flag_string( tarea->flags, area_flags ), "" ) )
            ch_printf( ch, "&c| &G%s ", flag_string( tarea->flags, area_flags ) );
         ch_printf( ch, "&c| &R%s ", sect );
         if( str_cmp( flag_string( troom->room_flags, r_flags ), "" ) )
         {
            ch_printf( ch, "&c| &G" );
#ifdef COMPRESSION_TEST
            if( troom->compress )
               ch_printf( ch, "compressed " );
#endif
            ch_printf( ch, "%s &c]\r\n", flag_string( troom->room_flags, r_flags ) );
         }
#ifdef COMPRESSION_TEST
         else if( troom->compress )
            ch_printf( ch, "&c| &Gcompressed &c]\r\n" );
#endif
         else
            ch_printf( ch, "&c]\r\n" );
         ch_printf( ch, "&c[ &W%d &c] &W%s\r\n", troom->vnum, troom->name );
      }
      else
         ch_printf( ch, "%s\r\n", troom->name );

      set_char_color( ( IS_OUTSIDE( ch ) ? AT_RMDESC : AT_GREY ), ch );

      if( arg1[0] == '\0' || ( !IS_NPC( ch ) && !xIS_SET( ch->act, PLR_BRIEF ) ) )
      {
         /*
          * Moved to a seperate function for use in other areas as well
          * Kratas
          */
         showRoom( ch, ch->in_room );
         /*
          * if( !ch->in_room->compress )
          * ch_printf( ch, "%s", ch->in_room->description );
          * else if (xIS_SET(ch->act, PLR_HOLYLIGHT))
          * {
          * decompressRoom( ch->in_room );
          * ch_printf( ch, "&z&WCompressed&w -\r\n%s", ch->in_room->uncomp_desc );
          * DISPOSE( ch->in_room->uncomp_desc );
          * }
          * else
          * {
          * decompressRoom( ch->in_room );
          * ch_printf( ch, "%s", ch->in_room->uncomp_desc );
          * DISPOSE( ch->in_room->uncomp_desc );
          * } 
          */
      }

      /*
       * if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOMAP) )  
       *//*
       * maps 
       */
/*	{
	    if(ch->in_room->map != NULL)
	    {
	       do_lookmap(ch, NULL);
	    }
	} 
*/
      if( !IS_NPC( ch ) && xIS_SET( ch->act, PLR_AUTOEXIT ) )
         do_exits( ch, "auto" );
      else if( xIS_SET( ch->act, PLR_AUTOMAP ) )   /* config +map now works -Garinan */
         do_map( ch, NULL );


      show_list_to_char( ch->in_room->first_content, ch, FALSE, FALSE );
      show_char_to_char( ch->in_room->first_person, ch );
      return;
   }

   if( !str_cmp( arg1, "under" ) )
   {
      int count;

      /*
       * 'look under' 
       */
      if( arg2[0] == '\0' )
      {
         send_to_char( "Look beneath what?\r\n", ch );
         return;
      }

      if( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
      {
         send_to_char( "You do not see that here.\r\n", ch );
         return;
      }
      if( !CAN_WEAR( obj, ITEM_TAKE ) && ch->level < sysdata.level_getobjnotake )
      {
         send_to_char( "You can't seem to get a grip on it.\r\n", ch );
         return;
      }
      if( ch->carry_weight + obj->weight > can_carry_w( ch ) )
      {
         send_to_char( "It's too heavy for you to look under.\r\n", ch );
         return;
      }
      count = obj->count;
      obj->count = 1;
      act( AT_PLAIN, "You lift $p and look beneath it:", ch, obj, NULL, TO_CHAR );
      act( AT_PLAIN, "$n lifts $p and looks beneath it:", ch, obj, NULL, TO_ROOM );
      obj->count = count;
      if( IS_OBJ_STAT( obj, ITEM_COVERING ) )
         show_list_to_char( obj->first_content, ch, TRUE, TRUE );
      else
         send_to_char( "Nothing.\r\n", ch );
      if( doexaprog )
         oprog_examine_trigger( ch, obj );
      return;
   }

   if( !str_cmp( arg1, "i" ) || !str_cmp( arg1, "in" ) )
   {
      int count;

      /*
       * 'look in' 
       */
      if( arg2[0] == '\0' )
      {
         send_to_char( "Look in what?\r\n", ch );
         return;
      }

      if( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
      {
         send_to_char( "You do not see that here.\r\n", ch );
         return;
      }

      switch ( obj->item_type )
      {
         default:
            send_to_char( "That is not a container.\r\n", ch );
            break;

         case ITEM_DRINK_CON:
            if( obj->value[1] <= 0 )
            {
               send_to_char( "It is empty.\r\n", ch );
               if( doexaprog )
                  oprog_examine_trigger( ch, obj );
               break;
            }

            ch_printf( ch, "It's %s full of a %s liquid.\r\n",
                       obj->value[1] < obj->value[0] / 4
                       ? "less than" :
                       obj->value[1] < 3 * obj->value[0] / 4 ? "about" : "more than", liq_table[obj->value[2]].liq_color );

            if( doexaprog )
               oprog_examine_trigger( ch, obj );
            break;

         case ITEM_PORTAL:
            for( pexit = ch->in_room->first_exit; pexit; pexit = pexit->next )
            {
               if( pexit->vdir == DIR_PORTAL && IS_SET( pexit->exit_info, EX_PORTAL ) )
               {
                  if( room_is_private( pexit->to_room ) && get_trust( ch ) < sysdata.level_override_private )
                  {
                     set_char_color( AT_WHITE, ch );
                     send_to_char( "That room is private buster!\r\n", ch );
                     return;
                  }
                  original = ch->in_room;
                  char_from_room( ch );
                  char_to_room( ch, pexit->to_room );
                  do_look( ch, "auto" );
                  char_from_room( ch );
                  char_to_room( ch, original );
                  return;
               }
            }
            send_to_char( "You see swirling chaos...\r\n", ch );
            break;
         case ITEM_CONTAINER:
         case ITEM_QUIVER:
         case ITEM_CORPSE_NPC:
         case ITEM_CORPSE_PC:
            if( IS_SET( obj->value[1], CONT_CLOSED ) )
            {
               send_to_char( "It is closed.\r\n", ch );
               break;
            }

         case ITEM_KEYRING:
            count = obj->count;
            obj->count = 1;
            if( obj->item_type == ITEM_CONTAINER )
               act( AT_PLAIN, "$p contains:", ch, obj, NULL, TO_CHAR );
            else
               act( AT_PLAIN, "$p holds:", ch, obj, NULL, TO_CHAR );
            obj->count = count;
            show_list_to_char_type( obj->first_content, ch, TRUE, TRUE, atoi( argument ) );
            if( doexaprog )
               oprog_examine_trigger( ch, obj );
            break;
      }
      return;
   }

   if( ( pdesc = get_extra_descr( arg1, ch->in_room->first_extradesc ) ) != NULL )
   {
      send_to_char_color( pdesc, ch );
      return;
   }

   door = get_door( arg1 );
   if( ( pexit = find_door( ch, arg1, TRUE ) ) != NULL )
   {
      if( IS_SET( pexit->exit_info, EX_CLOSED ) && !IS_SET( pexit->exit_info, EX_WINDOW ) )
      {
         if( ( IS_SET( pexit->exit_info, EX_SECRET ) || IS_SET( pexit->exit_info, EX_DIG ) ) && door != -1 )
            send_to_char( "Nothing special there.\r\n", ch );
         else
            act( AT_PLAIN, "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
         return;
      }
      if( IS_SET( pexit->exit_info, EX_BASHED ) )
         act( AT_RED, "The $d has been bashed from its hinges!", ch, NULL, pexit->keyword, TO_CHAR );

      if( pexit->description && pexit->description[0] != '\0' )
         send_to_char( pexit->description, ch );
      else
         send_to_char( "Nothing special there.\r\n", ch );

      /*
       * Ability to look into the next room        -Thoric
       */
      if( pexit->to_room
          && ( IS_AFFECTED( ch, AFF_SCRYING )
               || is_class( ch, CLASS_THIEF )
               || is_class( ch, CLASS_ASSASSIN )
               || IS_SET( pexit->exit_info, EX_xLOOK ) || get_trust( ch ) >= LEVEL_IMMORTAL ) )
      {
         if( !IS_SET( pexit->exit_info, EX_xLOOK ) && get_trust( ch ) < LEVEL_IMMORTAL )
         {
            set_char_color( AT_MAGIC, ch );
            send_to_char( "You attempt to scry...\r\n", ch );
            /*
             * Change by Narn, Sept 96 to allow characters who don't have the
             * scry spell to benefit from objects that are affected by scry.
             */
            if( !IS_NPC( ch ) )
            {
               int percent = LEARNED( ch, skill_lookup( "scry" ) );
               if( !percent )
               {
                  if( is_class( ch, CLASS_THIEF ) )
                     percent = 95;
                  else
                     percent = 55;  /* 95 was too good -Thoric */
               }

               if( number_percent(  ) > percent )
               {
                  send_to_char( "You fail.\r\n", ch );
                  return;
               }
            }
         }
         if( room_is_private( pexit->to_room ) && get_trust( ch ) < sysdata.level_override_private )
         {
            set_char_color( AT_WHITE, ch );
            send_to_char( "That room is private buster!\r\n", ch );
            return;
         }
         original = ch->in_room;
         char_from_room( ch );
         char_to_room( ch, pexit->to_room );
         do_look( ch, "auto" );
         char_from_room( ch );
         char_to_room( ch, original );
      }
      return;
   }
   else if( door != -1 )
   {
      send_to_char( "Nothing special there.\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg1 ) ) != NULL )
   {
      show_char_to_char_1( victim, ch );
      return;
   }

   if( !str_cmp( arg1, "sky" ) )
   {
      if( !IS_OUTSIDE( ch ) )
      {
         send_to_char( "You can't see the sky indoors.\r\n", ch );
         return;
      }
      else
      {
         look_sky( ch );
         return;
      }
   }


   /*
    * finally fixed the annoying look 2.obj desc bug -Thoric 
    */
   number = number_argument( arg1, arg );
   for( cnt = 0, obj = ch->last_carrying; obj; obj = obj->prev_content )
   {
      /*
       * hiscore shtuff -- Scion 
       */
      kwd = is_hiscore_obj( obj );
      if( kwd )
      {
         show_hiscore( kwd, ch );
         return;
      }  /* end hiscore shtuff -- Scion */
      if( can_see_obj( ch, obj ) )
      {
         if( ( pdesc = get_extra_descr( arg, obj->first_extradesc ) ) != NULL )
         {
            if( ( cnt += obj->count ) < number )
               continue;
            send_to_char_color( pdesc, ch );
            if( doexaprog )
               oprog_examine_trigger( ch, obj );
            return;
         }

         if( ( pdesc = get_extra_descr( arg, obj->pIndexData->first_extradesc ) ) != NULL )
         {
            if( ( cnt += obj->count ) < number )
               continue;
            send_to_char_color( pdesc, ch );
            if( doexaprog )
               oprog_examine_trigger( ch, obj );
            return;
         }
         if( nifty_is_name_prefix( arg, obj->name ) )
         {
            if( ( cnt += obj->count ) < number )
               continue;
            pdesc = get_extra_descr( obj->name, obj->pIndexData->first_extradesc );
            if( !pdesc )
               pdesc = get_extra_descr( obj->name, obj->first_extradesc );
            if( !pdesc )
               send_to_char_color( "You see nothing special.\r\n", ch );
            else
               send_to_char_color( pdesc, ch );
            if( doexaprog )
               oprog_examine_trigger( ch, obj );
            return;
         }
      }
   }

   for( obj = ch->in_room->last_content; obj; obj = obj->prev_content )
   {
      /*
       * hiscore shtuff -- Scion 
       */
      kwd = is_hiscore_obj( obj );
      if( kwd )
      {
         show_hiscore( kwd, ch );
         return;
      }  /* end hiscore shtuph */
      if( can_see_obj( ch, obj ) )
      {
         if( ( pdesc = get_extra_descr( arg, obj->first_extradesc ) ) != NULL )
         {
            if( ( cnt += obj->count ) < number )
               continue;
            send_to_char_color( pdesc, ch );
            if( doexaprog )
               oprog_examine_trigger( ch, obj );
            return;
         }

         if( ( pdesc = get_extra_descr( arg, obj->pIndexData->first_extradesc ) ) != NULL )
         {
            if( ( cnt += obj->count ) < number )
               continue;
            send_to_char_color( pdesc, ch );
            if( doexaprog )
               oprog_examine_trigger( ch, obj );
            return;
         }
         if( nifty_is_name_prefix( arg, obj->name ) )
         {
            if( ( cnt += obj->count ) < number )
               continue;
            pdesc = get_extra_descr( obj->name, obj->pIndexData->first_extradesc );
            if( !pdesc )
               pdesc = get_extra_descr( obj->name, obj->first_extradesc );
            if( !pdesc )
               send_to_char( "You see nothing special.\r\n", ch );
            else
               send_to_char_color( pdesc, ch );
            if( doexaprog )
               oprog_examine_trigger( ch, obj );
            return;
         }
      }
   }

   send_to_char( "You do not see that here.\r\n", ch );
   return;
}

void show_race_line( CHAR_DATA * ch, CHAR_DATA * victim )
{
   char buf[MAX_STRING_LENGTH];
   int feet, inches;


   if( !IS_NPC( victim ) && ( victim != ch ) )
   {
      feet = victim->height / 12;
      inches = victim->height % 12;
      sprintf( buf, "%s is %d'%d\" and weighs %d pounds.\r\n", PERS( victim, ch ), feet, inches, victim->weight );
      send_to_char( buf, ch );
      sprintf( buf, "%s is %s, %s, %s %s haired with %s eyes and %s skin.\r\n", victim->sex == 0
               ? "It" : victim->sex == 1 ? "He" : "She", victim->pcdata->ch_height,
               victim->pcdata->ch_build, victim->pcdata->hair_length, victim->pcdata->hair_color,
               victim->pcdata->eye_color, victim->pcdata->skin_tone );
      send_to_char( buf, ch );
      return;
   }
   if( !IS_NPC( victim ) && ( victim == ch ) )
   {
      feet = victim->height / 12;
      inches = victim->height % 12;
      sprintf( buf, "You are %d'%d\" and weigh %d pounds.\r\n", feet, inches, victim->weight );
      send_to_char( buf, ch );
      return;
   }

}


void show_condition( CHAR_DATA * ch, CHAR_DATA * victim )
{
   char buf[MAX_STRING_LENGTH];
   int percent;

   if( victim->max_hit > 0 )
      percent = ( 100 * victim->hit ) / victim->max_hit;
   else
      percent = -1;


   if( victim != ch )
   {
      strcpy( buf, PERS( victim, ch ) );
      if( percent >= 100 )
         strcat( buf, " is in perfect health.\r\n" );
      else if( percent >= 90 )
         strcat( buf, " is slightly scratched.\r\n" );
      else if( percent >= 80 )
         strcat( buf, " has a few bruises.\r\n" );
      else if( percent >= 70 )
         strcat( buf, " has some cuts.\r\n" );
      else if( percent >= 60 )
         strcat( buf, " has several wounds.\r\n" );
      else if( percent >= 50 )
         strcat( buf, " has many nasty wounds.\r\n" );
      else if( percent >= 40 )
         strcat( buf, " is bleeding freely.\r\n" );
      else if( percent >= 30 )
         strcat( buf, " is covered in blood.\r\n" );
      else if( percent >= 20 )
         strcat( buf, " is leaking guts.\r\n" );
      else if( percent >= 10 )
         strcat( buf, " is almost dead.\r\n" );
      else
         strcat( buf, " is DYING.\r\n" );
   }
   else
   {
      strcpy( buf, "You" );
      if( percent >= 100 )
         strcat( buf, " are in perfect health.\r\n" );
      else if( percent >= 90 )
         strcat( buf, " are slightly scratched.\r\n" );
      else if( percent >= 80 )
         strcat( buf, " have a few bruises.\r\n" );
      else if( percent >= 70 )
         strcat( buf, " have some cuts.\r\n" );
      else if( percent >= 60 )
         strcat( buf, " have several wounds.\r\n" );
      else if( percent >= 50 )
         strcat( buf, " have many nasty wounds.\r\n" );
      else if( percent >= 40 )
         strcat( buf, " are bleeding freely.\r\n" );
      else if( percent >= 30 )
         strcat( buf, " are covered in blood.\r\n" );
      else if( percent >= 20 )
         strcat( buf, " are leaking guts.\r\n" );
      else if( percent >= 10 )
         strcat( buf, " are almost dead.\r\n" );
      else
         strcat( buf, " are DYING.\r\n" );
   }

   buf[0] = UPPER( buf[0] );
   send_to_char( buf, ch );
   return;
}

/* A much simpler version of look, this function will show you only
the condition of a mob or pc, or if used without an argument, the
same you would see if you enter the room and have config +brief.
-- Narn, winter '96
*/
void do_glance( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   bool brief;

   if( !ch->desc )
      return;

   if( ch->position < POS_SLEEPING )
   {
      send_to_char( "You can't see anything but stars!\r\n", ch );
      return;
   }

   if( ch->position == POS_SLEEPING )
   {
      send_to_char( "You can't see anything, you're sleeping!\r\n", ch );
      return;
   }

   if( !check_blind( ch ) )
      return;

   set_char_color( AT_ACTION, ch );
   argument = one_argument( argument, arg1 );

   if( arg1[0] == '\0' )
   {
      if( xIS_SET( ch->act, PLR_BRIEF ) )
         brief = TRUE;
      else
         brief = FALSE;
      xSET_BIT( ch->act, PLR_BRIEF );
      do_look( ch, "auto" );
      if( !brief )
         xREMOVE_BIT( ch->act, PLR_BRIEF );
      return;
   }

   if( ( victim = get_char_room( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They're not here.\r\n", ch );
      return;
   }
   else
   {
      if( can_see( victim, ch ) )
      {
         act( AT_ACTION, "$n glances at you.", ch, NULL, victim, TO_VICT );
         act( AT_ACTION, "$n glances at $N.", ch, NULL, victim, TO_NOTVICT );
      }
      if( IS_IMMORTAL( ch ) && victim != ch )
      {
         if( IS_NPC( victim ) )
            ch_printf( ch, "Mobile #%d '%s' ", victim->pIndexData->vnum, victim->name );
         else
            ch_printf( ch, "%s ", victim->name );
         ch_printf( ch, "is a level %d %s %s, a level %d %s and a level %d %s.\r\n",
                    victim->level,
                    victim->race < MAX_NPC_RACE && victim->race >= 0 ?
                    npc_race[victim->race] : "unknown",
                    victim->class < MAX_NPC_CLASS && victim->class >= 0 ?
                    npc_class[victim->class] : "unknown",
                    victim->level2,
                    victim->class2 < MAX_NPC_CLASS && victim->class2 >= 0 ?
                    npc_class[victim->class2] : "unknown",
                    victim->level3,
                    victim->class3 < MAX_NPC_CLASS && victim->class3 >= 0 ? npc_class[victim->class3] : "unknown" );
      }
      show_condition( ch, victim );

      return;
   }

   return;
}


void do_examine( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   BOARD_DATA *board;
   short dam;
   short value;

   if( !argument )
   {
      bug( "do_examine: null argument.", 0 );
      return;
   }

   if( !ch )
   {
      bug( "do_examine: null ch.", 0 );
      return;
   }

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Examine what?\r\n", ch );
      return;
   }

   sprintf( buf, "%s noprog", arg );
   do_look( ch, buf );

   /*
    * Support for looking at boards, checking equipment conditions,
    * and support for trigger positions by Thoric
    */
   if( ( obj = get_obj_here( ch, arg ) ) != NULL )
   {
      if( ( board = get_board( obj ) ) != NULL )
      {
         if( board->num_posts )
            ch_printf( ch, "There are about %d notes posted here.  Type 'note list' to list them.\r\n", board->num_posts );
         else
            send_to_char( "There aren't any notes posted here.\r\n", ch );
      }

      switch ( obj->item_type )
      {
         default:
            break;

         case ITEM_ARMOR:
            if( obj->value[1] == 0 )
               obj->value[1] = obj->value[0];
            if( obj->value[1] == 0 )
               obj->value[1] = 1;
            dam = ( short )( ( obj->value[0] * 10 ) / obj->value[1] );
            strcpy( buf, "As you look more closely, you notice that it is " );
            if( dam >= 10 )
               strcat( buf, "in superb condition." );
            else if( dam == 9 )
               strcat( buf, "in very good condition." );
            else if( dam == 8 )
               strcat( buf, "in good shape." );
            else if( dam == 7 )
               strcat( buf, "showing a bit of wear." );
            else if( dam == 6 )
               strcat( buf, "a little run down." );
            else if( dam == 5 )
               strcat( buf, "in need of repair." );
            else if( dam == 4 )
               strcat( buf, "in great need of repair." );
            else if( dam == 3 )
               strcat( buf, "in dire need of repair." );
            else if( dam == 2 )
               strcat( buf, "very badly worn." );
            else if( dam == 1 )
               strcat( buf, "practically worthless." );
            else if( dam <= 0 )
               strcat( buf, "broken." );
            strcat( buf, "\r\n" );
            send_to_char( buf, ch );
            break;

         case ITEM_WEAPON:
            dam = INIT_WEAPON_CONDITION - obj->value[0];
            strcpy( buf, "As you look more closely, you notice that it is " );
            if( dam == 0 )
               strcat( buf, "in superb condition." );
            else if( dam == 1 )
               strcat( buf, "in excellent condition." );
            else if( dam == 2 )
               strcat( buf, "in very good condition." );
            else if( dam == 3 )
               strcat( buf, "in good shape." );
            else if( dam == 4 )
               strcat( buf, "showing a bit of wear." );
            else if( dam == 5 )
               strcat( buf, "a little run down." );
            else if( dam == 6 )
               strcat( buf, "in need of repair." );
            else if( dam == 7 )
               strcat( buf, "in great need of repair." );
            else if( dam == 8 )
               strcat( buf, "in dire need of repair." );
            else if( dam == 9 )
               strcat( buf, "very badly worn." );
            else if( dam == 10 )
               strcat( buf, "practically worthless." );
            else if( dam == 11 )
               strcat( buf, "almost broken." );
            else if( dam == 12 )
               strcat( buf, "broken." );
            strcat( buf, "\r\n" );
            send_to_char( buf, ch );
            break;

         case ITEM_COOK:
            strcpy( buf, "As you examine it carefully you notice that it " );
            dam = obj->value[2];
            if( dam >= 3 )
               strcat( buf, "is burned to a crisp." );
            else if( dam == 2 )
               strcat( buf, "is a little over cooked." );
            else if( dam == 1 )
               strcat( buf, "is perfectly roasted." );
            else
               strcat( buf, "is raw." );
            strcat( buf, "\r\n" );
            send_to_char( buf, ch );
         case ITEM_FOOD:
            if( obj->timer > 0 && obj->value[1] > 0 )
               dam = ( obj->timer * 10 ) / obj->value[1];
            else
               dam = 10;
            if( obj->item_type == ITEM_FOOD )
               strcpy( buf, "As you examine it carefully you notice that it " );
            else
               strcpy( buf, "Also it " );
            if( dam >= 10 )
               strcat( buf, "is fresh." );
            else if( dam == 9 )
               strcat( buf, "is nearly fresh." );
            else if( dam == 8 )
               strcat( buf, "is perfectly fine." );
            else if( dam == 7 )
               strcat( buf, "looks good." );
            else if( dam == 6 )
               strcat( buf, "looks ok." );
            else if( dam == 5 )
               strcat( buf, "is a little stale." );
            else if( dam == 4 )
               strcat( buf, "is a bit stale." );
            else if( dam == 3 )
               strcat( buf, "smells slightly off." );
            else if( dam == 2 )
               strcat( buf, "smells quite rank." );
            else if( dam == 1 )
               strcat( buf, "smells revolting!" );
            else if( dam <= 0 )
               strcat( buf, "is crawling with maggots!" );
            strcat( buf, "\r\n" );
            send_to_char( buf, ch );
            break;


         case ITEM_SWITCH:
         case ITEM_LEVER:
         case ITEM_PULLCHAIN:
            if( IS_SET( obj->value[0], TRIG_UP ) )
               send_to_char( "You notice that it is in the up position.\r\n", ch );
            else
               send_to_char( "You notice that it is in the down position.\r\n", ch );
            break;
         case ITEM_BUTTON:
            if( IS_SET( obj->value[0], TRIG_UP ) )
               send_to_char( "You notice that it is depressed.\r\n", ch );
            else
               send_to_char( "You notice that it is not depressed.\r\n", ch );
            break;

/* Not needed due to check in do_look already
	case ITEM_PORTAL:
	    sprintf( buf, "in %s noprog", arg );
	    do_look( ch, buf );
	    break;
*/

         case ITEM_CORPSE_PC:
         case ITEM_CORPSE_NPC:
         {
            short timerfrac = obj->timer;
            if( obj->item_type == ITEM_CORPSE_PC )
               timerfrac = ( int )obj->timer / 8 + 1;

            switch ( timerfrac )
            {
               default:
                  send_to_char( "This corpse has recently been slain.\r\n", ch );
                  break;
               case 4:
                  send_to_char( "This corpse was slain a little while ago.\r\n", ch );
                  break;
               case 3:
                  send_to_char( "A foul smell rises from the corpse, and it is covered in flies.\r\n", ch );
                  break;
               case 2:
                  send_to_char( "A writhing mass of maggots and decay, you can barely go near this corpse.\r\n", ch );
                  break;
               case 1:
               case 0:
                  send_to_char( "Little more than bones, there isn't much left of this corpse.\r\n", ch );
                  break;
            }
         }
         case ITEM_CONTAINER:
            if( IS_OBJ_STAT( obj, ITEM_COVERING ) )
               break;
         case ITEM_DRINK_CON:
         case ITEM_QUIVER:
            send_to_char( "When you look inside, you see:\r\n", ch );
         case ITEM_KEYRING:
            value = get_otype( argument );
            if( value > 0 )
               sprintf( buf, "in %s noprog %d", arg, value );
            else
               sprintf( buf, "in %s noprog", arg );
            do_look( ch, buf );
            break;
      }

      if( IS_OBJ_STAT( obj, ITEM_COVERING ) )
      {
         sprintf( buf, "under %s noprog", arg );
         do_look( ch, buf );
      }
      oprog_examine_trigger( ch, obj );
      if( char_died( ch ) || obj_extracted( obj ) )
         return;

      check_for_trap( ch, obj, TRAP_EXAMINE );
   }
   return;
}


void do_exits( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   EXIT_DATA *pexit;
   bool found;
   bool fAuto;
   bool showmap = FALSE;
   int min, max, counter;

   set_char_color( AT_EXITS, ch );
   buf[0] = '\0';
   fAuto = !str_cmp( argument, "auto" );

   if( !check_blind( ch ) )
      return;

   min = ( 215 / 2 ) - 6 / 2;
   max = ( 215 / 2 ) + 6 / 2;


   if( xIS_SET( ch->act, PLR_AUTOMAP ) )  // was commented
   {
      showmap = TRUE;
      prep_map( ch->in_room, min, max );
      strcpy( buf, "&C&c." );
      for( counter = 0; counter < 6; counter++ )
         strcat( buf, "-" );
      strcat( buf, ". " );
      strcat( buf, fAuto ? "Exits:\r\n" : "Obvious exits:\r\n" );
   }
   else  // was commented
      strcpy( buf, fAuto ? "Exits:" : "Obvious exits:\r\n" );  // was commented

   found = FALSE;
   counter = min;
   for( pexit = ch->in_room->first_exit; pexit; pexit = pexit->next )
   {

      if( !IS_NPC( ch ) && xIS_SET( ch->act, PLR_HOLYLIGHT ) )
      {
         if( pexit->to_room )
         {
            if( showmap && counter < max )
            {
               strcat( buf, make_map_line( counter, min, max ) );
               counter += 1;

            }
            if( IS_SET( pexit->exit_info, EX_CLOSED ) &&
                ( IS_SET( pexit->exit_info, EX_WINDOW ) || IS_SET( pexit->exit_info, EX_ISDOOR ) ) )
               sprintf( buf + strlen( buf ), " %s&c[ &W%-5d &c| &W(%-9s) &c] - %s\r\n",
                        ( fAuto && !showmap ) ? found ? "      " : "" : "",
                        pexit->to_room->vnum, capitalize( dir_name[pexit->vdir] ), pexit->to_room->name );
            else
               sprintf( buf + strlen( buf ), " %s&c[ &W%-5d &c| &W%-11s &c] - %s\r\n",
                        ( fAuto && !showmap ) ? found ? "      " : "" : "",
                        pexit->to_room->vnum, capitalize( dir_name[pexit->vdir] ), pexit->to_room->name );
            found = TRUE;
         }
         /*
          * make life easier - shogar 
          */
         /*
          * if ( !IS_NPC(ch) && xIS_SET(ch->act, PLR_HOLYLIGHT) )
          * sprintf(buf+strlen(buf),"[%d]-",pexit->to_room->vnum); 
          */
      }
      else if( !IS_NPC( ch ) && IS_SET( ch->in_room->area->flags, AFLAG_EXIT_NAMES ) && !xIS_SET( ch->act, PLR_BRIEF ) )
      {
         if( pexit->to_room )
         {
            if( showmap && counter < max )
            {
               strcat( buf, make_map_line( counter, min, max ) );
               counter += 1;
            }
            if( !IS_SET( pexit->exit_info, EX_CLOSED )
                && ( !IS_SET( pexit->exit_info, EX_WINDOW )
                     || IS_SET( pexit->exit_info, EX_ISDOOR ) ) && !IS_SET( pexit->exit_info, EX_HIDDEN ) )
               sprintf( buf + strlen( buf ), " %s&c[ &W%-5d &c| &W(%-9s) &c] - %s\r\n",
                        ( fAuto && !showmap ) ? found ? "      " : "" : "",
                        pexit->to_room->vnum, capitalize( dir_name[pexit->vdir] ), pexit->to_room->name );
            else
               sprintf( buf + strlen( buf ), " %s&c[ &W%-5d &c| &W%-11s &c] - %s\r\n",
                        ( fAuto && !showmap ) ? found ? "      " : "" : "",
                        pexit->to_room->vnum, capitalize( dir_name[pexit->vdir] ), pexit->to_room->name );
         }
      }
      else
      {
         if( pexit->to_room

/* List closed exits right after room desc
*             && !IS_SET( pexit->exit_info, EX_CLOSED )
*/
             && ( !IS_SET( pexit->exit_info, EX_WINDOW )
                  || IS_SET( pexit->exit_info, EX_ISDOOR ) ) && !IS_SET( pexit->exit_info, EX_HIDDEN ) )
         {
            if( showmap && counter < max )
            {
               strcat( buf, make_map_line( counter, min, max ) );
               counter += 1;
            }
            found = TRUE;
            if( fAuto )
            {
               strcat( buf, " " );
               if (IS_SET( pexit->exit_info, EX_CLOSED ))
               {
                 strcat( buf, "[");
                 strcat( buf, dir_name[pexit->vdir] );
                 strcat( buf, "]");
               } else {
                 strcat( buf, dir_name[pexit->vdir] );
               }
               if( showmap )
                  strcat( buf, "\r\n" );
            }
            else
            {
               if (IS_SET( pexit->exit_info, EX_CLOSED ))
               {
                 sprintf( buf + strlen( buf ), "[%-5s] - %s\r\n",
                        capitalize( dir_name[pexit->vdir] ),
                        room_is_dark( pexit->to_room ) ? "Too dark to tell" : pexit->to_room->name );
               }
               else
               {
                 sprintf( buf + strlen( buf ), "%-5s - %s\r\n",
                        capitalize( dir_name[pexit->vdir] ),
                        room_is_dark( pexit->to_room ) ? "Too dark to tell" : pexit->to_room->name );
               }
            }
         }
      }
   }
   if( !found )
   {
      if( showmap && counter < max )
      {
         strcat( buf, make_map_line( counter, min, max ) );
         counter += 1;
      }

      strcat( buf, fAuto ? " none.\r\n" : "None.\r\n" );
   }
   else if( fAuto && !xIS_SET( ch->act, PLR_HOLYLIGHT ) && !showmap )
      strcat( buf, ".\r\n" );
   while( showmap && counter < max )
   {
      strcat( buf, make_map_line( counter, min, max ) );
      strcat( buf, "\r\n" );
      counter += 1;
   }

   if( showmap )
   {
      strcat( buf, "&C&c'" );
      for( counter = 0; counter < 6; counter++ )
         strcat( buf, "-" );
      strcat( buf, "'\r\n" );
   }
   send_to_char( buf, ch );
   return;
}

char *const day_name[] = {
   "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
   "the Great Gods", "the Sun"
};

char *const month_name[] = {
   "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
   "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
   "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
   "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void do_time( CHAR_DATA * ch, char *argument )
{
   char *suf;
   int day;

   day = time_info.day + 1;

   if( day > 4 && day < 20 )
      suf = "th";
   else if( day % 10 == 1 )
      suf = "st";
   else if( day % 10 == 2 )
      suf = "nd";
   else if( day % 10 == 3 )
      suf = "rd";
   else
      suf = "th";

   set_char_color( AT_YELLOW, ch );
   ch_printf( ch,
              "\r\nIt is %d o'clock %s\r\nDay of %s, %d%s the Month of %s, in the year %d.\r\n"
              "The mud started up at:    %s\r"
              "The system time (E.S.T.): %s\r"
              "Next Reboot is set for:   %s\r",
              ( time_info.hour % 12 == 0 ) ? 12 : time_info.hour % 12,
              time_info.hour >= 12 ? "pm" : "am",
              day_name[day % 7],
              day, suf,
              month_name[time_info.month], time_info.year, str_boot_time, ( char * )ctime( &current_time ), reboot_time );
   if( sysdata.CLEANPFILES )
      ch_printf( ch, "Next pfile cleanup is scheduled for: %s\r\n", ( char * )ctime( &new_pfile_time_t ) );

   return;
}

/*
void do_weather( CHAR_DATA *ch, char *argument )
{
    static char * const sky_look[4] =
    {
	"cloudless",
	"cloudy",
	"rainy",
	"lit by flashes of lightning"
    };

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You can't see the sky from here.\r\n", ch );
	return;
    }

    set_char_color( AT_BLUE, ch );
    ch_printf( ch, "The sky is %s and %s.\r\n",
	sky_look[weather_info.sky],
	weather_info.change >= 0
	? "a warm southerly breeze blows"
	: "a cold northern gust blows"
	);
    return;
}
*/

/*
 * Produce a description of the weather based on area weather using
 * the following sentence format:
 *		<combo-phrase> and <single-phrase>.
 * Where the combo-phrase describes either the precipitation and
 * temperature or the wind and temperature. The single-phrase
 * describes either the wind or precipitation depending upon the
 * combo-phrase.
 * Last Modified: July 31, 1997
 * Fireblade - Under Construction
 */
void do_weather( CHAR_DATA * ch, char *argument )
{
   char *combo, *single;
   int temp, precip, wind;

   if( !IS_OUTSIDE( ch ) )
   {
      ch_printf( ch, "You can't see the sky from here.\r\n" );
      return;
   }

   temp = ( ch->in_room->area->weather->temp + 3 * weath_unit - 1 ) / weath_unit;
   precip = ( ch->in_room->area->weather->precip + 3 * weath_unit - 1 ) / weath_unit;
   wind = ( ch->in_room->area->weather->wind + 3 * weath_unit - 1 ) / weath_unit;

   if( precip >= 3 )
   {
      combo = preciptemp_msg[precip][temp];
      single = wind_msg[wind];
   }
   else
   {
      combo = windtemp_msg[wind][temp];
      single = precip_msg[precip];
   }

   set_char_color( AT_BLUE, ch );
   ch_printf( ch, "%s and %s.\r\n", combo, single );
}

/*
 * Moved into a separate function so it can be used for other things
 * ie: online help editing				-Thoric
 */
HELP_DATA *get_help( CHAR_DATA * ch, char *argument )
{
   char argall[MAX_INPUT_LENGTH];
   char argone[MAX_INPUT_LENGTH];
   char argnew[MAX_INPUT_LENGTH];
   HELP_DATA *pHelp;
   int lev;

   if( argument[0] == '\0' )
      argument = "summary";

   if( isdigit( argument[0] ) )
   {
      lev = number_argument( argument, argnew );
      argument = argnew;
   }
   else
      lev = -2;
   /*
    * Tricky argument handling so 'help a b' doesn't match a.
    */
   argall[0] = '\0';
   while( argument[0] != '\0' )
   {
      argument = one_argument( argument, argone );
      if( argall[0] != '\0' )
         strcat( argall, " " );
      strcat( argall, argone );
   }

   for( pHelp = first_help; pHelp; pHelp = pHelp->next )
   {
      if( pHelp->level > get_trust( ch ) )
         continue;
      if( lev != -2 && pHelp->level != lev )
         continue;

      if( is_name( argall, pHelp->keyword ) )
         return pHelp;
   }

   return NULL;
}

/*
 * LAWS command
 */
void do_laws( CHAR_DATA * ch, char *argument )
{
   char buf[1024];

   if( argument == NULL )
      do_help( ch, "laws" );
   else
   {
      sprintf( buf, "law %s", argument );
      do_help( ch, buf );
   }
}

/*
 * Now this is cleaner
 */
/* Updated do_help command provided by Remcon of The Lands of Pabulum 03/20/2004 */
void do_help( CHAR_DATA * ch, char *argument )
{
   HELP_DATA *pHelp;
   char *keyword;
   char arg[MAX_INPUT_LENGTH];
   char oneword[MAX_STRING_LENGTH], lastmatch[MAX_STRING_LENGTH];
   short matched = 0, checked = 0, totalmatched = 0, found = 0;
   bool uselevel = FALSE;
   int value = 0;
   FILE *nohelp;

   set_pager_color( AT_NOTE, ch );

   if( !argument || argument[0] == '\0' )
      argument = "summary";
   if( !( pHelp = get_help( ch, argument ) ) )
   {
      pager_printf( ch, "No help on \'%s\' found.\r\n", argument );
      if((nohelp = fopen( "../system/nohelp.txt", "a" )) == 0)
      {
         log_string("Error opening nohelp.txt");
      } else
      {
         fprintf( nohelp, "%s\n", argument );
         fclose(nohelp);
      }
      /*
       * Get an arg incase they do a number seperate 
       */
      one_argument( argument, arg );
      /*
       * See if arg is a number if so update argument 
       */
      if( is_number( arg ) )
      {
         argument = one_argument( argument, arg );
         if( argument && argument[0] != '\0' )
         {
            value = atoi( arg );
            uselevel = TRUE;
         }
         else  /* If no more argument put arg as argument */
            argument = arg;
      }
      if( value > 0 )
         pager_printf( ch, "Checking for suggested helps that are level %d.\r\n", value );
      send_to_pager( "Suggested Help Files:\r\n", ch );
      strncpy( lastmatch, " ", MAX_STRING_LENGTH );
      for( pHelp = first_help; pHelp; pHelp = pHelp->next )
      {
         matched = 0;
         if( !pHelp || !pHelp->keyword || pHelp->keyword[0] == '\0' || pHelp->level > get_trust( ch ) )
            continue;
         /*
          * Check arg if its avaliable 
          */
         if( uselevel && pHelp->level != value )
            continue;
         keyword = pHelp->keyword;
         while( keyword && keyword[0] != '\0' )
         {
            matched = 0;   /* Set to 0 for each time we check lol */
            keyword = one_argument( keyword, oneword );
            /*
             * Lets check only up to 10 spots
             */
            for( checked = 0; checked <= 10; checked++ )
            {
               if( !oneword[checked] || !argument[checked] )
                  break;
               if( LOWER( oneword[checked] ) == LOWER( argument[checked] ) )
                  matched++;
            }
            if( ( matched > 1 && matched > ( checked / 2 ) ) || ( matched > 0 && checked < 2 ) )
            {
               pager_printf( ch, " %-20s ", oneword );
               if( ++found % 4 == 0 )
               {
                  found = 0;
                  send_to_pager( "\r\n", ch );
               }
               strncpy( lastmatch, oneword, MAX_STRING_LENGTH );
               totalmatched++;
               break;
            }
         }
      }
      if( found != 0 )
         send_to_pager( "\r\n", ch );
      if( totalmatched == 0 )
      {
         send_to_pager( "No suggested help files.\r\n", ch );
         return;
      }
      if( totalmatched == 1 && lastmatch != NULL && lastmatch && lastmatch[0] != '\0' && str_cmp( lastmatch, argument ) )
      {
         send_to_pager( "Opening only suggested helpfile.\r\n", ch );
         do_help( ch, lastmatch );
         return;
      }
      return;
   }
   /*
    * Make newbies do a help start. --Shaddai
    */
   if( !IS_NPC( ch ) && !str_cmp( argument, "start" ) )
      SET_BIT( ch->pcdata->flags, PCFLAG_HELPSTART );

   if( IS_IMMORTAL( ch ) )
      pager_printf( ch, "Help level: %d\r\n", pHelp->level );

   set_pager_color( AT_NOTE, ch );

   /*
    * Strip leading '.' to allow initial blanks.
    */
   if( pHelp->text[0] == '.' )
      send_to_pager( pHelp->text + 1, ch );
   else
      send_to_pager( pHelp->text, ch );
   return;
}

extern char *help_greeting;   /* so we can edit the greeting online */

/*
 * Help editor							-Thoric
 */
void do_hedit( CHAR_DATA * ch, char *argument )
{
   HELP_DATA *pHelp;

   if( argument[0] == '\0' )
      argument = "help";
   //strcpy(argument, "help");

   if( !ch->desc )
   {
      send_to_char( "You have no descriptor.\r\n", ch );
      return;
   }

   switch ( ch->substate )
   {
      default:
         break;
      case SUB_HELP_EDIT:
         if( ( pHelp = ch->dest_buf ) == NULL )
         {
            bug( "hedit: sub_help_edit: NULL ch->dest_buf", 0 );
            stop_editing( ch );
            return;
         }

         if( help_greeting == pHelp->text )
            help_greeting = NULL;

         STRFREE( pHelp->text );
         pHelp->text = copy_buffer( ch );

         if( !help_greeting )
            help_greeting = pHelp->text;

         stop_editing( ch );
         return;
   }

   if( ( pHelp = get_help( ch, argument ) ) == NULL ) /* new help */
   {
      char argnew[MAX_INPUT_LENGTH];
      int lev;

      if( isdigit( argument[0] ) )
      {
         lev = number_argument( argument, argnew );
         argument = argnew;
      }
      else
         lev = get_trust( ch );
      CREATE( pHelp, HELP_DATA, 1 );
      pHelp->keyword = STRALLOC( strupper( argument ) );
      pHelp->text = STRALLOC( "" );
      pHelp->level = lev;
      add_help( pHelp );
   }
   ch->substate = SUB_HELP_EDIT;
   ch->dest_buf = pHelp;
   start_editing( ch, pHelp->text );
}

/*
 * Stupid leading space muncher fix				-Thoric
 */
char *help_fix( char *text )
{
   char *fixed;

   if( !text )
      return "";
   fixed = strip_cr( text );
   if( fixed[0] == ' ' )
      fixed[0] = '.';
   return fixed;
}

void do_hset( CHAR_DATA * ch, char *argument )
{
   HELP_DATA *pHelp;
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: hset <field> [value] [help page]\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "Field being one of:\r\n", ch );
      send_to_char( "  level keyword remove save\r\n", ch );
      return;
   }

   if( !str_cmp( arg1, "save" ) )
   {
      FILE *fpout;

      log_string_plus( "Saving help.are...", LOG_NORMAL, LEVEL_GREATER );

      rename( "help.are", "help.are.bak" );
      if( ( fpout = fopen( "help.are", "w" ) ) == NULL )
      {
         bug( "hset save: fopen", 0 );
         perror( "help.are" );
         return;
      }

      fprintf( fpout, "#HELPS\n\n" );
      for( pHelp = first_help; pHelp; pHelp = pHelp->next )
         fprintf( fpout, "%d %s~\n%s~\n\n", pHelp->level, pHelp->keyword, help_fix( pHelp->text ) );

      fprintf( fpout, "0 $~\n\n\n#$\n" );
      fclose( fpout );
      fpout = NULL;
      send_to_char( "Saved.\r\n", ch );
      return;
   }
   if( str_cmp( arg1, "remove" ) )
      argument = one_argument( argument, arg2 );

   if( ( pHelp = get_help( ch, argument ) ) == NULL )
   {
      send_to_char( "Cannot find help on that subject.\r\n", ch );
      return;
   }
   if( !str_cmp( arg1, "remove" ) )
   {
      UNLINK( pHelp, first_help, last_help, next, prev );
      STRFREE( pHelp->text );
      STRFREE( pHelp->keyword );
      DISPOSE( pHelp );
      send_to_char( "Removed.\r\n", ch );
      return;
   }
   if( !str_cmp( arg1, "level" ) )
   {
      pHelp->level = atoi( arg2 );
      send_to_char( "Done.\r\n", ch );
      return;
   }
   if( !str_cmp( arg1, "keyword" ) )
   {
      STRFREE( pHelp->keyword );
      pHelp->keyword = STRALLOC( strupper( arg2 ) );
      send_to_char( "Done.\r\n", ch );
      return;
   }

   do_hset( ch, "" );
}

void do_hl( CHAR_DATA * ch, char *argument )
{
   send_to_char( "If you want to use HLIST, spell it out.\r\n", ch );
   return;
}

/*
 * Show help topics in a level range				-Thoric
 * Idea suggested by Gorog
 * prefix keyword indexing added by Fireblade
 */
void do_hlist( CHAR_DATA * ch, char *argument )
{
   int min, max, minlimit, maxlimit, cnt;
   char arg[MAX_INPUT_LENGTH];
   HELP_DATA *help;
   bool minfound, maxfound;
   char *idx;

   maxlimit = get_trust( ch );
   minlimit = maxlimit >= LEVEL_GREATER ? -1 : 0;

   min = minlimit;
   max = maxlimit;

   idx = NULL;
   minfound = FALSE;
   maxfound = FALSE;

   for( argument = one_argument( argument, arg ); arg[0] != '\0'; argument = one_argument( argument, arg ) )
   {
      if( !isdigit( arg[0] ) )
      {
         if( idx )
         {
            set_char_color( AT_GREEN, ch );
            ch_printf( ch, "You may only use a single keyword to index the list.\r\n" );
            return;
         }
         idx = STRALLOC( arg );
      }
      else
      {
         if( !minfound )
         {
            min = URANGE( minlimit, atoi( arg ), maxlimit );
            minfound = TRUE;
         }
         else if( !maxfound )
         {
            max = URANGE( minlimit, atoi( arg ), maxlimit );
            maxfound = TRUE;
         }
         else
         {
            set_char_color( AT_GREEN, ch );
            ch_printf( ch, "You may only use two level limits.\r\n" );
            return;
         }
      }
   }

   if( min > max )
   {
      int temp = min;

      min = max;
      max = temp;
   }

   set_pager_color( AT_GREEN, ch );
   pager_printf( ch, "&cHelp Topics in level range &W%d &cto &W%d&c:\r\n\r\n", min, max );
   for( cnt = 0, help = first_help; help; help = help->next )
      if( help->level >= min && help->level <= max && ( !idx || nifty_is_name_prefix( idx, help->keyword ) ) )
      {
         pager_printf( ch, "&c  %3d &w%s\r\n", help->level, help->keyword );
         ++cnt;
      }
   if( cnt )
      pager_printf( ch, "\r\n&Y%d pages found.&w\r\n", cnt );
   else
      send_to_char( "None found.\r\n", ch );

   if( idx )
      STRFREE( idx );

   return;
}

/* 
 * New do_who with WHO REQUEST, clan, race and homepage support.  -Thoric
 *
 * Latest version of do_who eliminates redundant code by using linked lists.
 * Shows imms separately, indicates guest and retired immortals.
 * Narn, Oct/96
 *
 * Who group by Altrag, Feb 28/97
 *
 * Fixed it so guests morts and imms see each other and took out the
 * different lists for morts and imms -- Scion
 */
struct whogr_s
{
   struct whogr_s *next;
   struct whogr_s *follower;
   struct whogr_s *l_follow;
   DESCRIPTOR_DATA *d;
   int indent;
} *first_whogr, *last_whogr;

struct whogr_s *find_whogr( DESCRIPTOR_DATA * d, struct whogr_s *first )
{
   struct whogr_s *whogr, *whogr_t;

   for( whogr = first; whogr; whogr = whogr->next )
      if( whogr->d == d )
         return whogr;
      else if( whogr->follower && ( whogr_t = find_whogr( d, whogr->follower ) ) )
         return whogr_t;
   return NULL;
}

void indent_whogr( CHAR_DATA * looker, struct whogr_s *whogr, int ilev )
{
   for( ; whogr; whogr = whogr->next )
   {
      if( whogr->follower )
      {
         int nlev = ilev;
         CHAR_DATA *wch = ( whogr->d->original ? whogr->d->original : whogr->d->character );

         if( can_see( looker, wch ) && !IS_IMMORTAL( wch ) )
            nlev += 3;
         indent_whogr( looker, whogr->follower, nlev );
      }
      whogr->indent = ilev;
   }
}

/* This a great big mess to backwards-structure the ->leader character
   fields */
void create_whogr( CHAR_DATA * looker )
{
   DESCRIPTOR_DATA *d;
   CHAR_DATA *wch;
   struct whogr_s *whogr, *whogr_t;
   int dc = 0, wc = 0;

   while( ( whogr = first_whogr ) != NULL )
   {
      first_whogr = whogr->next;
      DISPOSE( whogr );
   }
   first_whogr = last_whogr = NULL;
   /*
    * Link in the ones without leaders first 
    */
   for( d = last_descriptor; d; d = d->prev )
   {
      if( ( d->connected != CON_PLAYING
            && d->connected != CON_EDITING ) || ( d->connected <= CON_NOTE_TO && d->connected >= CON_NOTE_FINISH ) )
         continue;
      ++dc;
      wch = ( d->original ? d->original : d->character );
      if( !wch->leader || wch->leader == wch || !wch->leader->desc ||
          IS_NPC( wch->leader ) || IS_IMMORTAL( wch ) || IS_IMMORTAL( 
           wch->leader ) )
      {
         CREATE( whogr, struct whogr_s, 1 );
         if( !last_whogr )
            first_whogr = last_whogr = whogr;
         else
         {
            last_whogr->next = whogr;
            last_whogr = whogr;
         }
         whogr->next = NULL;
         whogr->follower = whogr->l_follow = NULL;
         whogr->d = d;
         whogr->indent = 0;
         ++wc;
      }
   }
   /*
    * Now for those who have leaders.. 
    */
   while( wc < dc )
      for( d = last_descriptor; d; d = d->prev )
      {
         if( ( d->connected != CON_PLAYING
               && d->connected != CON_EDITING ) || ( d->connected <= CON_NOTE_TO && d->connected >= CON_NOTE_FINISH ) )
            continue;
         if( find_whogr( d, first_whogr ) )
            continue;
         wch = ( d->original ? d->original : d->character );
         if( wch->leader && wch->leader != wch && wch->leader->desc &&
             !IS_NPC( wch->leader ) && !IS_IMMORTAL( wch ) &&
             !IS_IMMORTAL( wch->leader ) && ( whogr_t = find_whogr( wch->leader->desc, first_whogr ) ) )
         {
            CREATE( whogr, struct whogr_s, 1 );
            if( !whogr_t->l_follow )
               whogr_t->follower = whogr_t->l_follow = whogr;
            else
            {
               whogr_t->l_follow->next = whogr;
               whogr_t->l_follow = whogr;
            }
            whogr->next = NULL;
            whogr->follower = whogr->l_follow = NULL;
            whogr->d = d;
            whogr->indent = 0;
            ++wc;
         }
      }
   /*
    * Set up indentation levels 
    */
   indent_whogr( looker, first_whogr, 0 );

   /*
    * And now to linear link them.. 
    */
   for( whogr_t = NULL, whogr = first_whogr; whogr; )
      if( whogr->l_follow )
      {
         whogr->l_follow->next = whogr;
         whogr->l_follow = NULL;
         if( whogr_t )
            whogr_t->next = whogr = whogr->follower;
         else
            first_whogr = whogr = whogr->follower;
      }
      else
      {
         whogr_t = whogr;
         whogr = whogr->next;
      }
}

void do_who( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
  
     char clan_name[MAX_INPUT_LENGTH];
     char council_name[MAX_INPUT_LENGTH];
      char council2_name[MAX_INPUT_LENGTH];
   
   char invis_str[MAX_INPUT_LENGTH];
   char char_name[MAX_INPUT_LENGTH];
   char *extra_title;
   char class_text[MAX_INPUT_LENGTH];
   struct whogr_s *whogr, *whogr_p;
   DESCRIPTOR_DATA *d;
   int iClass, iRace;
   int iLevelLower;
   int iLevelUpper;
   int nNumber;
   int nMatch;
   bool rgfClass[MAX_CLASS];
   bool rgfRace[MAX_RACE];
   bool fClassRestrict;
   bool fRaceRestrict;
   bool fImmortalOnly;
   bool fLeader;
   bool fPkill;
   bool fShowHomepage;
   bool fClanMatch;  /* SB who clan (order),who guild, and who council */
   bool fCouncilMatch;
   bool fDeityMatch;
   bool fGroup;
   // bool fRPWho;
   CLAN_DATA *pClan = NULL;
   COUNCIL_DATA *pCouncil = NULL;
   DEITY_DATA *pDeity = NULL;
   FILE *whoout = NULL;
   extern int reboot_in;

   WHO_DATA *cur_who = NULL;
   WHO_DATA *next_who = NULL;
   WHO_DATA *first_mortal = NULL;
   // WHO_DATA *first_imm = NULL;
   WHO_DATA *first_deadly = NULL;
   WHO_DATA *first_grouped = NULL;
   WHO_DATA *first_groupwho = NULL;

   /*
    * Redone immortal displaying 
    */
   WHO_DATA *first_guest = NULL;
   // WHO_DATA *first_staff = NULL;
   WHO_DATA *first_admin = NULL;

   /*
    * Set default arguments.
    */
   iLevelLower = 0;
   iLevelUpper = MAX_LEVEL;
   fClassRestrict = FALSE;
   fRaceRestrict = FALSE;
   fImmortalOnly = FALSE;
   //fPkill         = FALSE;
   fShowHomepage = FALSE;
   fClanMatch = FALSE;  /* SB who clan (order), who guild, who council */
   fCouncilMatch = FALSE;
   fDeityMatch = FALSE;
   fGroup = FALSE;   /* Alty who group */
   fLeader = FALSE;
   //fRPWho     = (IS_IMMORTAL(ch) ? FALSE : TRUE);
   for( iClass = 0; iClass < MAX_CLASS; iClass++ )
      rgfClass[iClass] = FALSE;
   for( iRace = 0; iRace < MAX_RACE; iRace++ )
      rgfRace[iRace] = FALSE;

#ifdef REQWHOARG
   /*
    * The who command must have at least one argument because we often
    * have up to 500 players on. Too much spam if a player accidentally
    * types "who" with no arguments.           --Gorog
    */
   if( ch && argument[0] == '\0' )
   {
      send_to_pager_color( "\r\n&GYou must specify at least one argument.\r\nUse 'who 1' to view the entire who list.\r\n",
                           ch );
      return;
   }
#endif

   /*
    * Parse arguments.
    */
   nNumber = 0;
   for( ;; )
   {
      char arg[MAX_STRING_LENGTH];

      argument = one_argument( argument, arg );

      if( arg[0] == '\0' )
         break;

      if( is_number( arg ) )
      {
         switch ( ++nNumber )
         {
            case 1:
               iLevelLower = atoi( arg );
               break;
            case 2:
               iLevelUpper = atoi( arg );
               break;
            default:
               send_to_char( "Only two level numbers allowed.\r\n", ch );
               return;
         }
      }
      else
      {
         if( strlen( arg ) < 3 )
         {
            send_to_char( "Arguments must be longer than that.\r\n", ch );
            return;
         }

         /*
          * Look for classes to turn on.
          */
	if( !str_cmp( arg, "deadly" ) || !str_cmp( arg, "pkill" ) )
          fPkill = TRUE;
         else if( !str_cmp( arg, "imm" ) || !str_cmp( arg, "gods" ) )
           fImmortalOnly = TRUE;
         else if( !str_cmp( arg, "leader" ) )
           fLeader = TRUE;
         else if( !str_cmp( arg, "www" ) )
           fShowHomepage = TRUE;
         else if( !str_cmp( arg, "group" ) && ch )
          fGroup = TRUE;
         else if( ( pClan = get_clan( arg ) ) )
            fClanMatch = TRUE; 
         else if( ( pCouncil = get_council( arg ) ) )
            fCouncilMatch = TRUE;
         else if( ( pDeity = get_deity( arg ) ) )
            fDeityMatch = TRUE;
         else
         {
       for( iClass = 0; iClass < MAX_CLASS; iClass++ )
            {
       	  if( !str_cmp( arg, class_table[iClass]->who_name) )
               {
                  rgfClass[iClass] = TRUE;
		send_to_char("157. \r\n", ch);
       		  break;
               }
            }
            if( iClass != MAX_CLASS )
               fClassRestrict = TRUE;

            for( iRace = 0; iRace < MAX_RACE; iRace++ )
            {
          if( !str_cmp( arg, race_table[iRace]->race_name ) )
               {
                  rgfRace[iRace] = TRUE;
                  break;
               }
            }
            if( iRace != MAX_RACE )
               fRaceRestrict = TRUE;

            if( iClass == MAX_CLASS && iRace == MAX_RACE
                && fClanMatch == FALSE && fCouncilMatch == FALSE && fDeityMatch == FALSE )
            {
               send_to_char( "That's not a class, race, order, guild," " council or deity.\r\n", ch );
               return;
            }
         }
      }
   }

   /*
    * Now find matching chars.
    */
   nMatch = 0;
   buf[0] = '\0';
   if( ch )
      set_pager_color( AT_GREEN, ch );
   else
   {
      if( fShowHomepage )
         whoout = fopen( WEBWHO_FILE, "w" );
      else
         whoout = fopen( WHO_FILE, "w" );
      if( !whoout )
      {
         bug( "do_who: cannot open who file!" );
         return;
      }
   }

/* start from last to first to get it in the proper order */
   if( fGroup )
   {
      create_whogr( ch );
      whogr = first_whogr;
      d = whogr->d;
   }
   else
   {
      whogr = NULL;
      d = last_descriptor;
   }
   whogr_p = NULL;
   for( ; d; whogr_p = whogr, whogr = ( fGroup ? whogr->next : NULL ),
        d = ( fGroup ? ( whogr ? whogr->d : NULL ) : d->prev ) )
   {
      CHAR_DATA *wch;
      char const *class;

      if( ( ( d->connected != CON_PLAYING
              && d->connected != CON_EDITING )
            || ( d->connected <= CON_NOTE_TO
                 && d->connected >= CON_NOTE_FINISH ) ) || !can_see( ch, d->character ) || d->original )
         continue;
      wch = d->original ? d->original : d->character;
      if( wch->level < iLevelLower || wch->level > iLevelUpper
          /*
           * || ( !CAN_PKILL( wch ) ) 
           *//*
           * If ya want only level 10 players and up to show, uncomment the line to the left 
           */
          || ( fImmortalOnly && wch->level < LEVEL_IMMORTAL ) || ( fClassRestrict && !rgfClass[wch->class] ) || ( fClassRestrict && !rgfClass[wch->class2] ) || ( fClassRestrict && !rgfClass[wch->class3] ) || ( fRaceRestrict && !rgfRace[wch->race] ) || ( fClanMatch && ( pClan != wch->pcdata->clan ) )   /* SB */
          || ( ( fCouncilMatch && ( pCouncil != wch->pcdata->council ) )   /* SB */
               && ( fCouncilMatch && ( pCouncil != wch->pcdata->council2 ) ) ) /*RTD*/
          || ( fDeityMatch && ( pDeity != wch->pcdata->deity ) ) )
         continue;
      if( fLeader && !( wch->pcdata->council &&
                        ( ( wch->pcdata->council->head2 &&
                            !str_cmp( wch->pcdata->council->head2, wch->name ) ) ||
                          ( wch->pcdata->council->head &&
                            !str_cmp( wch->pcdata->council->head, wch->name ) ) ) ) &&
          !( wch->pcdata->clan && ( ( wch->pcdata->clan->deity &&
                                      !str_cmp( wch->pcdata->clan->deity, wch->name ) )
                                    || ( wch->pcdata->clan->leader
                                         && !str_cmp( wch->pcdata->clan->leader, wch->name ) )
                                    || ( wch->pcdata->clan->number1
                                         && !str_cmp( wch->pcdata->clan->number1, wch->name ) )
                                    || ( wch->pcdata->clan->number2
                                         && !str_cmp( wch->pcdata->clan->number2, wch->name ) ) ) ) )
         continue;

      if( fGroup && !wch->leader && !IS_SET( wch->pcdata->flags, PCFLAG_GROUPWHO ) && ( !whogr_p || !whogr_p->indent ) )
         continue;

      nMatch++;

      if( fShowHomepage && wch->pcdata->homepage && wch->pcdata->homepage[0] != '\0' )
         sprintf( char_name, "<A HREF=\"%s\">%s</A>", show_tilde( wch->pcdata->homepage ), wch->name );
      else
         strcpy( char_name, wch->name );

      /*
       * Got a better way? Please... do tell 
       */
      if( wch->level2 == -1 )
         sprintf( class_text, "%3d %3.3s", wch->level,
                  !str_cmp( class_table[wch->class]->who_name, "Assassin" ) ? "Asn" : class_table[wch->class]->who_name );
      else if( wch->level3 == -1 )
         sprintf( class_text, "%3d %2.2s/%3d %2.2s",
                  wch->level,
                  !str_cmp( class_table[wch->class]->who_name, "Assassin" )
                  ? "Asn" : class_table[wch->class]->who_name,
                  wch->level2,
                  !str_cmp( class_table[wch->class2]->who_name, "Assassin" ) ? "Asn" : class_table[wch->class2]->who_name );
      else
         /*
          * Just chenged the last two to class name only, this makes em fit better
          * and both will be level 100 by default anyway -Garinan 
          */
         sprintf( class_text, "%3d %2.2s/%2.2s/%2.2s",
                  wch->level,
                  !str_cmp( class_table[wch->class]->who_name, "Assassin" ) ? "Asn" : class_table[wch->class]->who_name,
                  /*
                   * wch->level2, 
                   */
                  !str_cmp( class_table[wch->class2]->who_name, "Assassin" ) ? "Asn" : class_table[wch->class2]->who_name,
                  /*
                   * wch->level3, 
                   */
                  !str_cmp( class_table[wch->class3]->who_name, "Assassin" ) ? "Asn" : class_table[wch->class3]->who_name );

      class = class_text;
      switch ( wch->level )
      {
         default:
            break;
         case MAX_LEVEL - 0:
            class = "&ROwner";
            break;
         case MAX_LEVEL - 1:
            class = "&RHead Imp.";
            break;
         case MAX_LEVEL - 2:
            class = "&RImplementor";
            break;
         case MAX_LEVEL - 3:
            class = "&RAsst. Imp.";
            break;
         case MAX_LEVEL - 4:
            class = "&RAdmin.";
            break;
         case MAX_LEVEL - 5:
            class = "&cCoder";
            break;
         case MAX_LEVEL - 6:
            class = "&cAdmin Rep.";
            break;
         case MAX_LEVEL - 7:
            class = "&cMaster Bldr";
            break;
         case MAX_LEVEL - 8:
            class = "&cGreater Bldr";
            break;
         case MAX_LEVEL - 9:
            class = "&cBuilder";
            break;
         case MAX_LEVEL - 10:
            class = "&cLesser Bldr";
            break;
         case MAX_LEVEL - 11:
            class = "&cNew Builder";
            break;
         case MAX_LEVEL - 12:
            class = "&cGeneral Staff";
            break;
         case MAX_LEVEL - 13:
            class = "&BImm Family";
            break;
         case MAX_LEVEL - 14:
            class = "&BMUD Guest";
            break;
         case MAX_LEVEL - 15:
            class = "&BAmbassador";
            break;
         case MAX_LEVEL - 16:
            class = "&YDemi-God";
            break;
      }

      if( !str_cmp( wch->name, sysdata.guild_overseer ) )
         extra_title = " &c[&WOverseer of Clubs&c]&w";
      else if( !str_cmp( wch->name, sysdata.guild_advisor ) )
         extra_title = " &c[&WAdvisor to Clubs&c]&w";
      else
         extra_title = "";

      if( IS_RETIRED( wch ) )
         class = "&BRetired";
      else if( IS_GUEST( wch ) )
         class = "&BGuest";
      else if( wch->pcdata->rank && wch->pcdata->rank[0] != '\0' )
         class = wch->pcdata->rank;

      if( IS_SET( wch->pcdata->flags, PCFLAG_ANONYMOUS ) && ( !ch || !IS_IMMORTAL( ch ) ) )
         class = "Anonymous";

      if( wch->pcdata->clan )
       {
         CLAN_DATA *pclan = wch->pcdata->clan;
         if( pclan->clan_type == CLAN_GUILD )
            strcpy( clan_name, " &B<&W" );
         else if( pclan->clan_type == CLAN_ORDER )
            strcpy( clan_name, " &p<&W" );
         else
            strcpy( clan_name, " &r(&W" );
      
         if( pclan->clan_type == CLAN_ORDER )
         {
            if( !str_cmp( wch->name, pclan->deity ) )
               strcat( clan_name, "Deity, Order of " );
            else if( !str_cmp( wch->name, pclan->leader ) )
               strcat( clan_name, "Leader, Order of " );
            else if( !str_cmp( wch->name, pclan->number1 ) )
               strcat( clan_name, "Number One, Order of " );
            else if( !str_cmp( wch->name, pclan->number2 ) )
               strcat( clan_name, "Number Two, Order of " );
            else
               strcat( clan_name, "Order of " );
         }
         else if( pclan->clan_type == CLAN_GUILD )
         {
            if( !str_cmp( wch->name, pclan->leader ) )
               strcat( clan_name, "Leader, " );
           if( !str_cmp( wch->name, pclan->number1 ) )
              strcat( clan_name, "First, " );
            if( !str_cmp( wch->name, pclan->number2 ) )
               strcat( clan_name, "Second, " );
         }
         else
         {
            if( !str_cmp( wch->name, pclan->deity ) )
               strcat( clan_name, "Deity of " );
            else if( !str_cmp( wch->name, pclan->leader ) )
               strcat( clan_name, "Leader of " );
            else if( !str_cmp( wch->name, pclan->number1 ) )
               strcat( clan_name, "Number One " );
           else if( !str_cmp( wch->name, pclan->number2 ) )
              strcat( clan_name, "Number Two " );
        }
         strcat( clan_name, pclan->name );
         if( pclan->clan_type == CLAN_GUILD )
            strcat( clan_name, "&B>&w" );
         else if( pclan->clan_type == CLAN_ORDER )
            strcat( clan_name, "&p>&w" );
        else
           strcat( clan_name, "&r)&w" );
      }
       else
         clan_name[0] = '\0';
      

      if( wch->pcdata->council )
      {
         strcpy( council_name, " &c[&W" );
         if( wch->pcdata->council->head2 == NULL || wch->pcdata->council->head2[0] == '\0' )
         {
            if( !str_cmp( wch->name, wch->pcdata->council->head ) )
               strcat( council_name, "Head of " );
         }
         else
         {
            if( !str_cmp( wch->name, wch->pcdata->council->head ) || !str_cmp( wch->name, wch->pcdata->council->head2 ) )
               strcat( council_name, "Co-Head of " );
         }
         strcat( council_name, wch->pcdata->council_name );
         strcat( council_name, "&c]&w" );
      }
      else
         council_name[0] = '\0';

      if( wch->pcdata->council2 )
      {
         strcpy( council2_name, " &c[&W" );
         if( wch->pcdata->council2->head2 == NULL || wch->pcdata->council2->head2[0] == '\0' )
         {
            if( !str_cmp( wch->name, wch->pcdata->council2->head ) )
               strcat( council2_name, "Head of " );
         }
         else
         {
            if( !str_cmp( wch->name, wch->pcdata->council2->head ) || !str_cmp( wch->name, wch->pcdata->council2->head2 ) )
               strcat( council2_name, "Co-Head of " );
         }
         strcat( council2_name, wch->pcdata->council2_name );
         strcat( council2_name, "&c]&w" );
      }
      else
         council2_name[0] = '\0';

      if( xIS_SET( wch->act, PLR_WIZINVIS ) )
         sprintf( invis_str, " &R(&W%d&R)&w", wch->pcdata->wizinvis );
      else
         invis_str[0] = '\0';

      if( ch && IS_IMMORTAL( ch ) )
      {
         bool multi = FALSE;
         int count = 0;
         DESCRIPTOR_DATA *dcheck;

         for( dcheck = first_descriptor; dcheck; dcheck = dcheck->next )
            if( dcheck->character && wch->desc && dcheck->host == wch->desc->host )
               count++;

         if( count > 1 )
            multi = TRUE;
	 int idletime=d->idle/4;
         char idletim[20]; 
	 sprintf(idletim,"%d",idletime);
         sprintf( buf, "&c[&W%s%s%s%s&c]&C[&W%s&C]&w %s %s%s%s.%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\r\n",
                  IS_SET( wch->pcdata->flags, PCFLAG_DEADLY ) ? "&WP" : "-",
                  ( wch->fighting ? "&gF&W" : "-" ),
                  ( multi ? "&RM&W" : "-" ),
                  ( wch->position == POS_INCAP ) ? "&bI&W" : "-",
                  const_color_align( class, 13, ALIGN_CENTER ),
                  ( wch->pcdata->extraname ? wch->pcdata->extraname : "" ),
                  char_name,
                  ( wch->pcdata->lastname ? wch->pcdata->lastname : "" ),
                  ( wch->pcdata->afkmsg ) ? wch->pcdata->afkmsg : wch->pcdata->title,
                  extra_title,
                  invis_str,
                  clan_name,
                  council_name,
                  council2_name,
                  ( wch->bounty > 0 ) ? " &W(&RBOUNTY&W)&w " : "",
                  xIS_SET( wch->act, PLR_MARRIED ) ? " &Y[&WWED&Y]&w " : "",
                  ( wch->desc && wch->desc->connected ) ? " &Y[&WWRITING&Y]&w " : "",
                  xIS_SET( wch->act, PLR_AFK ) ? " &Y[&WAWAY&Y:&W" : "",
                  xIS_SET( wch->act, PLR_AFK ) ? idletim : "",
                  xIS_SET( wch->act, PLR_AFK ) ? "&Y]&w " : "",
                  xIS_SET( wch->act, PLR_ATTACKER ) ? " &R(&W^zATTACKER^x&R)&w " : "",
                  xIS_SET( wch->act, PLR_COWARD ) ? " &R(&W^zCOWARD^x&R)&w " : "",
                  xIS_SET( wch->act, PLR_KILLER ) ? " &R(&W^zKILLER^x&R)&w " : "",
                  xIS_SET( wch->act, PLR_THIEF ) ? " &R(&W^zTHIEF^x&R)&w " : "" );
      }
      else
      {
         int idletime=d->idle/4;
         char idletim[20];
         sprintf(idletim,"%d",idletime);

         sprintf( buf, "&R[&W%s&R]&C[&W%s&C]&w %s %s%s%s.%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\r\n",
                  IS_SET( wch->pcdata->flags, PCFLAG_DEADLY ) ? "&WPK" : "&W--",
                  const_color_align( class, 14, ALIGN_CENTER ),
                  ( wch->pcdata->extraname ? wch->pcdata->extraname : "" ),
                  char_name,
                  ( wch->pcdata->lastname ? wch->pcdata->lastname : "" ),
                  ( wch->pcdata->afkmsg ? wch->pcdata->afkmsg : wch->pcdata->title),
                  extra_title,
                  invis_str,
                  clan_name,
                  council_name,
                  council2_name,
                  ( wch->bounty > 0 ) ? " &W(&RBOUNTY&W)&w " : "",
                  xIS_SET( wch->act, PLR_MARRIED ) ? " &Y[&WWED&Y]&w " : "",
                  ( wch->desc && wch->desc->connected ) ? " &Y[&WWRITING&Y]&w " : "",
                  xIS_SET( wch->act, PLR_AFK ) ? " &Y[&WAWAY&Y:&W" : "", 
                  xIS_SET( wch->act, PLR_AFK ) ? idletim : "",
                  xIS_SET( wch->act, PLR_AFK ) ? "&Y]&w " : "",
                  xIS_SET( wch->act, PLR_ATTACKER ) ? " &R(&W^zATTACKER^x&R)&w " : "",
                  xIS_SET( wch->act, PLR_COWARD ) ? " &R(&W^zCOWARD^x&R)&w " : "",
                  xIS_SET( wch->act, PLR_KILLER ) ? " &R(&W^zKILLER^x&R)&w " : "",
                  xIS_SET( wch->act, PLR_THIEF ) ? " &R(&W^zTHIEF^x&R)&w " : "" );
      }
      /*
       * This is where the old code would display the found player to the ch.
       * What we do instead is put the found data into a linked list
       */

      /*
       * First make the structure. 
       */
      CREATE( cur_who, WHO_DATA, 1 );
      cur_who->text = str_dup( buf );
      if( IS_IMMORTAL( wch ) )
      {
         if( get_trust( wch ) <= 103 || get_trust( wch ) == 110 )
            cur_who->type = WT_GUEST;
         /*
          * else if ( get_trust(wch) <= 111 )
          * cur_who->type = WT_STAFF;
          */
         else if( get_trust( wch ) <= MAX_LEVEL && get_trust( wch ) != 110 )
            cur_who->type = WT_ADMIN;
      }
      else if( fGroup )
      {
         if( wch->leader || ( whogr_p && whogr_p->indent ) )
            cur_who->type = WT_GROUPED;
         else
            cur_who->type = WT_GROUPWHO;
      }
      else if( CAN_PKILL( wch ) )
         cur_who->type = WT_DEADLY;
      else
         cur_who->type = WT_MORTAL;

      /*
       * Then put it into the appropriate list. 
       */
      switch ( cur_who->type )
      {
         case WT_MORTAL:
            cur_who->next = first_mortal;
            first_mortal = cur_who;
            break;
         case WT_DEADLY:
            cur_who->next = first_deadly;
            first_deadly = cur_who;
            break;
         case WT_GROUPED:
            cur_who->next = first_grouped;
            first_grouped = cur_who;
            break;
         case WT_GROUPWHO:
            cur_who->next = first_groupwho;
            first_groupwho = cur_who;
            break;
         case WT_GUEST:
            cur_who->next = first_guest;
            first_guest = cur_who;
            break;
            /*
             * case WT_STAFF:
             * cur_who->next = first_staff;
             * first_staff = cur_who;
             * break;
             */
         case WT_ADMIN:
            cur_who->next = first_admin;
            first_admin = cur_who;
            break;
      }
   }


   /*
    * Ok, now we have three separate linked lists and what remains is to
    * * display the information and clean up.
    */
   /*
    * Two extras now for grouped and groupwho (wanting group). -- Alty
    */

   if( ch )
   {
      /*
       * Center the mudname, which is set from cset 
       */
      {
         int amount; /* How much we want to put in first */
         int x;
         char obuf[300]; /* Stores inital string */
         char outbuf[400]; /* Stores final string */

         outbuf[0] = '\0';

         sprintf( obuf, "&YVisitors on %s", sysdata.mud_name );
         amount = 78 - strlen_color( obuf );  /* Determine amount to put in
                                              * front of line
                                              */
         if( amount < 1 )
            amount = 1;

         amount = amount / 2;

         for( x = 0; x < amount; x++ )
            strcat( outbuf, " " );

         strcat( outbuf, obuf );
         strcat( outbuf, "\r\n\r\n" );
         send_to_pager_color( outbuf, ch );
      }

      if( first_mortal || first_deadly || first_grouped || first_groupwho )
      {
         set_char_color( AT_ORANGE, ch );
         set_pager_color( AT_ORANGE, ch );
         send_to_pager_color
            ( "&O=================================[ &WMortals &O]==================================&w\r\n\r\n", ch );
         set_char_color( AT_GREEN, ch );
         set_pager_color( AT_GREEN, ch );
      }
   }

   for( cur_who = first_mortal; cur_who; cur_who = next_who )
   {
      if( !ch )
         fprintf( whoout, "%s", cur_who->text );
      else
         send_to_pager( cur_who->text, ch );
      next_who = cur_who->next;
      DISPOSE( cur_who->text );
      DISPOSE( cur_who );
   }
/*
 *
 * Just commented this whole thing to leave this intact if you need.
 *							--Callidyrr

    if ( first_deadly )
    {
      if ( !ch )
        fprintf( whoout, "\r\n&r----------------------------{-=-[&W Deadly Mortals &r]-=-}------------------------\r\n\r\n" );
      else {
	set_char_color(AT_BLOOD, ch);
	set_pager_color(AT_BLOOD,ch);
        send_to_pager_color( "\r\n&r--------------------------{-=-[ ",ch);
	set_char_color(AT_WHITE, ch);
	set_pager_color(AT_WHITE, ch);
	send_to_pager_color("&WDeadly Mortals ", ch);
	set_char_color(AT_BLOOD, ch);
	set_pager_color(AT_BLOOD, ch);
        send_to_pager_color("&r]-=-}--------------------------&w\r\n\r\n",ch );
    	set_char_color(AT_GREEN, ch);
	set_pager_color(AT_GREEN, ch);

	  }
    } 
 *
 *
 */

   for( cur_who = first_deadly; cur_who; cur_who = next_who )
   {
      if( !ch )
         fprintf( whoout, "%s", cur_who->text );
      else
         send_to_pager( cur_who->text, ch );
      next_who = cur_who->next;
      DISPOSE( cur_who->text );
      DISPOSE( cur_who );
   }

   if( first_grouped )
   {
/*      if ( !ch )
        fprintf( whoout, "\r\n-------------------------{-=-[ GROUPED CHARACTERS ]-=-}-----------------------\r\n\r\n" );
        else*/
      send_to_pager( "\r\n-------------------------{-=-[ GROUPED CHARACTERS ]-=-}-----------------------\r\n\r\n", ch );
   }
   for( cur_who = first_grouped; cur_who; cur_who = next_who )
   {
      /*
       * if ( !ch )
       * fprintf( whoout, cur_who->text );
       * else
       */
      send_to_pager( cur_who->text, ch );
      next_who = cur_who->next;
      DISPOSE( cur_who->text );
      DISPOSE( cur_who );
   }

   if( first_groupwho )
   {
      if( !ch )
         fprintf( whoout,
                  "\r\n&Y---------------------------{-=-[&W WANTING GROUP &Y]-=-}--------------------------\r\n\r\n" );
      else
      {
         set_char_color( AT_YELLOW, ch );
         set_pager_color( AT_YELLOW, ch );
         send_to_pager_color( "\r\n&Y---------------------------{-=-[", ch );
         set_char_color( ( AT_WHITE + AT_BLINK ), ch );
         set_pager_color( ( AT_WHITE + AT_BLINK ), ch );
         send_to_pager_color( " ^z&WWANTING GROUP ", ch );
         set_char_color( AT_YELLOW, ch );
         set_pager_color( AT_YELLOW, ch );
         send_to_pager_color( "^x&Y]-=-}--------------------------&w\r\n\r\n", ch );
         set_char_color( AT_GREEN, ch );
         set_pager_color( AT_GREEN, ch );
      }
   }

   for( cur_who = first_groupwho; cur_who; cur_who = next_who )
   {
      if( !ch )
         fprintf( whoout, "%s", cur_who->text );
      else
         send_to_pager( cur_who->text, ch );
      next_who = cur_who->next;
      DISPOSE( cur_who->text );
      DISPOSE( cur_who );
   }

   if( first_guest )
   {
      if( !ch )
         fprintf( whoout,
                  "\r\n&B=================================[&W  Guests &B]===================================\r\n\r\n" );
      else
      {
         set_char_color( AT_BLUE, ch );
         set_pager_color( AT_BLUE, ch );
         send_to_pager_color( "\r\n&B===================================[", ch );
         set_char_color( AT_WHITE, ch );
         set_pager_color( AT_WHITE, ch );
         send_to_pager_color( " &WGuests ", ch );
         set_char_color( AT_BLUE, ch );
         set_pager_color( AT_BLUE, ch );
         send_to_pager_color( "&B]==================================&w\r\n\r\n", ch );
         set_char_color( AT_GREEN, ch );
         set_pager_color( AT_GREEN, ch );
      }
   }

   for( cur_who = first_guest; cur_who; cur_who = next_who )
   {
      if( !ch )
         fprintf( whoout, "%s", cur_who->text );
      else
         send_to_pager( cur_who->text, ch );
      next_who = cur_who->next;
      DISPOSE( cur_who->text );
      DISPOSE( cur_who );
   }
   /*
    * if ( first_staff )
    * {
    * if ( !ch )
    * fprintf( whoout, "\r\n&c------------------------------[&W Resident Staff &c]------------------------------\r\n\r\n" );
    * else {
    * set_char_color(AT_CYAN,ch);
    * set_pager_color(AT_CYAN,ch);
    * send_to_pager_color( "\r\n&c------------------------------[",ch);
    * set_char_color(AT_WHITE,ch);
    * set_pager_color(AT_WHITE,ch);
    * send_to_pager_color(" &WResident Staff ",ch);
    * set_char_color(AT_CYAN,ch);
    * set_pager_color(AT_CYAN,ch);
    * send_to_pager_color("&c]------------------------------&w\r\n\r\n", ch );
    * set_char_color(AT_GREEN,ch);
    * set_pager_color(AT_GREEN,ch);
    * }}
    * for ( cur_who = first_staff; cur_who; cur_who = next_who )
    * {
    * if ( !ch )
    * fprintf( whoout, cur_who->text );
    * else
    * send_to_pager( cur_who->text, ch );
    * next_who = cur_who->next;
    * DISPOSE( cur_who->text );
    * DISPOSE( cur_who );
    * }
    */
   if( first_admin )
   {
      if( !ch )
         fprintf( whoout,
                  "\r\n&R================================[ &WImmortals &R]=================================\r\n\r\n" );
      else
      {
         set_char_color( AT_RED, ch );
         set_pager_color( AT_RED, ch );
         send_to_pager_color( "\r\n&R================================[", ch );
         set_char_color( AT_WHITE, ch );
         set_pager_color( AT_WHITE, ch );
         send_to_pager_color( " &WImmortals ", ch );
         set_char_color( AT_RED, ch );
         set_pager_color( AT_RED, ch );
         send_to_pager_color( "&R]=================================&w\r\n\r\n", ch );
         set_char_color( AT_GREEN, ch );
         set_pager_color( AT_GREEN, ch );
      }
   }
   for( cur_who = first_admin; cur_who; cur_who = next_who )
   {
      if( !ch )
         fprintf( whoout, "%s", cur_who->text );
      else
         send_to_pager( cur_who->text, ch );
      next_who = cur_who->next;
      DISPOSE( cur_who->text );
      DISPOSE( cur_who );
   }

   if( !ch )
   {
      fprintf( whoout, "\r\n&Y[&W%d player%s&Y]&w\r\n", nMatch, nMatch == 1 ? "" : "s" );
      fclose( whoout );
      return;
   }

   send_to_pager( "&x\r\n", ch );
   set_char_color( AT_CYAN, ch );
/*    ch_printf( ch, "&Y[&W%d player%s&Y]&w\r\n", nMatch, nMatch == 1 ? "" : "s" );*/
/*    ch_printf( ch, "&Y[&W%d Player%s&Y] [&WOur Homepage: %s&Y] [&WWizlock is %s&Y]&w\r\n",  nMatch,
	            nMatch == 1 ? "" : "s", show_tilde(sysdata.homepage), wizlock == TRUE ? "&RON" : "&GOFF"); */
/*    ch_printf( ch, "&Y[&W%d Player%s&Y] [&WOur Homepage: %s&Y] [&WDouble Exp is %s&Y]&w\r\n",  nMatch,
	            nMatch == 1 ? "" : "s", show_tilde(sysdata.homepage), (sysdata.double_exp) == TRUE ? "&GON" : "&ROFF"); */
/*    ch_printf( ch, "&Y[&W%d Player%s&Y] [&WOur Homepage: %s&Y] [&W%3d Max Since Reboot&Y]&w\r\n", 
nMatch,
               nMatch == 1 ? "" : "s",
               show_tilde(sysdata.homepage),
               sysdata.maxplayers ); */
   ch_printf( ch, "\r\nYou see [" );
   ch_printf( ch, "%d", nMatch );
   ch_printf( ch, "] player%s online.\r\n", nMatch == 1 ? "" : "s" );
   ch_printf( ch, "There have been a max of [" );
   ch_printf( ch, "%d", sysdata.maxplayers );
   ch_printf( ch, "] players on at one time since Reboot.\r\n" );
   ch_printf( ch, "There have been a total of [" );
   ch_printf( ch, "%d", sysdata.totalplayers );
   ch_printf( ch, "] logins since reboot.\r\n" );
   ch_printf( ch, "Mud has been up since " );
   ch_printf( ch, "%s\r\n", str_boot_time );
   set_char_color( AT_YELLOW, ch );
   ch_printf( ch, "Our Homepage:" );
   ch_printf( ch, "%s\r\n", show_tilde( sysdata.homepage ) );

   if( reboot_in > 0 )
   {
      if( reboot_in <= 60 )
         ch_printf_color( ch, "&RReboot in: %d second%s\r\n", reboot_in, reboot_in == 1 ? "" : "s" );
      else
         ch_printf_color( ch, "&RReboot in: %d minute%s, %d second%s\r\n", reboot_in / 60,
                          ( reboot_in / 60 == 1 ) ? "" : "s", reboot_in % 60, ( reboot_in % 60 == 1 ) ? "" : "s" );
   }
   return;
}


/* This command SUCKS! It's misleading enough to be dangerous to morts. -- Scion

void do_compare( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    int value1;
    int value2;
    char *msg;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Compare what to what?\r\n", ch );
	return;
    }

    if ( ( obj1 = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that item.\r\n", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	for ( obj2 = ch->first_carrying; obj2; obj2 = obj2->next_content )
	{
	    if ( obj2->wear_loc != WEAR_NONE
	    &&   can_see_obj( ch, obj2 )
	    &&   obj1->item_type == obj2->item_type
	    && ( obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
		break;
	}

	if ( !obj2 )
	{
	    send_to_char( "You aren't wearing anything comparable.\r\n", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj2 = get_obj_carry( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\r\n", ch );
	    return;
	}
    }

    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
    {
	msg = "You compare $p to itself.  It looks about the same.";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
	msg = "You can't compare $p and $P.";
    }
    else
    {
	switch ( obj1->item_type )
	{
	default:
	    msg = "You can't compare $p and $P.";
	    break;

	case ITEM_ARMOR:
	    value1 = obj1->value[0];
	    value2 = obj2->value[0];
	    break;

	case ITEM_WEAPON:
	    value1 = obj1->value[1] + obj1->value[2];
	    value2 = obj2->value[1] + obj2->value[2];
	    break;
	}
    }

    if ( !msg )
    {
	     if ( value1 == value2 ) msg = "$p and $P look about the same.";
	else if ( value1  > value2 ) msg = "$p looks better than $P.";
	else                         msg = "$p looks worse than $P.";
    }

    act( AT_PLAIN, msg, ch, obj1, obj2, TO_CHAR );
    return;
}
*/


void do_where( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   DESCRIPTOR_DATA *d;
   bool found;

   one_argument( argument, arg );

   set_pager_color( AT_PERSON, ch );
   if( arg[0] == '\0' )
   {
      pager_printf( ch, "\r\nPlayers near you in %s:\r\n", ch->in_room->area->name );
      found = FALSE;
      for( d = first_descriptor; d; d = d->next )
         if( ( d->connected == CON_PLAYING || d->connected == CON_EDITING
               || ( d->connected >= CON_NOTE_TO
                    && d->connected <= CON_NOTE_FINISH ) )
             && ( victim = d->character ) != NULL
             && !IS_NPC( victim ) && victim->in_room && victim->in_room->area == ch->in_room->area && can_see( ch, victim ) )
         {
            found = TRUE;
            if( CAN_PKILL( victim ) )
               set_pager_color( AT_PURPLE, ch );
            else
               set_pager_color( AT_PERSON, ch );

            pager_printf_color( ch, "&P%-13s  ", victim->name );
            if( CAN_PKILL( victim ) && victim->pcdata->clan )
               pager_printf_color( ch, "%-18s\t", victim->pcdata->clan->badge );
            else if( CAN_PKILL( victim ) )
               send_to_pager_color( "(&wUnclanned&P)\t", ch );
            else
               send_to_pager( "\t\t\t", ch );
            pager_printf_color( ch, "&P%s\r\n", victim->in_room->name );
         }
      if( !found )
         send_to_char( "None\r\n", ch );
   }
   else
   {
      found = FALSE;
      for( victim = first_char; victim; victim = victim->next )
         if( victim->in_room
             && victim->in_room->area == ch->in_room->area
             && !IS_AFFECTED( victim, AFF_HIDE )
             && !IS_AFFECTED( victim, AFF_SNEAK ) && can_see( ch, victim ) && is_name( arg, victim->name ) )
         {
            found = TRUE;
            pager_printf( ch, "%-28s %s\r\n", PERS( victim, ch ), victim->in_room->name );
            break;
         }
      if( !found )
         act( AT_PLAIN, "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
   }

   return;
}




void do_consider( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   char *msg;
   int diff;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Consider killing whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They're not here.\r\n", ch );
      return;
   }

   diff = victim->level - ch->level;

   if( diff <= -10 )
      msg = "You are far more experienced than $N.";
   else if( diff <= -5 )
      msg = "$N is not nearly as experienced as you.";
   else if( diff <= -2 )
      msg = "You are more experienced than $N.";
   else if( diff <= 1 )
      msg = "You are just about as experienced as $N.";
   else if( diff <= 4 )
      msg = "You are not nearly as experienced as $N.";
   else if( diff <= 9 )
      msg = "$N is far more experienced than you!";
   else
      msg = "$N would make a great teacher for you!";
   act( AT_CONSIDER, msg, ch, NULL, victim, TO_CHAR );

   diff = ( int )( victim->max_hit - ch->max_hit ) / 6;

   if( diff <= -200 )
      msg = "$N looks like a feather!";
   else if( diff <= -150 )
      msg = "You could kill $N with your hands tied!";
   else if( diff <= -100 )
      msg = "Hey! Where'd $N go?";
   else if( diff <= -50 )
      msg = "$N is a wimp.";
   else if( diff <= 0 )
      msg = "$N looks weaker than you.";
   else if( diff <= 50 )
      msg = "$N looks about as strong as you.";
   else if( diff <= 100 )
      msg = "It would take a bit of luck...";
   else if( diff <= 150 )
      msg = "It would take a lot of luck, and equipment!";
   else if( diff <= 200 )
      msg = "Why don't you dig a grave for yourself first?";
   else
      msg = "$N is built like a TANK!";
   act( AT_CONSIDER, msg, ch, NULL, victim, TO_CHAR );

   act( AT_CONSIDER, "$n looks at you very closely...", ch, NULL, victim, TO_VICT );
   act( AT_CONSIDER, "$n looks at $N very closely.", ch, NULL, victim, TO_NOTVICT );

   return;
}



/*
 * Place any skill types you don't want them to be able to practice
 * normally in this list.  Separate each with a space.
 * (Uses an is_name check). -- Altrag
 */
#define CANT_PRAC "Tongue"

void do_practice( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   int sn;

   if( IS_NPC( ch ) )
      return;

/*
    if ( ch->level < 2 )
    {
	send_to_char(
	"You must be second level to practice.  Seek out monsters to kill!\r\n",
	    ch );
	return;
    }
*/
   if( argument[0] == '\0' )
   {
      int col;
      short lasttype, cnt;

      col = cnt = 0;
      lasttype = SKILL_SPELL;
      set_pager_color( AT_MAGIC, ch );
      for( sn = 0; sn < top_sn; sn++ )
      {
         if( !skill_table[sn]->name )
            break;

         if( strcmp( skill_table[sn]->name, "reserved" ) == 0 && ( IS_IMMORTAL( ch ) || CAN_CAST( ch ) ) )
         {
            if( col % 3 != 0 )
               send_to_pager( "\r\n", ch );
            set_pager_color( AT_MAGIC, ch );
            send_to_pager_color
               ( " &c---------------------------------[&W Practice &c]---------------------------------&w\r\n", ch );
            col = 0;
         }
         if( skill_table[sn]->type != lasttype )
         {
            if( !cnt )
               send_to_pager( "                                   (none)\r\n", ch );
            else if( col % 3 != 0 )
               send_to_pager( "\r\n", ch );
            set_pager_color( AT_MAGIC, ch );
            pager_printf_color( ch, " &c---------------------------------- &W%ss&c ----------------------------------&w\r\n",
                                skill_tname[skill_table[sn]->type] );
            col = cnt = 0;
         }
         lasttype = skill_table[sn]->type;

         if( !IS_IMMORTAL( ch )
             && ( skill_table[sn]->guild != CLASS_NONE
                  && ( !IS_GUILDED( ch ) || ( ch->pcdata->clan->class != skill_table[sn]->guild ) ) ) )
            continue;

         if( ch->level2 == -1 )
         {
            if( ch->level < skill_table[sn]->skill_level[ch->class]
                || ( !IS_IMMORTAL( ch ) && skill_table[sn]->skill_level[ch->class] == 0 ) )
               continue;
         }
         else if( ch->level3 == -1 )
         {
            if( ( ch->level < skill_table[sn]->skill_level[ch->class]
                  && ch->level < skill_table[sn]->skill_level[ch->class2] )
                || ( !IS_IMMORTAL( ch ) && ( skill_table[sn]->skill_level[ch->class] == 0
                                             && skill_table[sn]->skill_level[ch->class2] == 0 ) ) )
               continue;
         }
         else
         {
            if( ( ch->level < skill_table[sn]->skill_level[ch->class]
                  && ch->level < skill_table[sn]->skill_level[ch->class2]
                  && ch->level < skill_table[sn]->skill_level[ch->class3] )
                || ( !IS_IMMORTAL( ch ) && ( skill_table[sn]->skill_level[ch->class] == 0
                                             && skill_table[sn]->skill_level[ch->class2] == 0
                                             && skill_table[sn]->skill_level[ch->class3] == 0 ) ) )
               continue;
         }

         if( ch->pcdata->learned[sn] <= 0 && SPELL_FLAG( skill_table[sn], SF_SECRETSKILL ) )
            continue;

         ++cnt;
         set_pager_color( AT_MAGIC, ch );
         if( ch->pcdata->learned[sn] == 0 )
            pager_printf( ch, "&W%20.20s", skill_table[sn]->name );
         else if( ch->pcdata->learned[sn] > 0
                  && ( ch->pcdata->learned[sn] < skill_table[sn]->skill_adept[ch->class]
                       || ch->pcdata->learned[sn] < skill_table[sn]->skill_adept[ch->class2]
                       || ch->pcdata->learned[sn] < skill_table[sn]->skill_adept[ch->class3] ) )
            pager_printf( ch, "&Y%20.20s", skill_table[sn]->name );
         else if( ch->pcdata->learned[sn] > 0 )
            pager_printf( ch, "&R%20.20s", skill_table[sn]->name );
         if( ch->pcdata->learned[sn] > 0 )
            set_pager_color( AT_SCORE, ch );
         pager_printf( ch, " &W%3d%%&w ", ch->pcdata->learned[sn] );
         if( ++col % 3 == 0 )
            send_to_pager( "\r\n", ch );
      }

      if( col % 3 != 0 )
         send_to_pager( "\r\n", ch );
      set_pager_color( AT_MAGIC, ch );
      pager_printf( ch, "&cYou have %d practice sessions left.\r\n", ch->practice );
   }
   else
   {
      CHAR_DATA *mob;
      int adept;
      bool can_prac = TRUE;

      if( !IS_AWAKE( ch ) )
      {
         send_to_char( "In your dreams, or what?\r\n", ch );
         return;
      }

      for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
         if( IS_NPC( mob ) && xIS_SET( mob->act, ACT_PRACTICE ) )
            break;

      if( !mob )
      {
         send_to_char( "You can't do that here.\r\n", ch );
         return;
      }

      if( ch->practice <= 0 )
      {
         act( AT_TELL, "$n tells you 'You must earn some more practice sessions.'", mob, NULL, ch, TO_VICT );
         return;
      }

      sn = skill_lookup( argument );

      if( ch->level2 == -1 )
      {
         if( can_prac && ( ( sn == -1 ) || ( !IS_NPC( ch ) && ch->level < skill_table[sn]->skill_level[ch->class]
                                             /*
                                              * OUT FOR THIS PORT -SHADDAI
                                              * &&  ch->level < skill_table[sn]->race_level[ch->race]
                                              */
                            ) ) )
         {
            act( AT_TELL, "$n tells you 'You're not ready to learn that yet...'", mob, NULL, ch, TO_VICT );
            return;
         }
      }
      else if( ch->level3 == -1 )
      {
         if( can_prac
             && ( ( sn == -1 )
                  || ( !IS_NPC( ch )
                       && ( ch->level < skill_table[sn]->skill_level[ch->class]
                            && ch->level < skill_table[sn]->skill_level[ch->class2] ) ) ) )
         {
            act( AT_TELL, "$n tells you 'You're not ready to learn that yet...'", mob, NULL, ch, TO_VICT );
            return;
         }
      }
      else
      {
         if( can_prac
             && ( ( sn == -1 )
                  || ( !IS_NPC( ch )
                       && ( ch->level < skill_table[sn]->skill_level[ch->class]
                            && ch->level < skill_table[sn]->skill_level[ch->class2]
                            && ch->level < skill_table[sn]->skill_level[ch->class3] )
                       /*
                        * OUT FOR THIS PORT -SHADDAI
                        * &&  ch->level < skill_table[sn]->race_level[ch->race]
                        */
                   ) ) )
         {
            act( AT_TELL, "$n tells you 'You're not ready to learn that yet...'", mob, NULL, ch, TO_VICT );
            return;
         }
      }

      if( is_name( skill_tname[skill_table[sn]->type], CANT_PRAC ) )
      {
         act( AT_TELL, "$n tells you 'I do not know how to teach that.'", mob, NULL, ch, TO_VICT );
         return;
      }

      /*
       * Skill requires a special teacher
       */
      if( skill_table[sn]->teachers && skill_table[sn]->teachers[0] != '\0' )
      {
         sprintf( buf, "%d", mob->pIndexData->vnum );
         if( !is_name( buf, skill_table[sn]->teachers ) )
         {
            act( AT_TELL, "$n tells you, 'I know not how to teach that.'", mob, NULL, ch, TO_VICT );
            return;
         }
      }

/*
 * Guild checks - right now, cant practice guild skills - done on 
 * induct/outcast
 */
/*	
	if ( !IS_NPC(ch) && !IS_GUILDED(ch)
	&&    skill_table[sn]->guild != CLASS_NONE)
	{
	    act( AT_TELL, "$n tells you 'Only guild members can use that..'"
		mob, NULL, ch, TO_VICT );
	    return;
	}

	if ( !IS_NPC(ch) && skill_table[sn]->guild != CLASS_NONE 
	     && ch->pcdata->clan->class != skill_table[sn]->guild )
	{
	    act( AT_TELL, "$n tells you 'That can not be used by your guild.'"
		mob, NULL, ch, TO_VICT );
	    return;
	}
*/
      if( !IS_NPC( ch ) && skill_table[sn]->guild != CLASS_NONE )
      {
         act( AT_TELL, "$n tells you 'That is only for members of guilds...'", mob, NULL, ch, TO_VICT );
         return;
      }

      /*
       * Disabled for now
       if ( mob->level < skill_table[sn]->skill_level[ch->class]
       ||   mob->level < skill_table[sn]->skill_level[mob->class] )
       {
       act( AT_TELL, "$n tells you 'You must seek another to teach you that...'",
       mob, NULL, ch, TO_VICT );
       return;
       }
       */

      adept = class_table[ch->class]->skill_adept * 0.2;

      if( ch->pcdata->learned[sn] >= adept )
      {
         sprintf( buf, "$n tells you, 'I've taught you everything I can about %s.'", skill_table[sn]->name );
         act( AT_TELL, buf, mob, NULL, ch, TO_VICT );
         act( AT_TELL, "$n tells you, 'You'll have to practice it on your own now...'", mob, NULL, ch, TO_VICT );
      }
      else
      {
         ch->practice--;
         ch->pcdata->learned[sn] += int_app[get_curr_int( ch )].learn;
         act( AT_ACTION, "You practice $T.", ch, NULL, skill_table[sn]->name, TO_CHAR );
         act( AT_ACTION, "$n practices $T.", ch, NULL, skill_table[sn]->name, TO_ROOM );
         if( ch->pcdata->learned[sn] >= adept )
         {
            ch->pcdata->learned[sn] = adept;
            act( AT_TELL, "$n tells you. 'You'll have to practice it on your own now...'", mob, NULL, ch, TO_VICT );
         }
      }
   }
   return;
}


void do_wimpy( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int wimpy;

   set_char_color( AT_YELLOW, ch );
   one_argument( argument, arg );
   if( !str_cmp( arg, "max" ) )
   {
      if( IS_PKILL( ch ) )
         wimpy = ( int )ch->max_hit / 2.25;
      else
         wimpy = ( int )ch->max_hit / 1.2;
   }
   else if( arg[0] == '\0' )
      wimpy = ( int )ch->max_hit / 5;
   else
      wimpy = atoi( arg );

   if( wimpy < 0 )
   {
      send_to_char( "Your courage exceeds your wisdom.\r\n", ch );
      return;
   }
   if( IS_PKILL( ch ) && wimpy > ( int )ch->max_hit / 2.25 )
   {
      send_to_char( "Such cowardice ill becomes you.\r\n", ch );
      return;
   }
   else if( wimpy > ( int )ch->max_hit / 1.2 )
   {
      send_to_char( "Such cowardice ill becomes you.\r\n", ch );
      return;
   }
   ch->wimpy = wimpy;
   ch_printf( ch, "Wimpy set to %d hit points.\r\n", wimpy );
   return;
}



void do_password( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char *pArg;
   char *pwdnew;
   char *p;
   char cEnd;

   if( IS_NPC( ch ) )
      return;

   /*
    * Can't use one_argument here because it smashes case.
    * So we just steal all its code.  Bleagh.
    */
   pArg = arg1;
   while( isspace( *argument ) )
      argument++;

   cEnd = ' ';
   if( *argument == '\'' || *argument == '"' )
      cEnd = *argument++;

   while( *argument != '\0' )
   {
      if( *argument == cEnd )
      {
         argument++;
         break;
      }
      *pArg++ = *argument++;
   }
   *pArg = '\0';

   pArg = arg2;
   while( isspace( *argument ) )
      argument++;

   cEnd = ' ';
   if( *argument == '\'' || *argument == '"' )
      cEnd = *argument++;

   while( *argument != '\0' )
   {
      if( *argument == cEnd )
      {
         argument++;
         break;
      }
      *pArg++ = *argument++;
   }
   *pArg = '\0';

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: password <new> <again>.\r\n", ch );
      return;
   }

/*
    if ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\r\n", ch );
	return;
    }
*/

/* This should stop all the mistyped password problems --Shaddai */
   if( strcmp( arg1, arg2 ) )
   {
      send_to_char( "Passwords don't match try again.\r\n", ch );
      return;
   }
   if( strlen( arg2 ) < 5 )
   {
      send_to_char( "New password must be at least five characters long.\r\n", ch );
      return;
   }

   /*
    * No tilde allowed because of player file format.
    */
   pwdnew = crypt( arg2, ch->name );
   for( p = pwdnew; *p != '\0'; p++ )
   {
      if( *p == '~' )
      {
         send_to_char( "New password not acceptable, try again.\r\n", ch );
         return;
      }
   }

   DISPOSE( ch->pcdata->pwd );
   ch->pcdata->pwd = str_dup( pwdnew );
   if( IS_SET( sysdata.save_flags, SV_PASSCHG ) )
      save_char_obj( ch );
   send_to_char( "Ok.\r\n", ch );
   return;
}



void do_socials( CHAR_DATA * ch, char *argument )
{
   int iHash;
   int col = 0;
   SOCIALTYPE *social;

   set_pager_color( AT_PLAIN, ch );
   for( iHash = 0; iHash < 27; iHash++ )
      for( social = social_index[iHash]; social; social = social->next )
      {
         pager_printf( ch, "%-12s", social->name );
         if( ++col % 6 == 0 )
            send_to_pager( "\r\n", ch );
      }

   if( col % 6 != 0 )
      send_to_pager( "\r\n", ch );
   return;
}


void do_commands( CHAR_DATA * ch, char *argument )
{
   int col;
   bool found;
   int hash;
   CMDTYPE *command;

   col = 0;
   set_pager_color( AT_PLAIN, ch );
   if( argument[0] == '\0' )
   {
      for( hash = 0; hash < 126; hash++ )
         for( command = command_hash[hash]; command; command = command->next )
            if( command->level < LEVEL_HERO && command->level <= get_trust( ch ) && ( command->cshow == 1 ) )
            {
               pager_printf( ch, "%-12s", command->name );
               if( ++col % 6 == 0 )
                  send_to_pager( "\r\n", ch );
            }
      if( col % 6 != 0 )
         send_to_pager( "\r\n", ch );
   }
   else
   {
      found = FALSE;
      for( hash = 0; hash < 126; hash++ )
         for( command = command_hash[hash]; command; command = command->next )
            if( command->level < LEVEL_HERO
                && command->level <= get_trust( ch ) && !str_prefix( argument, command->name ) && ( command->cshow == 1 ) )
            {
               pager_printf( ch, "%-12s", command->name );
               found = TRUE;
               if( ++col % 6 == 0 )
                  send_to_pager( "\r\n", ch );
            }

      if( col % 6 != 0 )
         send_to_pager( "\r\n", ch );
      if( !found )
         ch_printf( ch, "No command found under %s.\r\n", argument );
   }
   return;
}


/*
 * display WIZLIST file						-Thoric
 */
void do_wizlist( CHAR_DATA * ch, char *argument )
{
   set_pager_color( AT_IMMORT, ch );
   show_file( ch, WIZLIST_FILE );
}

/*
 * Display guestlist -- Kratas
 */
void do_guestlist( CHAR_DATA * ch, char *argument )
{
   set_pager_color( AT_IMMORT, ch );
   show_file( ch, GUESTLIST_FILE );
}


/*
 * Contributed by Grodyn.
 * Display completely overhauled, 2/97 -- Blodkai
 */
void do_config( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

   if( IS_NPC( ch ) )
      return;

   one_argument( argument, arg );

   set_char_color( AT_GREEN, ch );

   if( arg[0] == '\0' )
   {
      set_char_color( AT_DGREEN, ch );
      send_to_char( "\r\nConfigurations ", ch );
      set_char_color( AT_GREEN, ch );
      send_to_char( "(use 'config +/- <keyword>' to toggle, see 'help config')\r\n\r\n", ch );
      set_char_color( AT_DGREEN, ch );
      send_to_char( "Display:   ", ch );
      set_char_color( AT_GREY, ch );
      ch_printf( ch,
                 "%-12s   %-12s   %-12s   %-12s\r\n           %-12s   %-12s   %-12s   %-12s\r\n           %-12s   %-12s   %-12s   %-12s",
                 IS_SET( ch->pcdata->flags, PCFLAG_PAGERON ) ? "[+] PAGER" : "[-] pager", IS_SET( ch->pcdata->flags,
                                                                                                  PCFLAG_GAG ) ? "[+] GAG" :
                 "[-] gag", xIS_SET( ch->act, PLR_BRIEF ) ? "[+] BRIEF" : "[-] brief", xIS_SET( ch->act,
                                                                                                PLR_COMBINE ) ? "[+] COMBINE"
                 : "[-] combine", xIS_SET( ch->act, PLR_BLANK ) ? "[+] BLANK" : "[-] blank", xIS_SET( ch->act,
                                                                                                      PLR_PROMPT ) ?
                 "[+] PROMPT" : "[-] prompt", xIS_SET( ch->act, PLR_ANSI ) ? "[+] ANSI" : "[-] ansi", xIS_SET( ch->act,
                                                                                                               PLR_RIP ) ?
                 "[+] RIP" : "[-] rip", xIS_SET( ch->act, PLR_NOMENU ) ? "[+] NOMENU" : "[-] nomenu",
                 IS_SET( ch->pcdata->flags, PCFLAG_NOBEEP ) ? "[+] NOBEEP" : "[-] nobeep", IS_SET( ch->pcdata->flags,
                                                                                                   PCFLAG_ANONYMOUS ) ?
                 "[+] ANONYMOUS" : "[-] anonymous", xIS_SET( ch->act, PLR_AUTOMAP ) ? "[+] SHOWMAP" : "[-] showmap" );
      set_char_color( AT_DGREEN, ch );
      send_to_char( "\r\n\r\nAuto:      ", ch );
      set_char_color( AT_GREY, ch );
      ch_printf( ch, "%-12s   %-12s   %-12s   %-12s",
                 xIS_SET( ch->act, PLR_AUTOSAC ) ? "[+] AUTOSAC"
                 : "[-] autosac",
                 xIS_SET( ch->act, PLR_AUTOGOLD ) ? "[+] AUTOGOLD"
                 : "[-] autogold",
                 xIS_SET( ch->act, PLR_AUTOLOOT ) ? "[+] AUTOLOOT"
                 : "[-] autoloot", xIS_SET( ch->act, PLR_AUTOEXIT ) ? "[+] AUTOEXIT" : "[-] autoexit" );

      set_char_color( AT_DGREEN, ch );
      send_to_char( "\r\n\r\nSafeties:  ", ch );
      set_char_color( AT_GREY, ch );
      ch_printf( ch, "%-12s   %-12s   %-12s   %-12s\r\n        ",
                 IS_SET( ch->pcdata->flags, PCFLAG_NORECALL ) ? "[+] NORECALL"
                 : "[-] norecall",
                 IS_SET( ch->pcdata->flags, PCFLAG_NOSTUN ) ? "[+] NOSTUN"
                 : "[-] nostun",
                 IS_SET( ch->pcdata->flags, PCFLAG_NOSUMMON ) ? "[+] NOSUMMON"
                 : "[-] nosummon", IS_SET( ch->pcdata->flags, PCFLAG_IMMPROOF ) ? "[+] IMMPROOF" : "[-] immproof" );
      ch_printf( ch, "   %-12s   %-12s",
                 xIS_SET( ch->act, PLR_SHOVEDRAG ) ? "[+] DRAG"
                 : "[-] drag", xIS_SET( ch->act, PLR_NICE ) ? "[+] NICE" : "[-] nice" );

      set_char_color( AT_DGREEN, ch );
      send_to_char( "\r\n\r\nMisc:      ", ch );
      set_char_color( AT_GREY, ch );
      ch_printf( ch, "%-12s   %-12s   %-12s",
                 xIS_SET( ch->act, PLR_TELNET_GA ) ? "[+] TELNETGA"
                 : "[-] telnetga",
                 IS_SET( ch->pcdata->flags, PCFLAG_GROUPWHO ) ? "[+] GROUPWHO"
                 : "[-] groupwho", IS_SET( ch->pcdata->flags, PCFLAG_NOINTRO ) ? "[+] NOINTRO" : "[-] nointro" );

      set_char_color( AT_DGREEN, ch );
      send_to_char( "\r\n\r\nSettings:  ", ch );
      set_char_color( AT_GREY, ch );
      ch_printf_color( ch, "Pager Length (%d)    Wimpy (&W%d&w)", ch->pcdata->pagerlen, ch->wimpy );

      if( IS_IMMORTAL( ch ) )
      {
         set_char_color( AT_DGREEN, ch );
         send_to_char( "\r\n\r\nImmortal toggles:  ", ch );
         set_char_color( AT_GREY, ch );
         /*
          * ch_printf( ch, "Roomvnum [%s]    Automap [%s]",
          * xIS_SET(ch->act, PLR_ROOMVNUM )      ? "+"
          * : " ",
          * xIS_SET(ch->act, PLR_AUTOMAP  )      ? "+"
          * : " " ); 
          */
         ch_printf( ch, "%-12s   %-12s   %-12s\r\n        ",
                    xIS_SET( ch->act, PLR_ROOMVNUM ) ? "[+] VNUM" : "[-] vnum", "", "" );
/*	xIS_SET( ch->act, PLR_AUTOMAP )			? "[+] MAP"
							: "[-] map",
							"" ); added to mortal -Garinan */
         /*
          * IS_SET( ch->pcdata->flags, PCFLAG_IMMPROOF )  ? "[+] IMMPROOF"
          * : "[-] immproof" );
          */
      }

      set_char_color( AT_DGREEN, ch );
      send_to_char( "\r\n\r\nSentences imposed on you (if any):", ch );
      set_char_color( AT_YELLOW, ch );
      ch_printf( ch, "\r\n%s%s%s%s%s%s",
                 xIS_SET( ch->act, PLR_SILENCE ) ?
                 " For your abuse of channels, you are currently silenced.\r\n" : "",
                 xIS_SET( ch->act, PLR_NO_EMOTE ) ?
                 " The gods have removed your emotes.\r\n" : "",
                 xIS_SET( ch->act, PLR_NO_TELL ) ?
                 " You are not permitted to send 'tells' to others.\r\n" : "",
                 xIS_SET( ch->act, PLR_LITTERBUG ) ?
                 " A convicted litterbug.  You cannot drop anything.\r\n" : "",
                 xIS_SET( ch->act, PLR_THIEF ) ?
                 " A proven thief, you will be hunted by the authorities.\r\n" : "",
                 xIS_SET( ch->act, PLR_KILLER ) ? " For the crime of murder you are sentenced to death...\r\n" : "" );
   }
   else
   {
      bool fSet;
      int bit = 0;

      if( arg[0] == '+' )
         fSet = TRUE;
      else if( arg[0] == '-' )
         fSet = FALSE;
      else
      {
         send_to_char( "Config -option or +option?\r\n", ch );
         return;
      }

      if( !str_prefix( arg + 1, "autoexit" ) )
         bit = PLR_AUTOEXIT;
      else if( !str_prefix( arg + 1, "autoloot" ) )
         bit = PLR_AUTOLOOT;
      else if( !str_prefix( arg + 1, "autosac" ) )
         bit = PLR_AUTOSAC;
      else if( !str_prefix( arg + 1, "autogold" ) )
         bit = PLR_AUTOGOLD;
      else if( !str_prefix( arg + 1, "blank" ) )
         bit = PLR_BLANK;
      else if( !str_prefix( arg + 1, "brief" ) )
         bit = PLR_BRIEF;
      else if( !str_prefix( arg + 1, "combine" ) )
         bit = PLR_COMBINE;
      else if( !str_prefix( arg + 1, "prompt" ) )
         bit = PLR_PROMPT;
      else if( !str_prefix( arg + 1, "telnetga" ) )
         bit = PLR_TELNET_GA;
      else if( !str_prefix( arg + 1, "ansi" ) )
         bit = PLR_ANSI;
      else if( !str_prefix( arg + 1, "rip" ) )
         bit = PLR_RIP;
/*	else if ( !str_prefix( arg+1, "flee"     ) ) bit = PLR_FLEE; */
      else if( !str_prefix( arg + 1, "nice" ) )
         bit = PLR_NICE;
      else if( !str_prefix( arg + 1, "drag" ) )
         bit = PLR_SHOVEDRAG;
      else if( !str_prefix( arg + 1, "nomenu" ) )
         bit = PLR_NOMENU; /* Scion */
      else if( !str_prefix( arg + 1, "showmap" ) )
         bit = PLR_AUTOMAP;   /* maps (added to mortals -Garinan) */
      else if( IS_IMMORTAL( ch ) && !str_prefix( arg + 1, "vnum" ) )
         bit = PLR_ROOMVNUM;

      if( bit )
      {
         if( ( bit == PLR_FLEE || bit == PLR_NICE || bit == PLR_SHOVEDRAG ) && !IS_IMMORTAL( ch ) )
         {
            send_to_char( "Config which option?\r\n", ch );
            return;
         }

         if( fSet )
            xSET_BIT( ch->act, bit );
         else
            xREMOVE_BIT( ch->act, bit );
         send_to_char( "Ok.\r\n", ch );
         return;
      }
      else
      {
         if( !str_prefix( arg + 1, "norecall" ) )
            bit = PCFLAG_NORECALL;
         else if( !str_prefix( arg + 1, "nointro" ) )
            bit = PCFLAG_NOINTRO;
         else if( !str_prefix( arg + 1, "nostun" ) )
            bit = PCFLAG_NOSTUN;
         else if( !str_prefix( arg + 1, "nosummon" ) )
            bit = PCFLAG_NOSUMMON;
         else if( !str_prefix( arg + 1, "immproof" ) )
            bit = PCFLAG_IMMPROOF;
         else if( !str_prefix( arg + 1, "gag" ) )
            bit = PCFLAG_GAG;
         else if( !str_prefix( arg + 1, "pager" ) )
            bit = PCFLAG_PAGERON;
         else if( !str_prefix( arg + 1, "groupwho" ) )
            bit = PCFLAG_GROUPWHO;
         else if( !str_prefix( arg + 1, "@@hgflag_" ) )
            bit = PCFLAG_HIGHGAG;
         else if( !str_prefix( arg + 1, "nobeep" ) )
            bit = PCFLAG_NOBEEP;
         else if( !str_prefix( arg + 1, "anonymous" ) )
            bit = PCFLAG_ANONYMOUS;
         else
         {
            send_to_char( "Config which option?\r\n", ch );
            return;
         }

         if( fSet )
            SET_BIT( ch->pcdata->flags, bit );
         else
            REMOVE_BIT( ch->pcdata->flags, bit );

         send_to_char( "Ok.\r\n", ch );
         return;
      }
   }

   return;
}


void do_credits( CHAR_DATA * ch, char *argument )
{
   do_help( ch, "credits" );
}

/*
 * Structure used to build wizlist
 */
typedef struct immlist IMMLIST;
struct immlist
{
   IMMLIST *next;
   IMMLIST *last;
   short level;
   char *name;
   char *email;
};

IMMLIST *first_imm;
IMMLIST *last_imm;

/*
 * Makes IMMList
 */
void toimmfile( const char *line )
{
   char outline[MAX_STRING_LENGTH];
   FILE *wfp;

   strcpy( outline, line );
   strcat( outline, "\r\n" );

   wfp = fopen( IMMLIST_FILE, "a" );
   fputs( outline, wfp );
   fclose( wfp );
}

void toimmweb( const char *line )
{
   char outline[MAX_STRING_LENGTH];
   FILE *wfp;

   outline[0] = '\0';

   strcat( outline, " " );
   strcat( outline, line );

   strcat( outline, "\r\n" );
   wfp = fopen( IMMWEB_FILE, "a" );
   if( wfp )
   {
      fputs( outline, wfp );
      fclose( wfp );
   }
}

void add_to_immlist( char *name, char *email, char *lastname, int level )
{
   IMMLIST *imm, *tmp;
   char newname[1024];

   CREATE( imm, IMMLIST, 1 );
   if( lastname[0] != '\0' )
   {
      sprintf( newname, "%s %s", name, lastname );
      imm->name = str_dup( newname );
   }
   else
      imm->name = str_dup( name );
   if( email[0] != '\0' )
      imm->email = str_dup( email );
   else
      imm->email = str_dup( "None" );
   imm->level = level;

   if( !first_imm )
   {
      imm->last = NULL;
      imm->next = NULL;
      first_imm = imm;
      last_imm = imm;
      return;
   }

   /*
    * insert sort, of sorts 
    */
   for( tmp = first_imm; tmp; tmp = tmp->next )
      if( ( level > tmp->level ) )
      {
         if( !tmp->last )
            first_imm = imm;
         else
            tmp->last->next = imm;
         imm->last = tmp->last;
         imm->next = tmp;
         tmp->last = imm;
         return;
      }

   imm->last = last_imm;
   imm->next = NULL;
   last_imm->next = imm;
   last_imm = imm;
   return;
}

void read_entry( FILE * fp )
{
   int ilevel = 0, iflags = 0;
   char *name = NULL, *lastname = NULL, *email = NULL;
   bool found;

   for( ;; )
   {
      char *word;
      word = feof( fp ) ? "End" : fread_word( fp );
      found = FALSE;

      switch ( UPPER( word[0] ) )
      {
         case 'E':
            if( !str_cmp( word, "Email" ) )
            {
               email = fread_string_nohash( fp );
               found = TRUE;
               break;
            }
            if( !str_cmp( word, "End" ) )
            {
               if( !name )
               {
                  bug( "read_entry: No name was set!" );
                  return;
               }
               if( !lastname )
                  lastname = str_dup( "" );
               if( !email )
                  email = str_dup( "" );

               if( IS_SET( iflags, PCFLAG_RETIRED ) )
                  ilevel = MAX_LEVEL - 16;
               if( IS_SET( iflags, PCFLAG_GUEST ) )
                  ilevel = MAX_LEVEL - 17;

               add_to_immlist( name, email, lastname, ilevel );
               DISPOSE( name );
               DISPOSE( email );
               DISPOSE( lastname );
               return;
            }

         case 'L':
            if( !str_cmp( word, "Level" ) )
            {
               found = TRUE;
               ilevel = fread_number( fp );
               break;
            }
            if( !str_cmp( word, "Lastname" ) )
            {
               found = TRUE;
               lastname = fread_string_nohash( fp );
               break;
            }

         case 'N':
            if( !str_cmp( word, "Name" ) )
            {
               found = TRUE;
               name = fread_string_nohash( fp );
               break;
            }

         case 'P':
            if( !str_cmp( word, "Pcflags" ) )
            {
               found = TRUE;
               iflags = fread_number( fp );
               break;
            }
      }
      if( !found )
         bug( "read_entry: Unknown word: %s", word );
   }
}

void make_immlist(  )
{
   DIR *dp;
   struct dirent *dentry;
   FILE *fp;
   int ilevel;
   IMMLIST *imm, *immnext;
   char buf[MAX_STRING_LENGTH], *s;

   first_imm = NULL;
   last_imm = NULL;

   dp = opendir( GOD_DIR );

   ilevel = 0;

   dentry = readdir( dp );

   while( dentry )
   {
      if( dentry->d_name[0] == '.' )
      {
         dentry = readdir( dp );
         continue;
      }
      s = strstr( dentry->d_name, "immlist" );
      if( s == NULL )
      {
         dentry = readdir( dp );
         continue;
      }
      sprintf( buf, "%s%s", GOD_DIR, dentry->d_name );
      fp = fopen( buf, "r" );
      if( !fp )
      {
         dentry = readdir( dp );
         continue;
      }
      read_entry( fp );
      fclose( fp );
      dentry = readdir( dp );
   }
   unlink( IMMLIST_FILE );

   sprintf( buf, "&c,-&w[ Rank / Level / Name ]&c----------------------&w[ Email ]" "&c---------------------." );
   toimmfile( buf );

   buf[0] = '\0';
   ilevel = 65535;
   for( imm = first_imm; imm; imm = imm->next )
   {
      if( imm->level < ilevel )
      {
         if( buf[0] )
         {
            toimmfile( buf );
            buf[0] = '\0';
         }
         if( ilevel != 65535 )
            toimmfile( "&c:                                                                            :" );
         ilevel = imm->level;

         /*
          * I know this has extras here, but why remove it since it may
          * be wanted later -- Kratas 
          */
         switch ( ilevel )
         {
            case MAX_LEVEL - 0:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &ROmnipotent / Owner&W" );
               break;
            case MAX_LEVEL - 1:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &RSupreme Entity / Head Implementors&W" );
               break;
            case MAX_LEVEL - 2:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &RInfinite / Implementors&W" );
               break;
            case MAX_LEVEL - 3:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &REternal / Assistant Implementors&W" );
               break;
            case MAX_LEVEL - 4:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &RAncient / Administrators&W" );
               break;
            case MAX_LEVEL - 5:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &cExalted / Coders&W" );
               break;
            case MAX_LEVEL - 6:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &cAscendant / Representatives&W" );
               break;
            case MAX_LEVEL - 7:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &cMaster Gods / Master Builders&W" );
               break;
            case MAX_LEVEL - 8:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &cGreater Gods / Greater Builders&W" );
               break;
            case MAX_LEVEL - 9:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &cGods / Builders&W" );
               break;
            case MAX_LEVEL - 10:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &cLesser Gods / Lesser Builders&W" );
               break;
            case MAX_LEVEL - 11:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &cNew Gods / New Builders&W" );
               break;
            case MAX_LEVEL - 12:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &cSaviors / General Staff&W" );
               break;
            case MAX_LEVEL - 13:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &BCreators / Immortal Family&W" );
               break;
            case MAX_LEVEL - 14:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &BAcolytes / Inter-MUD Guest&W" );
               break;
            case MAX_LEVEL - 15:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &BNeophytes / Ambassador&W" );
               break;
            case MAX_LEVEL - 16:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &BRetired&W" );
               break;
            case MAX_LEVEL - 17:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &BGuests&W" );
               break;
            default:
               sprintf( buf + strlen( buf ), "&c:%-80s&c:", " &BServants&W" );
               break;
         }
         toimmfile( buf );

         buf[0] = '\0';
      }
      sprintf( buf, "&c: &W[Level %3d]  &Y%-25s %-35s &c:", ilevel, imm->name, imm->email );
      toimmfile( buf );
      buf[0] = '\0';
   }
   sprintf( buf + strlen( buf ), "`----------------------------------------------------------------------------'" );

   if( buf[0] )
   {
      toimmfile( buf ); /* Would it be otherwise? */
   }

   for( imm = first_imm; imm; imm = immnext )
   {
      immnext = imm->next;
      DISPOSE( imm->name );
      DISPOSE( imm );
   }

   toimmfile( "&w" );

   first_imm = NULL;
   last_imm = NULL;
}

void make_immweb(  )
{
   DIR *dp;
   struct dirent *dentry;
   FILE *fp;
   int ilevel;
   IMMLIST *imm, *immnext;
   char buf[MAX_STRING_LENGTH];

   first_imm = NULL;
   last_imm = NULL;

   dp = opendir( GOD_DIR );

   ilevel = 0;

   dentry = readdir( dp );

   while( dentry )
   {
      char *s;

      if( dentry->d_name[0] == '.' )
      {
         dentry = readdir( dp );
         continue;
      }

      s = strstr( dentry->d_name, "immlist" );
      if( s == NULL )
      {
         dentry = readdir( dp );
         continue;
      }

      sprintf( buf, "%s%s", GOD_DIR, dentry->d_name );
      fp = fopen( buf, "r" );
      if( !fp )
      {
         dentry = readdir( dp );
         continue;
      }
      read_entry( fp );
      fclose( fp );
      dentry = readdir( dp );
   }
   unlink( IMMWEB_FILE );
/*
    sprintf( buf, "<BR>&c,-&w[ Rank / Level / Name ]&c------------------&w[ Email ]"
                  "&c-----------------." );
    toimmweb( buf );
*/
   buf[0] = '\0';
   ilevel = 65535;
   for( imm = first_imm; imm; imm = imm->next )
   {
      if( imm->level < ilevel )
      {
         if( buf[0] )
         {
            toimmweb( buf );
            buf[0] = '\0';
         }
         if( ilevel != 65535 )
            toimmweb( "" );
         ilevel = imm->level;

         /*
          * I know this has extras here, but why remove it since it may
          * be wanted later -- Kratas 
          */
         switch ( ilevel )
         {
            case MAX_LEVEL - 0:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &ROmnipotent / Owner&W" );
               break;
            case MAX_LEVEL - 1:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &RSupreme Entity / Head Implementors&W" );
               break;
            case MAX_LEVEL - 2:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &RInfinite / Implementors&W" );
               break;
            case MAX_LEVEL - 3:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &REternal / Assistant Implementors&W" );
               break;
            case MAX_LEVEL - 4:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &RAncient / Administrators&W" );
               break;
            case MAX_LEVEL - 5:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &cExalted / Coders&W" );
               break;
            case MAX_LEVEL - 6:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &cAscendant / Representatives&W" );
               break;
            case MAX_LEVEL - 7:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &cMaster Gods / Master Builders&W" );
               break;
            case MAX_LEVEL - 8:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &cGreater Gods / Greater Builders&W" );
               break;
            case MAX_LEVEL - 9:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &cGods / Builders&W" );
               break;
            case MAX_LEVEL - 10:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &cLesser Gods / Lesser Builders&W" );
               break;
            case MAX_LEVEL - 11:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &cNew Gods / New Builders&W" );
               break;
            case MAX_LEVEL - 12:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &cSaviors / General Staff&W" );
               break;
            case MAX_LEVEL - 13:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &BCreators / Immortal Family&W" );
               break;
            case MAX_LEVEL - 14:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &BAcolytes / Inter-MUD Guest&W" );
               break;
            case MAX_LEVEL - 15:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &BNeophytes / Ambassador&W" );
               break;
            case MAX_LEVEL - 16:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &BRetired&W" );
               break;
            case MAX_LEVEL - 17:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &BGuests&W" );
               break;
            default:
               sprintf( buf + strlen( buf ), "<P>%-75s", " &BServants&W" );
               break;
         }
         toimmweb( buf );

         buf[0] = '\0';
      }
      sprintf( buf, "<P>&W[Level %3d] &&nbsp; &&nbsp; &Y%-25s &&nbsp; &&nbsp; %-35s<br>", ilevel, imm->name, imm->email );
      toimmweb( buf );
      buf[0] = '\0';
/*    }
    sprintf( buf+strlen( buf ), "<P>`----------------------------------------------------------------------------'" );

    if ( buf[0] )
    {
                    toimmweb( buf ); *//*
       * Would it be otherwise? 
       */
   }

   for( imm = first_imm; imm; imm = immnext )
   {
      immnext = imm->next;
      DISPOSE( imm->name );
      DISPOSE( imm );
   }

   toimmweb( "&w" );

   first_imm = NULL;
   last_imm = NULL;
}

void do_immlist( CHAR_DATA * ch, char *argument )
{
   set_pager_color( AT_IMMORT, ch );
   show_file( ch, IMMLIST_FILE );
}

extern int top_area;

/***********************************************************************************
 * New New do_areas.
 *
 * Author: ??  Modified by Mordred August 03, 2009.
 *
 * Notes:  This is a rewrite of the old do_area.  As I recall, a bad reference 
 * was causing a memory leak that crashed the mud.  Whoever the author was of this
 * did a fine job, it just needed a little tweaking, and was sitting out here, 
 * commented out and broken.  Should work fine.  I removed the other old do_areas
 * it's in a .bak or out there online if anyone misses it too badly.
 **********************************************************************************/

void do_areas( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   AREA_DATA *pArea;
   int count = 0;
   int softhi = -1;
   int softlo = -1;

   set_pager_color( AT_PLAIN, ch );
   sscanf(argument, "%d %d", &softlo, &softhi);
   sprintf( buf, "softlo: %d    softhi: %d\n\r", softlo, softhi );
   send_to_pager(buf, ch);
   if( softlo == -1 )
   {
      set_pager_color( AT_PLAIN, ch );
      send_to_pager( "\r\n", ch );
      send_to_pager( "&W Num1   Author            Name of the Zone Listed              Rcmd Lvls\r\n", ch );
      send_to_pager( "&z ===   ==============    ===================================  ===========\r\n", ch );

      for( pArea = first_area; pArea; pArea = pArea->next )
      {
         count++;
         sprintf( buf, "&W%3d&w> &c  %-15s   &w%-33s    &B%3d &z-&B %3d&g\r\n",
             count, pArea->author, pArea->name, pArea->low_soft_range, pArea->hi_soft_range );
         send_to_pager( buf, ch );
      }
   send_to_pager( "&z ===   ==============    ===================================  ===========\r\n", ch );
   return;
   } 
   else
   {
      if( softhi == -1 )
      {
         send_to_char( "You need to specify a second argument for the end of the level range.", ch ); 
         return;
      } else
      {
         set_pager_color( AT_PLAIN, ch );
         send_to_pager( "\r\n", ch );
         send_to_pager( "&W Num2   Author            Name of the Zone Listed              Rcmd Lvls\r\n", ch );
         send_to_pager( "&z ===   ==============    ===================================  ===========\r\n", ch );

         for( pArea = first_area ; pArea ; pArea=pArea->next )
         {
            count++;
            if( pArea->low_soft_range >= softlo && pArea->hi_soft_range <= softhi )
            {
               sprintf( buf, "&W%3d&w> &c  %-15s   &w%-33s    &B%3d &z-&B %3d&g\r\n",
             count, pArea->author, pArea->name, pArea->low_soft_range, pArea->hi_soft_range );
               send_to_pager( buf, ch );
            }
         }
      }
      send_to_pager("&z ===   ==============    ===================================  ===========\r\n", ch );
      return;
   }
}   


void do_afk( CHAR_DATA * ch, char *argument )
{
   if( IS_NPC( ch ) )
      return;

   if( xIS_SET( ch->act, PLR_AFK ) )
   {
      xREMOVE_BIT( ch->act, PLR_AFK );
      if( ch->pcdata->afkmsg )
      {
         STRFREE( ch->pcdata->afkmsg );
         ch->pcdata->afkmsg = NULL;
      }
      send_to_char( "You are no longer away.\r\n", ch );
      /*
       * act(AT_GREY,"$n is no longer away.", ch, NULL, NULL, TO_ROOM);
       */
      act( AT_GREY, "$n is no longer away.", ch, NULL, NULL, TO_CANSEE );
   }
   else
   {
      // Max out at 50 characters
      if( strlen( argument ) > 50 )
      {
         send_to_char( "Message is too long. Try to keep under 50 characters", ch );
         return;
      }

      xSET_BIT( ch->act, PLR_AFK );

      if( argument[0] != '\0' )
      {
         char buf[MAX_INPUT_LENGTH];

         sprintf( buf, " (%s) ", argument );
         ch->pcdata->afkmsg = STRALLOC( buf );
         ch_printf( ch, "You are now away: (%s)\r\n", argument );
         act( AT_GREY, "$n is now away: ($t).", ch, argument, NULL, TO_CANSEE );
      }
      else
      {
         send_to_char( "You are now away.\r\n", ch );
         /*
          * act(AT_GREY,"$n is now away.", ch, NULL, NULL, TO_ROOM);
          */
         act( AT_GREY, "$n is now away.", ch, NULL, NULL, TO_CANSEE );
      }
      return;
   }

}

/***************************************************************************
*     Shows mana and blood (if vampire) requirements to cast a spell.      *
*          Created by Desden, el Chaman Tibetano - Jun 1999                *
* Snippets page: http://personal2.redestb.es/jlalbatros/smaug_snippets.htm *
*                Email: jlalbatros@mx2.redestb.es                          *
***************************************************************************/
void do_mana( CHAR_DATA * ch, char *argument )
{
   SKILLTYPE *skill = NULL;
   char arg1[MAX_INPUT_LENGTH];
   int sn;
   int col = 0;

   argument = one_argument( argument, arg1 );

   if( IS_NPC( ch ) )
   {
      send_to_char( "Mobs cannot use this command.\r\n", ch );
      return;
   }

   if( ch->class == CLASS_WARRIOR )
   {
      send_to_char( "Warriors don't use mana to cast.\r\n", ch );
      return;
   }

   if( arg1[0] == '\0' )
   {
      if( IS_VAMPIRE( ch ) )
         send_to_char( "Syntax: blood all\r\n        blood <spell>\r\n", ch );
      else
         send_to_char( "Syntax: mana all\r\n        mana <spell>\r\n", ch );
      return;
   }

   if( !strcmp( arg1, "all" ) )
   {
      set_pager_color( AT_YELLOW, ch );

      if( IS_VAMPIRE( ch ) )
         send_to_pager( "                         BLOOD REQUIRED TO CAST\r\n", ch );
      else
         send_to_pager( "                         MANA REQUIRED TO CAST\r\n", ch );

      send_to_pager( "                        -----------------------\r\n", ch );

      for( sn = 0; sn < top_sn; sn++ )
      {
         skill = get_skilltype( sn );
         if( ch->pcdata->learned[sn] < 1 || !skill->name || !skill->min_mana )
            continue;

         if( ch->level >= skill->skill_level[ch->class] )
         {
            if( IS_VAMPIRE( ch ) )
            {
               if( ch->pcdata->condition[COND_BLOODTHIRST] >= BLOOD )
                  pager_printf_color( ch, "&Y%-12.12s:%4d   ", skill->name, BLOOD );
               else
                  pager_printf_color( ch, "&R%-12.12s:%4d   ", skill->name, BLOOD );
            }
            else
            {
               if( ch->mana >= MANA )
                  pager_printf_color( ch, "&Y%-12.12s:%4d   ", skill->name, MANA );
               else
                  pager_printf_color( ch, "&R%-12.12s:%4d   ", skill->name, MANA );
            }

            if( ++col % 4 == 0 )

               pager_printf( ch, "\r\n" );

         }
      }

      pager_printf( ch, "\r\n" );
   }

   else

   {
      if( ( sn = skill_lookup( arg1 ) ) > 0 )
      {
         skill = get_skilltype( sn );

         if( !skill->min_mana )
         {
            ch_printf( ch, "%s, '%s' doesn't use %s points.\r\n", ch->name, skill->name,
                       IS_VAMPIRE( ch ) ? "blood" : "mana" );
            return;
         }

         if( ch->pcdata->learned[sn] < 1 )
         {
            if( ch->level < skill->skill_level[ch->class] )
            {
               send_to_char( "You have not enough level to cast it.\r\n", ch );
               return;
            }
            else
            {
               send_to_char( "You must practice a spell before using it.\r\n", ch );
               return;
            }
         }

         pager_printf( ch, "You need %d %s points to cast '%s' with success.\r\n", IS_VAMPIRE( ch ) ? BLOOD : MANA,
                       IS_VAMPIRE( ch ) ? "blood" : "mana", skill->name );
      }
      else
         ch_printf( ch, "That is not a spell or a skill.\r\n" );
   }
   return;
}


/* Cleaned up slist out of boredom -- Senir */
void do_slist( CHAR_DATA * ch, char *argument )
{
   char skn[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   int sn, i, temp;
   bool lFound, charclass;
   int lowlev, hilev, classnum;
   short lasttype = SKILL_SPELL;

   if( IS_NPC( ch ) )
      return;

   classnum = ch->class;
   lowlev = 1;
   hilev = LEVEL_HERO;
   charclass = TRUE;

   argument = one_argument( argument, arg1 );

   if( arg1 && arg1[0] != '\0' && !is_number( arg1 ) )
   {
      for( classnum = 0; classnum < MAX_CLASS; classnum++ )
      {
         if( class_table[classnum] && class_table[classnum]->who_name &&
             class_table[classnum]->who_name[0] != '\0' && str_cmp( class_table[classnum]->who_name, "unused" ) )
         {
            if( toupper( arg1[0] ) == toupper( class_table[classnum]->who_name[0] )
                && !str_prefix( arg1, class_table[classnum]->who_name ) )
            {
               break;
            }
         }
      }


      if( classnum == MAX_CLASS )
      {
         send_to_char( "&C&wThat is not a valid class.\r\n", ch );
         return;
      }

      argument = one_argument( argument, arg1 );
      pager_printf( ch, "&c%s&w\r\n", class_table[classnum]->who_name );
      charclass = FALSE;
   }
   else
   {
      set_pager_color( AT_MAGIC, ch );
      send_to_pager( "&W    SPELL & SKILL LIST\r\n", ch );
      send_to_pager( "&c(&WSome may not be shown!&c)\r\n", ch );
      send_to_pager( "&c------------------------\r\n\r\n", ch );
   }

   argument = one_argument( argument, arg2 );

   if( is_number( arg1 ) )
      lowlev = atoi( arg1 );

   if( is_number( arg2 ) )
      hilev = atoi( arg2 );

   if( ( lowlev < 1 ) || ( lowlev > LEVEL_IMMORTAL ) )
      lowlev = 1;

   if( ( hilev < 0 ) || ( hilev > LEVEL_IMMORTAL ) )
      hilev = LEVEL_HERO;

   if( lowlev > hilev )
      lowlev = hilev;

   for( i = lowlev; i <= hilev; i++ )
   {
      lFound = FALSE;
      sprintf( skn, "Spell" );

      for( sn = 0; sn < top_sn; sn++ )
      {

         if( !skill_table[sn]->name )
            break;

         if( skill_table[sn]->type != lasttype )
         {
            lasttype = skill_table[sn]->type;
            strcpy( skn, skill_tname[lasttype] );
         }

         if( ch->pcdata->learned[sn] <= 0 && SPELL_FLAG( skill_table[sn], SF_SECRETSKILL ) )
            continue;

         if( i != skill_table[sn]->skill_level[classnum]
             && ( !charclass || ch->level2 == -1 || ch->level3 == -1
                  || i != skill_table[sn]->skill_level[ch->class2]
                  || i >= ch->level2 || i != skill_table[sn]->skill_level[ch->class3] || i >= ch->level3 ) )
            continue;

         if( !lFound )
         {
            lFound = TRUE;
            pager_printf( ch, "&c-----[ &WLevel %d &c]-----&w\r\n", i );
         }

         switch ( skill_table[sn]->minimum_position )
         {
            default:
               sprintf( buf, "&CInvalid&w" );
               bug( "do_slist: skill with invalid minpos, skill=%s", skill_table[sn]->name );
               break;
            case POS_DEAD:
               sprintf( buf, "&gany&w" );
               break;
            case POS_MORTAL:
               sprintf( buf, "&rmortally wounded&w" );
               break;
            case POS_INCAP:
               sprintf( buf, "&Oincapacitated&w" );
               break;
            case POS_STUNNED:
               sprintf( buf, "&Ystunned&w" );
               break;
            case POS_SLEEPING:
               sprintf( buf, "&Psleeping&w" );
               break;
            case POS_RECOVER:
               sprintf( buf, "&precovering&w" );
               break;
            case POS_RESTING:
               sprintf( buf, "&presting&w" );
               break;
            case POS_STANDING:
               sprintf( buf, "&Gstanding&w" );
               break;
            case POS_FIGHTING:
               sprintf( buf, "&Rfighting&w" );
               break;
            case POS_EVASIVE:
               sprintf( buf, "&Rfighting &c(&Wevasive&c)&w" ); /* Fighting style support -haus */
               break;
            case POS_DEFENSIVE:
               sprintf( buf, "&Rfighting &c(&Wdefensive&c)&w" );
               break;
            case POS_AGGRESSIVE:
               sprintf( buf, "&Rfighting &c(&Waggressive&c)&w" );
               break;
            case POS_BERSERK:
               sprintf( buf, "&Rfighting &c(&Wberserk&c)&w" );
               break;
            case POS_MOUNTED:
               sprintf( buf, "&Cmounted&w" );
               break;
            case POS_SITTING:
               sprintf( buf, "&Bsitting&w" );
               break;
         }

         if( i == skill_table[sn]->skill_level[classnum] )
            temp = classnum;
         else if( i == skill_table[sn]->skill_level[ch->class2] )
            temp = ch->class2;
         else
            temp = ch->class3;

         pager_printf( ch, "&c%7s&w: %20.20s \t &cCurrent&w: %-3d &cMax&w: %-3d  &cMinPos&w: %s \r\n",
                       skn, skill_table[sn]->name, ch->pcdata->learned[sn], skill_table[sn]->skill_adept[temp], buf );

      }
   }

   return;

}

void do_whois( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];

   buf[0] = '\0';

   if( IS_NPC( ch ) )
      return;

   if( argument[0] == '\0' )
   {
      if( get_trust( ch ) < 112 )
         send_to_pager( "You must input the name of an online character.\r\n", ch );
      else
         send_to_pager( "You must input the name of an online or offline character.\r\n", ch );

      return;
   }

   strcat( buf, "0." );
   strcat( buf, argument );
   if( ( ( victim = get_char_world( ch, buf ) ) == NULL ) )
   {
      send_to_pager( "No such character online.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_pager( "That's not a player!\r\n", ch );
      return;
   }

   set_pager_color( AT_GREY, ch );
   if( victim->level3 == -1 )
      sprintf( buf, " and a %d %s", victim->level2, npc_class[victim->class2] );
   else
      sprintf( buf, ", a %d %s and a %d %s", victim->level2, npc_class[victim->class2], victim->level3,
               npc_class[victim->class3] );
   if( ( IS_SET( victim->pcdata->flags, PCFLAG_ANONYMOUS ) && !IS_SET( victim->pcdata->flags, PCFLAG_DEADLY ) )
       || !IS_SET( victim->pcdata->flags, PCFLAG_ANONYMOUS ) )
   {
      pager_printf( ch, "\r\n'&W%s %s%s.'\r\n %s is a %s %s, level %d %s%s, %d years of age.\r\n",
                    victim->pcdata->extraname ? victim->pcdata->extraname : "",
                    victim->name,
                    victim->pcdata->lastname ? victim->pcdata->lastname : "",
                    victim->sex == SEX_MALE ? "&CHe&W" :
                    victim->sex == SEX_FEMALE ? "&pShe&W" : "&GIt&W",
                    victim->sex == SEX_MALE ? "&Cmale&W" :
                    victim->sex == SEX_FEMALE ? "&pfemale&W" : "&Gneutral&W",
                    capitalize( npc_race[victim->race] ),
                    victim->level, npc_class[victim->class], ( victim->level2 != -1 ) ? buf : "", get_age( victim ) );
   }
   else
   {
      pager_printf( ch, "\r\n'&W%s%s.'\r\n %s is a %s, %d years of age.\r\n",
                    victim->name,
                    victim->pcdata->title ? victim->pcdata->title : "",
                    victim->sex == SEX_MALE ? "&CHe&W" :
                    victim->sex == SEX_FEMALE ? "&pShe&W" : "&GIt&W",
                    capitalize( npc_race[victim->race] ), get_age( victim ) );
   }
   sprintf( buf, " &C%s&W is &C%s&W, &C%s&W, &C%s %s&W haired with &C%s&W eyes and &C%s&W skin.\r\n",
            victim->sex == 0 ? "It" :
            victim->sex == 1 ? "He" : "She",
            victim->pcdata->ch_height, victim->pcdata->ch_build, victim->pcdata->hair_length,
            victim->pcdata->hair_color, victim->pcdata->eye_color, victim->pcdata->skin_tone );
   send_to_pager( buf, ch );

   if( victim->pcdata->clan )
   {
      CLAN_DATA *pclan = victim->pcdata->clan;
      if( pclan->clan_type == CLAN_ORDER )
      {
         if( !str_cmp( victim->name, pclan->deity ) )
            send_to_pager( "&P Deity of the Order of ", ch );
         else if( !str_cmp( victim->name, pclan->leader ) )
            send_to_pager( "&P Leader of the Order of ", ch );
         else if( !str_cmp( victim->name, pclan->number1 ) )
            send_to_pager( "&P Number One of the Order of ", ch );
         else if( !str_cmp( victim->name, pclan->number2 ) )
            send_to_pager( "&P Number Two of the Order of ", ch );
         else
            send_to_pager( "&P Member of the Order of ", ch );
      }
      else if( pclan->clan_type == CLAN_GUILD )
      {
         if( !str_cmp( victim->name, pclan->deity ) )
            send_to_pager( "&c Deity of the Guild ", ch );
         else if( !str_cmp( victim->name, pclan->leader ) )
            send_to_pager( "&c Leader of the Guild ", ch );
         else if( !str_cmp( victim->name, pclan->number1 ) )
            send_to_pager( "&c Number One of the Guild ", ch );
         else if( !str_cmp( victim->name, pclan->number2 ) )
            send_to_pager( "&c Number Two of the Guild ", ch );
         else
            send_to_pager( "&c Member of the Guild ", ch );
      }
      else
      {
         if( !str_cmp( victim->name, pclan->deity ) )
            send_to_pager( "&r Deity of the clan ", ch );
         else if( !str_cmp( victim->name, pclan->leader ) )
            send_to_pager( "&r Leader of the clan ", ch );
         else if( !str_cmp( victim->name, pclan->number1 ) )
            send_to_pager( "&r Number One of the clan ", ch );
         else if( !str_cmp( victim->name, pclan->number2 ) )
            send_to_pager( "&r Number Two of the clan ", ch );
         else
            send_to_pager( "&r Member of the clan ", ch );
      }

      send_to_pager( victim->pcdata->clan->name, ch );
   }
   send_to_pager( "\r\n", ch );

   if( victim->pcdata->council )
      pager_printf( ch, "&B %s holds a seat on: %s\r\n",
                    victim->sex == SEX_MALE ? "&CHe&B" :
                    victim->sex == SEX_FEMALE ? "&pShe&B" : "&GIt&B", victim->pcdata->council->name );
   if( victim->pcdata->council2 )
      pager_printf( ch, "&B %s holds a seat on: %s\r\n",
                    victim->sex == SEX_MALE ? "&CHe&B" :
                    victim->sex == SEX_FEMALE ? "&pShe&B" : "&GIt&B", victim->pcdata->council2->name );
   if( victim->pcdata->deity )
      pager_printf( ch, "&Y %s has found succor in the deity %s.\r\n",
                    victim->sex == SEX_MALE ? "&CHe&Y" :
                    victim->sex == SEX_FEMALE ? "&pShe&Y" : "&GIt&Y", victim->pcdata->deity->name );

   if( victim->pcdata->homepage && victim->pcdata->homepage[0] != '\0' )
      pager_printf( ch, "&O %s homepage is: %s\r\n",
                    victim->sex == SEX_MALE ? "&CHis&O" :
                    victim->sex == SEX_FEMALE ? "&pHer&O" : "&GIts&O", show_tilde( victim->pcdata->homepage ) );

   if( victim->pcdata->email && victim->pcdata->email[0] != '\0' )
      pager_printf( ch, "&O %s email is: mailto:%s\r\n",
                    victim->sex == SEX_MALE ? "&CHis&O" :
                    victim->sex == SEX_FEMALE ? "&pHer&O" : "&GIts&O", show_tilde( victim->pcdata->email ) );

   if( xIS_SET( victim->act, PLR_MARRIED ) )
   {
      if( !victim->pcdata->wedto )
      {
         bug( "%s has marriage flag set, but wedto is not. Fixing. (Line: %d, File: %s)", victim->name, __LINE__, __FILE__ );
         xREMOVE_BIT( victim->act, PLR_MARRIED );
      }
      else
         pager_printf( ch, "&g %s is married to %s.\r\n",
                       victim->sex == SEX_MALE ? "&CHe&g" :
                       victim->sex == SEX_FEMALE ? "&pShe&g" : "&GIt&g", victim->pcdata->wedto );
   }
   else
   {
      pager_printf( ch, "&g %s is still single!\r\n",
                    victim->sex == SEX_MALE ? "&CHe&g" : victim->sex == SEX_FEMALE ? "&pShe&g" : "&GIt&g" );
   }
/* Limited bounty info for morts - Garinan */
   if( victim->bounty > 0 )
   {
      pager_printf( ch, "&Y %s has a bounty on %s head!\r\n", victim->name,
                    victim->sex == SEX_MALE ? "his" : victim->sex == SEX_FEMALE ? "her" : "its" );
   }
   if( victim->belig != 1 && victim->belig != 0 )
   {
      if( IS_IMMORTAL( ch ) )
      {
         pager_printf( ch, "&p%s has an invalid belig value: %d (Correcting)", victim->name, victim->belig );
      }
      victim->belig = 0;
   }
   if( victim->belig == 1 && !victim->bounty > 0 && !IS_IMMORTAL( victim ) )
   {
      pager_printf( ch, "&Y %s is eligible have a bounty on %s head!\r\n", victim->name,
                    victim->sex == SEX_MALE ? "his" : victim->sex == SEX_FEMALE ? "her" : "its" );
   }
/* End of bounty info - Garinan */
   if( victim->pcdata->bio && victim->pcdata->bio[0] != '\0' )
      pager_printf( ch, "\r\n&c-----[ &W%s's personal bio &c]-----&w\r\n%s", victim->name, victim->pcdata->bio );
   else
      pager_printf( ch, "\r\n &w%s has yet to create a bio.\r\n", victim->name );

   if( IS_IMMORTAL( ch ) )
   {
      send_to_pager( "\r\n&c-----[ &WInfo for immortals &c]-----\r\n", ch );

/* Bounty Info for immortals  - Garinan */
      if( victim->bounty > 0 )
      {
         pager_printf( ch, "&Y%s's bounty is worth %d.\r\n", victim->name, victim->bounty );
      }

      if( victim->first_abit )
      {
         BIT_DATA *bit;

         for( bit = victim->first_abit; bit; bit = bit->next )
         {
            if( strlen( bit->desc ) > 3 )
               pager_printf( ch, "%s\r\n", bit->desc );
         }
      }
      if( victim->pcdata->first_qbit )
      {
         BIT_DATA *bit;

         for( bit = victim->pcdata->first_qbit; bit; bit = bit->next )
         {
            if( strlen( bit->desc ) > 3 )
               pager_printf( ch, "%s\r\n", bit->desc );
         }
      }

      if( victim->pcdata->authed_by && victim->pcdata->authed_by[0] != '\0' )
         pager_printf( ch, "&Y%s was authorized by %s.\r\n", victim->name, victim->pcdata->authed_by );

      pager_printf( ch, "&R%s has killed %d mobiles, and been killed by a mobile %d times.\r\n",
                    victim->name, victim->pcdata->mkills, victim->pcdata->mdeaths );
      if( victim->pcdata->pkills || victim->pcdata->pdeaths )
         pager_printf( ch, "&r%s has killed %d players, and been killed by a player %d times.\r\n",
                       victim->name, victim->pcdata->pkills, victim->pcdata->pdeaths );
      if( victim->pcdata->illegal_pk )
         pager_printf( ch, "^z&r%s has committed %d illegal player kills.^x\r\n", victim->name, victim->pcdata->illegal_pk );

      pager_printf( ch, "&O%s is %shelled at the moment.\r\n",
                    victim->name, ( victim->pcdata->release_date == 0 ) ? "&Gnot&O " : "" );

      if( victim->pcdata->nuisance )
      {
         pager_printf_color( ch, "&RNuisance   &cStage: (&R%d&c/%d)  Power:  &w%d  &cTime:  &w%s.\r\n",
                             victim->pcdata->nuisance->flags, MAX_NUISANCE_STAGE, victim->pcdata->nuisance->power,
                             ctime( &victim->pcdata->nuisance->time ) );
      }
      if( victim->pcdata->release_date != 0 )
         pager_printf( ch, "&O%s was helled by %s, and will be released on %24.24s.\r\n",
                       victim->sex == SEX_MALE ? "&CHe&O" :
                       victim->sex == SEX_FEMALE ? "&pShe&O" : "&GIt&O",
                       victim->pcdata->helled_by, ctime( &victim->pcdata->release_date ) );

      if( get_trust( victim ) < get_trust( ch ) )
      {
         sprintf( buf2, "list %s", buf );
         do_comment( ch, buf2 );
      }

      if( xIS_SET( victim->act, PLR_SILENCE ) || xIS_SET( victim->act, PLR_NO_EMOTE )
          || xIS_SET( victim->act, PLR_NO_TELL ) || xIS_SET( victim->act, PLR_THIEF ) || xIS_SET( victim->act, PLR_KILLER ) )
      {
         sprintf( buf2, "&WThis player has the following flags set:" );
         if( xIS_SET( victim->act, PLR_SILENCE ) )
            strcat( buf2, "&Y silence&W" );
         if( xIS_SET( victim->act, PLR_NO_EMOTE ) )
            strcat( buf2, "&O noemote&W" );
         if( xIS_SET( victim->act, PLR_NO_TELL ) )
            strcat( buf2, "&w notell&W" );
         if( xIS_SET( victim->act, PLR_THIEF ) )
            strcat( buf2, "&R thief&W" );
         if( xIS_SET( victim->act, PLR_KILLER ) )
            strcat( buf2, "&r killer&W" );
         if( xIS_SET( victim->act, PLR_COWARD ) )
            strcat( buf2, "&C coward&W" );
         strcat( buf2, ".\r\n" );
         send_to_pager( buf2, ch );
      }
      if( victim->desc && victim->desc->host[0] != '\0' )   /* added by Gorog */
      {
         sprintf( buf2, "&C%s's IP info: %s ", victim->name, victim->desc->host );
         if( get_trust( ch ) >= LEVEL_GOD )
            strcat( buf2, victim->desc->user );
         strcat( buf2, "\r\n" );
         send_to_pager( buf2, ch );
      }
   }
}

void do_pager( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

   if( IS_NPC( ch ) )
      return;
   set_char_color( AT_NOTE, ch );
   argument = one_argument( argument, arg );
   if( !*arg )
   {
      if( IS_SET( ch->pcdata->flags, PCFLAG_PAGERON ) )
      {
         send_to_char( "Pager disabled.\r\n", ch );
         do_config( ch, "-pager" );
      }
      else
      {
         ch_printf( ch, "Pager is now enabled at %d lines.\r\n", ch->pcdata->pagerlen );
         do_config( ch, "+pager" );
      }
      return;
   }
   if( !is_number( arg ) )
   {
      send_to_char( "Set page pausing to how many lines?\r\n", ch );
      return;
   }
   ch->pcdata->pagerlen = atoi( arg );
   if( ch->pcdata->pagerlen < 5 )
      ch->pcdata->pagerlen = 5;
   ch_printf( ch, "Page pausing set to %d lines.\r\n", ch->pcdata->pagerlen );
   return;
}

/*
 * The ignore command allows players to ignore up to MAX_IGN
 * other players. Players may ignore other characters whether
 * they are online or not. This is to prevent people from
 * spamming someone and then logging off quickly to evade
 * being ignored.
 * Syntax:
 *	ignore		-	lists players currently ignored
 *	ignore none	-	sets it so no players are ignored
 *	ignore <player>	-	start ignoring player if not already
 *				ignored otherwise stop ignoring player
 *	ignore reply	-	start ignoring last player to send a
 *				tell to ch, to deal with invis spammers
 * Last Modified: June 26, 1997
 * - Fireblade
 */
void do_ignore( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   IGNORE_DATA *temp, *next;
   char fname[1024];
   struct stat fst;
   CHAR_DATA *victim;

   if( IS_NPC( ch ) )
      return;

   argument = one_argument( argument, arg );

   sprintf( fname, "%s%c/%s", PLAYER_DIR, tolower( arg[0] ), capitalize( arg ) );

   victim = NULL;

   /*
    * If no arguements, then list players currently ignored 
    */
   if( arg[0] == '\0' )
   {
      set_char_color( AT_DIVIDER, ch );
      ch_printf( ch, "\r\n----------------------------------------\r\n" );
      set_char_color( AT_DGREEN, ch );
      ch_printf( ch, "You are currently ignoring:\r\n" );
      set_char_color( AT_DIVIDER, ch );
      ch_printf( ch, "----------------------------------------\r\n" );
      set_char_color( AT_IGNORE, ch );

      if( !ch->pcdata->first_ignored )
      {
         ch_printf( ch, "\t    no one\r\n" );
         return;
      }

      for( temp = ch->pcdata->first_ignored; temp; temp = temp->next )
      {
         ch_printf( ch, "\t  - %s\r\n", temp->name );
      }

      return;
   }
   /*
    * Clear players ignored if given arg "none" 
    */
   else if( !strcmp( arg, "none" ) )
   {
      for( temp = ch->pcdata->first_ignored; temp; temp = next )
      {
         next = temp->next;
         UNLINK( temp, ch->pcdata->first_ignored, ch->pcdata->last_ignored, next, prev );
         STRFREE( temp->name );
         DISPOSE( temp );
      }

      set_char_color( AT_IGNORE, ch );
      ch_printf( ch, "You now ignore no one.\r\n" );

      return;
   }
   /*
    * Prevent someone from ignoring themself... 
    */
   else if( !strcmp( arg, "self" ) || nifty_is_name( arg, ch->name ) )
   {
      set_char_color( AT_IGNORE, ch );
      ch_printf( ch, "Did you type something?\r\n" );
      return;
   }
   else if( !check_parse_name( capitalize( arg ), TRUE ) )
   {
      set_char_color( AT_IGNORE, ch );
      ch_printf( ch, "There is no player by that name?\r\n" );
      return;
   }

   else
   {
      int i;

      /*
       * get the name of the char who last sent tell to ch 
       */
      if( !strcmp( arg, "reply" ) )
      {
         if( !ch->reply )
         {
            set_char_color( AT_IGNORE, ch );
            ch_printf( ch, "They're not here.\r\n" );
            return;
         }
         else
         {
            strcpy( arg, ch->reply->name );
         }
      }

      /*
       * Loop through the linked list of ignored players 
       */
      /*
       * keep track of how many are being ignored     
       */
      for( temp = ch->pcdata->first_ignored, i = 0; temp; temp = temp->next, i++ )
      {
         /*
          * If the argument matches a name in list remove it 
          */
         if( !strcmp( temp->name, capitalize( arg ) ) )
         {
            UNLINK( temp, ch->pcdata->first_ignored, ch->pcdata->last_ignored, next, prev );
            set_char_color( AT_IGNORE, ch );
            ch_printf( ch, "You no longer ignore %s.\r\n", temp->name );
            STRFREE( temp->name );
            DISPOSE( temp );
            return;
         }
      }

      /*
       * if there wasn't a match check to see if the name   
       */
      /*
       * is valid. This if-statement may seem like overkill 
       */
      /*
       * but it is intended to prevent people from doing the
       */
      /*
       * spam and log thing while still allowing ya to      
       */
      /*
       * ignore new chars without pfiles yet...             
       */
      if( stat( fname, &fst ) == -1 &&
          ( !( victim = get_char_world( ch, arg ) ) || IS_NPC( victim ) || strcmp( capitalize( arg ), victim->name ) != 0 ) )
      {
         set_char_color( AT_IGNORE, ch );
         ch_printf( ch, "No player exists by that" " name.\r\n" );
         return;
      }

      if( victim )
      {
         strcpy( capitalize( arg ), victim->name );
      }

      if( !check_parse_name( capitalize( arg ), TRUE ) )
      {
         set_char_color( AT_IGNORE, ch );
         send_to_char( "No player exists by that name.\r\n", ch );
         return;
      }

      /*
       * If its valid and the list size limit has not been 
       */
      /*
       * reached create a node and at it to the list       
       */
      if( i < MAX_IGN )
      {
         IGNORE_DATA *new;
         CREATE( new, IGNORE_DATA, 1 );
         new->name = STRALLOC( capitalize( arg ) );
         new->next = NULL;
         new->prev = NULL;
         LINK( new, ch->pcdata->first_ignored, ch->pcdata->last_ignored, next, prev );
         set_char_color( AT_IGNORE, ch );
         ch_printf( ch, "You now ignore %s.\r\n", new->name );
         return;
      }
      else
      {
         set_char_color( AT_IGNORE, ch );
         ch_printf( ch, "You may only ignore %d players.\r\n", MAX_IGN );
         return;
      }
   }
}

/*
 * This function simply checks to see if ch is ignoring ign_ch.
 * Last Modified: October 10, 1997
 * - Fireblade
 */
bool is_ignoring( CHAR_DATA * ch, CHAR_DATA * ign_ch )
{
   IGNORE_DATA *temp;

   if( IS_NPC( ch ) || IS_NPC( ign_ch ) )
      return FALSE;

   for( temp = ch->pcdata->first_ignored; temp; temp = temp->next )
   {
      if( nifty_is_name( temp->name, ign_ch->name ) )
         return TRUE;
   }

   return FALSE;
}

/*
 * Mortal help requests. Stored until they quit or are cleared.
 * Used to assist mortals with problems/etc.
 *
 * Kratas
 */
void do_request( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   DESCRIPTOR_DATA *d;
   char arg[MAX_INPUT_LENGTH];
   bool found = FALSE;

   one_argument( argument, arg );

   if( IS_IMMORTAL( ch ) )
   {
      if( arg[0] == '\0' )
      {
         send_to_char( "Those waiting:\r\n" "--------------\r\n", ch );

         for( d = first_descriptor; d; d = d->next )
            if( ( victim = d->character ) != NULL && IS_NEEDHELP( victim ) )
            {
               ch_printf_color( ch, "&c%-10s &W(&c %s &W)&w\r\n", victim->name, victim->pcdata->help_message );
               found = TRUE;
            }
         if( !found )
            ch_printf( ch, "None found.\r\n" );
         return;
      }
      else
      {
         for( d = first_descriptor; d; d = d->next )
            if( ( victim = d->character ) != NULL && IS_NEEDHELP( victim ) && !str_cmp( victim->name, arg ) )  /* We want it to be exact */
            {
               xREMOVE_BIT( victim->act, PLR_NEEDHELP );
               ch_printf( ch, "&W%s&c has been removed from the list.&w\r\n", victim->name );
               send_to_char( "Your request for assistance has been cleared.\r\n", victim );
               if( victim->pcdata->help_message )
                  DISPOSE( victim->pcdata->help_message );
               return;
            }
         send_to_char( "That name was not found.\r\n", ch );
         return;
      }
   }

   if( !IS_NEEDHELP( ch ) )
   {
      char buf[MAX_STRING_LENGTH];

      if( argument[0] == '\0' )
      {
         ch_printf( ch, "Might be more useful to specify what you want help with.\r\n" );
         return;
      }
      ch_printf( ch, "The request &c\"%s\"&w has been sent to the admins. Please wait.\r\n", argument );
      xSET_BIT( ch->act, PLR_NEEDHELP );
      sprintf( buf, "%s is requesting help: %s", ch->name, argument );
      /*
       * talk_channel( ch, buf, CHANNEL_HELP, "help" ); 
       */
      ch->pcdata->help_message = str_dup( argument );
      return;
   }
   else
   {
      send_to_char( "You are already being helped.\r\n", ch );
      return;
   }
}

/*
 * Shows kill history to a player
 *
 * Kratas
 */
void do_kills( CHAR_DATA * ch, char *argument )
{
   MOB_INDEX_DATA *tmob;
   int track;

   if( IS_NPC( ch ) )
   {
      no_find( ch );
      return;
   }

   set_char_color( AT_BLOOD, ch );
   ch_printf( ch, "Kill history:\r\n" );

   for( track = 0; track < MAX_KILLTRACK && ch->pcdata->killed[track].vnum; track++ )
   {
      tmob = get_mob_index( ch->pcdata->killed[track].vnum );

      if( !tmob )
      {
         bug( "killhistory: unknown mob vnum" );
         continue;
      }

      set_char_color( AT_RED, ch );
      ch_printf( ch, "   %-25s", capitalize( tmob->short_descr ) );
      set_char_color( AT_BLOOD, ch );
      set_char_color( AT_RED, ch );
      ch_printf( ch, "    - killed %d times.\r\n", ch->pcdata->killed[track].count );
   }

   return;
}

/*
 * Shows level 50 commands and up to level 100 for avatars.
 *
 * Kratas
 */
void do_avhelp( CHAR_DATA * ch, char *argument )
{
   int col;
   bool found;
   int hash;
   CMDTYPE *command;

   col = 0;
   set_pager_color( AT_PLAIN, ch );
   if( argument[0] == '\0' )
   {
      for( hash = 0; hash < 126; hash++ )
         for( command = command_hash[hash]; command; command = command->next )
            if( ( command->level < LEVEL_AVATAR
                  && command->level >= LEVEL_PCAVATAR )
                && command->level <= get_trust( ch ) && ( command->name[0] != 'm' && command->name[1] != 'p' ) )
            {
               pager_printf( ch, "%-12s", command->name );
               if( ++col % 6 == 0 )
                  send_to_pager( "\r\n", ch );
            }
      if( col % 6 != 0 )
         send_to_pager( "\r\n", ch );
   }
   else
   {
      found = FALSE;
      for( hash = 0; hash < 126; hash++ )
         for( command = command_hash[hash]; command; command = command->next )
            if( ( command->level < LEVEL_AVATAR
                  && command->level > LEVEL_PCAVATAR )
                && command->level <= get_trust( ch )
                && !str_prefix( argument, command->name ) && ( command->name[0] != 'm' && command->name[1] != 'p' ) )
            {
               pager_printf( ch, "%-12s", command->name );
               found = TRUE;
               if( ++col % 6 == 0 )
                  send_to_pager( "\r\n", ch );
            }

      if( col % 6 != 0 )
         send_to_pager( "\r\n", ch );
      if( !found )
         ch_printf( ch, "No command found under %s.\r\n", argument );
   }
   return;
}

/* NoHelp added August 1 2001 -Garinan */
void do_nohelps( CHAR_DATA * ch, char *argument )
{
   CMDTYPE *command;
   AREA_DATA *tArea;
   char arg[MAX_STRING_LENGTH];
   int hash, col = 0, sn = 0;

   argument = one_argument( argument, arg );

   if( !IS_IMMORTAL( ch ) || IS_NPC( ch ) )
   {
      no_find( ch );
      return;
   }

   if( arg[0] == '\0' || !str_cmp( arg, "all" ) )
   {
      do_nohelps( ch, "commands" );
      send_to_char( "\r\n", ch );
      do_nohelps( ch, "skills" );
      send_to_char( "\r\n", ch );
      do_nohelps( ch, "areas" );
      send_to_char( "\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "commands" ) )
   {
      send_to_char_color( "&C&YCommands for which there are no help files:\r\n\r\n", ch );

      for( hash = 0; hash < 126; hash++ )
      {
         for( command = command_hash[hash]; command; command = command->next )
         {
            if( !get_help( ch, command->name ) )
            {
               ch_printf_color( ch, "&W%-15s", command->name );
               if( ++col % 5 == 0 )
               {
                  send_to_char( "\r\n", ch );
               }
            }
         }
      }

      send_to_char( "\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "skills" ) || !str_cmp( arg, "spells" ) )
   {
      send_to_char_color( "&CSkills/Spells for which there are no help files:\r\n\r\n", ch );

      for( sn = 0; sn < top_sn && skill_table[sn] && skill_table[sn]->name; sn++ )
      {
         if( !get_help( ch, skill_table[sn]->name ) )
         {
            ch_printf_color( ch, "&W%-20s", skill_table[sn]->name );
            if( ++col % 4 == 0 )
            {
               send_to_char( "\r\n", ch );
            }
         }
      }

      send_to_char( "\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "areas" ) )
   {
      send_to_char_color( "&GAreas for which there are no help files:\r\n\r\n", ch );

      for( tArea = first_area; tArea; tArea = tArea->next )
      {
         if( !get_help( ch, tArea->name ) )
         {
            ch_printf_color( ch, "&W%-35s", tArea->name );
            if( ++col % 2 == 0 )
            {
               send_to_char( "\r\n", ch );
            }
         }
      }

      send_to_char( "\r\n", ch );
      return;
   }

   send_to_char( "Syntax:  nohelps <all|areas|commands|skills>\r\n", ch );
   return;
}

/* Trying to make a way to easily see what a character had, eq-wise, when they hit a DT.
 * This function MUST be called, BEFORE the character is extracted!  -Sadiq
 */
void record_death_trap( CHAR_DATA * ch )
{
   OBJ_DATA *obj;
   int iWear;
   char buf[MAX_STRING_LENGTH];
   FILE *fp = fopen( DT_FILE, "a" );
   fprintf( fp, "%s hit a DT in room %d.\n", ch->name, ch->in_room->vnum );
   fprintf( fp, "They lost the following eq:\n\n" );

   for( iWear = 0; iWear < MAX_WEAR; iWear++ )
   {
      if( ( obj = get_eq_char( ch, iWear ) ) != NULL )
      {

         if( ( !IS_NPC( ch ) ) && ( ch->race > 0 ) && ( ch->race < MAX_RACE ) )
         {
            fprintf( fp, "     %s", race_table[ch->race]->where_name[iWear] );
         }
         else
         {
            fprintf( fp, "     %s", where_name[iWear] );
         }

         fprintf( fp, "     %-10d   %s\n", obj->pIndexData->vnum, obj->name );
      }
   }
   fprintf( fp, "\n\n" );
   fclose( fp );
   sprintf(buf, "cp -v %s%c/%s %sdt/%s.bak >> cp.out", PLAYER_DIR, tolower( ch->pcdata->filename[0] ), capitalize( ch->pcdata->filename ), PLAYER_DIR, capitalize( 
ch->pcdata->filename ));
   system(buf);
   return;
}

void do_classes( CHAR_DATA * ch, char *argument )
{
   int iClass = 0;
   int counter = 0;

   if( !IS_IMMORTAL( ch ) )
      counter = 1;

   send_to_pager( "\r\n", ch );
   for( iClass = 0; iClass < MAX_CLASS; iClass++ )
   {
      pager_printf( ch, "&c[&C%2d&c]&W %-10s", counter, class_table[iClass]->who_name );
      if( IS_IMMORTAL( ch ) )
         pager_printf( ch, "%-1s &cExpbase:&W %-5d &cHpmin: &W%-3d &cHpmax: &W%-3d", "",
                       class_table[iClass]->exp_base, class_table[iClass]->hp_min, class_table[iClass]->hp_max );
      pager_printf( ch, "\r\n" );
      counter++;
   }
   return;
}

void do_races( CHAR_DATA * ch, char *argument )
{
   int iRace = 0;
   int counter = 0;

   if( !IS_IMMORTAL( ch ) )
      counter = 1;

   send_to_pager( "\r\n", ch );
   for( iRace = 0; iRace < MAX_RACE; iRace++ )
   {
      pager_printf( ch, "&g[&G%2d&g]&W %-10s", counter, race_table[iRace]->race_name );
      if( IS_IMMORTAL( ch ) )
         pager_printf( ch, "%-1s &gMultiplier:&W %-5d &gHit: &W%-3d &gMana: &W%-3d", "",
                       race_table[iRace]->exp_multiplier, race_table[iRace]->hit, race_table[iRace]->mana );
      pager_printf( ch, "\r\n" );
      counter++;
   }
   return;
}
