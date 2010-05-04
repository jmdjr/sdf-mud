/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*
 * Disable Module
 *
 * Disables commands.
 *
 * Syntax: disable cmd name
 * Kratas
 */
#include <string.h>
#include "mud.h"

#define DISABLE_FILE SYSTEM_DIR "disable.dat"

void save_disabled(  );

typedef struct disabled_data DISABLED_DATA;
struct disabled_data
{
   DISABLED_DATA *next;
   CMDTYPE *command;
   char *who;
   int level;
};

DISABLED_DATA *first_disabled;

void do_disable( CHAR_DATA * ch, char *argument )
{
   DISABLED_DATA *disabled;
   CMDTYPE *cmd;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Hi, I'm sorry but npcs can't use this command!\r\n", ch );
      return;
   }

   if( argument == NULL || argument[0] == '\0' )
   {
      char buf[MIL];

      if( !first_disabled )
      {
         send_to_char( "No commands are disabled.\r\n", ch );
         return;
      }
      sprintf( buf, "Disabled commands:\r\n%-10s %5s     %-15s\r\n", "Command", "Level", "Who Disabled" );

      for( disabled = first_disabled; disabled; disabled = disabled->next )
      {
         sprintf( buf + strlen( buf ), "%-10s %5d     %-15s\r\n", disabled->command->name, disabled->level, disabled->who );
      }
      send_to_pager( buf, ch );
      return;
   }

   cmd = find_command( argument );

   for( disabled = first_disabled; disabled; disabled = disabled->next )
      if( disabled->command == cmd )
         break;

   if( disabled )
   {
      /*
       * Do they have access? 
       */
      if( disabled->level > get_trust( ch ) )
      {
         send_to_char( "You don't have the access to un-disable this command.\r\n", ch );
         return;
      }

      /*
       * They do, remove it 
       */
      if( first_disabled == disabled )
         first_disabled = disabled->next;
      else
      {
         DISABLED_DATA *tmp;

         for( tmp = first_disabled; tmp->next != disabled; tmp = tmp->next )
            ;
         tmp->next = disabled->next;
      }

      STRFREE( disabled->who );
      DISPOSE( disabled );
      save_disabled(  );
      send_to_char( "Command is now enabled.\r\n", ch );
   }
   else
   {
      /*
       * DOn't let them disable this command. 
       */
      if( !str_cmp( argument, "disable" ) )
      {
         send_to_char( "You cannot disable this command.\r\n", ch );
         return;
      }

      cmd = find_command( argument );

      if( !cmd )
      {
         send_to_char( "No such command", ch );
         return;
      }

      if( cmd->level > get_trust( ch ) )
      {
         send_to_char( "You don't have access to this command.\r\n", ch );
         return;
      }

      CREATE( disabled, DISABLED_DATA, 1 );
      disabled->command = cmd;
      disabled->who = STRALLOC( ch->name );
      disabled->level = get_trust( ch );
      disabled->next = first_disabled;
      first_disabled = disabled;

      ch_printf( ch, "%s is now disabled.\r\n", cmd->name );
      save_disabled(  );
      return;
   }
}

void load_disabled(  )
{
   FILE *fp;
   DISABLED_DATA *disabled;
   CMDTYPE *cmd;
   char *word;

   fp = fopen( DISABLE_FILE, "r" );

   if( !fp )
   {
      bug( "Unable to open disabled commands list." );
      return;
   }

   for( ;; )
   {
      word = fread_word( fp );

      if( !str_cmp( word, "End" ) )
         break;

      cmd = find_command( word );
      if( !cmd )
      {
         bug( "Unknown command in disabled list." );
         fread_number( fp );
         fread_word( fp );
         continue;
      }

      CREATE( disabled, DISABLED_DATA, 1 );
      disabled->command = cmd;
      disabled->level = fread_number( fp );
      disabled->who = STRALLOC( fread_word( fp ) );
      disabled->next = first_disabled;
      first_disabled = disabled;
   }

   fclose( fp );
}

void save_disabled(  )
{
   DISABLED_DATA *disabled;
   FILE *fp;

   fp = fopen( DISABLE_FILE, "w" );

   if( !fp )
   {
      bug( "Unable to open disabled commands list." );
      return;
   }

   for( disabled = first_disabled; disabled; disabled = disabled->next )
      fprintf( fp, "%s %d %s\n", disabled->command->name, disabled->level, disabled->who );

   fprintf( fp, "End\n" );

   fclose( fp );
}

bool check_disabled( CMDTYPE * cmd )
{
   DISABLED_DATA *disabled;

   if( !cmd )
      return FALSE;

   for( disabled = first_disabled; disabled; disabled = disabled->next )
      if( disabled->command->do_fun == cmd->do_fun )
         return TRUE;

   return FALSE;
}
