/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ResortMud Version 5.0b programed By Badastaz and Garinan
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/* Original code by Xkilla Cleaned up by Dreimas Converted for Smaug by Zarius */

#include <time.h>
#include "mud.h"
#include "changes.h"

char *current_date( void );
int num_changes( void );
int maxChanges;
#define NULLSTR(str)(str == NULL || str[0] == '\0')
CHANGE_DATA *changes_table;

void load_changes( void )
{
   FILE *fp;
   int i;

   if( !( fp = fopen( CHANGES_FILE, "r" ) ) )
   {
      bug( "%s", "Could not open Changes File for reading." );
      return;
   }

   fscanf( fp, "%d\n", &maxChanges );

   /*
    * Use malloc so we can realloc later on 
    */
   changes_table = malloc( sizeof( CHANGE_DATA ) * ( maxChanges + 1 ) );

   for( i = 0; i < maxChanges; i++ )
   {
      changes_table[i].change = fread_string( fp );
      changes_table[i].coder = fread_string( fp );
      changes_table[i].date = fread_string( fp );
      changes_table[i].mudtime = fread_number( fp );
   }
   changes_table[maxChanges].coder = str_dup( "" );
   fclose( fp );
   return;
}

char *current_date(  )
{
   static char buf[128];
   struct tm *datetime;

   datetime = localtime( &current_time );
   sprintf( buf, "%-2.2d/%-2.2d/%2.2d", datetime->tm_mon + 1, datetime->tm_mday, datetime->tm_year - 100 );
   return buf;
}

void save_changes( void )
{
   FILE *fp;
   int i, count = 0;

   if( !( fp = fopen( CHANGES_FILE, "w" ) ) )
   {
      perror( CHANGES_FILE );
      return;
   }

   fprintf( fp, "%d\n", maxChanges );
   for( i = 0; i < maxChanges; i++ )
   {
      count += 1;
      fprintf( fp, "%s~\n", changes_table[i].change );
      fprintf( fp, "%s~\n", changes_table[i].coder );
      fprintf( fp, "%s~\n", changes_table[i].date );
      fprintf( fp, "%d\n", changes_table[i].mudtime );
      fprintf( fp, "\n" );
   }
   fclose( fp );
   return;
}

void delete_change( int iChange )
{
   int i, j;
   CHANGE_DATA *new_table;

   new_table = malloc( sizeof( CHANGE_DATA ) * maxChanges );

   if( !new_table )
      return;

   for( i = 0, j = 0; i < maxChanges + 1; i++ )
   {
      if( i != ( iChange - 1 ) )
      {
         new_table[j] = changes_table[i];
         j++;
      }
   }

   free( changes_table );
   changes_table = new_table;
   maxChanges--;
   return;
}

void do_addchange( CHAR_DATA * ch, char *argument )
{
   CHANGE_DATA *new_table;

   if( IS_NPC( ch ) )
      return;

   if( argument[0] == '\0' )
   {
      send_to_char( "Syntax: Addchange ChangeString\r\n", ch );
      send_to_char( "&cType '&cchanges&c' to view the list.&d\r\n", ch );
      return;
   }

   maxChanges++;
   new_table = realloc( changes_table, sizeof( CHANGE_DATA ) * ( maxChanges + 1 ) );

   if( !new_table )  /* realloc failed */
   {
      send_to_char( "Memory allocation failed. Brace for impact.\r\n", ch );
      return;
   }

   changes_table = new_table;

   changes_table[maxChanges - 1].change = str_dup( argument );
   changes_table[maxChanges - 1].coder = str_dup( ch->name );
   changes_table[maxChanges - 1].date = str_dup( current_date(  ) );
   changes_table[maxChanges - 1].mudtime = current_time;

   send_to_char( "Changes Created.\r\n", ch );
   send_to_char( "Type 'changes' to see the changes.\r\n", ch );
   do_info( ch, " &zNew Change added to the mud, type '&Cchanges&z' to see it&z" );
   save_changes(  );
   return;
}

void do_chsave( CHAR_DATA * ch, char *argument )
{
   char arg1[MIL], arg2[MIL], buf[MSL];

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( IS_NPC( ch ) )
      return;

   if( !ch->desc || NULLSTR( arg1 ) )
   {
      send_to_char( "Syntax: chsave load/save\r\n", ch );
      send_to_char( "Syntax: chsave delete (change number)\r\n", ch );
      return;
   }

   if( !str_cmp( arg1, "load" ) )
   {
      load_changes(  );
      send_to_char( "Changes Loaded.\r\n", ch );
      return;
   }

   if( !str_cmp( arg1, "save" ) )
   {
      save_changes(  );
      send_to_char( "Changes Saved.\r\n", ch );
      return;
   }

   if( !str_cmp( arg1, "delete" ) )
   {
      int num;

      if( NULLSTR( arg2 ) || !is_number( arg2 ) )
      {
         send_to_char( "&wFor chsave delete, you must provide a change number.{x\r\n", ch );
         send_to_char( "Syntax: chsave delete (change number)\r\n", ch );
         return;
      }

      num = atoi( arg2 );
      if( num < 1 || num > maxChanges )
      {
         sprintf( buf, "Valid changes are from 1 to %d.\r\n", maxChanges );
         send_to_char( buf, ch );
         return;
      }
      delete_change( num );
      send_to_char( "Change deleted.\r\n", ch );
      return;
   }
   return;
}

int num_changes( void )
{
   char *test = current_date(  );
   int today = 0, i = 0;

   for( i = 0; i < maxChanges; i++ )
      if( !str_cmp( test, changes_table[i].date ) )
         today++;

   return today;
}

void do_changes( CHAR_DATA * ch, char *argument )
{
   char arg[MIL], buf[MSL], *test;
   int i, change, today;
   bool fAll, fchange = FALSE;

   one_argument( argument, arg );

   if( IS_NPC( ch ) || maxChanges < 1 )
      return;

   change = i = today = 0;
   test = current_date(  );

   for( i = 0; i < maxChanges; i++ )
      if( !str_cmp( test, changes_table[i].date ) )
         today++;

   if( NULLSTR( arg ) )
      fAll = FALSE;
   else
      fAll = !str_cmp( arg, "all" );

   if( is_number( arg ) )
   {
      if( atoi( arg ) < 1 || atoi( arg ) > maxChanges )
      {
         ch_printf( ch, "\r\n&WValid numbers are from 1 to %d&D\r\n", maxChanges );
         return;
      }
      change = atoi( arg );
      fAll = FALSE;
      fchange = TRUE;
   }

   send_to_char( "&wNo.  Coder        Date        Change&d\r\n", ch );
   for( i = 0; i < maxChanges; i++ )
   {
      if( !fchange && !fAll && changes_table[i].mudtime + ( 7 * 24L * 3600L ) < current_time )
         continue;
      if( !fchange && !fAll && str_cmp( test, changes_table[i].date ) )
         continue;
      if( fchange && change != i + 1 )
         continue;
      if( !fAll )
         send_to_char( "\r\n", ch );
      sprintf( buf, "&W[&R%4d&W]&B %-9s &C*%-6s &D\r\n", ( i + 1 ), changes_table[i].coder, changes_table[i].date );
      send_to_char( buf, ch );
      ch_printf( ch, "&D%s&D\r\n", changes_table[i].change );
   }

   if( today > 0 )
   {
      if( !fAll )
         send_to_char( "\r\n", ch );
      sprintf( buf, "&cThere&C-&cwas&C-&W[&R%4d&W]&C-&cchange%s-&ctoday.&C----------&D\r\n", today, today >
               1 ? "s&C" : "&C-" );
      send_to_char( buf, ch );
   }
   else
   {
      if( !fAll )
         send_to_char( "\r\n", ch );
   }
   sprintf( buf, "&cThere&C-&care&C-&W[&R%4d&W]&C-&ctotal&C-&cchanges.&C----------&D\r\n", maxChanges );
   send_to_char( buf, ch );

   send_to_char( "&cType&C-&c'changes #'&C-&cto&C-&csee&C-&cthat&C-&cchange.&C-----&D\r\n", ch );
   send_to_char( "&cType&C-&c'changes all'&C-&cto&C-&csee&C-&call&C-&cchanges.&C---&D\r\n", ch );
   return;
}
