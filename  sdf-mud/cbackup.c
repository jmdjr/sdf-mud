/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ResortMud Version 5.0b programed By Badastaz and Garinan
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*******************************************************
		Crimson Blade Codebase
	Copyright 2000-2001 Noplex (John Bellone)
	      http://www.crimsonblade.org
		admin@crimsonblade.org
	   Coders: Noplex, Krowe, Callinon
		 Based on Smaug 1.4a
*******************************************************/

/*
** Backup Version 2.0
** ------------------
** Changes and Updates from 1.0
*******************************
** - Killed about 100 lines of code, much cleaner.
** + Now moves files to a specified backup directory.
** + Even easiler installation.
** + Got rid of that nasty 'Lag' warning.
*/

#include "mud.h"

char *backup_fname_strings[] = {
   "pfiles_backup.tgz", "system_backup.tgz", "area_backup.tgz", "build_backup.tgz",
   "src_backup.tgz"
};

char *backup_directory_strings[] = {
   "../player/", "../system/", "../area/", "../build/", "../src/", "../mbackup/"
};

typedef enum
{
   BACKUP_PFILES, BACKUP_SYSTEM, BACKUP_AREAS, BACKUP_BUILD, BACKUP_SRC, BACKUP_DIRECTORY
} backup_types;

/*
======
Locals
======
*/
void cbackup args( ( CHAR_DATA * ch, int type ) );

/*
===============
Backup command	-Nopey
===============
*/
void do_cbackup( CHAR_DATA * ch, char *argument )
{
   if( !IS_IMMORTAL( ch ) || IS_NPC( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "Syntax: cbackup <type>\r\n", ch );
      send_to_char( "Types being one of the following:\r\n" " pfiles systemfiles areas buildfiles src\r\n", ch );
      return;
   }

   if( !str_cmp( argument, "pfiles" ) )
   {
      cbackup( ch, BACKUP_PFILES );
      send_to_char( "Backup complete.\r\n", ch );
      return;
   }
   else if( !str_cmp( argument, "systemfiles" ) || !str_cmp( argument, "system" ) )
   {
      cbackup( ch, BACKUP_SYSTEM );
      send_to_char( "Backup complete.\r\n", ch );
      return;
   }
   else if( !str_cmp( argument, "area" ) || !str_cmp( argument, "areas" ) )
   {
      cbackup( ch, BACKUP_AREAS );
      send_to_char( "Backup complete.\r\n", ch );
      return;
   }
   else if( !str_cmp( argument, "build" ) || !str_cmp( argument, "buildfiles" ) )
   {
      cbackup( ch, BACKUP_BUILD );
      send_to_char( "Backup complete.\r\n", ch );
      return;
   }
   else if( !str_cmp( argument, "src" ) || !str_cmp( argument, "src" ) )
   {
      cbackup( ch, BACKUP_SRC );
      send_to_char( "Backup complete.\r\n", ch );
      return;
   }
   do_cbackup( ch, "" );
   return;
}

/*
================
Backup handler	-Nopey
================
*/
void cbackup( CHAR_DATA * ch, int type )
{
   char cmd[1024];
   char buf[MAX_INPUT_LENGTH];

   /*
    * backup command 
    */
   sprintf( cmd, "tar -zcf %s %s", backup_fname_strings[type], backup_directory_strings[type] );
   system( cmd );

   /*
    * move file to backup directory 
    */
   sprintf( cmd, "mv %s %s", backup_fname_strings[type], backup_directory_strings[BACKUP_DIRECTORY] );
   system( cmd );

   sprintf( buf, "BACKUP(%s): %s file written, moved to backup directory.", ch->name, backup_fname_strings[type] );
   log_string( buf );
   return;
}
