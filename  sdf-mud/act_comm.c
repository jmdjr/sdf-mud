/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ResortMud Version 5.0b programed By Badastaz and Garinan
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
 *			   Player communication module			    *
 ****************************************************************************/

#include <string.h>
#include <ctype.h>
#include <time.h>
#ifndef MUD_HFILE
#include "mud.h"
#endif

/*
 *  Externals
 */
void send_obj_page_to_char( CHAR_DATA * ch, OBJ_INDEX_DATA * idx, char page );
void send_room_page_to_char( CHAR_DATA * ch, ROOM_INDEX_DATA * idx, char page );
void send_page_to_char( CHAR_DATA * ch, MOB_INDEX_DATA * idx, char page );
void send_control_page_to_char( CHAR_DATA * ch, char page );
char *act_string( const char *format, CHAR_DATA * to, CHAR_DATA * ch, const void *arg1, const void *arg2, int flags );
/* mobchat.c -- Scion */
void do_mobchat( CHAR_DATA * vict, CHAR_DATA * ch, char *message );

/*
 * Local functions.
 */
void talk_channel args( ( CHAR_DATA * ch, char *argument, int channel, const char *verb ) );

bool check_cuss args( ( char *cuss ) );
char *scramble args( ( const char *argument, int modifier ) );
char *drunk_speech args( ( const char *argument, CHAR_DATA * ch ) );
void sort_filtered args( ( FILTER_DATA * pRes ) ); /* db.c -Gar */

void drop_artifacts( CHAR_DATA * ch, OBJ_DATA * obj );   /* Scion */

/*
 * Channel Data
 */
struct channel_data
{
   int channel;
   char *prefix;
   char *act_text;
   char *buf_text;
   int color;
   int type;
   char *history[10][2];
   int history_level[10];
} local_cfields[] =
{
   {
   CHANNEL_CHAT, "&W[&G%s&W] &G%s&w", "&W[&G%s&W] &w$n&W: &G$t&w", "&W[&G%s&W] &wYou&W: &G%s&w\r\n", AT_GOSSIP, 1},
   {
   CHANNEL_WARTALK, "&W[&r%s&W] &r%s&w", "&W[&r%s&W] &w$n&W: &r$t&w", "&W[&r%s&W] &wYou&W: &r%s&w\r\n", AT_WARTALK, 1},
   {
   CHANNEL_IMMTALK, "&W[&Y%s&W] &Y%s&w", "&W[&Y%s&W] &w$n&W: &Y$t&w", "&W[&Y%s&W] &wYou&W: &Y%s&w\r\n", AT_IMMORT, 2},
   {
   CHANNEL_AVTALK, "&W[&O%s&W] &O%s&w", "&W[&O%s&W] &w$n&W: &O$t&w", "&W[&O%s&W] &wYou&W: &O%s&w\r\n", AT_AVTALK, 2},
   {
   CHANNEL_RACETALK, "&W[&B%s&W] &B%s&w", "&W[&B%s&W] &w$n&W: &B$t&w", "&W[&B%s&W] &wYou&W: &B%s&w\r\n", AT_RACETALK, 1},
   {
   CHANNEL_MUSIC, "&W[&P%s&W] &P%s&w", "&W[&P%s&W] &w$n&W: &P$t&w", "&W[&P%s&W] &wYou&W: &P%s&w\r\n", AT_MUSIC, 1},
   {
   CHANNEL_CLAN, "&W[&R%s&W] &R%s&w", "&W[&R%s&W] &w$n&W: &R$t&w", "&W[&R%s&W] &wYou&W: &R%s&w\r\n", AT_CLANTALK, 1},
   {
   CHANNEL_DEMI, "&C[&P%s&C] &C%s&w", "&C[&P%s&C] &w$n&W: &C$t&w", "&C[&P%s&C] &wYou&W: &C%s&w\r\n", AT_DEMI, 1},
   {
   CHANNEL_THINK, "&W[&C%s&W] &C%s&w", "&W[&C%s&W] &w$n&W: &C$t&w", "&W[&C%s&W] &wYou&W: &C%s&w\r\n", AT_CYAN, 1},
   {
   -1, "&W[&c%s&W] &c%s&w", "&W[&c%s&W] &w$n&W: &c$t&w", "&W[&c%s&W] &wYou&W: &c%s&w\r\n", AT_CHAN_DEFAULT, 1}
};

bool room_is_silent( CHAR_DATA * ch )
{
   ROOM_INDEX_DATA *room = ch->in_room;

   if( IS_SET( room->room_flags, ROOM_SILENCE ) )
      return TRUE;

   /*
    * if ( xIS_SET( room->affected_by, ROOM_AFF_SILENCE ) )
    * return TRUE; 
    */

   return FALSE;
}

/* Text scrambler -- Altrag */
char *scramble( const char *argument, int modifier )
{
   static char arg[MAX_INPUT_LENGTH];
   short position;
   short conversion = 0;

   modifier %= number_range( 80, 300 );   /* Bitvectors get way too large #s */
   for( position = 0; position < MAX_INPUT_LENGTH; position++ )
   {
      if( argument[position] == '\0' )
      {
         arg[position] = '\0';
         return arg;
      }
      else if( argument[position] >= 'A' && argument[position] <= 'Z' )
      {
         conversion = -conversion + position - modifier + argument[position] - 'A';
         conversion = number_range( conversion - 5, conversion + 5 );
         while( conversion > 25 )
            conversion -= 26;
         while( conversion < 0 )
            conversion += 26;
         arg[position] = conversion + 'A';
      }
      else if( argument[position] >= 'a' && argument[position] <= 'z' )
      {
         conversion = -conversion + position - modifier + argument[position] - 'a';
         conversion = number_range( conversion - 5, conversion + 5 );
         while( conversion > 25 )
            conversion -= 26;
         while( conversion < 0 )
            conversion += 26;
         arg[position] = conversion + 'a';
      }
      else if( argument[position] >= '0' && argument[position] <= '9' )
      {
         conversion = -conversion + position - modifier + argument[position] - '0';
         conversion = number_range( conversion - 2, conversion + 2 );
         while( conversion > 9 )
            conversion -= 10;
         while( conversion < 0 )
            conversion += 10;
         arg[position] = conversion + '0';
      }
      else
         arg[position] = argument[position];
   }
   arg[position] = '\0';
   return arg;
}

/* I'll rewrite this later if its still needed.. -- Altrag
char *translate( CHAR_DATA *ch, CHAR_DATA *victim, const char *argument )
{
	return "";
}
*/

LANG_DATA *get_lang( const char *name )
{
   LANG_DATA *lng;

   for( lng = first_lang; lng; lng = lng->next )
      if( !str_cmp( lng->name, name ) )
         return lng;
   return NULL;
}

/* percent = percent knowing the language. */
char *translate( int percent, const char *in, const char *name )
{
   LCNV_DATA *cnv;
   static char buf[256];
   char buf2[256];
   const char *pbuf;
   char *pbuf2 = buf2;
   LANG_DATA *lng;

   if( percent > 99 || !str_cmp( name, "common" ) )
   {
      strncpy( buf, in, 256 );
      return buf;
   }

   /*
    * If we don't know this language... use "default" 
    */
   if( !( lng = get_lang( name ) ) )
   {
      if( !( lng = get_lang( "default" ) ) )
      {
         strncpy( buf, in, 256 );
         return buf;
      }
   }

   for( pbuf = in; *pbuf; )
   {
      for( cnv = lng->first_precnv; cnv; cnv = cnv->next )
      {
         if( !str_prefix( cnv->old, pbuf ) )
         {
            if( percent && ( rand(  ) % 100 ) < percent )
            {
               strncpy( pbuf2, pbuf, cnv->olen );
               pbuf2[cnv->olen] = '\0';
               pbuf2 += cnv->olen;
            }
            else
            {
               strcpy( pbuf2, cnv->new );
               pbuf2 += cnv->nlen;
            }
            pbuf += cnv->olen;
            break;
         }
      }
      if( !cnv )
      {
         if( isalpha( *pbuf ) && ( !percent || ( rand(  ) % 100 ) > percent ) )
         {
            *pbuf2 = lng->alphabet[LOWER( *pbuf ) - 'a'];
            if( isupper( *pbuf ) )
               *pbuf2 = UPPER( *pbuf2 );
         }
         else
            *pbuf2 = *pbuf;
         pbuf++;
         pbuf2++;
      }
   }
   *pbuf2 = '\0';
   for( pbuf = buf2, pbuf2 = buf; *pbuf; )
   {
      for( cnv = lng->first_cnv; cnv; cnv = cnv->next )
         if( !str_prefix( cnv->old, pbuf ) )
         {
            strcpy( pbuf2, cnv->new );
            pbuf += cnv->olen;
            pbuf2 += cnv->nlen;
            break;
         }
      if( !cnv )
         *( pbuf2++ ) = *( pbuf++ );
   }
   *pbuf2 = '\0';
#if 0
   for( pbuf = in, pbuf2 = buf; *pbuf && *pbuf2; pbuf++, pbuf2++ )
      if( isupper( *pbuf ) )
         *pbuf2 = UPPER( *pbuf2 );
   /*
    * Attempt to align spacing.. 
    */
      else if( isspace( *pbuf ) )
         while( *pbuf2 && !isspace( *pbuf2 ) )
            pbuf2++;
#endif
   return buf;
}


char *drunk_speech( const char *argument, CHAR_DATA * ch )
{
   const char *arg = argument;
   static char buf[MAX_INPUT_LENGTH * 2];
   char buf1[MAX_INPUT_LENGTH * 2];
   short drunk;
   char *txt;
   char *txt1;

   if( IS_NPC( ch ) || !ch->pcdata )
   {
      strncpy( buf, argument, 256 );
      return buf;
   }

   drunk = ch->pcdata->condition[COND_DRUNK];

   if( drunk <= 0 )
   {
      strncpy( buf, argument, 256 );
      return buf;
   }

   buf[0] = '\0';
   buf1[0] = '\0';

   if( !argument )
   {
      bug( "Drunk_speech: NULL argument", 0 );
      return "";
   }

   txt = buf;
   txt1 = buf1;

   while( *arg != '\0' )
   {
      if( toupper( *arg ) == 'T' )
      {
         if( number_percent(  ) < ( drunk * 2 ) )  /* add 'h' after an 'T' */
         {
            *txt++ = *arg;
            *txt++ = 'h';
         }
         else
            *txt++ = *arg;
      }
      else if( toupper( *arg ) == 'X' )
      {
         if( number_percent(  ) < ( drunk * 2 / 2 ) )
         {
            *txt++ = 'c', *txt++ = 's', *txt++ = 'h';
         }
         else
            *txt++ = *arg;
      }
      else if( number_percent(  ) < ( drunk * 2 / 5 ) )  /* slurred letters */
      {
         short slurn = number_range( 1, 2 );
         short currslur = 0;

         while( currslur < slurn )
            *txt++ = *arg, currslur++;
      }
      else
         *txt++ = *arg;

      arg++;
   };

   *txt = '\0';

   txt = buf;

   while( *txt != '\0' )   /* Let's mess with the string's caps */
   {
      if( number_percent(  ) < ( 2 * drunk / 2.5 ) )
      {
         if( isupper( *txt ) )
            *txt1 = tolower( *txt );
         else if( islower( *txt ) )
            *txt1 = toupper( *txt );
         else
            *txt1 = *txt;
      }
      else
         *txt1 = *txt;

      txt1++, txt++;
   };

   *txt1 = '\0';
   txt1 = buf1;
   txt = buf;

   while( *txt1 != '\0' )  /* Let's make them stutter */
   {
      if( *txt1 == ' ' )   /* If there's a space, then there's gotta be a */
      {  /* along there somewhere soon */

         while( *txt1 == ' ' )   /* Don't stutter on spaces */
            *txt++ = *txt1++;

         if( ( number_percent(  ) < ( 2 * drunk / 4 ) ) && *txt1 != '\0' )
         {
            short offset = number_range( 0, 2 );
            short pos = 0;

            while( *txt1 != '\0' && pos < offset )
               *txt++ = *txt1++, pos++;

            if( *txt1 == ' ' )   /* Make sure not to stutter a space after */
            {  /* the initial offset into the word */
               *txt++ = *txt1++;
               continue;
            }

            pos = 0;
            offset = number_range( 2, 4 );
            while( *txt1 != '\0' && pos < offset )
            {
               *txt++ = *txt1;
               pos++;
               if( *txt1 == ' ' || pos == offset ) /* Make sure we don't stick */
               {  /* A hyphen right before a space */
                  txt1--;
                  break;
               }
               *txt++ = '-';
            }
            if( *txt1 != '\0' )
               txt1++;
         }
      }
      else
         *txt++ = *txt1++;
   }

   *txt = '\0';

   return buf;
}

/*
 * An attempt to reduce redundancy                 - Tagith
 */
bool can_see_chan( CHAR_DATA * ch, DESCRIPTOR_DATA * d, int channel )
{
   CHAR_DATA *och;
   CHAR_DATA *vch;

   if( !d )
      return FALSE;  /* catch the link dead - shogar */
   och = CH( d );
   vch = d->character;

   if( IS_SET( och->deaf, channel ) )
      return FALSE;

   if( channel != CHANNEL_NEWBIE && NOT_AUTHED( och ) )
      return FALSE;
   if( channel == CHANNEL_IMMTALK && !IS_IMMORTAL( och ) )
      return FALSE;
   if( channel == CHANNEL_WARTALK && NOT_AUTHED( och ) )
      return FALSE;
   if( channel == CHANNEL_AVTALK && !IS_HERO( och ) )
      return FALSE;
   if( channel == CHANNEL_DEMI && !IS_HERO( och ) )
      if( channel == CHANNEL_HIGHGOD && get_trust( och ) < sysdata.muse_level )
         return FALSE;
   if( channel == CHANNEL_HIGH && get_trust( och ) < sysdata.think_level )
      return FALSE;

   /*
    * Fix by Narn to let newbie council members see the newbie channel. 
    */
   if( channel == CHANNEL_NEWBIE &&
       ( !IS_IMMORTAL( och ) && !NOT_AUTHED( och )
         && ( !str_cmp( och->pcdata->council_name, "Newbie Council" )
              || !str_cmp( och->pcdata->council2_name, "Newbie Council" ) ) ) )
      return FALSE;

   if( room_is_silent( vch ) )
      return FALSE;

   if( channel == CHANNEL_YELL && vch->in_room->area != ch->in_room->area )
      return FALSE;

   if( channel == CHANNEL_CLAN || channel == CHANNEL_ORDER || channel == CHANNEL_GUILD )
   {
      if( IS_NPC( vch ) )
         return FALSE;
      if( vch->pcdata->clan != ch->pcdata->clan )
         return FALSE;
   }

   /*
    * if ( channel == CHANNEL_GUILD )
    * {
    * if ( IS_NPC( vch ) )
    * return FALSE;
    * if (vch->pcdata->guild != ch->pcdata->guild )
    * return FALSE;
    * } 
    */

   if( channel == CHANNEL_COUNCIL )
   {
      if( IS_NPC( vch ) )
         return FALSE;
      if( ( vch->pcdata->council != ch->pcdata->council && vch->pcdata->council2 != ch->pcdata->council ) )
         return FALSE;
   }
   if( channel == CHANNEL_COUNCIL2 )
   {
      if( IS_NPC( vch ) )
         return FALSE;
      if( ( vch->pcdata->council != ch->pcdata->council2 && vch->pcdata->council2 != ch->pcdata->council2 ) )
         return FALSE;
   }
   return TRUE;
}

/*
 * Allows people to do socials, even targeted, over channels	   - Tagith
 */
void chan_social( CHAR_DATA * ch, char *argument, int channel, const char *verb )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char otherbuf[MAX_STRING_LENGTH];
   CHAR_DATA *victim = NULL;
   SOCIALTYPE *social;
   DESCRIPTOR_DATA *d;
   short AType;
   int position;
   char *txt;

   argument++;
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( ( social = find_social( arg1 ) ) == NULL )
   {
      send_to_char( "No such social.\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && xIS_SET( ch->act, PLR_NO_EMOTE ) )
   {
      send_to_char( "You are anti-social!\r\n", ch );
      return;
   }

   switch ( channel )
   {
      default:
         AType = AT_GOSSIP;
         break;
      case CHANNEL_IMMTALK:
      case CHANNEL_AVTALK:
         AType = AT_IMMORT;
         break;
      case CHANNEL_DEMI:
         AType = AT_DEMI;
         break;
      case CHANNEL_WARTALK:
         AType = AT_WARTALK;
         break;
   }

   otherbuf[0] = '\0';
   if( arg2[0] == '\0' )
   {
      position = ch->position;
      ch->position = POS_STANDING;
      sprintf( buf, "[%s] %s", verb, social->char_no_arg );
      act( AType, buf, ch, NULL, victim, TO_CHAR );
      ch->position = position;

      if( social->others_no_arg && social->others_no_arg[0] != '\0' )
         sprintf( otherbuf, "[%s] %s", verb, social->others_no_arg );
   }
   else if( ( victim = get_char_world( ch, arg2 ) ) == NULL || IS_NPC( victim ) )
      send_to_char( "They aren't here.\r\n", ch );
   else if( victim == ch )
   {
      sprintf( buf, "[%s] %s", verb, social->char_auto );
      position = ch->position;
      ch->position = POS_STANDING;
      act( AType, buf, ch, NULL, victim, TO_CHAR );
      ch->position = position;

      if( social->others_auto && social->others_auto[0] != '\0' )
         sprintf( otherbuf, "[%s] %s", verb, social->others_auto );
   }
   else
   {
      sprintf( buf, "[%s] %s", verb, social->char_found );
      position = ch->position;
      ch->position = POS_STANDING;
      act( AType, buf, ch, NULL, victim, TO_CHAR );
      ch->position = position;
      if( social->vict_found && social->vict_found[0] != '\0' && can_see_chan( ch, victim->desc, channel ) )
      {
         sprintf( buf, "[%s] %s", verb, social->vict_found );
         position = victim->position;
         victim->position = POS_STANDING;
         act( AType, buf, ch, NULL, victim, TO_VICT );
         victim->position = position;
      }

      if( social->others_found && social->others_found[0] != '\0' )
         sprintf( otherbuf, "[%s] %s", verb, social->others_found );
   }

   /*
    * An attempt at reducing redundancy 
    */
   if( !otherbuf || otherbuf[0] == '\0' )
      return;

   for( d = first_descriptor; d; d = d->next )
   {
      CHAR_DATA *vch = d->character;

      if( d->connected == CON_PLAYING && vch != ch && vch != victim && can_see_chan( ch, d, channel ) )
      {

         if( IS_IMMORTAL( vch ) )
            txt = act_string( otherbuf, vch, ch, NULL, victim, STRING_IMM );
         else
            txt = act_string( otherbuf, vch, ch, NULL, victim, STRING_NONE );

         set_char_color( AType, vch );
         send_to_char_color( txt, vch );
      }
      else
         continue;
   }
   return;
}

/*
 * Generic channel function.  Revamped by Justice of AaernMUD, based on SMAUG 1.02a.
 * August 3, 1998.  Removal of redundant code, and implementation of global socials.
 * August 25, 1998.  Added global emotes and help comments.
 * New talk_channel code originally written for GOTA
 * act_string prototype necessary for global social code.
 * October 7, 1998  Aaern came up yesterday, and I am now converting my talk_channel
 * code to SMAUG 1.4
 * March 19, 1999   Adding AaernMUD Talk_Channel code to UCMM --Justice
 */
/*
 * beep snippet installed by Scion 3/30/99 
 */
void do_beep( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   argument = one_argument( argument, arg );

   if( !*arg || !( victim = get_char_world( ch, arg ) ) )
   {
      send_to_char( "Beep who?\r\n", ch );
      return;
   }

   /*
    * NPC check added by Samson 2-15-98 
    */
   if( IS_NPC( victim ) )
   {
      send_to_char( "Beep who?\r\n", ch );
      return;
   }

   /*
    * PCFLAG_NOBEEP check added by Samson 2-15-98 
    */
   if( IS_SET( victim->pcdata->flags, PCFLAG_NOBEEP ) )
   {
      ch_printf( ch, "%s is not accepting beeps at this time.\r\n", victim->name );
      return;
   }

   if( argument[0] != '\0' )
   {
      ch_printf( victim, "&W^z%s beeps: %s^x\r\n", PERS( ch, victim ), argument );
      ch_printf( ch, "&WYou beep %s: %s\r\n", PERS( victim, ch ), argument );
   }
   else
   {
      ch_printf( victim, "&W^z%s beeps you from '%s'^x&w\a\r\n", PERS( ch, victim ), ch->in_room->name );
      ch_printf( ch, "&WYou beep %s.&w\r\n", PERS( victim, ch ) );
   }
   return;
}

void do_bug( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   struct tm *t = localtime( &current_time );

   set_char_color( AT_PLAIN, ch );
   if( argument[0] == '\0' )
   {
      send_to_char( "\r\nUsage:  'bug <message>'  (your location is automatically recorded)\r\n", ch );
      return;
   }
   if( memcmp( argument, "list\0", 5 ) == 0)
   {
      send_to_char( "\r\nProper form: 'fshow bugs'.  If you don't see the list, you're not a high enough level\r\n", ch );
      return;
   }
   sprintf( buf, "(%-2.2d/%-2.2d):  %s", t->tm_mon + 1, t->tm_mday, argument );
   append_file( ch, PBUG_FILE, buf );
   send_to_char( "Thanks, your bug notice has been recorded.\r\n", ch );
   return;
}

void do_ide( CHAR_DATA * ch, char *argument )
{
   set_char_color( AT_PLAIN, ch );
   send_to_char( "\r\nIf you want to send an idea, type 'idea <message>'.\r\n", ch );
   send_to_char( "If you want to identify an object, use the identify spell.\r\n", ch );
   return;
}

void do_idea( CHAR_DATA * ch, char *argument )
{
   set_char_color( AT_PLAIN, ch );
   if( argument[0] == '\0' )
   {
      send_to_char( "\r\nUsage:  'idea <message>'\r\n", ch );
      return;
   }
   append_file( ch, IDEA_FILE, argument );
   send_to_char( "Thanks, your idea has been recorded.\r\n", ch );
   return;
}

void do_typo( CHAR_DATA * ch, char *argument )
{
   set_char_color( AT_PLAIN, ch );
   if( argument[0] == '\0' )
   {
      send_to_char( "\r\nUsage:  'typo <message>'  (your location is automatically recorded)\r\n", ch );
      if( get_trust( ch ) >= LEVEL_ASCENDANT )
         send_to_char( "Usage:  'typo list' or 'typo clear now'\r\n", ch );
      return;
   }
   if( !str_cmp( argument, "clear now" ) && get_trust( ch ) >= LEVEL_ASCENDANT )
   {
      FILE *fp = fopen( TYPO_FILE, "w" );
      if( fp )
         fclose( fp );
      send_to_char( "Typo file cleared.\r\n", ch );
      return;
   }
   if( !str_cmp( argument, "list" ) )
   {
      send_to_char( "\r\n VNUM \r\n.......\r\n", ch );
      show_file( ch, TYPO_FILE );
   }
   else
   {
      append_file( ch, TYPO_FILE, argument );
      send_to_char( "Thanks, your typo notice has been recorded.\r\n", ch );
   }
   return;
}

void do_rent( CHAR_DATA * ch, char *argument )
{
   set_char_color( AT_WHITE, ch );
   send_to_char( "There is no rent here.  Just save and quit.\r\n", ch );
   return;
}



void do_qui( CHAR_DATA * ch, char *argument )
{
   set_char_color( AT_RED, ch );
   send_to_char( "If you want to QUIT, you have to spell it out.\r\n", ch );
   return;
}

void do_quit( CHAR_DATA * ch, char *argument )
{
   /*
    * OBJ_DATA *obj;   unused 
    */
   CHAR_DATA *vch;
   int x, y;
   int level;

   if( IS_NPC( ch ) )
      return;

   if( ch->position == POS_FIGHTING
       || ch->position == POS_EVASIVE
       || ch->position == POS_DEFENSIVE || ch->position == POS_AGGRESSIVE || ch->position == POS_BERSERK )
   {
      set_char_color( AT_RED, ch );
      send_to_char( "No way! You are fighting.\r\n", ch );
      return;
   }

   if( ch->position < POS_STUNNED )
   {
      set_char_color( AT_BLOOD, ch );
      send_to_char( "You're not DEAD yet.\r\n", ch );
      return;
   }

   if( get_timer( ch, TIMER_RECENTFIGHT ) > 0 && !IS_IMMORTAL( ch ) )
   {
      set_char_color( AT_RED, ch );
      send_to_char( "Your adrenaline is pumping too hard to quit now!\r\n", ch );
      return;
   }

   if( auction->item != NULL && ( ( ch == auction->buyer ) || ( ch == auction->seller ) ) )
   {
      send_to_char( "Wait until you have bought/sold the item on auction.\r\n", ch );
      return;

   }

   if( IS_PKILL( ch ) && ch->wimpy > ( int )ch->max_hit / 2.25 )
   {
      send_to_char( "Your wimpy has been adjusted to the maximum level for deadlies.\r\n", ch );
      do_wimpy( ch, "max" );
   }

   if( in_arena( ch ) )
   {  /* Scion arenacode */
      send_to_char( "Get out of the arena first!\r\n", ch );
      return;
   }

   /*
    * Make them lose artifact items. Can't leave the game with those! -- Scion 
    */
   drop_artifacts( ch, ch->last_carrying );

   /*
    * Get 'em dismounted until we finish mount saving -- Blodkai, 4/97 
    */
   if( ch->position == POS_MOUNTED )
      do_dismount( ch, "" );
   set_char_color( AT_WHITE, ch );
   send_to_char( "You are wracked with uncontrollable pain as you are extracted from the matrix.\r\n\r\n", ch );
   act( AT_SAY, "A strange voice says, 'We await your return, $n...'", ch, NULL, NULL, TO_CHAR );
   act( AT_BYE, "$n has escaped the realm.", ch, NULL, NULL, TO_CANSEE );
   set_char_color( AT_GREY, ch );

   sprintf( log_buf, "%s has quit (Room %d).", ch->name, ( ch->in_room ? ch->in_room->vnum : -1 ) );
   quitting_char = ch;
   save_char_obj( ch );

   if( sysdata.save_pets && ch->pcdata->pet )
   {
      act( AT_BYE, "$N follows $S master into the Void.", ch, NULL, ch->pcdata->pet, TO_ROOM );
      extract_char( ch->pcdata->pet, TRUE );
   }

   /*
    * Synch clandata up only when clan member quits now. --Shaddai
    */
   if( ch->pcdata->clan )
      save_clan( ch->pcdata->clan );

   if( ch->level >= 1 )
      /*
       * This should be coded to just do players.
       * * NPC's have no need to see it. And as you can see below, it'll
       * * just waste resources looping through character list and
       * * skipping npc's (can you imagine the cpu usage through 50+ areas?).
       */
      for( vch = first_char; vch; vch = vch->next )
      {
         if( IS_NPC( vch ) || vch == ch )
            continue;
         act( AT_GREEN, "&R[&WINFO&R]&c $n has escaped the realm!", ch, NULL, vch, TO_VICT );
/*	Don't really need this.. just annoys me :)   -- Scion */
      }

   saving_char = NULL;

   level = get_trust( ch );
   /*
    * After extract_char the ch is no longer valid!
    */
   extract_char( ch, TRUE );
   for( x = 0; x < MAX_WEAR; x++ )
      for( y = 0; y < MAX_LAYERS; y++ )
         save_equipment[x][y] = NULL;

   /*
    * don't show who's logging off to leaving player 
    */
/*
    to_channel( log_buf, CHANNEL_MONITOR, "Monitor", level ); 
*/
   log_string_plus( log_buf, LOG_COMM, level );
   return;
}

/* Made this a function so it could work recursively on containers.
   Make them drop artifact items. Can't leave the game with those! -- Scion */
void drop_artifacts( CHAR_DATA * ch, OBJ_DATA * obj )
{
   /*
    * Expecting a ch->last_carrying or an obj->last_content
    * ( We go BACKWARDS through the lists!) 
    */

   if( !obj )
      return;



   while( obj )
   {
      if( obj->last_content )
         drop_artifacts( ch, obj->last_content );

      if( IS_OBJ_STAT( obj, ITEM_ARTIFACT ) )
      {
         OBJ_DATA *tobj;

         tobj = obj;
         obj = obj->prev_content;

         if( tobj->in_obj )
            obj_from_obj( tobj );
         if( tobj->carried_by )
            obj_from_char( tobj );

         obj_to_room( tobj, ch->in_room );
         act( AT_MAGIC, "$p falls to the ground, drawn by the magic of these lands!", ch, tobj, NULL, TO_CHAR );
         act( AT_MAGIC, "$p falls from $n's hands, drawn by the magic of these lands!", ch, tobj, NULL, TO_CANSEE );
      }
      else
         obj = obj->prev_content;
   }
}

void send_rip_screen( CHAR_DATA * ch )
{
   FILE *rpfile;
   int num = 0;
   char BUFF[MAX_STRING_LENGTH * 2];

   if( ( rpfile = fopen( RIPSCREEN_FILE, "r" ) ) != NULL )
   {
      while( ( BUFF[num] = fgetc( rpfile ) ) != EOF )
         num++;
      fclose( rpfile );
      BUFF[num] = 0;
      write_to_buffer( ch->desc, BUFF, num );
   }
}

void send_rip_title( CHAR_DATA * ch )
{
   FILE *rpfile;
   int num = 0;
   char BUFF[MAX_STRING_LENGTH * 2];

   if( ( rpfile = fopen( RIPTITLE_FILE, "r" ) ) != NULL )
   {
      while( ( BUFF[num] = fgetc( rpfile ) ) != EOF )
         num++;
      fclose( rpfile );
      BUFF[num] = 0;
      write_to_buffer( ch->desc, BUFF, num );
   }
}

void send_ansi_title( CHAR_DATA * ch )
{
   FILE *rpfile;
   int num = 0;
   char BUFF[MAX_STRING_LENGTH * 2];

   if( ( rpfile = fopen( ANSITITLE_FILE, "r" ) ) != NULL )
   {
      while( ( BUFF[num] = fgetc( rpfile ) ) != EOF )
         num++;
      fclose( rpfile );
      BUFF[num] = 0;
      write_to_buffer( ch->desc, BUFF, num );
   }
}

void send_ascii_title( CHAR_DATA * ch )
{
   FILE *rpfile;
   int num = 0;
   char BUFF[MAX_STRING_LENGTH];

   if( ( rpfile = fopen( ASCTITLE_FILE, "r" ) ) != NULL )
   {
      while( ( BUFF[num] = fgetc( rpfile ) ) != EOF )
         num++;
      fclose( rpfile );
      BUFF[num] = 0;
      write_to_buffer( ch->desc, BUFF, num );
   }
}

void do_rip( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Rip ON or OFF?\r\n", ch );
      return;
   }
   if( ( strcmp( arg, "on" ) == 0 ) || ( strcmp( arg, "ON" ) == 0 ) )
   {
      send_rip_screen( ch );
      xSET_BIT( ch->act, PLR_RIP );
      xSET_BIT( ch->act, PLR_ANSI );
      return;
   }

   if( ( strcmp( arg, "off" ) == 0 ) || ( strcmp( arg, "OFF" ) == 0 ) )
   {
      xREMOVE_BIT( ch->act, PLR_RIP );
      send_to_char( "!|*\r\nRIP now off...\r\n", ch );
      return;
   }
}

void do_ansi( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "ANSI ON or OFF?\r\n", ch );
      return;
   }
   if( ( strcmp( arg, "on" ) == 0 ) || ( strcmp( arg, "ON" ) == 0 ) )
   {
      xSET_BIT( ch->act, PLR_ANSI );
      set_char_color( AT_WHITE + AT_BLINK, ch );
      send_to_char( "Ansi On, Way Cool!!!\r\n", ch );
      return;
   }

   if( ( strcmp( arg, "off" ) == 0 ) || ( strcmp( arg, "OFF" ) == 0 ) )
   {
      xREMOVE_BIT( ch->act, PLR_ANSI );
      send_to_char( "Okay... ANSI support is now off.\r\n", ch );
      return;
   }
}

void do_save( CHAR_DATA * ch, char *argument )
{
   if( IS_NPC( ch ) )
      return;
   if( ch->level < 2 && ch->level2 == -1 && ch->level3 == -1 )
   {
      send_to_char_color( "&BYou must be at least second level to save.\r\n", ch );
      return;
   }
   WAIT_STATE( ch, 2 ); /* For big muds with save-happy players, like RoD */
   update_aris( ch );   /* update char affects and RIS */
   save_char_obj( ch );
   saving_char = NULL;
   send_to_char( "Saved...\r\n", ch );
   return;
}


/*
 * Something from original DikuMUD that Merc yanked out.
 * Used to prevent following loops, which can cause problems if people
 * follow in a loop through an exit leading back into the same room
 * (Which exists in many maze areas)			-Thoric
 */
bool circle_follow( CHAR_DATA * ch, CHAR_DATA * victim )
{
   CHAR_DATA *tmp;

   for( tmp = victim; tmp; tmp = tmp->master )
      if( tmp == ch )
         return TRUE;
   return FALSE;
}


void do_dismiss( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Dismiss whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( ( IS_AFFECTED( victim, AFF_CHARM ) ) && ( IS_NPC( victim ) ) && ( victim->master == ch ) )
   {
      stop_follower( victim );
      stop_hating( victim );
      stop_hunting( victim );
      stop_fearing( victim );
      act( AT_ACTION, "$n dismisses $N.", ch, NULL, victim, TO_NOTVICT );
      act( AT_ACTION, "You dismiss $N.", ch, NULL, victim, TO_CHAR );
   }
   else
   {
      send_to_char( "You cannot dismiss them.\r\n", ch );
   }

   return;
}

void do_follow( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Follow whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_AFFECTED( ch, AFF_CHARM ) && ch->master )
   {
      act( AT_PLAIN, "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
      return;
   }

   if( victim == ch )
   {
      if( !ch->master )
      {
         send_to_char( "You already follow yourself.\r\n", ch );
         return;
      }
      stop_follower( ch );
      return;
   }
/*
    if ( ( ch->level - victim->level < -10 || ch->level - victim->level >  10 )
    &&   !IS_HERO(ch) && !(ch->level < 15 && !IS_NPC(victim) 
    && victim->pcdata->council 
    && !str_cmp(victim->pcdata->council->name,"Newbie Council")))
    {
	send_to_char( "You are not of the right caliber to follow.\r\n", ch );
	return;
    }
-- Screw this, level restrictions on following are stupid. -- Scion */
   if( circle_follow( ch, victim ) )
   {
      stop_follower( victim );
      return;
   }

   if( ch->master )
      stop_follower( ch );

   add_follower( ch, victim );
   return;
}



void add_follower( CHAR_DATA * ch, CHAR_DATA * master )
{
   if( ch->master )
   {
      bug( "Add_follower: non-null master.", 0 );
      return;
   }

   ch->master = master;
   ch->leader = NULL;

   /*
    * Support for saving pets --Shaddai 
    */
   if( IS_NPC( ch ) && xIS_SET( ch->act, ACT_PET ) && !IS_NPC( master ) )
      master->pcdata->pet = ch;

   if( can_see( master, ch ) )
      act( AT_ACTION, "$n now follows you.", ch, NULL, master, TO_VICT );

   act( AT_ACTION, "You now follow $N.", ch, NULL, master, TO_CHAR );

   return;
}



void stop_follower( CHAR_DATA * ch )
{
   if( !ch->master )
   {
      bug( "Stop_follower: null master.", 0 );
      return;
   }

   if( IS_NPC( ch ) && !IS_NPC( ch->master ) && ch->master->pcdata->pet == ch )
      ch->master->pcdata->pet = NULL;

   if( IS_AFFECTED( ch, AFF_CHARM ) )
   {
      xREMOVE_BIT( ch->affected_by, AFF_CHARM );
      affect_strip( ch, gsn_charm_person );
   }

   if( can_see( ch->master, ch ) )
      if( !( !IS_NPC( ch->master ) && IS_IMMORTAL( ch ) && !IS_IMMORTAL( ch->master ) ) )
         act( AT_ACTION, "$n stops following you.", ch, NULL, ch->master, TO_VICT );
   act( AT_ACTION, "You stop following $N.", ch, NULL, ch->master, TO_CHAR );

   ch->master = NULL;
   ch->leader = NULL;
   return;
}



void die_follower( CHAR_DATA * ch )
{
   CHAR_DATA *fch;

   if( ch->master )
      stop_follower( ch );

   ch->leader = NULL;

   for( fch = first_char; fch; fch = fch->next )
   {
      if( fch->master == ch )
         stop_follower( fch );
      if( fch->leader == ch )
         fch->leader = fch;
   }
   return;
}



void do_order( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char argbuf[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   CHAR_DATA *och;
   CHAR_DATA *och_next;
   bool found;
   bool fAll;

   strcpy( argbuf, argument );
   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "Order whom to do what?\r\n", ch );
      return;
   }

   if( IS_AFFECTED( ch, AFF_CHARM ) )
   {
      send_to_char( "You feel like taking, not giving, orders.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "all" ) )
   {
      fAll = TRUE;
      victim = NULL;
   }
   else
   {
      fAll = FALSE;
      if( ( victim = get_char_room( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\r\n", ch );
         return;
      }

      if( victim == ch )
      {
         send_to_char( "Aye aye, right away!\r\n", ch );
         return;
      }

      if( !IS_AFFECTED( victim, AFF_CHARM ) || victim->master != ch )
      {
         send_to_char( "Do it yourself!\r\n", ch );
         return;
      }
   }

   found = FALSE;
   for( och = ch->in_room->first_person; och; och = och_next )
   {
      och_next = och->next_in_room;

      if( IS_AFFECTED( och, AFF_CHARM ) && och->master == ch && ( fAll || och == victim ) )
      {
         found = TRUE;
         act( AT_ACTION, "$n orders you to '$t'.", ch, argument, och, TO_VICT );
         interpret( och, argument );
      }
   }

   if( found )
   {
      sprintf( log_buf, "%s: order %s.", ch->name, argbuf );
      log_string_plus( log_buf, LOG_NORMAL, ch->level );
      send_to_char( "Ok.\r\n", ch );
      WAIT_STATE( ch, 12 );
   }
   else
      send_to_char( "You have no followers here.\r\n", ch );
   return;
}

/*
char *itoa(int foo)
{
  static char bar[256];

  sprintf(bar,"%d",foo);
  return(bar);

}
*/

/* Overhauled 2/97 -- Blodkai */
void do_group( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      CHAR_DATA *gch;
      CHAR_DATA *leader;

      leader = ch->leader ? ch->leader : ch;
      set_char_color( AT_DGREEN, ch );
      ch_printf( ch, "\r\nFollowing %-12.12s     [hitpnts]   [ magic ] [mst] [mvs] [race]%s\r\n",
                 PERS( leader, ch ), ch->level < LEVEL_AVATAR ? " [to lvl]" : "" );
      for( gch = first_char; gch; gch = gch->next )
      {
         if( is_same_group( gch, ch ) )
         {
            set_char_color( AT_DGREEN, ch ); /*
                                              * if (IS_AFFECTED(gch, AFF_POSSESS) || IS_SET( ch->pcdata->flags, PCFLAG_ANONYMOUS))
                                              * ch_printf( ch,
                                              * "[%2d %s] %-16s ????/???? hp ????/???? %s ????/???? mv ????? xp\r\n",
                                              * gch->level,
                                              * IS_NPC(gch) ? "Mob" : CLASS_NAME(gch),
                                              * capitalize( PERS(gch, ch) ),
                                              * IS_VAMPIRE(gch) ? "bp" : "mana" );
                                              * else */
            if( gch->alignment > 750 )
               sprintf( buf, " A" );
            else if( gch->alignment > 350 )
               sprintf( buf, "-A" );
            else if( gch->alignment > 150 )
               sprintf( buf, "+N" );
            else if( gch->alignment > -150 )
               sprintf( buf, " N" );
            else if( gch->alignment > -350 )
               sprintf( buf, "-N" );
            else if( gch->alignment > -750 )
               sprintf( buf, "+S" );
            else
               sprintf( buf, " S" );
            set_char_color( AT_DGREEN, ch );
            send_to_char( "[", ch );
            set_char_color( AT_GREEN, ch );
            ch_printf( ch, "%-2d %2.2s %3.3s", gch->level, buf,
                       /*
                        * IS_NPC(gch) ? "Mob" : class_table[gch->class]->who_name 
                        */
                       IS_NPC( gch ) ? "Mob" : CLASS_NAME( gch ) );
            set_char_color( AT_DGREEN, ch );
            send_to_char( "]  ", ch );
            set_char_color( AT_GREEN, ch );
            ch_printf( ch, "%-12.12s ", capitalize( PERS( gch, ch ) ) );
            if( gch->hit < gch->max_hit / 4 )
               set_char_color( AT_DANGER, ch );
            else if( gch->hit < gch->max_hit / 2.5 )
               set_char_color( AT_YELLOW, ch );
            else
               set_char_color( AT_GREY, ch );
            ch_printf( ch, "%5d", gch->hit );
            set_char_color( AT_GREY, ch );
            ch_printf( ch, "/%-5d ", gch->max_hit );
            if( IS_VAMPIRE( gch ) )
               set_char_color( AT_BLOOD, ch );
            else
               set_char_color( AT_LBLUE, ch );
            if( !is_class( gch, CLASS_WARRIOR ) )
               ch_printf( ch, "%5d/%-5d ",
                          IS_VAMPIRE( gch ) ? gch->pcdata->condition[COND_BLOODTHIRST] : gch->mana,
                          IS_VAMPIRE( gch ) ? 10 + gch->level : gch->max_mana );
            else
               send_to_char( "            ", ch );
            if( gch->mental_state < -25 || gch->mental_state > 25 )
               set_char_color( AT_YELLOW, ch );
            else
               set_char_color( AT_GREEN, ch );
            ch_printf( ch, "%3.3s  ",
                       gch->mental_state > 75 ? "+++" :
                       gch->mental_state > 50 ? "=++" :
                       gch->mental_state > 25 ? "==+" :
                       gch->mental_state > -25 ? "===" :
                       gch->mental_state > -50 ? "-==" : gch->mental_state > -75 ? "--=" : "---" );
            set_char_color( AT_DGREEN, ch );
            ch_printf( ch, "%5d ", gch->move );
            ch_printf( ch, "%6s ", npc_race[gch->race] );
            set_char_color( AT_GREEN, ch );
            if( gch->level < LEVEL_AVATAR )
               ch_printf( ch, "%8d ", exp_level( gch, gch->level + 1 ) - gch->exp );
            send_to_char( "\r\n", ch );
         }
      }
      return;
   }

   if( !strcmp( arg, "disband" ) )
   {
      CHAR_DATA *gch;
      int count = 0;

      if( ch->leader || ch->master )
      {
         send_to_char( "You cannot disband a group if you're following someone.\r\n", ch );
         return;
      }

      for( gch = first_char; gch; gch = gch->next )
      {
         if( is_same_group( ch, gch ) && ( ch != gch ) )
         {
            gch->leader = NULL;
            gch->master = NULL;
            count++;
            send_to_char( "Your group is disbanded.\r\n", gch );
         }
      }

      if( count == 0 )
         send_to_char( "You have no group members to disband.\r\n", ch );
      else
         send_to_char( "You disband your group.\r\n", ch );

      return;
   }

   if( !strcmp( arg, "all" ) )
   {
      CHAR_DATA *rch;
      int count = 0;

      for( rch = ch->in_room->first_person; rch; rch = rch->next_in_room )
      {
         if( ch != rch && !IS_NPC( rch ) && can_see( ch, rch ) && rch->master == ch && !ch->master && !ch->leader
/*    	   &&   abs( ch->level - rch->level ) < 8  Level restrict on groups is stupid -- Scion */
             && !is_same_group( rch, ch ) && IS_PKILL( ch ) == IS_PKILL( rch ) )
         {
            rch->leader = ch;
            count++;
         }
      }

      if( count == 0 )
         send_to_char( "You have no eligible group members.\r\n", ch );
      else
      {
         act( AT_ACTION, "$n groups $s followers.", ch, NULL, NULL, TO_ROOM );
         send_to_char( "You group your followers.\r\n", ch );
      }
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( ch->master || ( ch->leader && ch->leader != ch ) )
   {
      send_to_char( "But you are following someone else!\r\n", ch );
      return;
   }

   if( victim->master != ch && ch != victim )
   {
      act( AT_PLAIN, "$N isn't following you.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( victim == ch )
   {
      act( AT_PLAIN, "You can't group yourself.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( is_same_group( victim, ch ) && ch != victim )
   {
      victim->leader = NULL;
      act( AT_ACTION, "$n removes $N from $s group.", ch, NULL, victim, TO_NOTVICT );
      act( AT_ACTION, "$n removes you from $s group.", ch, NULL, victim, TO_VICT );
      act( AT_ACTION, "You remove $N from your group.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( ch->level - victim->level < -8 || ch->level - victim->level > 8 || ( IS_PKILL( ch ) != IS_PKILL( victim ) ) )
   {
      act( AT_PLAIN, "$N cannot join $n's group.", ch, NULL, victim, TO_NOTVICT );
      act( AT_PLAIN, "You cannot join $n's group.", ch, NULL, victim, TO_VICT );
      act( AT_PLAIN, "$N cannot join your group.", ch, NULL, victim, TO_CHAR );
      return;
   }

   victim->leader = ch;
   act( AT_ACTION, "$N joins $n's group.", ch, NULL, victim, TO_NOTVICT );
   act( AT_ACTION, "You join $n's group.", ch, NULL, victim, TO_VICT );
   act( AT_ACTION, "$N joins your group.", ch, NULL, victim, TO_CHAR );
   return;
}



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *gch;
   int members;
   int amount;
   int share;
   int extra;
   int type;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );


   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Split <Amount> <Coin Type>\r\n", ch );
      send_to_char( "\r\nValid coin types: Gold, Silver, Copper\r\n", ch );
      return;
   }

   /*
    * type 0 = gold
    * type 1 = silver
    * type 2 = copper
    */
   if( !str_cmp( "gold", arg2 ) )
      type = 0;
   else if( !str_cmp( "silver", arg2 ) )
      type = 1;
   else if( !str_cmp( "silv", arg2 ) )
      type = 1;
   else if( !str_cmp( "copper", arg2 ) )
      type = 2;
   else if( !str_cmp( "cop", arg2 ) )
      type = 2;
   else
   {
      sprintf( buf, "%s is not a valid coin type.\r\n", arg2 );
      send_to_char( buf, ch );
      return;
   }
   amount = atoi( arg1 );

   if( amount < 0 )
   {
      send_to_char( "Your group wouldn't like that.\r\n", ch );
      return;
   }

   if( amount == 0 )
   {
      send_to_char( "You hand out zero coins, but no one notices.\r\n", ch );
      return;
   }
   /*
    * Check for amount of Gold/Silver/Copper -Druid
    */
   if( type == 0 )
      if( ch->gold < amount )
      {
         send_to_char( "You don't have that much gold.\r\n", ch );
         return;
      }
   if( type == 1 )
      if( ch->silver < amount )
      {
         send_to_char( "You don't have that much silver.\r\n", ch );
         return;
      }
   if( type == 2 )
      if( ch->copper < amount )
      {
         send_to_char( "You don't have that much copper.\r\n", ch );
         return;
      }

   if( type < 0 || type > 3 )
   {
      send_to_char( "Invalid coin type, please try again\r\n", ch );
      bug( "Invalid coin type: in do_split", 0 );
      return;
   }
   members = 0;
   for( gch = ch->in_room->first_person; gch; gch = gch->next_in_room )
   {
      if( is_same_group( gch, ch ) )
         members++;
   }

   if( xIS_SET( ch->act, PLR_AUTOGOLD ) && members < 2 )
      return;

   if( members < 2 )
   {
      send_to_char( "Just keep it all.\r\n", ch );
      return;
   }

   share = amount / members;
   extra = amount % members;

   if( share == 0 )
   {
      send_to_char( "Don't even bother, cheapskate.\r\n", ch );
      return;
   }
   if( type == 0 )
   {
      ch->gold -= amount;
      ch->gold += share + extra;
   }
   if( type == 1 )
   {
      ch->silver -= amount;
      ch->silver += share + extra;
   }
   if( type == 2 )
   {
      ch->copper -= amount;
      ch->copper += share + extra;
   }
   set_char_color( AT_GOLD, ch );
   if( type == 0 )
   {
      ch_printf( ch, "You split %d gold coins.  Your share is %d gold coins.\r\n", amount, share + extra );

      sprintf( buf, "$n splits %d gold coins.  Your share is %d gold coins.", amount, share );
   }
   if( type == 1 )
   {
      ch_printf( ch, "You split %d silver coins.  Your share is %d silver coins.\r\n", amount, share + extra );

      sprintf( buf, "$n splits %d silver coins.  Your share is %d silver coins.", amount, share );
   }
   if( type == 2 )
   {
      ch_printf( ch, "You split %d copper coins.  Your share is %d copper coins.\r\n", amount, share + extra );

      sprintf( buf, "$n splits %d copper coins.  Your share is %d copper coins.", amount, share );
   }
   for( gch = ch->in_room->first_person; gch; gch = gch->next_in_room )
   {
      if( gch != ch && is_same_group( gch, ch ) )
      {
         act( AT_GOLD, buf, ch, NULL, gch, TO_VICT );
         if( type == 0 )
            gch->gold += share;
         if( type == 1 )
            gch->silver += share;
         if( type == 2 )
            gch->copper += share;
      }
   }
   return;
}


void do_gtell( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *gch;
#ifndef SCRAMBLE
   int speaking = -1, lang;

   for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      if( ch->speaking & lang_array[lang] )
      {
         speaking = lang;
         break;
      }
#endif

   if( argument[0] == '\0' )
   {
      send_to_char( "Tell your group what?\r\n", ch );
      return;
   }

   if( xIS_SET( ch->act, PLR_NO_TELL ) )
   {
      send_to_char( "Your message didn't get through!\r\n", ch );
      return;
   }

   /*
    * Note use of send_to_char, so gtell works on sleepers.
    */
/*    sprintf( buf, "%s tells the group '%s'.\r\n", ch->name, argument );*/
   for( gch = first_char; gch; gch = gch->next )
   {
      if( is_same_group( gch, ch ) )
      {
         set_char_color( AT_GTELL, gch );
         /*
          * Groups unscrambled regardless of clan language.  Other languages
          * still garble though. -- Altrag 
          */
#ifndef SCRAMBLE
         if( speaking != -1 && ( !IS_NPC( ch ) || ch->speaking ) )
         {
            int speakswell = UMIN( knows_language( gch, ch->speaking ),
                                   knows_language( ch, ch->speaking ) );

            if( speakswell < 85 )
               ch_printf( gch, "%s tells the group '%s'.\r\n", ch->name,
                          translate( speakswell, argument, lang_names[speaking] ) );
            else
               ch_printf( gch, "%s tells the group '%s'.\r\n", ch->name, argument );
         }
         else
            ch_printf( gch, "%s tells the group '%s'.\r\n", ch->name, argument );
#else
         if( knows_language( gch, ch->speaking ) || ( IS_NPC( ch ) && !ch->speaking ) )
            ch_printf( gch, "%s tells the group '%s'.\r\n", ch->name, argument );
         else
            ch_printf( gch, "%s tells the group '%s'.\r\n", ch->name, scramble( argument, ch->speaking ) );
#endif
      }
   }
   return;
}

/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA * ach, CHAR_DATA * bch )
{
   if( ach->leader )
      ach = ach->leader;
   if( bch->leader )
      bch = bch->leader;
   return ach == bch;
}

/*
 * this function sends raw argument over the AUCTION: channel
 * I am not too sure if this method is right..
 */

void talk_auction( char *argument )
{
   DESCRIPTOR_DATA *d;
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *original;

   sprintf( buf, "&W[&GAuction&W]&G %s", argument );  /* last %s to reset color */

   for( d = first_descriptor; d; d = d->next )
   {
      original = d->original ? d->original : d->character;  /* if switched */
      if( ( d->connected == CON_PLAYING ) && !IS_SET( original->deaf, CHANNEL_AUCTION )
          && !IS_SET( original->in_room->room_flags, ROOM_SILENCE ) && !NOT_AUTHED( original ) )
         act( AT_GOSSIP, buf, original, NULL, NULL, TO_CHAR );
   }
}

/*
 * Language support functions. -- Altrag
 * 07/01/96
 *
 * Modified to return how well the language is known 04/04/98 - Thoric
 * Currently returns 100% for known languages... but should really return
 * a number based on player's wisdom (maybe 50+((25-wisdom)*2) ?)
 */
int knows_language( CHAR_DATA * ch, int language )
{
   short sn;

   if( !IS_NPC( ch ) && IS_IMMORTAL( ch ) )
      return 100;
   if( IS_NPC( ch ) && !ch->speaks )   /* No langs = knows nothing for npcs */
      return 0;
   if( IS_NPC( ch ) && IS_SET( ch->speaks, ( language & ~LANG_CLAN ) ) )
      return ( 50 + ( 25 - ch->mod_int ) * 2 );
   if( !IS_NPC( ch ) )
   {
      int lang;

      /*
       * Racial languages for PCs 
       */
      if( IS_SET( race_table[ch->race]->language, language ) )
         return ( ( 65 + ( 25 - ch->mod_int ) * 2 ) <= 100 ? ( 65 + ( 25 - ch->mod_int ) * 2 ) : 100 );

      for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
         if( IS_SET( language, lang_array[lang] ) && IS_SET( ch->speaks, lang_array[lang] ) )
         {
            if( ( sn = skill_lookup( lang_names[lang] ) ) != -1 )
               return ch->pcdata->learned[sn];
         }
   }
   return 0;
}

bool can_learn_lang( CHAR_DATA * ch, int language )
{
   if( language & LANG_CLAN )
      return FALSE;
   if( IS_NPC( ch ) || IS_IMMORTAL( ch ) )
      return FALSE;
   if( race_table[ch->race]->language & language )
      return FALSE;
   if( ch->speaks & language )
   {
      int lang;

      for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
         if( language & lang_array[lang] )
         {
            int sn;

            if( !( VALID_LANGS & lang_array[lang] ) )
               return FALSE;
            if( ( sn = skill_lookup( lang_names[lang] ) ) < 0 )
            {
               bug( "Can_learn_lang: valid language without sn: %d", lang );
               continue;
            }
            if( ch->pcdata->learned[sn] >= 99 )
               return FALSE;
         }
   }
   if( VALID_LANGS & language )
      return TRUE;
   return FALSE;
}

int const lang_array[] = {
   LANG_COMMON, LANG_ELVEN, LANG_DWARVEN, LANG_PIXIE,
   LANG_OGRE, LANG_ORCISH, LANG_TROLLISH, LANG_RODENT,
   LANG_INSECTOID, LANG_MAMMAL, LANG_REPTILE,
   LANG_DRAGON, LANG_SPIRITUAL, LANG_MAGICAL,
   LANG_GOBLIN, LANG_GOD, LANG_ANCIENT, LANG_HALFLING,
   LANG_CLAN, LANG_GITH, LANG_GNOME, LANG_UNKNOWN
};

char *const lang_names[] = {
   "common", "elvish", "dwarven", "pixie", "ogre",
   "orcish", "trollese", "rodent", "insectoid",
   "mammal", "reptile", "dragon", "spiritual",
   "magical", "goblin", "god", "ancient",
   "halfling", "clan", "gith", "gnome", ""
};


/* Note: does not count racial language.  This is intentional (for now). */
int countlangs( int languages )
{
   int numlangs = 0;
   int looper;

   for( looper = 0; lang_array[looper] != LANG_UNKNOWN; looper++ )
   {
      if( lang_array[looper] == LANG_CLAN )
         continue;
      if( languages & lang_array[looper] )
         numlangs++;
   }
   return numlangs;
}

void do_speak( CHAR_DATA * ch, char *argument )
{
   int langs;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if( !str_cmp( arg, "all" ) && IS_IMMORTAL( ch ) )
   {
      set_char_color( AT_SAY, ch );
      ch->speaking = ~LANG_CLAN;
      send_to_char( "Now speaking all languages.\r\n", ch );
      return;
   }
   for( langs = 0; lang_array[langs] != LANG_UNKNOWN; langs++ )
      if( !str_prefix( arg, lang_names[langs] ) )
         if( knows_language( ch, lang_array[langs] ) )
         {
            if( lang_array[langs] == LANG_CLAN && ( IS_NPC( ch ) || !ch->pcdata->clan ) )
               continue;
            ch->speaking = lang_array[langs];
            set_char_color( AT_SAY, ch );
            ch_printf( ch, "You now speak %s.\r\n", lang_names[langs] );
            return;
         }
   set_char_color( AT_SAY, ch );
   send_to_char( "You do not know that language.\r\n", ch );
}

void do_languages( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int lang;

   argument = one_argument( argument, arg );
   if( arg[0] != '\0' && !str_prefix( arg, "learn" ) && !IS_IMMORTAL( ch ) && !IS_NPC( ch ) )
   {
      CHAR_DATA *sch;
      char arg2[MAX_INPUT_LENGTH];
      int sn;
      int prct;
      int prac;

      argument = one_argument( argument, arg2 );
      if( arg2[0] == '\0' )
      {
         send_to_char( "Learn which language?\r\n", ch );
         return;
      }
      for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      {
         if( lang_array[lang] == LANG_CLAN )
            continue;
         if( !str_prefix( arg2, lang_names[lang] ) )
            break;
      }
      if( lang_array[lang] == LANG_UNKNOWN )
      {
         send_to_char( "That is not a language.\r\n", ch );
         return;
      }
      if( !( VALID_LANGS & lang_array[lang] ) )
      {
         send_to_char( "You may not learn that language.\r\n", ch );
         return;
      }
      if( ( sn = skill_lookup( lang_names[lang] ) ) < 0 )
      {
         send_to_char( "That is not a language.\r\n", ch );
         return;
      }
      if( race_table[ch->race]->language & lang_array[lang] ||
          lang_array[lang] == LANG_COMMON || ch->pcdata->learned[sn] >= 99 )
      {
         act( AT_PLAIN, "You are already fluent in $t.", ch, lang_names[lang], NULL, TO_CHAR );
         return;
      }
      for( sch = ch->in_room->first_person; sch; sch = sch->next_in_room )
         if( IS_NPC( sch ) && xIS_SET( sch->act, ACT_SCHOLAR )
             && knows_language( sch, ch->speaking )
             && knows_language( sch, lang_array[lang] )
             && ( !sch->speaking || knows_language( ch, sch->speaking ) ) )
            break;
      if( !sch )
      {
         send_to_char( "There is no one who can teach that language here.\r\n", ch );
         return;
      }
      if( countlangs( ch->speaks ) >= ( ch->level / 10 ) && ch->pcdata->learned[sn] <= 0 )
      {
         act( AT_TELL, "$n tells you 'You may not learn a new language yet.'", sch, NULL, ch, TO_VICT );
         return;
      }
      /*
       * 0..16 cha = 2 pracs, 17..25 = 1 prac. -- Altrag 
       */
      prac = 2 - ( get_curr_cha( ch ) / 17 );
      if( ch->practice < prac )
      {
         act( AT_TELL, "$n tells you 'You do not have enough practices.'", sch, NULL, ch, TO_VICT );
         return;
      }
      ch->practice -= prac;
      /*
       * Max 12% (5 + 4 + 3) at 24+ int and 21+ wis. -- Altrag 
       */
      prct = 5 + ( get_curr_int( ch ) / 6 ) + ( get_curr_wis( ch ) / 7 );
      ch->pcdata->learned[sn] += prct;
      ch->pcdata->learned[sn] = UMIN( ch->pcdata->learned[sn], 99 );
      SET_BIT( ch->speaks, lang_array[lang] );
      if( ch->pcdata->learned[sn] == prct )
         act( AT_PLAIN, "You begin lessons in $t.", ch, lang_names[lang], NULL, TO_CHAR );
      else if( ch->pcdata->learned[sn] < 60 )
         act( AT_PLAIN, "You continue lessons in $t.", ch, lang_names[lang], NULL, TO_CHAR );
      else if( ch->pcdata->learned[sn] < 60 + prct )
         act( AT_PLAIN, "You feel you can start communicating in $t.", ch, lang_names[lang], NULL, TO_CHAR );
      else if( ch->pcdata->learned[sn] < 99 )
         act( AT_PLAIN, "You become more fluent in $t.", ch, lang_names[lang], NULL, TO_CHAR );
      else
         act( AT_PLAIN, "You now speak perfect $t.", ch, lang_names[lang], NULL, TO_CHAR );
      return;
   }
   for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      if( knows_language( ch, lang_array[lang] ) )
      {
         if( ch->speaking & lang_array[lang] || ( IS_NPC( ch ) && !ch->speaking ) )
            set_char_color( AT_SAY, ch );
         else
            set_char_color( AT_PLAIN, ch );
         send_to_char( lang_names[lang], ch );
         send_to_char( "\r\n", ch );
      }
   send_to_char( "\r\n", ch );
   return;
}

/*
 * Backup allows player to back up their own pfiles.
 * Saves imms alot of grief on losses due to crashing. - Garinan
 */
void do_backup( CHAR_DATA * ch, char *argument )
{
   char save[MIL];
   char back[MIL];

   if( IS_NPC( ch ) )
      return;
   if( ch->level < 10 && ch->level2 == -1 && ch->level3 == -1 )
   {
      send_to_char( "You must be level 10 to backup your file.\r\n", ch );
      return;
   }
   sprintf( save, "%s%c/%s", PLAYER_DIR, tolower( ch->pcdata->filename[0] ), capitalize( ch->pcdata->filename ) );
   sprintf( back, "%s%c/%s", BACKUP_DIR, tolower( ch->pcdata->filename[0] ), capitalize( ch->pcdata->filename ) );
   rename( save, back );
   send_to_char( "Backup created.\r\n", ch );
   do_save( ch, NULL );
   return;
}

/*
 * Immortal restoration of backup. -Garinan
 */
void do_retrieve( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   char save[MIL];
   char back[MIL];
   char arg[MIL];

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Retrieve whose file?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) != NULL )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "The player must be offline to do this!\r\n", ch );
         return;
      }
   }
   else
   {
      sprintf( save, "%s%c/%s", PLAYER_DIR, tolower( arg[0] ), capitalize( arg ) );
      sprintf( back, "%s%c/%s", BACKUP_DIR, tolower( arg[0] ), capitalize( arg ) );
      rename( back, save );
      if( !rename( back, save ) )
         bug( "do_retrieve: Backup file not found!\r\n" );
      else
         send_to_char( "Backup restored.\r\n", ch );
      return;
   }
}

/*
 * Language on public channels -Garinan
 */
bool check_cuss( char *cuss )
{
   FILTER_DATA *flt;
   int match = 0, matchnum;
   size_t i;

   for( flt = first_filtered; flt; flt = flt->next )
   {
      matchnum = strlen( flt->name );
      for( i = 0; i <= strlen( cuss ); i++ )
      {
         if( match == matchnum )
            return TRUE;

         if( toupper( cuss[i] ) == toupper( flt->name[match] ) )
            match++;
         else
            match = 0;
      }
   }
   return FALSE;
}

FILTER_DATA *first_filtered;
FILTER_DATA *last_filtered;
void save_filtered( void )
{
   FILTER_DATA *flt;
   FILE *fp;

   if( !( fp = fopen( SYSTEM_DIR FILTERED_LIST, "w" ) ) )
   {
      bug( "Save_filtered: cannot open " FILTERED_LIST, 0 );
      perror( FILTERED_LIST );
      return;
   }
   for( flt = first_filtered; flt; flt = flt->next )
      fprintf( fp, "%s~\n", flt->name );
   fprintf( fp, "$~\n" );
   fclose( fp );
   fp = NULL;
   return;
}

void do_wfilter( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   FILTER_DATA *flt;

   set_char_color( AT_PLAIN, ch );

   argument = one_argument( argument, arg );
   if( !*arg )
   {
      int wid = 0;

      send_to_char( "&WFiltered Words ----&w\r\n", ch );
      for( flt = first_filtered; flt; flt = flt->next )
      {
         ch_printf( ch, "%c%-17s ", ( *flt->name == '*' ? '*' : ' ' ), ( *flt->name == '*' ? flt->name + 1 : flt->name ) );
         if( ++wid % 4 == 0 )
            send_to_char( "\r\n", ch );
      }
      if( wid % 4 != 0 )
         send_to_char( "\r\n", ch );
      return;
   }
   for( flt = first_filtered; flt; flt = flt->next )
      if( !str_cmp( arg, flt->name ) )
      {
         UNLINK( flt, first_filtered, last_filtered, next, prev );
         DISPOSE( flt->name );
         DISPOSE( flt );
         save_filtered(  );
         send_to_char( "Word no longer filtered.\r\n", ch );
         return;
      }
   CREATE( flt, FILTER_DATA, 1 );
   flt->name = str_dup( arg );
   sort_filtered( flt );
   save_filtered(  );
   send_to_char( "Word filtered.\r\n", ch );
   return;
}

void do_semote( CHAR_DATA * ch, char *argument )
{
   bool namer = FALSE;

   if( argument[0] == '\0' )
   {
      send_to_char( "Semote what?\r\n", ch );
      return;
   }

   /*
    * gotta luv strstr! :) 
    */
   if( !strstr( argument, "$n" ) )
   {
      /*
       * should deal with that little bugger 
       */
      if( strstr( argument, capitalize( ch->name ) ) || strstr( argument, strlower( ch->name ) ) )
         namer = TRUE;
      else
      {
         send_to_char( "You must have a $n or your name somewhere in the emote.\r\n", ch );
         return;
      }
   }

   /*
    * no name, but a $n token 
    */
   if( !namer )
   {
      act( AT_SOCIAL, argument, ch, NULL, NULL, TO_CHAR );
      act( AT_SOCIAL, argument, ch, NULL, NULL, TO_ROOM );
   }
   else
   {
      CHAR_DATA *vic = NULL;

      /*
       * here we go! 
       */
      for( vic = ch->in_room->first_person; vic; vic = vic->next_in_room )
         pager_printf_color( vic, "&c%s.&g\r\n", argument );
   }
   return;
}
