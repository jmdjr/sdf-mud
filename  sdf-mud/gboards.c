/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#include <string.h>
#include <ctype.h>
#include <time.h>
#include "mud.h"
/* Global Boards */
#include "gboards.h"

/*
 
 Note Board system, (c) 1995-96 Erwin S. Andreasen, erwin@pip.dknet.dk
 =====================================================================
 
 Basically, the notes are split up into several boards. The boards do not
 exist physically, they can be read anywhere and in any position.
 
 Each of the note boards has its own file. Each of the boards can have its own
 "rights": who can read/write.
 
 Each character has an extra field added, namele the timestamp of the last note
 read by him/her on a certain board.
 
 The note entering system is changed too, making it more interactive. When
 entering a note, a character is put AFK and into a special CON_ state.
 Everything typed goes into the note.
 
 For the immortals it is possible to purge notes based on age. An Archive
 options is available which moves the notes older than X days into a special
 board. The file of this board should then be moved into some other directory
 during e.g. the startup script and perhaps renamed depending on date.
 
 Note that write_level MUST be >= read_level or else there will be strange
 output in certain functions.
 
 Board DEFAULT_BOARD must be at least readable by *everyone*.
 
*/

#define L_SUP (MAX_LEVEL - 1) /* if not already defined */

GLOBAL_BOARD_DATA boards[MAX_BOARD] = {
   {"Announce", "Announcements from Immortals", 0, LEVEL_IMMORTAL, "all", DEF_NORMAL, 60, NULL, FALSE}
   ,
   {"General", "General discussion", 0, 2, "all", DEF_INCLUDE, 21, NULL, FALSE}
   ,
   {"Ideas", "Suggestion for improvement", 0, 2, "all", DEF_NORMAL, 60, NULL, FALSE}
   ,
   {"Bugs", "Typos, bugs, errors", 0, 1, "imm", DEF_NORMAL, 60, NULL, FALSE}
   ,
   {"Admin", "Administative discussion", LEVEL_IMMORTAL, LEVEL_IMMORTAL, "all", DEF_NORMAL, 60, NULL, FALSE}
   ,
   {"Staff", "Staff discussion", LEVEL_IMMORTAL, LEVEL_IMMORTAL, "all", DEF_NORMAL, 60, NULL, FALSE}
   ,
   {"Personal", "Personal messages", 0, 1, "all", DEF_EXCLUDE, 28, NULL, FALSE}
};

/* The prompt that the character is given after finishing a note with ~ or END */
const char *szFinishPrompt = "(&WC&w)ontinue, (&WV&w)iew, (&WP&w)ost or (&WF&w)orget it?";

long last_note_stamp = 0;  /* To generate unique timestamps on notes */

#define BOARD_NOACCESS -1
#define BOARD_NOTFOUND -1

bool next_board( CHAR_DATA * ch );

void note_chan( char *argument, bool ITS )
{
   char buf[MAX_STRING_LENGTH];

   if( !ITS )
      sprintf( buf, "NEW NOTE: Board: %s", argument );
   else
      sprintf( buf, "NEW NOTE: ITS: %s", argument );

   do_info( NULL, buf );
}

/* Destroy a note */
void free_global_note( NOTE_DATA * note )
{
   if( note->sender )
      STRFREE( note->sender );
   if( note->to_list )
      STRFREE( note->to_list );
   if( note->subject )
      DISPOSE( note->subject );
   if( note->date )  /* was note->datestamp for some reason */
      DISPOSE( note->date );
   if( note->text )
      DISPOSE( note->text );

   DISPOSE( note );
}

/*
 * Allocates new memory for a note
 */
NOTE_DATA *new_note(  )
{
   NOTE_DATA *note;

   CREATE( note, NOTE_DATA, 1 );

   note->next = NULL;
   note->sender = NULL;
   note->expire = 0;
   note->to_list = NULL;
   note->subject = NULL;
   note->date = NULL;
   note->date_stamp = 0;
   note->text = NULL;

   return note;
}

/* append this note to the given file */
void append_note( FILE * fp, NOTE_DATA * note )
{
   fprintf( fp, "Sender  %s~\n", note->sender );
   fprintf( fp, "Date    %s~\n", note->date );
   fprintf( fp, "Stamp   %d\n", note->date_stamp );
   fprintf( fp, "Expire  %d\n", note->expire );
   fprintf( fp, "To      %s~\n", note->to_list );
   fprintf( fp, "Subject %s~\n", note->subject );
   fprintf( fp, "Text\n%s~\n\n", note->text );
}

/* Save a note in a given board */
void finish_note( GLOBAL_BOARD_DATA * board, NOTE_DATA * note )
{
   FILE *fp;
   NOTE_DATA *p;
   char filename[200];

   /*
    * The following is done in order to generate unique date_stamps 
    */

   if( last_note_stamp >= current_time )
      note->date_stamp = ++last_note_stamp;
   else
   {
      note->date_stamp = current_time;
      last_note_stamp = current_time;
   }
   if( board->note_first ) /* are there any notes in there now? */
   {
      for( p = board->note_first; p->next; p = p->next )
         ;  /* empty */

      p->next = note;
   }
   else  /* nope. empty list. */
      board->note_first = note;

   /*
    * append note to note file 
    */

   note_chan( board->short_name, FALSE );

   sprintf( filename, "%s%s", NOTE_DIR, board->short_name );

   fp = fopen( filename, "a" );
   if( !fp )
   {
      bug( "Could not open one of the note files in append mode", 0 );
      board->changed = TRUE;  /* set it to TRUE hope it will be OK later? */
      return;
   }

   append_note( fp, note );
   fclose( fp );
}

/* Find the number of a board */
int board_number( const GLOBAL_BOARD_DATA * board )
{
   int i;

   for( i = 0; i < MAX_BOARD; i++ )
      if( board == &boards[i] )
         return i;

   return -1;
}

/* Find a board number based on  a string */
int board_lookup( const char *name )
{
   int i;

   for( i = 0; i < MAX_BOARD; i++ )
      if( !str_cmp( boards[i].short_name, name ) )
         return i;

   return -1;
}

/* Remove list from the list. Do not free note */
void unlink_note( GLOBAL_BOARD_DATA * board, NOTE_DATA * note )
{
   NOTE_DATA *p;

   if( board->note_first == note )
      board->note_first = note->next;
   else
   {
      for( p = board->note_first; p && p->next != note; p = p->next )
         ;
      if( !p )
         bug( "unlink_note: could not find note.", 0 );
      else
         p->next = note->next;
   }
}

/* Find the nth note on a board. Return NULL if ch has no access to that note */
NOTE_DATA *find_note( CHAR_DATA * ch, GLOBAL_BOARD_DATA * board, int num )
{
   int count = 0;
   NOTE_DATA *p;

   for( p = board->note_first; p; p = p->next )
      if( ++count == num )
         break;

   if( ( count == num ) && is_note_to( ch, p ) )
      return p;
   else
      return NULL;

}

/* save a single board */
void save_board( GLOBAL_BOARD_DATA * board )
{
   FILE *fp;
   char filename[200];
   char buf[200];
   NOTE_DATA *note;

   sprintf( filename, "%s%s", NOTE_DIR, board->short_name );

   fp = fopen( filename, "w" );
   if( !fp )
   {
      sprintf( buf, "Error writing to: %s", filename );
      bug( buf, 0 );
   }
   else
   {
      for( note = board->note_first; note; note = note->next )
         append_note( fp, note );

      fclose( fp );
   }
}

/* Show one note to a character */
void show_note_to_char( CHAR_DATA * ch, NOTE_DATA * note, int num )
{
   char buf[4 * MAX_STRING_LENGTH];
   char buf2[50];
   int length = strlen( note->sender ) - 4;
   int x;

   buf2[0] = '\0';
   for( x = 0; x < length; x++ )
      strcat( buf2, " " );

   /*
    * Ugly colors ? 
    */
   sprintf( buf,
            "[&W%4d&w] &Y%s&w: &c%s\r\n"
            "&W       &YDate%s&w: %s\r\n"
            "&W       &YTo  %s&w: %s\r\n"
            "&c===========================================================================&w\r\n"
            "%s\r\n", num, note->sender, note->subject, buf2, note->date, buf2, note->to_list, note->text );

   send_to_char_color( buf, ch );
}

/* Save changed boards */
void save_notes(  )
{
   int i;

   for( i = 0; i < MAX_BOARD; i++ )
      if( boards[i].changed ) /* only save changed boards */
         save_board( &boards[i] );
}

/* Load a single board */
void load_board( GLOBAL_BOARD_DATA * board )
{
   FILE *fp, *fp_archive;
   NOTE_DATA *last_note;
   char filename[200];

   sprintf( filename, "%s%s", NOTE_DIR, board->short_name );

   fp = fopen( filename, "r" );

   /*
    * Silently return 
    */
   if( !fp )
      return;

   /*
    * Start note fetching. copy of db.c:load_notes() 
    */

   last_note = NULL;

   for( ;; )
   {
      NOTE_DATA *pnote;
      char letter;

      do
      {
         letter = getc( fp );
         if( feof( fp ) )
         {
            fclose( fp );
            return;
         }
      }
      while( isspace( letter ) );
      ungetc( letter, fp );

      CREATE( pnote, NOTE_DATA, sizeof( *pnote ) );

      if( str_cmp( fread_word( fp ), "sender" ) )
         break;
      pnote->sender = fread_string( fp );

      if( str_cmp( fread_word( fp ), "date" ) )
         break;
      pnote->date = fread_string_nohash( fp );

      if( str_cmp( fread_word( fp ), "stamp" ) )
         break;
      pnote->date_stamp = fread_number( fp );

      if( str_cmp( fread_word( fp ), "expire" ) )
         break;
      pnote->expire = fread_number( fp );

      if( str_cmp( fread_word( fp ), "to" ) )
         break;
      pnote->to_list = fread_string( fp );

      if( str_cmp( fread_word( fp ), "subject" ) )
         break;
      pnote->subject = fread_string_nohash( fp );

      if( str_cmp( fread_word( fp ), "text" ) )
         break;
      pnote->text = fread_string_nohash( fp );

      pnote->next = NULL;  /* jic */

      /*
       * Should this note be archived right now ? 
       */

      if( pnote->expire < current_time )
      {
         char archive_name[200];

         sprintf( archive_name, "%s%s.old", NOTE_DIR, board->short_name );
         fp_archive = fopen( archive_name, "a" );
         if( !fp_archive )
            bug( "Could not open archive boards for writing", 0 );
         else
         {
            append_note( fp_archive, pnote );
            fclose( fp_archive );   /* it might be more efficient to close this later */
         }

         free_global_note( pnote );
         board->changed = TRUE;
         continue;

      }


      if( board->note_first == NULL )
         board->note_first = pnote;
      else
         last_note->next = pnote;

      last_note = pnote;
   }

   bug( "Load_notes: bad key word.", 0 );
   return;  /* just return */
}

/* Initialize structures. Load all boards. */
void load_global_boards(  )
{
   int i;

   for( i = 0; i < MAX_BOARD; i++ )
      load_board( &boards[i] );
}

/* Returns TRUE if the specified note is address to ch */
bool is_note_to( CHAR_DATA * ch, NOTE_DATA * note )
{
   if( !str_cmp( ch->name, note->sender ) )
      return TRUE;

   if( is_full_name( "all", note->to_list ) )
      return TRUE;

   if( IS_IMMORTAL( ch ) && ( is_full_name( "imm", note->to_list ) ||
                              is_full_name( "imms", note->to_list ) ||
                              is_full_name( "immortal", note->to_list ) ||
                              is_full_name( "god", note->to_list ) ||
                              is_full_name( "gods", note->to_list ) || is_full_name( "immortals", note->to_list ) ) )
      return TRUE;

   if( ( get_trust( ch ) == MAX_LEVEL ) && ( is_full_name( "imp", note->to_list ) ||
                                             is_full_name( "imps", note->to_list ) ||
                                             is_full_name( "implementor", note->to_list ) ||
                                             is_full_name( "implementors", note->to_list ) ) )
      return TRUE;

   if( is_full_name( ch->name, note->to_list ) )
      return TRUE;

   /*
    * Allow a note to e.g. 40 to send to characters level 40 and above 
    */
   if( is_number( note->to_list ) && get_trust( ch ) >= atoi( note->to_list ) )
      return TRUE;

   return FALSE;
}

void start_editing_note( CHAR_DATA * ch, char *data )
{
   EDITOR_DATA *edit;
   short lines, size, lpos;
   char c;

   if( !ch->desc )
   {
      bug( "Fatal: start_editing_note: no desc", 0 );
      return;
   }

   if( ch->substate == SUB_RESTRICTED )
      bug( "NOT GOOD: start_editing_note: ch->substate == SUB_RESTRICTED", 0 );
   act( AT_ACTION, "$n starts writing.", ch, NULL, NULL, TO_CANSEE );
   set_char_color( AT_GREEN, ch );

   send_to_char_color( "\r\nEnter text now. (/? = help /s = exit and save /l = list /c = clear)\r\n", ch );
   send_to_char_color( "=================================================================\r\n> ", ch );

   if( ch->editor )
      stop_editing( ch );

   CREATE( edit, EDITOR_DATA, 1 );
   edit->numlines = 0;
   edit->on_line = 0;
   edit->size = 0;
   size = 0;
   lpos = 0;
   lines = 0;
   if( !data )
      bug( "start_editing_note: data is NULL!\r\n", 0 );
   else
      for( ;; )
      {
         c = data[size++];
         if( c == '\0' )
         {
            edit->line[lines][lpos] = '\0';
            break;
         }
         else if( c == '\r' )
            ;
         else if( c == '\n' || lpos > 78 )
         {
            edit->line[lines][lpos] = '\0';
            ++lines;
            lpos = 0;
         }
         else
            edit->line[lines][lpos++] = c;
         if( lines >= 49 || size > 4096 )
         {
            edit->line[lines][lpos] = '\0';
            break;
         }
      }
   if( lpos > 0 && lpos < 78 && lines < 49 )
   {
      edit->line[lines][lpos] = '~';
      edit->line[lines][lpos + 1] = '\0';
      ++lines;
      lpos = 0;
   }
   edit->numlines = lines;
   edit->size = size;
   edit->on_line = lines;
   ch->editor = edit;
   ch->desc->connected = CON_NOTE_TEXT;
}

void smush_tilde( char *str );

char *copy_buffer_nohash( CHAR_DATA * ch )
{
   char buf[MAX_STRING_LENGTH];
   char tmp[100];
   short x, len;

   if( !ch )
   {
      bug( "copy_buffer: null ch", 0 );
      return str_dup( "" );
   }

   if( !ch->editor )
   {
      bug( "copy_buffer: null editor", 0 );
      return str_dup( "" );
   }

   buf[0] = '\0';
   for( x = 0; x < ch->editor->numlines; x++ )
   {
      strcpy( tmp, ch->editor->line[x] );
      len = strlen( tmp );
      if( tmp && tmp[len - 1] == '~' )
         tmp[len - 1] = '\0';
      else
         strcat( tmp, "\n" );
      smush_tilde( tmp );
      strcat( buf, tmp );
   }
   return str_dup( buf );
}

/* Return the number of unread notes 'ch' has in 'board' */
/* Returns BOARD_NOACCESS if ch has no access to board */
int unread_notes( CHAR_DATA * ch, GLOBAL_BOARD_DATA * board )
{
   NOTE_DATA *note;
   time_t last_read;
   int count = 0;

   if( board->read_level > get_trust( ch ) )
      return BOARD_NOACCESS;

   last_read = ch->pcdata->last_note[board_number( board )];

   for( note = board->note_first; note; note = note->next )
      if( is_note_to( ch, note ) && ( ( long )last_read < ( long )note->date_stamp ) )
         count++;

   return count;
}

/*
 * COMMANDS
 */

/* Start writing a note */
void do_nwrite( CHAR_DATA * ch, char *argument )
{
   char *strtime;
   char buf[200];

   if( IS_NPC( ch ) )   /* NPC cannot post notes */
      return;

   if( get_trust( ch ) < ch->pcdata->board->write_level )
   {
      send_to_char_color( "You cannot post notes on this board.\r\n", ch );
      return;
   }

   /*
    * continue previous note, if any text was written
    */
   if( ch->pcdata->in_progress && ( !ch->pcdata->in_progress->text ) )
   {
      send_to_char_color( "Note in progress cancelled because you did not manage to write any text \r\n"
                          "before losing link.\r\n\r\n", ch );
      free_global_note( ch->pcdata->in_progress );
      ch->pcdata->in_progress = NULL;
   }


   if( !ch->pcdata->in_progress )
   {
      ch->pcdata->in_progress = new_note(  );
      ch->pcdata->in_progress->sender = STRALLOC( ch->name );

      /*
       * convert to ascii. ctime returns a string which last character is \n, so remove that 
       */
      strtime = ctime( &current_time );
      strtime[strlen( strtime ) - 1] = '\0';

      ch->pcdata->in_progress->date = str_dup( strtime );
   }

   act( AT_GREEN, "$n starts writing a note.", ch, NULL, NULL, TO_ROOM );

   /*
    * Begin writing the note ! 
    */
   sprintf( buf, "You are now %s a new note on the &W%s&w board.\r\n"
            "If you are using tintin, type #verbose to turn off alias expansion!\r\n\r\n",
            ch->pcdata->in_progress->text ? "continuing" : "posting", ch->pcdata->board->short_name );
   send_to_char_color( buf, ch );

   sprintf( buf, "&YFrom&w:    %s\r\n\r\n", ch->name );
   send_to_char_color( buf, ch );

   if( !ch->pcdata->in_progress->text )   /* Are we continuing an old note or not? */
   {
      switch ( ch->pcdata->board->force_type )
      {
         case DEF_NORMAL:
            sprintf( buf, "If you press Return, default recipient \"&W%s&w\" will be chosen.\r\n",
                     ch->pcdata->board->names );
            break;
         case DEF_INCLUDE:
            sprintf( buf, "The recipient list MUST include \"&W%s&w\". If not, it will be added automatically.\r\n",
                     ch->pcdata->board->names );
            break;

         case DEF_EXCLUDE:
            sprintf( buf, "The recipient of this note must NOT include: \"&W%s&w\".", ch->pcdata->board->names );

            break;
      }

      send_to_char_color( buf, ch );
      send_to_char_color( "\r\n&YTo&w:      ", ch );

      ch->desc->connected = CON_NOTE_TO;
      /*
       * nanny takes over from here 
       */

   }
   else  /* we are continuing, print out all the fields and the note so far */
   {
      sprintf( buf, "&YTo&w:      %s\r\n"
               "&YExpires&w: %s\r\n"
               "&YSubject&w: %s\r\n",
               ch->pcdata->in_progress->to_list,
               ctime( &ch->pcdata->in_progress->expire ), ch->pcdata->in_progress->subject );
      send_to_char_color( buf, ch );
      send_to_char_color( "&GYour note so far:\r\n&w", ch );
      send_to_char_color( ch->pcdata->in_progress->text, ch );

      ch->substate = SUB_GBOARD_NOTE;
      ch->dest_buf = ch->pcdata;
      ch->pcdata->in_progress->text = str_dup( "" );
      start_editing_note( ch, ch->pcdata->in_progress->text );

      ch->desc->connected = CON_NOTE_TEXT;

   }

}


/* Read next note in current group. If no more notes, go to next board */
void do_nread( CHAR_DATA * ch, char *argument )
{
   NOTE_DATA *p;
   int count = 0, number;
   time_t *last_note = &ch->pcdata->last_note[board_number( ch->pcdata->board )];

   if( !str_cmp( argument, "again" ) )
   {  /* read last note again */
      send_to_char( "Not Implemented.\r\n", ch );
   }
   else if( is_number( argument ) )
   {
      number = atoi( argument );

      for( p = ch->pcdata->board->note_first; p; p = p->next )
         if( ++count == number )
            break;

      if( !p || !is_note_to( ch, p ) )
         send_to_char_color( "No such note.\r\n", ch );
      else
      {
         show_note_to_char( ch, p, count );
         *last_note = UMAX( *last_note, p->date_stamp );
      }
   }
   else  /* just next one */
   {
      char buf[200];

      count = 1;
      for( p = ch->pcdata->board->note_first; p; p = p->next, count++ )
         if( ( p->date_stamp > *last_note ) && is_note_to( ch, p ) )
         {
            show_note_to_char( ch, p, count );
            /*
             * Advance if new note is newer than the currently newest for that char 
             */
            *last_note = UMAX( *last_note, p->date_stamp );
            return;
         }

      send_to_char_color( "No new notes in this board.\r\n", ch );

      if( next_board( ch ) )
         sprintf( buf, "Changed to next board, %s.\r\n", ch->pcdata->board->short_name );
      else
         sprintf( buf, "There are no more boards.\r\n" );

      send_to_char_color( buf, ch );
   }
}

/* Remove a note */
void do_nremove( CHAR_DATA * ch, char *argument )
{
   NOTE_DATA *p;

   if( !is_number( argument ) )
   {
      send_to_char_color( "Remove which note?\r\n", ch );
      return;
   }

   p = find_note( ch, ch->pcdata->board, atoi( argument ) );
   if( !p )
   {
      send_to_char_color( "No such note.\r\n", ch );
      return;
   }

   if( str_cmp( ch->name, p->sender ) && ( get_trust( ch ) < LEVEL_IMPLEMENTOR ) )
   {
      send_to_char_color( "You are not authorized to remove this note.\r\n", ch );
      return;
   }

   unlink_note( ch->pcdata->board, p );
   free_global_note( p );
   send_to_char_color( "Note removed!\r\n", ch );

   save_board( ch->pcdata->board ); /* save the board */
}


/* List all notes or if argument given, list N of the last notes */
/* Shows REAL note numbers! */
void do_nlist( CHAR_DATA * ch, char *argument )
{
   int count = 0, show = 0, num = 0, has_shown = 0;
   time_t last_note;
   NOTE_DATA *p;
   char buf[MAX_STRING_LENGTH];


   if( is_number( argument ) )   /* first, count the number of notes */
   {
      show = atoi( argument );

      for( p = ch->pcdata->board->note_first; p; p = p->next )
         if( is_note_to( ch, p ) )
            count++;
   }

   send_to_char_color( "&WNotes on this board:\r\n"
                       "&CNum>   Author        Subject\r\n" "&c====   ======        =======\r\n&w", ch );

   last_note = ch->pcdata->last_note[board_number( ch->pcdata->board )];

   for( p = ch->pcdata->board->note_first; p; p = p->next )
   {
      num++;
      if( is_note_to( ch, p ) )
      {
         has_shown++;   /* note that we want to see X VISIBLE note, not just last X */
         if( !show || ( ( count - show ) < has_shown ) )
         {
            sprintf( buf, "&W%3d&w> &B%c &C%-13s &c%s&w \r\n",
                     num, last_note < p->date_stamp ? '*' : ' ', p->sender, p->subject );
            send_to_char_color( buf, ch );
         }
      }

   }
}

/* catch up with some notes */
void do_ncatchup( CHAR_DATA * ch, char *argument )
{
   NOTE_DATA *p;

   /*
    * Find last note 
    */
   for( p = ch->pcdata->board->note_first; p && p->next; p = p->next );

   if( !p )
      send_to_char_color( "Alas, there are no notes in that board.\r\n", ch );
   else
   {
      ch->pcdata->last_note[board_number( ch->pcdata->board )] = p->date_stamp;
      send_to_char_color( "All mesages skipped.\r\n", ch );
   }
}

/* Dispatch function for backwards compatibility */
void do_global_note( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

   if( IS_NPC( ch ) )
      return;

   argument = one_argument( argument, arg );

   if( ( !arg[0] ) || ( !str_cmp( arg, "read" ) ) )   /* 'note' or 'note read X' */
      do_nread( ch, argument );
   else if( !str_cmp( arg, "list" ) )
      do_nlist( ch, argument );
   else if( !str_cmp( arg, "write" ) )
      do_nwrite( ch, argument );
   else if( !str_cmp( arg, "remove" ) )
      do_nremove( ch, argument );
   else if( !str_cmp( arg, "catchup" ) )
      do_ncatchup( ch, argument );
   else
      do_help( ch, "note" );
}

/* Show all accessible boards with their numbers of unread messages OR
 change board. New board name can be given as a number or as a name (e.g.
 board personal or board 4 */
void do_global_boards( CHAR_DATA * ch, char *argument )
{
   int i, count, number;
   char buf[200];

   if( IS_NPC( ch ) )
      return;

   if( !argument[0] )   /* show boards */
   {
      int unread;

      count = 1;
      send_to_char_color( "&CNum         Name Unread Description\r\n" "&c=== ============ ====== ===========&w\r\n", ch );
      for( i = 0; i < MAX_BOARD; i++ )
      {
         unread = unread_notes( ch, &boards[i] );  /* how many unread notes? */
         if( unread != BOARD_NOACCESS )
         {
            sprintf( buf, "&W%2d&w> &C%12s&w [%s%4d&w] &c%s&w\r\n",
                     count, boards[i].short_name, unread ? "&R" : "&G", unread, boards[i].long_name );
            send_to_char_color( buf, ch );
            count++;
         }  /* if has access */

      }  /* for each board */

      if( ch->pcdata->board == NULL )
         ch->pcdata->board = &boards[DEFAULT_BOARD];
      sprintf( buf, "\r\nYou current board is &W%s&w.\r\n", ch->pcdata->board->short_name );
      send_to_char_color( buf, ch );

      /*
       * Inform of rights 
       */
      if( ch->pcdata->board->read_level > get_trust( ch ) )
         send_to_char_color( "You cannot read nor write notes on this board.\r\n", ch );
      else if( ch->pcdata->board->write_level > get_trust( ch ) )
         send_to_char_color( "You can only read notes from this board.\r\n", ch );
      else
         send_to_char_color( "You can both read and write on this board.\r\n", ch );

      return;
   }  /* if empty argument */

   if( ch->pcdata->in_progress )
   {
      send_to_char_color( "Please finish your interrupted note first.\r\n", ch );
      return;
   }

   /*
    * Change board based on its number 
    */
   if( is_number( argument ) )
   {
      count = 0;
      number = atoi( argument );
      for( i = 0; i < MAX_BOARD; i++ )
         if( unread_notes( ch, &boards[i] ) != BOARD_NOACCESS )
            if( ++count == number )
               break;

      if( count == number )   /* found the board.. change to it */
      {
         ch->pcdata->board = &boards[i];
         sprintf( buf, "Current board changed to &W%s&w. %s.\r\n", boards[i].short_name,
                  ( get_trust( ch ) < boards[i].write_level )
                  ? "You can only read here" : "You can both read and write here" );
         send_to_char_color( buf, ch );
      }
      else  /* so such board */
         send_to_char_color( "No such board.\r\n", ch );

      return;
   }

   /*
    * Non-number given, find board with that name 
    */

   for( i = 0; i < MAX_BOARD; i++ )
      if( !str_cmp( boards[i].short_name, argument ) )
         break;

   if( i == MAX_BOARD )
   {
      send_to_char_color( "No such board.\r\n", ch );
      return;
   }

   /*
    * Does ch have access to this board? 
    */
   if( unread_notes( ch, &boards[i] ) == BOARD_NOACCESS )
   {
      send_to_char_color( "No such board.\r\n", ch );
      return;
   }

   ch->pcdata->board = &boards[i];
   sprintf( buf, "Current board changed to &W%s&w. %s.\r\n", boards[i].short_name,
            ( get_trust( ch ) < boards[i].write_level ) ? "You can only read here" : "You can both read and write here" );
   send_to_char_color( buf, ch );
}

/* Send a note to someone on the personal board */
void personal_message( const char *sender, const char *to, const char *subject, const int expire_days, const char *text )
{
   make_note( "Personal", sender, to, subject, expire_days, text );
}

void make_note( const char *board_name, const char *sender, const char *to, const char *subject, const int expire_days,
                const char *text )
{
   int board_index = board_lookup( board_name );
   GLOBAL_BOARD_DATA *board;
   NOTE_DATA *note;
   char *strtime;

   if( board_index == BOARD_NOTFOUND )
   {
      bug( "make_note: board not found", 0 );
      return;
   }

   if( strlen( text ) > MAX_NOTE_TEXT )
   {
      bug( "make_note: text too long (%d bytes)", strlen( text ) );
      return;
   }


   board = &boards[board_index];

   note = new_note(  ); /* allocate new note */

   note->sender = STRALLOC( ( char * )sender );
   note->to_list = STRALLOC( ( char * )to );
   note->subject = str_dup( ( char * )subject );
   note->expire = current_time + expire_days * 60 * 60 * 24;
   note->text = str_dup( ( char * )text );

   /*
    * convert to ascii. ctime returns a string which last character is \n, so remove that 
    */
   strtime = ctime( &current_time );
   strtime[strlen( strtime ) - 1] = '\0';

   note->date = str_dup( strtime );

   finish_note( board, note );

}

/* tries to change to the next accessible board */
bool next_board( CHAR_DATA * ch )
{
   int i = board_number( ch->pcdata->board ) + 1;

   while( ( i < MAX_BOARD ) && ( unread_notes( ch, &boards[i] ) == BOARD_NOACCESS ) )
      i++;

   if( i == MAX_BOARD )
   {
      /*
       * Would it be better to just start over? 
       */
      ch->pcdata->board = &boards[0];
      return TRUE;
   }
   else
   {
      ch->pcdata->board = &boards[i];
      return TRUE;
   }
}

void handle_con_note_to( DESCRIPTOR_DATA * d, char *argument )
{
   char buf[MAX_INPUT_LENGTH];
   CHAR_DATA *ch = d->character;

   if( !ch->pcdata->in_progress )
   {
      d->connected = CON_PLAYING;
      bug( "nanny: In CON_NOTE_TO, but no note in progress", 0 );
      return;
   }

   strcpy( buf, argument );
   smash_tilde( buf );  /* change ~ to - as we save this field as a string later */

   switch ( ch->pcdata->board->force_type )
   {
      case DEF_NORMAL: /* default field */
         if( !buf[0] )  /* empty string? */
         {
            ch->pcdata->in_progress->to_list = STRALLOC( ch->pcdata->board->names );
            sprintf( buf, "Assumed default recipient: &W%s&w\r\n", ch->pcdata->board->names );
            send_to_char_color( buf, ch );
         }
         else
            ch->pcdata->in_progress->to_list = STRALLOC( buf );

         break;

      case DEF_INCLUDE:   /* forced default */
         if( !is_full_name( ch->pcdata->board->names, buf ) )
         {
            strcat( buf, " " );
            strcat( buf, ch->pcdata->board->names );
            ch->pcdata->in_progress->to_list = STRALLOC( buf );

            sprintf( buf, "\r\nYou did not specify %s as recipient, so it was automatically added.\r\n"
                     "&YNew To&w :  %s&w\r\n", ch->pcdata->board->names, ch->pcdata->in_progress->to_list );
            send_to_char_color( buf, ch );
         }
         else
            ch->pcdata->in_progress->to_list = STRALLOC( buf );
         break;

      case DEF_EXCLUDE:   /* forced exclude */
         if( !buf[0] )
         {
            send_to_char_color( "You must specify a recipient.\r\n" "&YTo&w:      ", ch );
            return;
         }

         if( is_full_name( ch->pcdata->board->names, buf ) )
         {
            sprintf( buf, "You are not allowed to send notes to %s on this board. Try again.\r\n"
                     "&YTo&w:      ", ch->pcdata->board->names );
            send_to_char_color( buf, ch );
            return;  /* return from nanny, not changing to the next state! */
         }
         else
            ch->pcdata->in_progress->to_list = STRALLOC( buf );
         break;

   }

   send_to_char_color( "&Y\r\nSubject&w: ", ch );
   d->connected = CON_NOTE_SUBJECT;
}

void handle_con_note_subject( DESCRIPTOR_DATA * d, char *argument )
{
   char buf[MAX_INPUT_LENGTH];
   CHAR_DATA *ch = d->character;

   if( !ch->pcdata->in_progress )
   {
      d->connected = CON_PLAYING;
      bug( "nanny: In CON_NOTE_SUBJECT, but no note in progress", 0 );
      return;
   }

   strcpy( buf, argument );
   smash_tilde( buf );  /* change ~ to - as we save this field as a string later */

   /*
    * Do not allow empty subjects 
    */

   if( !buf[0] )
   {
      send_to_char_color( "Please find a meaningful subject!\r\n", ch );
      send_to_char_color( "&YSubject&w: ", ch );
   }
   else if( strlen( buf ) > 60 )
   {
      send_to_char_color( "No, no. This is just the Subject. You're not writing the note yet. Twit.\r\n", ch );
   }
   else
      /*
       * advance to next stage 
       */
   {
      ch->pcdata->in_progress->subject = str_dup( buf );
      if( IS_IMMORTAL( ch ) ) /* immortals get to choose number of expire days */
      {
         sprintf( buf, "\r\nHow many days do you want this note to expire in?\r\n"
                  "Press Enter for default value for this board, &W%d&w days.\r\n"
                  "&YExpire&w:  ", ch->pcdata->board->purge_days );
         send_to_char_color( buf, ch );
         d->connected = CON_NOTE_EXPIRE;
      }
      else
      {
         ch->pcdata->in_progress->expire = current_time + ch->pcdata->board->purge_days * 24L * 3600L;
         sprintf( buf, "This note will expire %s\r", ctime( &ch->pcdata->in_progress->expire ) );
         send_to_char_color( buf, ch );

         ch->substate = SUB_GBOARD_NOTE;
         ch->dest_buf = ch->pcdata;
         ch->pcdata->in_progress->text = str_dup( "" );
         start_editing_note( ch, ch->pcdata->in_progress->text );
         d->connected = CON_NOTE_TEXT;
      }
   }
}

void handle_con_note_expire( DESCRIPTOR_DATA * d, char *argument )
{
   CHAR_DATA *ch = d->character;
   char buf[MAX_STRING_LENGTH];
   time_t expire;
   int days;

   if( !ch->pcdata->in_progress )
   {
      d->connected = CON_PLAYING;
      bug( "nanny: In CON_NOTE_EXPIRE, but no note in progress", 0 );
      return;
   }

   /*
    * Numeric argument. no tilde smashing 
    */
   strcpy( buf, argument );
   if( !buf[0] )  /* assume default expire */
      days = ch->pcdata->board->purge_days;
   else if( !is_number( buf ) )  /* use this expire */
   {
      send_to_char_color( "Write the number of days!\r\n", ch );
      send_to_char_color( "&YExpire&w:  ", ch );
      return;
   }
   else
   {
      days = atoi( buf );
      if( days <= 0 )
      {
         send_to_char_color( "This is a positive MUD. Use positive numbers only! :)\r\n", ch );
         send_to_char_color( "&YExpire&w:  ", ch );
         return;
      }
   }

   expire = current_time + ( days * 24L * 3600L ); /* 24 hours, 3600 seconds */

   ch->pcdata->in_progress->expire = expire;

   /*
    * note that ctime returns XXX\n so we only need to add an \r 
    */

   ch->substate = SUB_GBOARD_NOTE;
   ch->dest_buf = ch->pcdata;
   ch->pcdata->in_progress->text = str_dup( "" );
   start_editing_note( ch, ch->pcdata->in_progress->text );
   d->connected = CON_NOTE_TEXT;
}

void handle_con_note_text( CHAR_DATA * ch, char *argument )
{
   DESCRIPTOR_DATA *d;
   EDITOR_DATA *edit;
   char cmd[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   short x, line, max_buf_lines;
   bool save;

   if( ( d = ch->desc ) == NULL )
   {
      send_to_char( "You have no character.\r\n", ch );
      return;
   }

   if( d->connected != CON_NOTE_TEXT )
   {
      send_to_char( "You can't do that!\r\n", ch );
      bug( "handle_con_note_text: d->connected != CON_NOTE_TEXT", 0 );
      return;
   }

   if( !ch->editor )
   {
      send_to_char( "You can't do that!\r\n", ch );
      bug( "handle_con_note_text: null editor", 0 );
      d->connected = CON_PLAYING;
      return;
   }

   edit = ch->editor;
   save = FALSE;
   max_buf_lines = 24;

   if( ch->substate == SUB_MPROG_EDIT || ch->substate == SUB_HELP_EDIT )
      max_buf_lines = 48;

   if( argument[0] == '/' || argument[0] == '\\' )
   {
      one_argument( argument, cmd );
      if( !str_cmp( cmd + 1, "?" ) )
      {
         send_to_char( "Editing commands\r\n---------------------------------\r\n", ch );
         send_to_char( "/l              list buffer\r\n", ch );
         send_to_char( "/c              clear buffer\r\n", ch );
         send_to_char( "/d [line]       delete line\r\n", ch );
         send_to_char( "/f              format text, needs /g first\r\n", ch );
         send_to_char( "/g <line>       goto line\r\n", ch );
         send_to_char( "/i <line>       insert line\r\n", ch );
         send_to_char( "/r <old> <new>  global replace\r\n", ch );
         send_to_char( "/a              abort editing\r\n", ch );
         if( get_trust( ch ) > LEVEL_IMMORTAL )
            send_to_char( "/! <command>    execute command (do not use another editing command)\r\n", ch );
         send_to_char( "/s              save buffer\r\n\r\n> ", ch );
         return;
      }
      if( !str_cmp( cmd + 1, "c" ) )
      {
         memset( edit, '\0', sizeof( EDITOR_DATA ) );
         edit->numlines = 0;
         edit->on_line = 0;
         send_to_char( "Buffer cleared.\r\n> ", ch );
         return;
      }

      /*
       * added format command - shogar 
       *
       * This has been redone to be more efficient, and to make format
       * start at beginning of buffer, not whatever line you happened
       * to be on, at the time.   
       */
      if( !str_cmp( cmd + 1, "f" ) )
      {
         char temp_buf[MAX_STRING_LENGTH + max_buf_lines];
         int ep, old_p, end_mark;
         int p = 0;

         pager_printf( ch, "Reformating...\r\n" );

         for( x = 0; x < edit->numlines; x++ )
         {
            strncpy( temp_buf + p, edit->line[x], MAX_STRING_LENGTH + max_buf_lines - p );
            p += strlen( edit->line[x] );
            temp_buf[p] = ' ';
            p++;
         }

         temp_buf[p] = '\0';
         end_mark = p;
         p = 75;
         old_p = 0;
         edit->on_line = 0;
         edit->numlines = 0;

         while( old_p < end_mark )
         {
            while( temp_buf[p] != ' ' && p > old_p )
               p--;

            if( p == old_p )
               p += 75;

            if( p > end_mark )
               p = end_mark;

            ep = 0;
            for( x = old_p; x < p; x++ )
            {
               edit->line[edit->on_line][ep] = temp_buf[x];
               ep++;
            }
            edit->line[edit->on_line][ep] = '\0';

            edit->on_line++;
            edit->numlines++;

            old_p = p + 1;
            p += 75;

         }
         pager_printf( ch, "Reformating done.\r\n> " );
         return;
      }

      if( !str_cmp( cmd + 1, "r" ) )
      {
         char word1[MAX_INPUT_LENGTH];
         char word2[MAX_INPUT_LENGTH];
         char *sptr, *wptr, *lwptr;
         int count, wordln, word2ln, lineln;

         sptr = one_argument( argument, word1 );
         sptr = one_argument( sptr, word1 );
         sptr = one_argument( sptr, word2 );
         if( word1[0] == '\0' || word2[0] == '\0' )
         {
            send_to_char( "Need word to replace, and replacement.\r\n> ", ch );
            return;
         }
         if( strcmp( word1, word2 ) == 0 )
         {
            send_to_char( "Done.\r\n> ", ch );
            return;
         }
         count = 0;
         wordln = strlen( word1 );
         word2ln = strlen( word2 );
         ch_printf( ch, "Replacing all occurrences of %s with %s...\r\n", word1, word2 );
         for( x = edit->on_line; x < edit->numlines; x++ )
         {
            lwptr = edit->line[x];
            while( ( wptr = strstr( lwptr, word1 ) ) != NULL )
            {
               ++count;
               lineln = sprintf( buf, "%s%s", word2, wptr + wordln );
               if( lineln + wptr - edit->line[x] > 79 )
                  buf[lineln] = '\0';
               strcpy( wptr, buf );
               lwptr = wptr + word2ln;
            }
         }
         ch_printf( ch, "Found and replaced %d occurrence(s).\r\n> ", count );
         return;
      }

      if( !str_cmp( cmd + 1, "i" ) )
      {
         if( edit->numlines >= max_buf_lines )
            send_to_char( "Buffer is full.\r\n> ", ch );
         else
         {
            if( argument[2] == ' ' )
               line = atoi( argument + 2 ) - 1;
            else
               line = edit->on_line;
            if( line < 0 )
               line = edit->on_line;
            if( line < 0 || line > edit->numlines )
               send_to_char( "Out of range.\r\n> ", ch );
            else
            {
               for( x = ++edit->numlines; x > line; x-- )
                  strcpy( edit->line[x], edit->line[x - 1] );
               strcpy( edit->line[line], "" );
               send_to_char( "Line inserted.\r\n> ", ch );
            }
         }
         return;
      }
      if( !str_cmp( cmd + 1, "d" ) )
      {
         if( edit->numlines == 0 )
            send_to_char( "Buffer is empty.\r\n> ", ch );
         else
         {
            if( argument[2] == ' ' )
               line = atoi( argument + 2 ) - 1;
            else
               line = edit->on_line;
            if( line < 0 )
               line = edit->on_line;
            if( line < 0 || line > edit->numlines )
               send_to_char( "Out of range.\r\n> ", ch );
            else
            {
               if( line == 0 && edit->numlines == 1 )
               {
                  memset( edit, '\0', sizeof( EDITOR_DATA ) );
                  edit->numlines = 0;
                  edit->on_line = 0;
                  send_to_char( "Line deleted.\r\n> ", ch );
                  return;
               }
               for( x = line; x < ( edit->numlines - 1 ); x++ )
                  strcpy( edit->line[x], edit->line[x + 1] );
               strcpy( edit->line[edit->numlines--], "" );
               if( edit->on_line > edit->numlines )
                  edit->on_line = edit->numlines;
               send_to_char( "Line deleted.\r\n> ", ch );
            }
         }
         return;
      }
      if( !str_cmp( cmd + 1, "g" ) )
      {
         if( edit->numlines == 0 )
            send_to_char( "Buffer is empty.\r\n> ", ch );
         else
         {
            if( argument[2] == ' ' )
               line = atoi( argument + 2 ) - 1;
            else
            {
               send_to_char( "Goto what line?\r\n> ", ch );
               return;
            }
            if( line < 0 )
               line = edit->on_line;
            if( line < 0 || line > edit->numlines )
               send_to_char( "Out of range.\r\n> ", ch );
            else
            {
               edit->on_line = line;
               ch_printf( ch, "(On line %d)\r\n> ", line + 1 );
            }
         }
         return;
      }
      if( !str_cmp( cmd + 1, "l" ) )
      {
         if( edit->numlines == 0 )
            send_to_char( "Buffer is empty.\r\n> ", ch );
         else
         {
            send_to_char( "------------------\r\n", ch );
            for( x = 0; x < edit->numlines; x++ )
               ch_printf( ch, "%2d> %s\r\n", x + 1, edit->line[x] );
            send_to_char( "------------------\r\n> ", ch );
         }
         return;
      }
      if( !str_cmp( cmd + 1, "a" ) )
      {
         send_to_char( "\r\nAborting... ", ch );
         stop_editing( ch );
         free_global_note( ch->pcdata->in_progress );
         ch->pcdata->in_progress = NULL;
         d->connected = CON_PLAYING;
         return;
      }
      if( get_trust( ch ) > LEVEL_IMMORTAL && !str_cmp( cmd + 1, "!" ) )
      {
         DO_FUN *last_cmd;
         int substate = ch->substate;

         last_cmd = ch->last_cmd;
         ch->substate = SUB_RESTRICTED;
         interpret( ch, argument + 3 );
         ch->substate = substate;
         ch->last_cmd = last_cmd;
         set_char_color( AT_GREEN, ch );
         send_to_char( "\r\n> ", ch );
         return;
      }
      if( !str_cmp( cmd + 1, "s" ) )
      {
         NOTE_DATA *note = ch->pcdata->in_progress;

         DISPOSE( note->text );
         note->text = copy_buffer_nohash( ch );
         stop_editing( ch );

         if( !str_cmp( note->text, "" ) )
         {
            d->connected = CON_PLAYING;
            free_global_note( ch->pcdata->in_progress );
            ch->pcdata->in_progress = NULL;
            send_to_char_color( "\r\nAborting note.\r\n", ch );
            return;
         }
         d->connected = CON_NOTE_FINISH;
         send_to_char_color( "\r\n\r\n", ch );
         send_to_char_color( szFinishPrompt, ch );
         send_to_char_color( "\r\n", ch );
      }
   }

   if( edit->size + strlen( argument ) + 1 >= MAX_STRING_LENGTH - 1 )
      send_to_char( "You buffer is full.\r\n", ch );
   else
   {
      if( strlen( argument ) > 79 )
      {
         strncpy( buf, argument, 79 );
         buf[79] = 0;
         send_to_char( "(Long line trimmed)\r\n> ", ch );
      }
      else if( argument[0] != '\0' )
         strcpy( buf, argument );
      else
         strcpy( buf, "" );
      strcpy( edit->line[edit->on_line++], buf );
      if( edit->on_line > edit->numlines )
         edit->numlines++;
      if( edit->numlines > max_buf_lines )
      {
         edit->numlines = max_buf_lines;
         send_to_char( "Buffer full.\r\n", ch );
         save = TRUE;
      }
   }

   if( save )
   {
      NOTE_DATA *note = ch->pcdata->in_progress;
      DISPOSE( note->text );
      note->text = copy_buffer_nohash( ch );
      stop_editing( ch );
      d->connected = CON_NOTE_FINISH;
      send_to_char_color( "\r\n\r\n", ch );
      send_to_char_color( szFinishPrompt, ch );
      send_to_char_color( "\r\n", ch );
   }
   send_to_char( "> ", ch );
}

void handle_con_note_finish( DESCRIPTOR_DATA * d, char *argument )
{
   CHAR_DATA *ch = d->character;

   if( !ch->pcdata->in_progress )
   {
      d->connected = CON_PLAYING;
      bug( "nanny: In CON_NOTE_FINISH, but no note in progress", 0 );
      return;
   }

   switch ( tolower( argument[0] ) )
   {
      case 'c':  /* keep writing */
         send_to_char_color( "Continuing note...\r\n", ch );
         ch->substate = SUB_GBOARD_NOTE;
         ch->dest_buf = ch->pcdata;
         ch->pcdata->in_progress->text = str_dup( "" );
         start_editing_note( ch, ch->pcdata->in_progress->text );
         d->connected = CON_NOTE_TEXT;
         break;

      case 'v':  /* view note so far */
         if( ch->pcdata->in_progress->text )
         {
            send_to_char_color( "&gText of your note so far:&w\r\n", ch );
            send_to_char_color( ch->pcdata->in_progress->text, ch );
         }
         else
            send_to_char_color( "You haven't written a thing!\r\n\r\n", ch );
         send_to_char_color( szFinishPrompt, ch );
         send_to_char_color( "\r\n", ch );
         break;

      case 'p':  /* post note */
         finish_note( ch->pcdata->board, ch->pcdata->in_progress );
         send_to_char_color( "Note posted.\r\n", ch );
         d->connected = CON_PLAYING;
         /*
          * remove AFK status 
          */
         ch->pcdata->in_progress = NULL;
         act( AT_GREEN, "$n finishes $s note.", ch, NULL, NULL, TO_ROOM );
         break;

      case 'f':
         send_to_char_color( "Note cancelled!\r\n", ch );
         free_global_note( ch->pcdata->in_progress );
         ch->pcdata->in_progress = NULL;
         d->connected = CON_PLAYING;
         /*
          * remove afk status 
          */
         break;

      default:   /* invalid response */
         send_to_char_color( "What? Valid answers are:\r\n\r\n", ch );
         send_to_char_color( szFinishPrompt, ch );
         send_to_char_color( "\r\n", ch );

   }
}
