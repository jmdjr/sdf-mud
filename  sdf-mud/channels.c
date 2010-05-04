/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ResortMud Version 5.0b programed By Badastaz and Garinan
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
 * AFKMud Copyright 1997-2002 Alsherok. Contributors: Samson, Dwip, Whir,   *
 * Cyberfox, Karangi, Rathian, Cam, Raine, and Tarl.                        *
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
 *                          Dynamic Channel System                          *
 ****************************************************************************/

#include <string.h>
#include <ctype.h>
#include <time.h>
#include "mud.h"

MUD_CHANNEL *first_channel;
MUD_CHANNEL *last_channel;
char *drunk_speech args( ( const char *argument, CHAR_DATA * ch ) );
bool check_cuss args( ( char *cuss ) );

char *const chan_types[] = {
   "Global", "Zone", "Guild", "Council", "PK", "Log"
};

int get_chantypes( char *name )
{
   size_t x;

   for( x = 0; x < sizeof( chan_types ) / sizeof( chan_types[0] ); x++ )
      if( !str_cmp( name, chan_types[x] ) )
         return x;
   return -1;
}

void read_channel( MUD_CHANNEL * channel, FILE * fp )
{
   char *word;
   bool fMatch;

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
            KEY( "ChanName", channel->name, fread_string( fp ) );
            KEY( "ChanLevel", channel->level, fread_number( fp ) );
            KEY( "ChanType", channel->type, fread_number( fp ) );
            KEY( "ChanHistory", channel->keephistory, fread_number( fp ) );
            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
               return;
            break;
      }
      if( !fMatch )
         bug( "read_channel: no match: %s", word );
   }
}

void load_mudchannels( void )
{
   FILE *fp;
   MUD_CHANNEL *channel;

   first_channel = NULL;
   last_channel = NULL;

   log_string( "Loading mud channels..." );

   if( ( fp = fopen( CHANNEL_FILE, "r" ) ) == NULL )
   {
      log_string( "No channel file found." );
      return;
   }

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
         bug( "load_channels: # not found.", 0 );
         break;
      }

      word = fread_word( fp );
      if( !str_cmp( word, "CHANNEL" ) )
      {
         CREATE( channel, MUD_CHANNEL, 1 );
         read_channel( channel, fp );

         LINK( channel, first_channel, last_channel, next, prev );
         continue;
      }
      else if( !str_cmp( word, "END" ) )
         break;
      else
      {
         bug( "load_channels: bad section: %s.", word );
         continue;
      }
   }
   fclose( fp );
   return;
}

void save_mudchannels( void )
{
   FILE *fp;
   MUD_CHANNEL *channel;

   if( ( fp = fopen( CHANNEL_FILE, "w" ) ) == NULL )
   {
      log_string( "Couldn't write to channel file." );
      return;
   }

   for( channel = first_channel; channel; channel = channel->next )
   {
      if( channel->name )
      {
         fprintf( fp, "#CHANNEL\n" );
         fprintf( fp, "ChanName    %s~\n", channel->name );
         fprintf( fp, "ChanLevel   %d\n", channel->level );
         fprintf( fp, "ChanType    %d\n", channel->type );
         fprintf( fp, "ChanHistory %d\n", channel->keephistory );
         fprintf( fp, "End\n\n" );
      }
   }
   fprintf( fp, "#END\n" );
   fclose( fp );
}

MUD_CHANNEL *find_channel( char *name )
{
   MUD_CHANNEL *channel = NULL;

   for( channel = first_channel; channel; channel = channel->next )
   {
      if( !str_cmp( channel->name, name ) )
         return channel;
   }
   return NULL;
}

void do_makechannel( CHAR_DATA * ch, char *argument )
{
   MUD_CHANNEL *channel;

   if( !argument || argument[0] == '\0' )
   {
      send_to_char( "&GSyntax: makechannel <name>\r\n", ch );
      return;
   }

   if( ( channel = find_channel( argument ) ) )
   {
      send_to_char( "&RA channel with that name already exists.\r\n", ch );
      return;
   }

   CREATE( channel, MUD_CHANNEL, 1 );
   channel->name = STRALLOC( argument );
   channel->level = LEVEL_IMMORTAL;
   channel->type = CHAN_GLOBAL;
   channel->keephistory = FALSE;
   LINK( channel, first_channel, last_channel, next, prev );
   ch_printf( ch, "New channel %s created.\r\n", argument );
   save_mudchannels(  );
   return;
}

void do_setchannel( CHAR_DATA * ch, char *argument )
{
   MUD_CHANNEL *channel;
   char arg[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];

   if( !argument || argument[0] == '\0' )
   {
      send_to_char( "Syntax: setchannel <channel> <field> <value>\r\n\r\n", ch );
      send_to_char( "Field may be one of the following:\r\n", ch );
      send_to_char( "name level type history\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg );

   if( !( channel = find_channel( arg ) ) )
   {
      send_to_char( "No channel by that name exists.\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg2 );

   if( !arg || arg2[0] == '\0' )
   {
      do_setchannel( ch, "" );
      return;
   }

   if( !str_cmp( arg2, "name" ) )
   {
      ch_printf( ch, "Channel %s renamed to %s\r\n", channel->name, argument );
      STRFREE( channel->name );
      channel->name = STRALLOC( argument );
      save_mudchannels(  );
      return;
   }

   if( !str_cmp( arg2, "level" ) )
   {
      int level;

      if( !is_number( argument ) )
      {
         send_to_char( "Level must be numerical.\r\n", ch );
         return;
      }

      level = atoi( argument );

      if( level < 1 || level > MAX_LEVEL )
      {
         ch_printf( ch, "Invalid level. Acceptable range is 1 to %d.\r\n", MAX_LEVEL );
         return;
      }

      channel->level = level;
      ch_printf( ch, "Channel %s level changed to %d\r\n", channel->name, level );
      save_mudchannels(  );
      return;
   }

   if( !str_cmp( arg2, "type" ) )
   {
      int type = get_chantypes( argument );

      if( type == -1 )
      {
         send_to_char( "Invalid channel type.\r\n", ch );
         return;
      }

      channel->type = type;
      ch_printf( ch, "Channel %s type changed to %s\r\n", channel->name, argument );
      save_mudchannels(  );
      return;
   }

   if( !str_cmp( arg2, "history" ) )
   {
      channel->keephistory = !channel->keephistory;

      if( channel->keephistory )
         ch_printf( ch, "Channel %s will now keep a history.\r\n", channel->name );
      else
         ch_printf( ch, "Channel %s will no longer keep a history.\r\n", channel->name );
      save_mudchannels(  );
      return;
   }

   do_setchannel( ch, "" );
   return;
}

void do_destroychannel( CHAR_DATA * ch, char *argument )
{
   MUD_CHANNEL *channel;

   if( !argument || argument[0] == '\0' )
   {
      send_to_char( "Syntax: destroychannel <name>\r\n", ch );
      return;
   }

   if( !( channel = find_channel( argument ) ) )
   {
      send_to_char( "No channel with that name exists.\r\n", ch );
      return;
   }

   STRFREE( channel->name );
   UNLINK( channel, first_channel, last_channel, next, prev );
   DISPOSE( channel );

   ch_printf( ch, "Channel %s destroyed.\r\n", argument );
   save_mudchannels(  );
   return;
}

void do_showchannels( CHAR_DATA * ch, char *argument )
{
   MUD_CHANNEL *channel;

   send_to_char( "&WName               Level Type       History?&D\r\n", ch );
   send_to_char( "&B-------------------------------------------&D\r\n", ch );
   for( channel = first_channel; channel; channel = channel->next )
      ch_printf( ch, "&Y%-18s %-4d  %-10s %s\r\n", capitalize( channel->name ),
                 channel->level, chan_types[channel->type], channel->keephistory ? "Yes" : "No" );
   return;
}

/* Stuff borrowed from I3/MUD-Net code to handle channel listening */

/*  changetarg: extract a single argument (with given max length) from
 *  argument to arg; if arg==NULL, just skip an arg, don't copy it out
 */
const char *getarg( const char *argument, char *arg, int length )
{
   int len = 0;

   while( *argument && isspace( *argument ) )
      argument++;

   if( arg )
      while( *argument && !isspace( *argument ) && len < length - 1 )
         *arg++ = *argument++, len++;
   else
      while( *argument && !isspace( *argument ) )
         argument++;

   while( *argument && !isspace( *argument ) )
      argument++;

   while( *argument && isspace( *argument ) )
      argument++;

   if( arg )
      *arg = 0;

   return argument;
}

/* Check for a name in a list */
int hasname( const char *list, const char *name )
{
   const char *p;
   char arg[MAX_INPUT_LENGTH];

   if( !list )
      return ( 0 );

   p = getarg( list, arg, MAX_INPUT_LENGTH );
   while( arg[0] )
   {
      if( !strcasecmp( name, arg ) )
         return 1;
      p = getarg( p, arg, MAX_INPUT_LENGTH );
   }

   return 0;
}

/* Add a name to a list */
void addname( char **list, const char *name )
{
   char buf[MAX_STRING_LENGTH];

   if( hasname( *list, name ) )
      return;

   if( *list && *list[0] != '\0' )
      sprintf( buf, "%s %s", *list, name );
   else
      strcpy( buf, name );

   if( *list )
      STRFREE( *list );
   *list = STRALLOC( buf );
}

/* Remove a name from a list */
void removename( char **list, const char *name )
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   const char *p;

   buf[0] = 0;
   p = getarg( *list, arg, MAX_INPUT_LENGTH );
   while( arg[0] )
   {
      if( strcasecmp( arg, name ) )
      {
         if( buf[0] )
            strcat( buf, " " );
         strcat( buf, arg );
      }
      p = getarg( p, arg, MAX_INPUT_LENGTH );
   }

   STRFREE( *list );
   *list = STRALLOC( buf );
}

void do_listen( CHAR_DATA * ch, char *argument )
{
   MUD_CHANNEL *channel;
   if( IS_NPC(ch) )
   {
   	send_to_char("Not for NPCs.", ch);
	return;
   }
   if( !argument || argument[0] == '\0' )
   {
      send_to_char( "Syntax: listen \r\n", ch );
      send_to_char( "Syntax: listen all\r\n", ch );
      send_to_char( "Syntax: listen none\r\n", ch );
      send_to_char( "For a list of channels, type channels\r\n", ch );
      send_to_char( "You are listenting to the following local mud channels:\r\n\r\n", ch );
      ch_printf( ch, "%s\r\n", ch->pcdata->chan_listen );
      return;
   }

   if( !str_cmp( argument, "all" ) )
   {
      for( channel = first_channel; channel; channel = channel->next )
      {
         if( ch->level >= channel->level && !hasname( ch->pcdata->chan_listen, channel->name ) )
            addname( &ch->pcdata->chan_listen, channel->name );
      }
      send_to_char( "You are now listening to all available channels.\r\n", ch );
      return;
   }

   if( !str_cmp( argument, "none" ) )
   {
      for( channel = first_channel; channel; channel = channel->next )
      {
         if( ch->level >= channel->level && hasname( ch->pcdata->chan_listen, channel->name ) )
            removename( &ch->pcdata->chan_listen, channel->name );
      }
      send_to_char( "You no longer listen to any available channel.\r\n", ch );
      return;
   }

   if( hasname( ch->pcdata->chan_listen, argument ) )
   {
      removename( &ch->pcdata->chan_listen, argument );
      ch_printf( ch, "You no longer listen to %s\r\n", argument );
   }
   else
   {
      if( !( channel = find_channel( argument ) ) )
      {
         send_to_char( "No such channel.\r\n", ch );
         return;
      }
      if( channel->level > ch->level )
      {
         send_to_char( "That channel is above your level.\r\n", ch );
         return;
      }
      addname( &ch->pcdata->chan_listen, argument );
      ch_printf( ch, "You now listen to %s\r\n", channel->name );
   }
   return;
}

/* Revised channel display by Zarius */
void do_channels( CHAR_DATA * ch, char *argument )
{
   MUD_CHANNEL *channel;
   int count = 0, second = 0, third = 0;
   MUD_CHANNEL *offchannel;
   int first = 0;

   send_to_char( "&YThe following channels are available:\r\n", ch );
   send_to_char( "To toggle a channel, use the &Wlisten &Ycommand.\r\n\r\n", ch );

   send_to_char( "&B----------------------------------------------------------------&D\r\n", ch );
   for( channel = first_channel; channel; channel = channel->next )
   {
      if( ch->level >= channel->level )
      {
         if( hasname( ch->pcdata->chan_listen, channel->name ) == 0 )
            continue;

         third += 1;
         first += 1;
         count += 1;
         if( third == 1 )
            ch_printf( ch, "&WYou are listening to these channels!\r\n" );

         ch_printf( ch, "&C%-15.15s &D", capitalize( channel->name ) );
         if( count == 4 )
         {
            ch_printf( ch, "\r\n" );
            count = 0;
         }
      }
   }
   if( count != 0 )
      ch_printf( ch, "\r\n" );
   if( first != 0 )
      send_to_char( "&B----------------------------------------------------------------&D\r\n", ch );
   third = 0;
   first = 0;
   for( offchannel = first_channel; offchannel; offchannel = offchannel->next )
   {
      if( ch->level >= offchannel->level )
      {
         if( hasname( ch->pcdata->chan_listen, offchannel->name ) == 1 )
            continue;

         third += 1;
         first += 1;
         second += 1;
         if( third == 1 )
            ch_printf( ch, "&WYou are not listening to these channels!\r\n" );

         ch_printf( ch, "&R%-15.15s &D", capitalize( offchannel->name ) );
         if( second == 4 )
         {
            ch_printf( ch, "\r\n" );
            second = 0;
         }
      }
   }
   if( second != 0 )
      send_to_char( "\r\n", ch );
   if( first != 0 )
      send_to_char( "&B----------------------------------------------------------------&D\r\n", ch );
   return;
}

void invert( char *arg1, char *arg2 )
{
   int i = 0;
   int len = strlen( arg1 ) - 1;

   while( i <= len )
   {
      *( arg2 + i ) = *( arg1 + ( len - i ) );
      i++;
   }
   *( arg2 + i ) = '\0';
}

/* Duplicate of to_channel from act_comm.c modified for dynamic channels */
void send_tochannel( CHAR_DATA * ch, MUD_CHANNEL * channel, char *argument )
{
   char buf[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH], word[MAX_INPUT_LENGTH], log_buf2[MAX_STRING_LENGTH];
   char *arg, *socbuf_char = NULL, *socbuf_vict = NULL, *socbuf_other = NULL;
   CHAR_DATA *victim = NULL;
   CHAR_DATA *vch;
   OBJ_DATA *obj; /* Burgundy Amulet */
   SOCIALTYPE *social = NULL;
   int position, x, msg;
   short color;
   char clr[MSL];
   struct tm *local;
   time_t t;
   bool emote = FALSE;

#ifndef SCRAMBLE
   int speaking = -1, lang;

   for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      if( ch->speaking & lang_array[lang] )
      {
         speaking = lang;
         break;
      }
#endif

   if( IS_NPC( ch ) && channel->type == CHAN_GUILD )
   {
      send_to_char( "Mobs can't be in clans/guilds.\r\n", ch );
      return;
   }

   if( !IS_PKILL( ch ) && channel->type == CHAN_PK )
   {
      if( !IS_IMMORTAL( ch ) )
      {
         send_to_char( "Peacefuls have no need to use wartalk.\r\n", ch );
         return;
      }
   }

   if( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) &&  !IS_IMMORTAL(ch) )
   {
      send_to_char( "The room absorbs your words!\r\n", ch );
      return;
   }

   if( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_CHARM ) )
   {
      if( ch->master )
         send_to_char( "I don't think so...\r\n", ch->master );
      return;
   }
   if( !IS_NPC( ch ) && !ch->pcdata->clan && channel->type == CHAN_GUILD )
   {
      ch_printf( ch, "You're not in a %s!\r\n",
                 !str_cmp( channel->name, "clantalk" ) ? "clan" : !str_cmp( channel->name,
                                                                            "guildtalk" ) ? "guild" : "order" );
      return;
   }

   if( !argument || argument[0] == '\0' )
   {
      char *name;

      if( !channel->keephistory )
      {
         ch_printf( ch, "%s what?\r\n", capitalize( channel->name ) );
         return;
      }

      ch_printf( ch, "The last 20 %s messages:\r\n", channel->name );
      for( x = 0; x < 20; x++ )
      {
         if( channel->history[x][0] != NULL )
         {
            switch ( channel->hlevel[x] )
            {
               case 0:
                  name = channel->history[x][0];
                  break;
               case 1:
                  if( IS_AFFECTED( ch, AFF_DETECT_INVIS ) || xIS_SET( ch->act, PLR_HOLYLIGHT ) )
                     name = channel->history[x][0];
                  else
                     name = "Someone";
                  break;
               case 2:
                  if( ch->level >= channel->hinvis[x] )
                     name = channel->history[x][0];
                  else
                     name = "Someone";
                  break;
               default:
                  name = "Someone";
            }
            ch_printf( ch, channel->history[x][1], name );
         }
         else
            break;
      }
      return;
   }

   if( xIS_SET( ch->act, PLR_SILENCE ) )
   {
      ch_printf( ch, "You can't %s.\r\n", channel->name );
      return;
   }

   /*
    * Inverts the speech of anyone carrying the burgundy amulet
    */
   for( obj = ch->first_carrying; obj; obj = obj->next_content )
   {
      if( obj->pIndexData->vnum == 1405 ) /* The amulet itself */
      {
         invert( argument, log_buf );
         strcpy( argument, log_buf );
         break;
      }
   }

   /*
    * OK, this is hackish - until I can figure out a better method
    */
   color = -1;
   if( !str_cmp( channel->name, "chat" ) )
      sprintf( clr, "G" );
   else if( !str_cmp( channel->name, "immtalk" ) )
      sprintf( clr, "Y" );
   else if( !str_cmp( channel->name, "avtalk" ) )
      sprintf( clr, "O" );
   else if( !str_cmp( channel->name, "ooc" ) )
      sprintf( clr, "G" );
   else if( !str_cmp( channel->name, "icc" ) )
      sprintf( clr, "G" );
   else if( !str_cmp( channel->name, "yell" ) )
      sprintf( clr, "c" );
   else if( !str_cmp( channel->name, "shout" ) )
      sprintf( clr, "c" );
   else if( !str_cmp( channel->name, "muse" ) )
      sprintf( clr, "c" );
   else if( !str_cmp( channel->name, "think" ) )
      sprintf( clr, "c" );
   else if( !str_cmp( channel->name, "auction" ) )
      sprintf( clr, "G" );
   else if( !str_cmp( channel->name, "wartalk" ) )
      sprintf( clr, "R" );
   else if( !str_cmp( channel->name, "music" ) )
      sprintf( clr, "P" );
   else if( !str_cmp( channel->name, "quest" ) )
      sprintf( clr, "c" );
   else if( !str_cmp( channel->name, "newbiechat" ) )
      sprintf( clr, "c" );
   else if( !str_cmp( channel->name, "guildtalk" ) )
      sprintf( clr, "c" );
   else if( !str_cmp( channel->name, "counciltalk" ) )
      sprintf( clr, "c" );
   else if( !str_cmp( channel->name, "clantalk" ) )
      sprintf( clr, "R" );
   else if( !str_cmp( channel->name, "ordertalk" ) )
      sprintf( clr, "C" );
   else if( !str_cmp( channel->name, "demi" ) )
      sprintf( clr, "C" );
   else if( !str_cmp( channel->name, "buildtalk" ) )
      sprintf( clr, "g" );
   else if( !str_cmp( channel->name, "quote" ) )
      sprintf( clr, "c" );
   else
      sprintf( clr, "G" );

   if( color == -1 )
      color = AT_GOSSIP;

   set_char_color( color, ch );

   arg = argument;
   arg = one_argument( arg, word );

   if( word[0] == '@' && ( social = find_social( word + 1 ) ) != NULL )
   {
      if( arg && *arg )
      {
         char name[MAX_INPUT_LENGTH];

         one_argument( arg, name );

         if( ( victim = get_char_world( ch, name ) ) )
            arg = one_argument( arg, name );

         if( !victim )
         {
            socbuf_char = social->char_no_arg;
            socbuf_vict = social->others_no_arg;
            socbuf_other = social->others_no_arg;
            if( !socbuf_char && !socbuf_other )
               social = NULL;
         }
         else if( victim == ch )
         {
            socbuf_char = social->char_auto;
            socbuf_vict = social->others_auto;
            socbuf_other = social->others_auto;
            if( !socbuf_char && !socbuf_other )
               social = NULL;
         }
         else if( victim != ch )
         {
            socbuf_char = social->char_found;
            socbuf_vict = social->vict_found;
            socbuf_other = social->others_found;
            if( !socbuf_char && !socbuf_other && !socbuf_vict )
               social = NULL;
         }
         else
            social = NULL;
      }
      else
      {
         socbuf_char = social->char_no_arg;
         socbuf_vict = social->others_no_arg;
         socbuf_other = social->others_no_arg;
         if( !socbuf_char && !socbuf_other )
            social = NULL;
      }
   }

   if( word[0] == ',' )
      emote = TRUE;

   if( social )
   {
      sprintf( buf, "&w[&%s%s&w] &%s%s", clr, capitalize( channel->name ), clr, socbuf_char );
      act( AT_PLAIN, buf, ch, argument, victim, TO_CHAR );
   }
   else if( emote )
   {
      sprintf( log_buf, "&w[&%s%s&w] &%s%s %s\r\n", clr, capitalize( channel->name ), clr, ch->name, argument + 1 );
      send_to_char( log_buf, ch );
      argument = argument + 1;
   }


else
   {  // Profanity Filter
      if( check_cuss( argument ) && !IS_IMMORTAL( ch ) && str_cmp( channel->name, "wartalk" ) )
      {
         msg = number_range( 0, 4 );
         if( msg == 0 )
            send_to_char( "Potty Mouth!\r\n", ch );
         if( msg == 1 )
            send_to_char( "Shame on You!\r\n", ch );
         if( msg == 2 )
            send_to_char( "Do you kiss your mother with that filthy mouth!?\r\n", ch );
         if( msg == 3 )
            send_to_char( "Git tha soap Ma, it's time fer a mouth washin'!\r\n", ch );
         if( msg == 4 )
            send_to_char( "Somewhere, a single tear is rolling down Garinan's cheek.\r\n", ch );

         sprintf( buf, "Profanity attempt by %s on channel %s: %s", ch->name, channel->name, argument );
         log_string( buf );
         return;
      }
      ch_printf( ch, "&w[&%s%s&w] You: &%s%s\r\n", clr, capitalize( channel->name ), clr, argument );
   }

   if( IS_SET( ch->in_room->room_flags, ROOM_LOGSPEECH ) )
   {
      sprintf( buf2, "%s: %s (%s)", IS_NPC( ch ) ? ch->short_descr : ch->name, argument, channel->name );
      append_to_file( LOG_FILE, buf2 );
   }

   /*
    * Channel history. Records the last 20 messages to channels which keep historys
    */
   if( channel->keephistory )
   {
      for( x = 0; x < 20; x++ )
      {
         int type;

         type = 0;
         if( IS_AFFECTED( ch, AFF_INVISIBLE ) )
            type = 1;
         if( xIS_SET( ch->act, PLR_WIZINVIS ) )
            type = 2;

         if( channel->history[x][0] == NULL )
         {
            if( IS_NPC( ch ) )
               channel->history[x][0] = str_dup( ch->short_descr );
            else
               channel->history[x][0] = str_dup( ch->name );

            t = time( NULL );
            local = localtime( &t );
            sprintf( log_buf2, "   [%-2.2d:%-2.2d] %%s%s %s\r\n",
                     local->tm_hour, local->tm_min, emote ? "" : ":", argument );

            channel->history[x][1] = str_dup( log_buf2 );
            channel->hlevel[x] = type;
            if( type == 2 )
               channel->hinvis[x] = ch->pcdata->wizinvis;
            else
               channel->hinvis[x] = 0;
            break;
         }

         if( x == 19 )
         {
            int y;

            for( y = 1; y < 20; y++ )
            {
               int z = y - 1;

               if( channel->history[z][0] != NULL )
               {
                  DISPOSE( channel->history[z][0] );
                  DISPOSE( channel->history[z][1] );
                  channel->history[z][0] = str_dup( channel->history[y][0] );
                  channel->history[z][1] = str_dup( channel->history[y][1] );
                  channel->hlevel[z] = channel->hlevel[y];
                  channel->hinvis[z] = channel->hinvis[y];
               }
            }
            if( IS_NPC( ch ) )
               channel->history[x][0] = str_dup( ch->short_descr );
            else
               channel->history[x][0] = str_dup( ch->name );

            t = time( NULL );
            local = localtime( &t );
            sprintf( log_buf2, "   [%-2.2d:%-2.2d] %%s%s %s\r\n",
                     local->tm_hour, local->tm_min, emote ? "" : ":", argument );
            channel->history[x][1] = str_dup( log_buf2 );

            channel->hlevel[x] = type;
            if( type == 2 )
               channel->hinvis[x] = ch->pcdata->wizinvis;
            else
               channel->hinvis[x] = 0;
         }
      }
   }

   for( vch = first_char; vch; vch = vch->next )
   {
      /*
       * Hackish solution to stop that damned "someone chat" bug - Matarael 17.3.2002
       */

      if( IS_NPC( vch ) || vch == ch || !vch->desc )
         continue;

      if( vch->desc->connected == CON_PLAYING && hasname( vch->pcdata->chan_listen, channel->name ) )
      {
         char *sbuf = argument;
         char lbuf[MAX_INPUT_LENGTH + 4]; /* invis level string + buf */

         if( vch->level < channel->level )
            continue;

         /*
          * Another hackish deal for now
          */
         if( !str_cmp( channel->name, "newbiechat" ) && ( !IS_IMMORTAL( vch ) && vch->level < 5 ) )
            continue;

         if( !str_cmp( channel->name, "immtalk" ) && !IS_IMMORTAL( vch ) )
            continue;

         if( !str_cmp( channel->name, "avtalk" ) && vch->level < LEVEL_PCAVATAR )
            continue;

         if( IS_SET( vch->in_room->room_flags, ROOM_SILENCE ) )
            continue;

         if( channel->type == CHAN_ZONE && vch->in_room->area != ch->in_room->area )
            continue;

         if( channel->type == CHAN_PK && !IS_PKILL( vch ) && !IS_IMMORTAL( vch ) )
            continue;

         if( channel->type == CHAN_GUILD )
         {
            if( IS_NPC( vch ) )
               continue;
            if( vch->pcdata->clan != ch->pcdata->clan )
               continue;
         }

         if( channel->type == CHAN_COUNCIL )
         {
            if( IS_NPC( vch ) )
               continue;
            if( vch->pcdata->council != ch->pcdata->council )
               continue;
         }

         position = vch->position;
         vch->position = POS_STANDING;

         set_char_color( color, vch );
         if( xIS_SET( ch->act, PLR_WIZINVIS ) && can_see( vch, ch ) && IS_IMMORTAL( vch ) )
            sprintf( lbuf, "(%d) ", ( !IS_NPC( ch ) ) ? ch->pcdata->wizinvis : ch->mobinvis );
         else
            lbuf[0] = '\0';

#ifndef SCRAMBLE
         if( speaking != -1 && ( !IS_NPC( ch ) || ch->speaking ) )
         {
            int speakswell = UMIN( knows_language( vch, ch->speaking ), knows_language( ch, ch->speaking ) );

            if( speakswell < 85 )
               sbuf = translate( speakswell, argument, lang_names[speaking] );
         }
#else
         if( !knows_language( vch, ch->speaking ) && ( !IS_NPC( ch ) || ch->speaking != 0 ) )
            sbuf = scramble( argument, ch->speaking );
#endif

         /*
          * Check to see if target is ignoring the sender
          */
         if( is_ignoring( vch, ch ) )
         {
            /*
             * If the sender is an imm then they cannot be ignored
             */
            if( !IS_IMMORTAL( ch ) || vch->level > ch->level )
            {
               /*
                * Off to oblivion!
                */
               continue;
            }
            else
               set_char_color( AT_IGNORE, vch );
         }

         MOBtrigger = FALSE;

         /*
          * Hackish solution to stop that damned "someone chat" bug - Matarael 17.3.2002
          */

         if( !social && !emote )
         {
            sprintf( buf, "&w[&%s%s&w] $n: &%s$t", clr, capitalize( channel->name ), clr );
            act( AT_PLAIN, strcat( lbuf, buf ), ch, sbuf, vch, TO_VICT );
         }
         if( emote )
         {
            sprintf( buf, "&w[&%s%s&w] &%s$n $t", clr, capitalize( channel->name ), clr );
            act( AT_PLAIN, strcat( lbuf, buf ), ch, sbuf, vch, TO_VICT );
         }
         if( social )
         {
            if( vch == victim )
            {
               sprintf( buf, "&w[&%s%s&w] &%s%s", clr, capitalize( channel->name ), clr, socbuf_vict );
               act( AT_PLAIN, buf, ch, NULL, vch, TO_VICT );
            }
            else if( vch != ch )
            {
               sprintf( buf, "&w[&%s%s&w] &%s%s", clr, capitalize( channel->name ), clr, socbuf_other );
               act( AT_PLAIN, buf, ch, vch, victim, TO_THIRD );
            }
         }
         vch->position = position;
         /*
          * Hackish solution to stop that damned "someone chat" bug - Matarael 17.3.2002
          */
      }
   }
   return;
}

void to_channel( const char *argument, char *xchannel, int level )
{
   MUD_CHANNEL *channel;
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;

   if( !first_descriptor || argument[0] == '\0' )
      return;

   if( !( channel = find_channel( xchannel ) ) )
      return;

   if( channel->type != CHAN_LOG )
      return;

   sprintf( buf, "%s: %s\r\n", capitalize( channel->name ), argument );

   for( d = first_descriptor; d; d = d->next )
   {
      CHAR_DATA *vch;

      vch = d->original ? d->original : d->character;

      if( !vch )
         continue;

      if( d->original )
         continue;

      /*
       * This could be coming in higher than the normal level, so check first
       */
      if( vch->level < level )
         continue;

      if( d->connected == CON_PLAYING && vch->level >= channel->level && hasname( vch->pcdata->chan_listen, channel->name ) )
      {
         set_char_color( AT_LOG, vch );
         send_to_char_color( buf, vch );
      }
   }
   return;
}

bool local_channel_hook( CHAR_DATA * ch, char *command, char *argument )
{
   MUD_CHANNEL *channel;

   if( !( channel = find_channel( command ) ) )
      return FALSE;

   if( ch->level < channel->level )
      return FALSE;

   /*
    * Logs are meant to be seen, not talked on
    */
   if( channel->type == CHAN_LOG )
      return FALSE;

   if( !IS_NPC( ch ) && !hasname( ch->pcdata->chan_listen, command ) )
   {
      ch_printf( ch, "You are not listening to the %s channel.\r\n", channel->name );
      return TRUE;
   }

   send_tochannel( ch, channel, argument );
   return TRUE;
}

void do_ask( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *vch;
   CHAR_DATA *victim;
   EXT_BV actflags;
#ifndef SCRAMBLE
   int speaking = -1, lang;

   for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      if( ch->speaking & lang_array[lang] )
      {
         speaking = lang;
         break;
      }
#endif

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "Ask who what?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL
       || ( IS_NPC( victim ) && victim->in_room != ch->in_room )
       || ( !NOT_AUTHED( ch ) && NOT_AUTHED( victim ) && !IS_IMMORTAL( ch ) ) )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) && !IS_IMMORTAL(ch) )
   {
      send_to_char( "You can't do that here.\r\n", ch );
      return;
   }

   actflags = ch->act;
   if( IS_NPC( ch ) )
      xREMOVE_BIT( ch->act, ACT_SECRETIVE );
   for( vch = ch->in_room->first_person; vch; vch = vch->next_in_room )
   {
      char *sbuf = argument;

      if( vch == ch )
         continue;

      /*
       * Check to see if character is ignoring speaker
       */
      if( is_ignoring( vch, ch ) )
      {
         /*
          * continue unless speaker is an immortal
          */
         if( !IS_IMMORTAL( ch ) || get_trust( vch ) > get_trust( ch ) )
            continue;
         else
         {
            set_char_color( AT_IGNORE, vch );
            ch_printf( vch, "You attempt to ignore %s, but are unable to do so.\r\n", ch->name );
         }
      }

#ifndef SCRAMBLE
      if( speaking != -1 && ( !IS_NPC( ch ) || ch->speaking ) )
      {
         int speakswell = UMIN( knows_language( vch, ch->speaking ), knows_language( ch, ch->speaking ) );
         if( speakswell < 75 )
            sbuf = translate( speakswell, argument, lang_names[speaking] );
      }
#else
      if( !knows_language( vch, ch->speaking ) && ( !IS_NPC( ch ) || ch->speaking != 0 ) )
         sbuf = scramble( argument, ch->speaking );
#endif
      sbuf = drunk_speech( sbuf, ch );

      MOBtrigger = FALSE;
   }
   ch->act = actflags;
   MOBtrigger = FALSE;

   act( AT_SAY, "You ask $N '$t'", ch, drunk_speech( argument, ch ), victim, TO_CHAR );
   act( AT_SAY, "$n asks you '$t'", ch, drunk_speech( argument, ch ), victim, TO_VICT );

   if( IS_SET( ch->in_room->room_flags, ROOM_LOGSPEECH ) )
   {
      sprintf( buf, "%s: %s", IS_NPC( ch ) ? ch->short_descr : ch->name, argument );
      append_to_file( LOG_FILE, buf );
   }

   mprog_speech_trigger( argument, ch );
   return;
}

void do_say( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *vch;
   EXT_BV actflags;
   char *to_vict, *to_char, *to_char_log;
   int x, arglen, rn;
   struct tm *lcl;
   time_t cur;

   /*
    * Various tables for randomness -- Kratas
    */
   struct
   {
      char *to_char;
      char *to_vict;
      char *to_char_log;
   }
   state_list[] =
   {
      {
      "say", "says", "said"},
      {
      "state", "states", "stated"}
   };
   struct
   {
      char *to_char;
      char *to_vict;
      char *to_char_log;
   }
   question_list[] =
   {
      {
      "ask", "asks", "asked"},
      {
      "inquire", "inquires", "inquired"}
   };
   struct
   {
      char *to_char;
      char *to_vict;
      char *to_char_log;
   }
   exclaim_list[] =
   {
      {
      "yell", "yells", "yelled"},
      {
      "exclaim", "exclaims", "exclaimed"}
   };

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
      char buffer[MAX_STRING_LENGTH];

      if( IS_NPC( ch ) )
      {
         send_to_char( "Say what?", ch );
         return;
      }

      send_to_char_color( "&cLast things you heard said&W:&w\r\n", ch );

      strcpy( buf, "" );
      for( x = 0; x < 10; x++ )
      {
         if( ch->pcdata->say_history[x] == NULL )
            break;
         sprintf( buffer, " &c%s&w\r\n", ch->pcdata->say_history[x] );
         strcat( buf, buffer );
      }
      strcat( buf, "\r\n" );
      send_to_char( buf, ch );

      return;
   }

   if( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) && !IS_IMMORTAL(ch) )
   {
      send_to_char( "You can't do that here.\r\n", ch );
      return;
   }


   actflags = ch->act;
   if( IS_NPC( ch ) )
      xREMOVE_BIT( ch->act, ACT_SECRETIVE );

   arglen = strlen( argument ) - 1;

   while( argument[arglen] == ' ' || argument[arglen] == '\t' )
      --arglen;

   switch ( argument[arglen] )
   {
      case '?':
         rn = rand(  ) % ( sizeof( question_list ) / sizeof( question_list[0] ) );
         to_vict = question_list[rn].to_vict;
         to_char = question_list[rn].to_char;
         to_char_log = question_list[rn].to_char_log;
         break;

      case '!':
         rn = rand(  ) % ( sizeof( exclaim_list ) / sizeof( exclaim_list[0] ) );
         to_vict = exclaim_list[rn].to_vict;
         to_char = exclaim_list[rn].to_char;
         to_char_log = exclaim_list[rn].to_char_log;
         break;

      case '"':
         if( argument[0] == '"' )
         {
            to_vict = "quotes";
            to_char = "quote";
            to_char_log = "quoted";
            break;
         }

      case '.':
         if( argument[arglen - 1] == '.' && argument[arglen - 2] == '.' )
         {
            to_vict = "trails off saying";
            to_char = "trail off saying";
            to_char_log = "trailed off saying";
            break;
         }

      default:
         rn = rand(  ) % ( sizeof( state_list ) / sizeof( state_list[0] ) );
         to_vict = state_list[rn].to_vict;
         to_char = state_list[rn].to_char;
         to_char_log = state_list[rn].to_char_log;
         break;
   }

   for( vch = ch->in_room->first_person; vch; vch = vch->next_in_room )
   {
      char *sbuf = argument;
      char tmpbuf[MAX_STRING_LENGTH];
      char actbuf[MAX_STRING_LENGTH];

      if( vch == ch )
         continue;

      /*
       * Check to see if character is ignoring speaker
       */
      if( is_ignoring( vch, ch ) )
      {
         /*
          * continue unless speaker is an immortal
          */
         if( !IS_IMMORTAL( ch ) || get_trust( vch ) > get_trust( ch ) )
            continue;
         else
         {
            set_char_color( AT_IGNORE, vch );
            ch_printf( vch, "You attempt to ignore %s, but" " are unable to do so.\r\n", ch->name );
         }
      }

#ifndef SCRAMBLE
      if( speaking != -1 && ( !IS_NPC( ch ) || ch->speaking ) )
      {
         int speakswell = UMIN( knows_language( vch, ch->speaking ),
                                knows_language( ch, ch->speaking ) );

         if( speakswell < 75 )
            sbuf = translate( speakswell, argument, lang_names[speaking] );
      }
#else
      if( !knows_language( vch, ch->speaking ) && ( !IS_NPC( ch ) || ch->speaking != 0 ) )
         sbuf = scramble( argument, ch->speaking );
#endif
      sbuf = drunk_speech( sbuf, ch );

      MOBtrigger = FALSE;

      cur = time( NULL );
      lcl = localtime( &cur );
      sprintf( tmpbuf, "[%-2.2d:%-2.2d] %s %s '%s&c'", lcl->tm_hour, lcl->tm_min, PERS( ch, vch ), to_char_log, sbuf );
      sprintf( actbuf, "$n %s '$t'", to_vict );
      //            act( AT_SAY, "$n says '$t'", ch, sbuf, vch, TO_VICT );

      act( AT_SAY, actbuf, ch, sbuf, vch, TO_VICT );

      if( !IS_NPC( vch ) )
      {
         for( x = 0; x < 10; x++ )
         {
            if( vch->pcdata->say_history[x] == '\0' )
            {
               vch->pcdata->say_history[x] = strdup( tmpbuf );
               break;
            }

            if( x == 9 )
            {
               int i;

               for( i = 1; i < 10; i++ )
               {
                  DISPOSE( vch->pcdata->say_history[i - 1] );
                  vch->pcdata->say_history[i - 1] = strdup( vch->pcdata->say_history[i] );
               }
               DISPOSE( vch->pcdata->say_history[x] );
               vch->pcdata->say_history[x] = strdup( tmpbuf );
            }
         }
      }
   }
   /*
    * MOBtrigger = FALSE;
    * act( AT_SAY, "$n says '$T'", ch, NULL, argument, TO_ROOM );
    */
   ch->act = actflags;
   MOBtrigger = FALSE;

   sprintf( buf, "You %s '$T'", to_char );
   act( AT_SAY, buf, ch, NULL, drunk_speech( argument, ch ), TO_CHAR );

   if( IS_SET( ch->in_room->room_flags, ROOM_LOGSPEECH ) )
   {
      sprintf( buf, "%s: %s", IS_NPC( ch ) ? ch->short_descr : ch->name, argument );
      append_to_file( LOG_FILE, buf );
   }
   mprog_speech_trigger( argument, ch );
   if( char_died( ch ) )
      return;
   oprog_speech_trigger( argument, ch );
   if( char_died( ch ) )
      return;
   rprog_speech_trigger( argument, ch );
   return;
}

void StoreTell( CHAR_DATA * ch, char *arg )
{
   int x;
   if( !IS_NPC( ch ) )
   {

      for( x = 0; x < 10; x++ )
      {
         if( ch->pcdata->tell_histories[x] == NULL )
         {
            ch->pcdata->tell_histories[x] = str_dup( arg );
            break;
         }
         if( x == 9 )
         {
            int i;

            for( i = 1; i < 10; i++ )
            {
               DISPOSE( ch->pcdata->tell_histories[i - 1] );
               ch->pcdata->tell_histories[i - 1] = str_dup( ch->pcdata->tell_histories[i] );
            }
            ch->pcdata->tell_histories[x] = str_dup( arg );
         }
      }
   }
}

void do_tell( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int position, x;
   CHAR_DATA *switched_victim = NULL;
   struct tm *lol;
   time_t cor;

#ifndef SCRAMBLE
   int speaking = -1, lang;

   for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      if( ch->speaking & lang_array[lang] )
      {
         speaking = lang;
         break;
      }
#endif

   MOBtrigger = TRUE;

   if( !IS_IMMORTAL( ch ) )
      REMOVE_BIT( ch->deaf, CHANNEL_TELLS );

   if( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) && !IS_IMMORTAL(ch) )
   {
      send_to_char( "You can't do that here.\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && ( xIS_SET( ch->act, PLR_SILENCE ) || xIS_SET( ch->act, PLR_NO_TELL ) ) )
   {
      send_to_char( "You can't do that.\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      if( IS_NPC( ch ) )
      {
         send_to_char( "Tell whom what?\r\n", ch );
         return;
      }
      send_to_char_color( "&cThe Last Tells You Have Recieved&W:&w\r\n", ch );

      buf[0] = '\0';
      for( x = 0; x < 10; x++ )
      {
         if( ch->pcdata->tell_histories[x] == NULL )
            break;
         sprintf( buf + strlen( buf ), "%s\r\n", ch->pcdata->tell_histories[x] );
      }
      strcat( buf, "\r\n" );
      send_to_char( buf, ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL
       || ( IS_NPC( victim ) && victim->in_room != ch->in_room )
       || ( !NOT_AUTHED( ch ) && NOT_AUTHED( victim ) && !IS_IMMORTAL( ch ) ) )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( ch == victim )
   {
      send_to_char( "You have a nice little chat with yourself.\r\n", ch );
      return;
   }

   if( NOT_AUTHED( ch ) && !NOT_AUTHED( victim ) && !IS_IMMORTAL( victim ) )
   {
      send_to_char( "They can't hear you because you are not authorized.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && ( victim->switched )
       && ( get_trust( ch ) > LEVEL_AVATAR ) && !IS_AFFECTED( victim->switched, AFF_POSSESS ) )
   {
      send_to_char( "That player is switched.\r\n", ch );
      return;
   }
   else if( !IS_NPC( victim ) && ( victim->switched ) && IS_AFFECTED( victim->switched, AFF_POSSESS ) )
      switched_victim = victim->switched;
   else if( !IS_NPC( victim ) && ( !victim->desc ) )
   {
      send_to_char( "That player is link-dead.\r\n", ch );
      return;
   }
   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_AFK ) )
   {
      send_to_char( "That player is away and may not have received your tell.\r\n", ch );
   }

   if( IS_SET( victim->deaf, CHANNEL_TELLS ) && ( !IS_IMMORTAL( ch ) || ( get_trust( ch ) <= get_trust( victim ) ) ) )
   {
      act( AT_PLAIN, "$E has $S tells turned off.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_SILENCE ) )
      send_to_char( "That player is silenced.  They will receive your message but can not respond.\r\n", ch );

   if( ( !IS_IMMORTAL( ch ) && !IS_AWAKE( victim ) )
       || ( !IS_NPC( victim ) && IS_SET( victim->in_room->room_flags, ROOM_SILENCE ) ) )
   {
      act( AT_PLAIN, "$E can't hear you.", ch, 0, victim, TO_CHAR );
      return;
   }

   if( victim->desc  /* make sure desc exists first  -Thoric */
       && ( victim->desc->connected == CON_EDITING
            || ( victim->desc->connected >= CON_NOTE_TO
                 && victim->desc->connected <= CON_NOTE_FINISH ) ) && get_trust( ch ) < LEVEL_GOD )
   {
      act( AT_PLAIN, "$E is currently in a writing buffer.  Please try again in a few minutes.", ch, 0, victim, TO_CHAR );
      return;
   }

   /*
    * Check to see if target of tell is ignoring the sender
    */
   if( is_ignoring( victim, ch ) )
   {
      /*
       * If the sender is an imm then they cannot be ignored
       */
      if( !IS_IMMORTAL( ch ) || get_trust( victim ) > get_trust( ch ) )
      {
         set_char_color( AT_IGNORE, ch );
         ch_printf( ch, "%s is ignoring you.\r\n", victim->name );
         return;
      }
      else
      {
         set_char_color( AT_IGNORE, victim );
         ch_printf( victim, "You attempt to ignore %s, but are unable to do so.\r\n", ch->name );
      }
   }

   ch->retell = victim;

   if( !IS_NPC( victim ) && IS_IMMORTAL( victim ) && victim->pcdata->tell_history &&
       isalpha( IS_NPC( ch ) ? ch->short_descr[0] : ch->name[0] ) )
   {
      sprintf( buf, "%s told you '%s'\r\n", capitalize( IS_NPC( ch ) ? ch->short_descr : ch->name ), argument );

      /*
       * get lasttell index... assumes names begin with characters
       */
      victim->pcdata->lt_index = tolower( IS_NPC( ch ) ? ch->short_descr[0] : ch->name[0] ) - 'a';

      /*
       * get rid of old messages
       */
      if( victim->pcdata->tell_history[victim->pcdata->lt_index] )
         STRFREE( victim->pcdata->tell_history[victim->pcdata->lt_index] );

      /*
       * store the new message
       */
      victim->pcdata->tell_history[victim->pcdata->lt_index] = STRALLOC( buf );
   }

   if( switched_victim )
      victim = switched_victim;

   /*
    * Bug fix by guppy@wavecomputers.net
    */
   MOBtrigger = FALSE;

   act( AT_TELL, "You tell $N '$t'", ch, argument, victim, TO_CHAR );
   position = victim->position;
   victim->position = POS_STANDING;
   if( speaking != -1 && ( !IS_NPC( ch ) || ch->speaking ) )
   {
      int speakswell = UMIN( knows_language( victim, ch->speaking ),
                             knows_language( ch, ch->speaking ) );

      if( speakswell < 85 )
      {
         MOBtrigger = FALSE;
         act( AT_TELL, "$n tells you '$t'", ch, translate( speakswell, argument, lang_names[speaking] ), victim, TO_VICT );
      }
      else
         act( AT_TELL, "$n tells you '$t'", ch, argument, victim, TO_VICT );
   }
   else
      act( AT_TELL, "$n tells you '$t'", ch, argument, victim, TO_VICT );

   MOBtrigger = TRUE;

   victim->position = position;
   victim->reply = ch;

   cor = time( NULL );
   lol = localtime( &cor );
   sprintf( buf2, "&R[%-2.2d:%-2.2d] %s told you &r'&R%s&r'&w", lol->tm_hour, lol->tm_min,
            capitalize( IS_NPC( ch ) ? ch->short_descr : ch->name ), argument );

   StoreTell( victim, buf2 );

   if( IS_SET( ch->in_room->room_flags, ROOM_LOGSPEECH ) )
   {
      sprintf( buf, "%s: %s (tell to) %s.",
               IS_NPC( ch ) ? ch->short_descr : ch->name, argument, IS_NPC( victim ) ? victim->short_descr : victim->name );
      append_to_file( LOG_FILE, buf );
   }


   mprog_speech_trigger( argument, ch );
   return;
}

void do_say_to_char( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *vch;
   CHAR_DATA *victim;
   EXT_BV actflags;

#ifndef SCRAMBLE
   int speaking = -1, lang;

   for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      if( ch->speaking & lang_array[lang] )
      {
         speaking = lang;
         break;
      }
#endif

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "Say what to whom?\r\n", ch );
      return;
   }
   if( ( victim = get_char_room( ch, arg ) ) == NULL
       || ( IS_NPC( victim )
            && victim->in_room != ch->in_room ) || ( !NOT_AUTHED( ch ) && NOT_AUTHED( victim ) && !IS_IMMORTAL( ch ) ) )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Do you enjoy talking to yourself?\r\n", ch );
      return;
   }

   if( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) && !IS_IMMORTAL(ch) )
   {
      send_to_char( "You can't do that here.\r\n", ch );
      return;
   }

   actflags = ch->act;
   if( IS_NPC( ch ) )
      xREMOVE_BIT( ch->act, ACT_SECRETIVE );
   for( vch = ch->in_room->first_person; vch; vch = vch->next_in_room )
   {
      char *sbuf = argument;

      if( vch == ch )
         continue;

      /*
       * Check to see if character is ignoring speaker
       */
      if( is_ignoring( vch, ch ) )
      {
         /*
          * continue unless speaker is an immortal
          */
         if( !IS_IMMORTAL( ch ) || get_trust( vch ) > get_trust( ch ) )
            continue;
         else
         {
            set_char_color( AT_IGNORE, vch );
            ch_printf( vch, "You attempt to ignore %s, but" " are unable to do so.\r\n", ch->name );
         }
      }

#ifndef SCRAMBLE
      if( speaking != -1 && ( !IS_NPC( ch ) || ch->speaking ) )
      {
         int speakswell = UMIN( knows_language( vch, ch->speaking ),
                                knows_language( ch, ch->speaking ) );
         if( speakswell < 75 )
            sbuf = translate( speakswell, argument, lang_names[speaking] );
      }
#else
      if( !knows_language( vch, ch->speaking ) && ( !IS_NPC( ch ) || ch->speaking != 0 ) )
         sbuf = scramble( argument, ch->speaking );
#endif
      sbuf = drunk_speech( sbuf, ch );

      MOBtrigger = FALSE;

   }
   ch->act = actflags;
   MOBtrigger = FALSE;

   act( AT_SAY, "You say to $N '$t'", ch, drunk_speech( argument, ch ), victim, TO_CHAR );
   act( AT_SAY, "$n says to $N '$t'", ch, drunk_speech( argument, ch ), victim, TO_NOTVICT );
   act( AT_SAY, "$n says to you '$t'", ch, drunk_speech( argument, ch ), victim, TO_VICT );

   /*
    * act( AT_SAY, "$n say$% to $O '$t'", ch, drunk_speech( argument, ch ), victim, TO_ALL );
    */
   if( IS_SET( ch->in_room->room_flags, ROOM_LOGSPEECH ) )
   {
      sprintf( buf, "%s: %s", IS_NPC( ch ) ? ch->short_descr : ch->name, argument );
      append_to_file( LOG_FILE, buf );
   }
   mprog_speech_trigger( argument, ch );
   if( char_died( ch ) )
      return;
   oprog_speech_trigger( argument, ch );
   if( char_died( ch ) )
      return;
   rprog_speech_trigger( argument, ch );
   return;
}

void do_whisper( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int position;
   int speaking = -1, lang;


#ifndef SCRAMBLE

   for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      if( ch->speaking & lang_array[lang] )
      {
         speaking = lang;
         break;
      }
#endif

   REMOVE_BIT( ch->deaf, CHANNEL_WHISPER );

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "Whisper to whom what?\r\n", ch );
      return;
   }


   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( ch == victim )
   {
      send_to_char( "You have a nice little chat with yourself.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && ( victim->switched ) && !IS_AFFECTED( victim->switched, AFF_POSSESS ) )
   {
      send_to_char( "That player is switched.\r\n", ch );
      return;
   }
   else if( !IS_NPC( victim ) && ( !victim->desc ) )
   {
      send_to_char( "That player is link-dead.\r\n", ch );
      return;
   }
   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_AFK ) )
   {
      send_to_char( "That player is away.\r\n", ch );
      return;
   }
   if( IS_SET( victim->deaf, CHANNEL_WHISPER ) && ( !IS_IMMORTAL( ch ) || ( get_trust( ch ) < get_trust( victim ) ) ) )
   {
      act( AT_PLAIN, "$E has $S whispers turned off.", ch, NULL, victim, TO_CHAR );
      return;
   }
   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_SILENCE ) )
      send_to_char( "That player is silenced.  They will receive your message but can not respond.\r\n", ch );

   if( victim->desc  /* make sure desc exists first  -Thoric */
       && ( victim->desc->connected == CON_EDITING
            || ( victim->desc->connected >= CON_NOTE_TO
                 && victim->desc->connected <= CON_NOTE_FINISH ) ) && get_trust( ch ) < LEVEL_GOD )
   {
      act( AT_PLAIN, "$E is currently in a writing buffer.  Please try again in a few minutes.", ch, 0, victim, TO_CHAR );
      return;
   }

   /*
    * Check to see if target of tell is ignoring the sender
    */
   if( is_ignoring( victim, ch ) )
   {
      /*
       * If the sender is an imm then they cannot be ignored
       */
      if( !IS_IMMORTAL( ch ) || get_trust( victim ) > get_trust( ch ) )
      {
         set_char_color( AT_IGNORE, ch );
         ch_printf( ch, "%s is ignoring you.\r\n", victim->name );
         return;
      }
      else
      {
         set_char_color( AT_IGNORE, victim );
         ch_printf( victim, "You attempt to ignore %s, but are unable to do so.\r\n", ch->name );
      }
   }
   /*
    * Bug fix by guppy@wavecomputers.net
    */
   MOBtrigger = FALSE;

   act( AT_WHISPER, "You whisper to $N '$t'", ch, argument, victim, TO_CHAR );
   position = victim->position;
   victim->position = POS_STANDING;
#ifndef SCRAMBLE
   if( speaking != -1 && ( !IS_NPC( ch ) || ch->speaking ) )
   {
      int speakswell = UMIN( knows_language( victim, ch->speaking ),
                             knows_language( ch, ch->speaking ) );

      if( speakswell < 85 )
      {
         act( AT_WHISPER, "$n whispers to you '$t'", ch,
              translate( speakswell, argument, lang_names[speaking] ), victim, TO_VICT );
#else
   if( !knows_language( vch, ch->speaking ) && ( !IS_NPC( ch ) || ch->speaking != 0 ) )
   {
      act( AT_WHISPER, "$n whispers to you '$t'", ch,
           translate( speakswell, argument, lang_names[speaking] ), victim, TO_VICT );
#endif

   }
   else
      act( AT_WHISPER, "$n whispers to you '$t'", ch, argument, victim, TO_VICT );
}

else
act( AT_WHISPER, "$n whispers to you '$t'", ch, argument, victim, TO_VICT );

if( !IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
   act( AT_WHISPER, "$n whispers something to $N.", ch, argument, victim, TO_NOTVICT );

MOBtrigger = TRUE;

victim->position = position;
if( IS_SET( ch->in_room->room_flags, ROOM_LOGSPEECH ) )
{
   sprintf( buf, "%s: %s (whisper to) %s.",
            IS_NPC( ch ) ? ch->short_descr : ch->name, argument, IS_NPC( victim ) ? victim->short_descr : victim->name );
   append_to_file( LOG_FILE, buf );
}


mprog_speech_trigger( argument, ch );
return;
}

void do_reply( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH], buf2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int position;
#ifndef SCRAMBLE
   int speaking = -1, lang;

   MOBtrigger = TRUE;

   for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      if( ch->speaking & lang_array[lang] )
      {
         speaking = lang;
         break;
      }
#endif

   if( !IS_IMMORTAL( ch ) )
      REMOVE_BIT( ch->deaf, CHANNEL_TELLS );

   if( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) && !IS_IMMORTAL(ch) )
   {
      send_to_char( "You can't do that here.\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && xIS_SET( ch->act, PLR_SILENCE ) )
   {
      send_to_char( "Your message didn't get through.\r\n", ch );
      return;
   }

   if( ( victim = ch->reply ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && ( victim->switched ) && can_see( ch, victim ) && ( get_trust( ch ) > LEVEL_AVATAR ) )
   {
      send_to_char( "That player is switched.\r\n", ch );
      return;
   }
   else if( !IS_NPC( victim ) && ( !victim->desc ) )
   {
      send_to_char( "That player is link-dead.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_AFK ) )
   {
      send_to_char( "That player is away, so they may not recieve your message.\r\n", ch );
   }

   if( IS_SET( victim->deaf, CHANNEL_TELLS ) && ( !IS_IMMORTAL( ch ) || ( get_trust( ch ) <= get_trust( victim ) ) ) )
   {
      act( AT_PLAIN, "$E has $S tells turned off.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( ( !IS_IMMORTAL( ch ) && !IS_AWAKE( victim ) )
       || ( !IS_NPC( victim ) && IS_SET( victim->in_room->room_flags, ROOM_SILENCE ) ) )
   {
      act( AT_PLAIN, "$E can't hear you.", ch, 0, victim, TO_CHAR );
      return;
   }

   if( victim->desc  /* make sure desc exists first  -Thoric */
       && ( victim->desc->connected == CON_EDITING
            || ( victim->desc->connected >= CON_NOTE_TO
                 && victim->desc->connected <= CON_NOTE_FINISH ) ) && get_trust( ch ) < LEVEL_GOD )
   {
      act( AT_PLAIN, "$E is currently in a writing buffer.  Please try again in a few minutes.", ch, 0, victim, TO_CHAR );
      return;
   }

   /*
    * Check to see if the receiver is ignoring the sender
    */
   if( is_ignoring( victim, ch ) )
   {
      /*
       * If the sender is an imm they cannot be ignored
       */
      if( !IS_IMMORTAL( ch ) || get_trust( victim ) > get_trust( ch ) )
      {
         set_char_color( AT_IGNORE, ch );
         ch_printf( ch, "%s is ignoring you.\r\n", victim->name );
         return;
      }
      else
      {
         set_char_color( AT_IGNORE, victim );
         ch_printf( victim, "You attempt to ignore %s, but " "are unable to do so.\r\n", ch->name );
      }
   }

   /*
    * Bug fix by guppy@wavecomputers.net
    */
   MOBtrigger = FALSE;

   act( AT_TELL, "You tell $N '$t'", ch, argument, victim, TO_CHAR );

   position = victim->position;
   victim->position = POS_STANDING;
#ifndef SCRAMBLE
   if( speaking != -1 && ( !IS_NPC( ch ) || ch->speaking ) )
   {
      int speakswell = UMIN( knows_language( victim, ch->speaking ),
                             knows_language( ch, ch->speaking ) );

      if( speakswell < 85 )
      {
         MOBtrigger = FALSE;
         act( AT_TELL, "$n tells you '$t'", ch, translate( speakswell, argument, lang_names[speaking] ), victim, TO_VICT );
      }
      else
         act( AT_TELL, "$n tells you '$t'", ch, argument, victim, TO_VICT );
   }
   else
      act( AT_TELL, "$n tells you '$t'", ch, argument, victim, TO_VICT );
#else
   if( knows_language( victim, ch->speaking ) || ( IS_NPC( ch ) && !ch->speaking ) )
      act( AT_TELL, "$n tells you '$t'", ch, argument, victim, TO_VICT );
   else
      act( AT_TELL, "$n tells you '$t'", ch, scramble( argument, ch->speaking ), victim, TO_VICT );
#endif

   MOBtrigger = TRUE;

   mprog_speech_trigger( argument, ch );

   sprintf( buf2, "&R%s told you &r'&R%s&r'&w", capitalize( IS_NPC( ch ) ? ch->short_descr : ch->name ), argument );

   StoreTell( victim, buf2 );

   victim->position = position;
   victim->reply = ch;
   ch->retell = victim;
   if( IS_SET( ch->in_room->room_flags, ROOM_LOGSPEECH ) )
   {
      sprintf( buf, "%s: %s (reply to) %s.",
               IS_NPC( ch ) ? ch->short_descr : ch->name, argument, IS_NPC( victim ) ? victim->short_descr : victim->name );
      append_to_file( LOG_FILE, buf );
   }

   if( !IS_NPC( victim ) && IS_IMMORTAL( victim ) && victim->pcdata->tell_history &&
       isalpha( IS_NPC( ch ) ? ch->short_descr[0] : ch->name[0] ) )
   {
      sprintf( buf, "%s told you '%s'\r\n", capitalize( IS_NPC( ch ) ? ch->short_descr : ch->name ), argument );

      /*
       * get lasttell index... assumes names begin with characters
       */
      victim->pcdata->lt_index = tolower( IS_NPC( ch ) ? ch->short_descr[0] : ch->name[0] ) - 'a';

      /*
       * get rid of old messages
       */
      if( victim->pcdata->tell_history[victim->pcdata->lt_index] )
         STRFREE( victim->pcdata->tell_history[victim->pcdata->lt_index] );

      /*
       * store the new message
       */
      victim->pcdata->tell_history[victim->pcdata->lt_index] = STRALLOC( buf );
   }

   return;
}

void do_retell( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int position;
   CHAR_DATA *switched_victim = NULL;
#ifndef SCRAMBLE
   int speaking = -1, lang;

   for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      if( ch->speaking & lang_array[lang] )
      {
         speaking = lang;
         break;
      }
#endif

   if( !IS_IMMORTAL( ch ) )
      REMOVE_BIT( ch->deaf, CHANNEL_TELLS );
   if( IS_SET( ch->in_room->room_flags, ROOM_SILENCE) && !IS_IMMORTAL(ch) )
   {
      send_to_char( "You can't do that here.\r\n", ch );
      return;
   }

   if( !IS_NPC( ch ) && ( xIS_SET( ch->act, PLR_SILENCE ) || xIS_SET( ch->act, PLR_NO_TELL ) ) )
   {
      send_to_char( "You can't do that.\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      ch_printf( ch, "What message do you wish to send?\r\n" );
      return;
   }

   victim = ch->retell;

   if( !victim )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && ( victim->switched ) &&
       ( get_trust( ch ) > LEVEL_AVATAR ) && !IS_AFFECTED( victim->switched, AFF_POSSESS ) )
   {
      send_to_char( "That player is switched.\r\n", ch );
      return;
   }
   else if( !IS_NPC( victim ) && ( victim->switched ) && IS_AFFECTED( victim->switched, AFF_POSSESS ) )
   {
      switched_victim = victim->switched;
   }
   else if( !IS_NPC( victim ) && ( !victim->desc ) )
   {
      send_to_char( "That player is link-dead.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_AFK ) )
   {
      send_to_char( "That player is away.\r\n", ch );
      return;
   }

   if( IS_SET( victim->deaf, CHANNEL_TELLS ) && ( !IS_IMMORTAL( ch ) || ( get_trust( ch ) < get_trust( victim ) ) ) )
   {
      act( AT_PLAIN, "$E has $S tells turned off.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( !IS_NPC( victim ) && xIS_SET( victim->act, PLR_SILENCE ) )
      send_to_char( "That player is silenced. They will receive your message, but can not respond.\r\n", ch );

   if( ( !IS_IMMORTAL( ch ) && !IS_AWAKE( victim ) ) ||
       ( !IS_NPC( victim ) && IS_SET( victim->in_room->room_flags, ROOM_SILENCE ) ) )
   {
      act( AT_PLAIN, "$E can't hear you.", ch, 0, victim, TO_CHAR );
      return;
   }

   if( victim->desc
       && ( victim->desc->connected == CON_EDITING
            || ( victim->desc->connected >= CON_NOTE_TO
                 && victim->desc->connected <= CON_NOTE_FINISH ) ) && get_trust( ch ) < LEVEL_GOD )
   {
      act( AT_PLAIN, "$E is currently in a writing buffer. Please " "try again in a few minutes.", ch, 0, victim, TO_CHAR );
      return;
   }

   /*
    * check to see if the target is ignoring the sender
    */
   if( is_ignoring( victim, ch ) )
   {
      /*
       * if the sender is an imm then they cannot be ignored
       */
      if( !IS_IMMORTAL( ch ) || get_trust( victim ) > get_trust( ch ) )
      {
         set_char_color( AT_IGNORE, ch );
         ch_printf( ch, "%s is ignoring you.\r\n", victim->name );
         return;
      }
      else
      {
         set_char_color( AT_IGNORE, victim );
         ch_printf( victim, "You attempy to ignore %s, but " "are unable to do so.\r\n", ch->name );
      }
   }

   /*
    * store tell history for victim
    */
   if( !IS_NPC( victim ) && IS_IMMORTAL( victim ) && victim->pcdata->tell_history &&
       isalpha( IS_NPC( ch ) ? ch->short_descr[0] : ch->name[0] ) )
   {
      sprintf( buf, "%s told you '%s'\r\n", capitalize( IS_NPC( ch ) ? ch->short_descr : ch->name ), argument );

      /*
       * get lasttel index... assumes names begin with chars
       */
      victim->pcdata->lt_index = tolower( IS_NPC( ch ) ? ch->short_descr[0] : ch->name[0] ) - 'a';

      /*
       * get rid of old messages
       */
      if( victim->pcdata->tell_history[victim->pcdata->lt_index] )
         STRFREE( victim->pcdata->tell_history[victim->pcdata->lt_index] );

      /*
       * store the new messagec
       */
      victim->pcdata->tell_history[victim->pcdata->lt_index] = STRALLOC( buf );
   }

   if( switched_victim )
      victim = switched_victim;

   /*
    * Bug fix by guppy@wavecomputers.net
    */
   MOBtrigger = FALSE;

   act( AT_TELL, "You tell $N '$t'", ch, argument, victim, TO_CHAR );
   position = victim->position;
   victim->position = POS_STANDING;
#ifndef SCRAMBLE
   if( speaking != -1 && ( !IS_NPC( ch ) || ch->speaking ) )
   {
      int speakswell = UMIN( knows_language( victim, ch->speaking ),
                             knows_language( ch, ch->speaking ) );

      if( speakswell < 85 )
      {
         MOBtrigger = FALSE;
         act( AT_TELL, "$n tells you '$t'", ch, translate( speakswell, argument, lang_names[speaking] ), victim, TO_VICT );
      }
      else
         act( AT_TELL, "$n tells you '$t'", ch, argument, victim, TO_VICT );
   }
   else
      act( AT_TELL, "$n tells you '$t'", ch, argument, victim, TO_VICT );
#else
   if( knows_language( victim, ch->speaking ) || ( IS_NPC( ch ) && !ch->speaking ) )
   {
      act( AT_TELL, "$n tells you '$t'", ch, argument, victim, TO_VICT );
   }
   else
   {
      act( AT_TELL, "$n tells you '$t'", ch, scramble( argument, ch->speaking ), victim, TO_VICT );
   }
#endif
   victim->position = position;
   victim->reply = ch;

   MOBtrigger = TRUE;

   if( IS_SET( ch->in_room->room_flags, ROOM_LOGSPEECH ) )
   {
      sprintf( buf, "%s: %s (retell to) %s.",
               IS_NPC( ch ) ? ch->short_descr : ch->name, argument, IS_NPC( victim ) ? victim->short_descr : victim->name );
      append_to_file( LOG_FILE, buf );
   }


   mprog_speech_trigger( argument, ch );
   return;
}

void do_repeat( CHAR_DATA * ch, char *argument )
{
   int tindex;

   if( IS_NPC( ch ) || !IS_IMMORTAL( ch ) || !ch->pcdata->tell_history )
   {
      no_find( ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      tindex = ch->pcdata->lt_index;
   }
   else if( isalpha( argument[0] ) && argument[1] == '\0' )
   {
      tindex = tolower( argument[0] ) - 'a';
   }
   else
   {
      send_to_char( "You may only index your tell history using a single letter.\r\n", ch );
      return;
   }

   if( ch->pcdata->tell_history[tindex] )
   {
      set_char_color( AT_TELL, ch );
      send_to_char( ch->pcdata->tell_history[tindex], ch );
   }
   else
   {
      send_to_char( "No one like that has sent you a tell.\r\n", ch );
   }
   return;
}

void do_emote( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char *plast;
   CHAR_DATA *vch;
   EXT_BV actflags;
#ifndef SCRAMBLE
   int speaking = -1, lang;

   for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      if( ch->speaking & lang_array[lang] )
      {
         speaking = lang;
         break;
      }
#endif

   if( !IS_NPC( ch ) && xIS_SET( ch->act, PLR_NO_EMOTE ) )
   {
      send_to_char( "You can't show your emotions.\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "Emote what?\r\n", ch );
      return;
   }

   actflags = ch->act;
   if( IS_NPC( ch ) )
      xREMOVE_BIT( ch->act, ACT_SECRETIVE );
   for( plast = argument; *plast != '\0'; plast++ )
      ;

   strcpy( buf, argument );
   if( isalpha( plast[-1] ) )
      strcat( buf, "." );
   for( vch = ch->in_room->first_person; vch; vch = vch->next_in_room )
   {
      char *sbuf = buf;

      /*
       * Check to see if character is ignoring emoter
       */
      if( is_ignoring( vch, ch ) )
      {
         /*
          * continue unless emoter is an immortal
          */
         if( !IS_IMMORTAL( ch ) || get_trust( vch ) > get_trust( ch ) )
            continue;
         else
         {
            set_char_color( AT_IGNORE, vch );
            ch_printf( vch, "You attempt to ignore %s, but" " are unable to do so.\r\n", ch->name );
         }
      }
#ifndef SCRAMBLE
      if( speaking != -1 && ( !IS_NPC( ch ) || ch->speaking ) )
      {
         int speakswell = UMIN( knows_language( vch, ch->speaking ),
                                knows_language( ch, ch->speaking ) );

         if( speakswell < 85 )
            sbuf = translate( speakswell, argument, lang_names[speaking] );
      }
#else
      if( !knows_language( vch, ch->speaking ) && ( !IS_NPC( ch ) && ch->speaking != 0 ) )
         sbuf = scramble( buf, ch->speaking );
#endif
      MOBtrigger = FALSE;
      act( AT_SOCIAL, "$n $t", ch, sbuf, vch, ( vch == ch ? TO_CHAR : TO_VICT ) );
   }
   /*
    * MOBtrigger = FALSE;
    * act( AT_ACTION, "$n $T", ch, NULL, buf, TO_ROOM );
    * MOBtrigger = FALSE;
    * act( AT_ACTION, "$n $T", ch, NULL, buf, TO_CHAR );
    */
   ch->act = actflags;
   if( IS_SET( ch->in_room->room_flags, ROOM_LOGSPEECH ) )
   {
      sprintf( buf, "%s %s (emote)", IS_NPC( ch ) ? ch->short_descr : ch->name, argument );
      append_to_file( LOG_FILE, buf );
   }
   return;
}
