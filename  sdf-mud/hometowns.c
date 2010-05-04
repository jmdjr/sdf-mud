/*****************************************************
**     _________       __                           **
**     \_   ___ \_____|__| _____  ________  ___     **
**      /    \  \/_  __ \ |/     \/  ___/_ \/   \   **
**      \     \___|  | \/ |  | |  \___ \  / ) |  \  **
**       \______  /__| |__|__|_|  /____ \__/__|  /  **
**         ____\/____ _        \/ ___ \/      \/    **
**         \______   \ |_____  __| _/___            **
**          |    |  _/ |\__  \/ __ | __ \           **
**          |    |   \ |_/ __ \  / | ___/_          **
**          |_____  /__/____  /_  /___  /           **
**               \/Antipode\/  \/    \/             **
******************************************************
**         Crimson Blade Codebase (CbC)             **
**     (c) 2000-2002 John Bellone (Noplex)          **
**           Coders: Noplex, Krowe                  **
**        http://www.crimsonblade.org               **
*****************************************************/

/*
 * File: hometowns.c
 * Name: Hometowns and Nations module v2.2
 * Author: John 'Noplex' Bellone (jbellone@comcast.net)
 * Terms:
 * If this file is to be re-disributed; you must send an email
 * to the author. All headers above the #include calls must be
 * kept intact. All license requirements must be met. License
 * can be found in the included license.txt document or on the
 * website.
 * Description:
 * This is the long needed overhaul of my original Hometowns snippet;
 * The changes have been recorded in the changes.txt file included with
 * this snippet. This snippet also includes Crimson Blade's Nations module
 * which was adapted recently to fit the use of this particular snippet.
 */

#include "mud.h"

#ifdef FCLOSE
#undef FCLOSE
#endif
#define FCLOSE(fp) fclose(fp); fp=NULL;

#ifdef NULLSTR
#undef NULLSTR
#endif
#define NULLSTR(str) (!str || str[0] == '\0')

/* Globals */
int top_nation;
int top_hometown;

/* Locals */
static void checkup( void );

/*
 * save hometowns                              -Nopey
 * rewritten 3/20/02
 * new handling system 7/10/02
 */
void save_hometowns( void )
{
   FILE *fp = NULL;
   char filename[256];
   int i;

   /*
    * grab the filename for the hometowns 
    */
   sprintf( filename, "%shometowns.dat", SYSTEM_DIR );
   if( ( fp = fopen( filename, "w" ) ) == NULL )
   {
      bug( "save_hometowns(): cannot open %s for writing", filename );
      return;
   }

   /*
    * save each one individually 
    */
   for( i = 0; i < top_hometown; i++ )
   {
      HOMETOWN_DATA *htown = hometown_list[i];

      fprintf( fp, "#HOMETOWN\n" );
      fprintf( fp, "Name          %s~\n", htown->name );
      fprintf( fp, "Recall        %d\n", htown->recall );
      fprintf( fp, "Vnum          %d\n", htown->vnum );
      fprintf( fp, "Citizens      %d\n", htown->citizens );
      /*
       * nation support 
       */
      if( htown->nation )
         fprintf( fp, "Nation        %d\n", htown->nation->vnum );
      fprintf( fp, "Description    %s~\n", htown->description );
      fprintf( fp, "End\n" );
   }
   fprintf( fp, "#END\n" );
   /*
    * close; close; close it! 
    */
   FCLOSE( fp );
   return;
}

/*
 * load_hometowns                                -Nopey
 * rewritten 3/20/02
 * new handling system 7/10/02
 */
void load_hometowns( void )
{
   FILE *fp = NULL;
   char filename[256];

   /*
    * filename to load from 
    */
   sprintf( filename, "%shometowns.dat", SYSTEM_DIR );
   if( ( fp = fopen( filename, "r" ) ) == NULL )
   {
      bug( "load_hometowns(): cannot open %s for reading", filename );
      return;
   }

   top_hometown = 0;

   for( ;; )
   {
      char letter = fread_letter( fp );
      char *word;

      if( letter == '*' )
      {
         fread_to_eol( fp );
         break;
      }

      if( letter != '#' )
      {
         bug( "load_hometowns(): # not found" );
         return;
      }

      word = fread_word( fp );
      if( !str_cmp( word, "HOMETOWN" ) )
      {
         HOMETOWN_DATA *htown = fread_hometown( fp );

         if( !htown )
         {
            bug( "load_hometowns(): fread_hometown returned NULL htown; aborting load" );
            return;
         }
         /*
          * link em' up 
          */
         hometown_list[top_hometown] = htown;
         top_hometown++;
         break;
      }
      else if( !str_cmp( word, "END" ) )
         break;
      else
      {
         bug( "load_hometowns(): unknown section %s", word );
         break;
      }
   }
   FCLOSE( fp );
   return;
}

/*
 * read the hometown and stuffs                   -Nopey
 * rewritten 3/20/02
 */
HOMETOWN_DATA *fread_hometown( FILE * fp )
{
   HOMETOWN_DATA *htown = NULL;
   bool fMatch = FALSE;

   /*
    * create htown entry; initalize stuffs 
    */
   CREATE( htown, HOMETOWN_DATA, 1 );
   htown->name = STRALLOC( "None" );
   htown->description = STRALLOC( "None" );
   htown->nation = NULL;
   htown->citizens = 0;
   htown->vnum = -1;
   htown->nation_vnum = -1;
   htown->recall = -1;

   for( ;; )
   {
      char *word = feof( fp ) ? "End" : fread_word( fp );

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fread_to_eol( fp );
            break;

         case 'C':
            KEY( "Citizens", htown->citizens, fread_number( fp ) );
            break;

         case 'D':
            KEY( "Description", htown->description, fread_string( fp ) );
            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
            {
               if( NULLSTR( htown->name ) )
                  htown->name = STRALLOC( "Not Set" );

               return htown;
            }
            break;

         case 'N':
            KEY( "Name", htown->name, fread_string( fp ) );
            KEY( "Nation", htown->nation_vnum, fread_number( fp ) );
            break;

         case 'R':
            KEY( "Recall", htown->recall, fread_number( fp ) );
            break;

         case 'V':
            KEY( "Vnum", htown->vnum, fread_number( fp ) );
            break;
      }
      if( !fMatch )
      {
         bug( "fread_hometown(): unknown section %s", word );
         break;
      }
   }
   return NULL;
}

/*
 * save the nations                          -Nopey
 * rewritten 3/21/02
 * new handling system 7/10/02
 */
void save_nations( void )
{
   FILE *list = NULL;
   char fname[256];
   int i;

   /*
    * nations list of files 
    */
   sprintf( fname, "%snations.lst", NATION_DIR );
   if( ( list = fopen( fname, "w" ) ) == NULL )
   {
      bug( "save_nations(): cannot open file for writing %s; aborting saving", fname );
      return;
   }

   for( i = 0; i < top_nation; i++ )
   {
      NATION_DATA *nation = nation_list[i];

      if( nation_list[i] && ( !NULLSTR( nation->fname ) || !NULLSTR( nation->name ) ) )
      {
         FILE *fp = NULL;
         /*
          * two birds with one stone; eh? 
          */
         sprintf( fname, "%s%s", NATION_DIR, nation->fname );
         if( ( fp = fopen( fname, "w" ) ) == NULL )
         {
            bug( "save_nations(): cannot open file for writing %s; moving to next", fname );
            break;
         }
         /*
          * print to the list 
          */
         fprintf( list, "%s~\n", nation->fname );
         fprintf( list, "$\n" );
         /*
          * print to the file 
          */
         fprintf( fp, "Filename              %s~\n", nation->fname );
         fprintf( fp, "Name                  %s~\n", nation->name );
         fprintf( fp, "Leader                %s~\n", nation->leader );
         fprintf( fp, "Vnum                  %d\n", nation->vnum );
         /*
          * hometown support 
          */
         if( nation->hometown )
            fprintf( fp, "Hometown              %d\n", nation->hometown->vnum );
         /*
          * race support 
          */
         if( nation->race > -1 )
            fprintf( fp, "Race                  %d\n", nation->race );
         fprintf( fp, "Description            %s~\n", nation->description );
         fprintf( fp, "$\n" );
         FCLOSE( fp );
      }
      else  /* no filename */
      {
         bug( "save_nations(): nation (%d) has a null filename", i );
         continue;
      }
   }
   fprintf( list, "$\n" );
   FCLOSE( list );
   return;
}

/*
 * load the nationfiles                         -Nopey
 */
void load_nations( void )
{
   FILE *fp = NULL;
   FILE *list = NULL;
   char fname[256];

   /*
    * load from nations list 
    */
   sprintf( fname, "%snations.lst", NATION_DIR );
   if( ( list = fopen( fname, "r" ) ) == NULL )
   {
      bug( "load_nations(): cannot open %s for reading; aborting load", fname );
      return;
   }

   top_nation = 0;

   for( ;; )
   {
      char *filename = feof( list ) ? "$" : fread_string( list );
      /*
       * end of the file 
       */
      if( filename[0] == '*' )
      {
         fread_to_eol( fp );
         continue;
      }

      else if( filename[0] == '$' )
         break;

      /*
       * load each nation indiviually 
       */
      {
         NATION_DATA *nation = NULL;

         nation = fread_nation( filename );
         if( !nation )
         {
            bug( "load_nations(): error returning back nation from filename %s", filename );
            return;
         }
         /*
          * linker 
          */
         nation_list[top_nation] = nation;
         top_nation++;
         continue;
      }
   }
   FCLOSE( list );
   checkup(  );
   return;
}

/*
 * Read the nation from a specific file
 * in the /nations/ directory      -Nopey
 */
NATION_DATA *fread_nation( char *file )
{
   char filename[256];
   FILE *fp = NULL;
   NATION_DATA *nation = NULL;

   sprintf( filename, "../nations/%s", file );
   if( ( fp = fopen( filename, "r" ) ) == NULL )
   {
      bug( "fread_nation(): cannot open %s for reading", filename );
      FCLOSE( fp );  /* just in case */
      return NULL;
   }

   /*
    * initalize entry 
    */
   CREATE( nation, NATION_DATA, 1 );
   nation->name = STRALLOC( "None" );
   nation->leader = STRALLOC( "None" );
   nation->fname = STRALLOC( "None" );
   nation->description = STRALLOC( "None" );
   nation->hometown = NULL;
   nation->htown_vnum = -1;
   nation->vnum = -1;
   nation->race = -1;

   for( ;; )
   {
      bool fMatch = FALSE;
      char *word = feof( fp ) ? "END" : fread_word( fp );

      if( word[0] == '*' )
      {
         fread_to_eol( fp );
         break;
      }

      switch ( UPPER( word[0] ) )
      {
         case '$':
         {
            FCLOSE( fp );
            return nation;
         }
            break;

         case 'D':
            KEY( "Description", nation->description, fread_string( fp ) );
            break;

         case 'F':
            KEY( "Filename", nation->fname, fread_string( fp ) );
            break;

         case 'H':
            KEY( "Hometown", nation->htown_vnum, fread_number( fp ) );
            break;

         case 'L':
            KEY( "Leader", nation->leader, fread_string( fp ) );
            break;

         case 'N':
            KEY( "Name", nation->name, fread_string( fp ) );
            break;

         case 'R':
            KEY( "Race", nation->race, fread_number( fp ) );
            break;

         case 'V':
            KEY( "Vnum", nation->vnum, fread_number( fp ) );
            break;
      }

      if( !fMatch )
         bug( "fread_nation(): no match for %s", word );
   }
   FCLOSE( fp );
   return NULL;
}

/*
 * Assign correct hometown and nations -Nopey
 */
static void checkup( void )
{
   int i, j;

   /*
    * I hope this works... :P 
    */
   for( i = 0; i < top_hometown; i++ )
      for( j = 0; j < top_nation; j++ )
         if( ( nation_list[j]->htown_vnum > -1 && nation_list[j]->htown_vnum == hometown_list[i]->vnum )
             || ( hometown_list[i]->nation_vnum > -1 && hometown_list[i]->nation_vnum == nation_list[j]->vnum ) )
         {
            nation_list[j]->hometown = hometown_list[i];
            hometown_list[i]->nation = nation_list[j];
            break;
         }
   return;
}

/*
 * Look-up functions          -Nopey
 */
HOMETOWN_DATA *hometown_lookup( char *name )
{
   int i;

   if( is_number( name ) )
   {
      int vnum = atoi( name );

      for( i = 0; i < top_hometown; i++ )
         if( hometown_list[i]->vnum == vnum )
            return hometown_list[i];
   }
   else
   {
      for( i = 0; i < top_hometown; i++ )
         if( !str_cmp( hometown_list[i]->name, name ) )
            return hometown_list[i];
   }
   return NULL;
}

NATION_DATA *nation_lookup( char *name )
{
   int i;

   if( is_number( name ) )
   {
      int vnum = atoi( name );

      for( i = 0; i < top_nation; i++ )
         if( nation_list[i]->vnum == vnum )
            return nation_list[i];
   }
   else
   {
      for( i = 0; i < top_nation; i++ )
         if( !str_cmp( nation_list[i]->name, name ) || !str_cmp( nation_list[i]->fname, name ) )
            return nation_list[i];
   }
   return NULL;
}

void do_hometowns( CHAR_DATA * ch, char *argument )
{
   HOMETOWN_DATA *htown = NULL;
   bool found = FALSE;
   int i;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      pager_printf_color( ch, "&GHometown                Nation               Citizens\r\n" );
      pager_printf_color( ch, "&G_____________________________________________________\r\n" );
      for( i = 0; i < top_hometown; i++ )
      {
         found = TRUE;
         pager_printf_color( ch, "&W%-24s %-23s %d&G\r\n", hometown_list[i]->name, hometown_list[i]->nation ?
                             hometown_list[i]->nation->name : "None", hometown_list[i]->citizens );
      }

      if( !found )
         pager_printf_color( ch, "&RThere are currently no hometowns formed.&g\r\n" );
      else
         send_to_char_color( "&GFor more information use 'hometowns <name>'&g\r\n", ch );
   }
   else if( ( htown = hometown_lookup( argument ) ) != NULL )
   {
      pager_printf_color( ch, "&GHometown name:&g %s\r\n&GCitizens:&g %d\r\n", htown->name, htown->citizens );
      if( htown->description[0] != '\0' )
         pager_printf_color( ch, "&GDescription:&g\r\n%s\r\n", htown->description );
   }
   return;
}

void do_nations( CHAR_DATA * ch, char *argument )
{
   NATION_DATA *nation = NULL;
   int i;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      bool found = FALSE;

      pager_printf_color( ch, "&gNation                   Leader             Hometown\r\n" );
      pager_printf_color( ch, "&g____________________________________________________\r\n" );
      for( i = 0; i < top_nation; i++ )
      {
         found = TRUE;
         pager_printf_color( ch, "&W%-28s %-20s %s&g\r\n", nation_list[i]->name, nation_list[i]->leader,
                             nation_list[i]->hometown ? nation_list[i]->hometown->name : "None" );
      }

      if( !found )
         pager_printf_color( ch, "&RThere are currently no nations formed.&g\r\n" );
      else
         pager_printf_color( ch, "&gUse nations 'nation' for more information.\r\n" );
      return;
   }
   else if( ( nation = nation_lookup( argument ) ) == NULL )
   {
      pager_printf_color( ch, "&gNo such nation formed. Use 'nations' to get a list of all nations formed.\r\n" );
      return;
   }

   pager_printf_color( ch, "&gNation:&W %s\r\n&gLeader:&W %s\r\n", nation->name, nation->leader );
   if( IS_IMMORTAL( ch ) )
      pager_printf_color( ch, "&gFilename:&W %s\r\n", nation->fname );
   if( nation->hometown )
      pager_printf_color( ch, "&gHometown:&W %s\r\n", nation->hometown->name );
   if( nation->race > -1 )
      pager_printf_color( ch, "&gRace:&W %s\r\n", race_table[nation->race]->race_name );
   if( nation->description[0] != '\0' )
      pager_printf_color( ch, "&gDescription:&W\r\n%s&g\r\n", nation->description );

   return;
}

/*
 * OLC functions          -Nopey
 */
void do_makenation( CHAR_DATA * ch, char *argument )
{
   if( !IS_IMMORTAL( ch ) || IS_NPC( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "Syntax: makenation <name>\r\n", ch );
      return;
   }

   if( nation_lookup( argument ) )
   {
      send_to_char( "There is already a nation with that name.\r\n", ch );
      return;
   }

   {
      NATION_DATA *nation = NULL;
      char buf[MAX_INPUT_LENGTH];

      sprintf( buf, "%s.nation", argument );

      CREATE( nation, NATION_DATA, 1 );
      nation->fname = STRALLOC( buf );
      nation->name = STRALLOC( argument );
      nation->leader = STRALLOC( "None" );
      nation->description = STRALLOC( "None" );
      nation->vnum = top_nation;
      nation->hometown = NULL;
      nation->htown_vnum = -1;
      nation->race = -1;

      nation_list[top_nation] = nation;
      top_nation++;
      save_nations(  );
      send_to_char( "Done.\r\n", ch );
      return;
   }
}

/*
 * create hometown; takes care of
 * creating a hometown; and linking
 * it online  -Nopey
 */
void do_makehometown( CHAR_DATA * ch, char *argument )
{
   if( !IS_IMMORTAL( ch ) || IS_NPC( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "Syntax: makehometown <name>\r\n", ch );
      return;
   }

   if( hometown_lookup( argument ) )
   {
      send_to_char( "There is already a hometown with that name.\r\n", ch );
      return;
   }

   {
      HOMETOWN_DATA *htown = NULL;

      CREATE( htown, HOMETOWN_DATA, 1 );
      htown->name = STRALLOC( argument );
      htown->description = STRALLOC( "None" );
      htown->vnum = top_hometown;
      htown->nation = NULL;
      htown->nation_vnum = -1;
      htown->recall = -1;
      htown->citizens = -1;

      hometown_list[top_hometown] = htown;
      top_hometown++;
      save_hometowns(  );
      send_to_char( "Done.\r\n", ch );
      return;
   }
}

/*
 * olc edit hometown function
 * takes care of all the editing
 * of hometowns online   -Nopey
 */
void do_edithometown( CHAR_DATA * ch, char *argument )
{
   HOMETOWN_DATA *htown = NULL;
   char ht_arg[MAX_INPUT_LENGTH];
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, ht_arg );
   if( IS_NPC( ch ) || !IS_IMMORTAL( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   switch ( ch->substate )
   {
      case SUB_HTOWN_DESC:
      {
         start_editing( ch, htown->description );
         STRFREE( htown->description );
         htown->description = copy_buffer( ch );
         stop_editing( ch );
         ch->substate = ch->tempnum;
         return;
      }
         break;
   }

   if( NULLSTR( ht_arg ) )
   {
      send_to_char( "Syntax: edithometown <hometown> <field> <value>\r\n", ch );
      send_to_char( " Fields being one of the following:\r\n", ch );
      send_to_char( " name recall nation description\r\n", ch );
      return;
   }

   if( !str_cmp( ht_arg, "save" ) )
   {
      save_hometowns(  );
      send_to_char( "Done.\r\n", ch );
      return;
   }

   {
      if( ( htown = hometown_lookup( ht_arg ) ) == NULL )
      {
         send_to_char( "That's not a hometown vnum or name.\r\n", ch );
         return;
      }

      argument = one_argument( argument, arg );
      if( NULLSTR( arg ) || NULLSTR( argument ) )
      {
         send_to_char( "Syntax: edithometown <hometown> <field> <value>\r\n", ch );
         send_to_char( " Fields being one of the following:\r\n", ch );
         send_to_char( " name recall nation description\r\n", ch );
         return;
      }

      if( !str_cmp( arg, "name" ) )
      {
         if( is_number( argument ) )
         {
            send_to_char( "Name cannot contain any numerals.\r\n", ch );
            return;
         }
         htown->name = STRALLOC( argument );
         send_to_char( "Done.\r\n", ch );
         return;
      }
      else if( !str_cmp( arg, "description" ) )
      {
         if( ch->substate == SUB_REPEATCMD )
            ch->tempnum = SUB_REPEATCMD;
         else
            ch->tempnum = SUB_NONE;

         ch->substate = SUB_HTOWN_DESC;
         send_to_char( "Done.\r\n", ch );
         return;
      }
      else if( !str_cmp( arg, "recall" ) )
      {
         if( !is_number( argument ) )
         {
            send_to_char( "Recall vnum cannot contain any letters.\r\n", ch );
            return;
         }
         htown->recall = atoi( argument );
         send_to_char( "Done.\r\n", ch );
         return;
      }
      else if( !str_cmp( arg, "nation" ) )
      {
         NATION_DATA *nation = NULL;

         if( ( nation = nation_lookup( argument ) ) == NULL )
         {
            send_to_char( "That's not a nation vnum or name.\r\n", ch );
            return;
         }

         htown->nation_vnum = nation->vnum;
         htown->nation = nation;
         nation->htown_vnum = htown->vnum;
         nation->hometown = htown;
         save_nations(  );
         save_hometowns(  );
         send_to_char( "Done.\r\n", ch );
         return;
      }
   }
   do_edithometown( ch, "" );
   return;
}

/*
 * nation editing function; takes care of
 * editing nations online   -Nopey
 */
void do_editnation( CHAR_DATA * ch, char *argument )
{
   NATION_DATA *nation = NULL;
   char nat_arg[MAX_INPUT_LENGTH];
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, nat_arg );
   if( IS_NPC( ch ) || !IS_IMMORTAL( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   switch ( ch->substate )
   {
      case SUB_NATION_DESC:
      {
         start_editing( ch, nation->description );
         STRFREE( nation->description );
         nation->description = copy_buffer( ch );
         stop_editing( ch );
         ch->substate = ch->tempnum;
         return;
      }
         break;
   }

   if( NULLSTR( nat_arg ) )
   {
      send_to_char( "Syntax: editnation <nation> <field> <value>\r\n", ch );
      send_to_char( " Field being one of the following:\r\n", ch );
      send_to_char( " name leader hometown race description\r\n", ch );
      return;
   }

   if( !str_cmp( nat_arg, "save" ) )
   {
      save_nations(  );
      send_to_char( "Done.\r\n", ch );
      return;
   }

   {
      if( ( nation = nation_lookup( nat_arg ) ) == NULL )
      {
         send_to_char( "That's not a nation name or vnum.\r\n", ch );
         return;
      }

      argument = one_argument( argument, arg );
      if( NULLSTR( arg ) || NULLSTR( argument ) )
      {
         send_to_char( "Syntax: editnation <nation> <field> <value>\r\n", ch );
         send_to_char( " Field being one of the following:\r\n", ch );
         send_to_char( " name leader hometown race description\r\n", ch );
         return;
      }

      if( !str_cmp( arg, "name" ) )
      {
         if( is_number( argument ) )
         {
            send_to_char( "Names cannot include numerals.\r\n", ch );
            return;
         }
         nation->name = STRALLOC( argument );
         send_to_char( "Done.\r\n", ch );
         return;
      }
      else if( !str_cmp( arg, "description" ) )
      {
         if( ch->substate == SUB_REPEATCMD )
            ch->tempnum = SUB_REPEATCMD;
         else
            ch->tempnum = SUB_NONE;

         ch->substate = SUB_NATION_DESC;
         send_to_char( "Done.\r\n", ch );
         return;
      }
      else if( !str_cmp( arg, "leader" ) )
      {
         CHAR_DATA *vic = NULL;

         if( ( vic = get_char_world( supermob, argument ) ) == NULL )
         {
            send_to_char( "Character isn't online.\r\n", ch );
            return;
         }
         nation->leader = STRALLOC( vic->name );
         vic->pcdata->nation = nation;
         vic->pcdata->nation_vnum = nation->vnum;
         send_to_char( "Done.\r\n", ch );
         pager_printf_color( vic, "You are now the nation leader of %s.", nation->name );
         return;
      }
      else if( !str_cmp( arg, "hometown" ) )
      {
         HOMETOWN_DATA *htown = NULL;

         if( ( htown = hometown_lookup( argument ) ) == NULL )
         {
            send_to_char( "That's not a hometown name or vnum.\r\n", ch );
            return;
         }

         nation->htown_vnum = htown->vnum;
         nation->hometown = htown;
         htown->nation_vnum = nation->vnum;
         htown->nation = nation;
         save_nations(  );
         save_hometowns(  );
         send_to_char( "Done.\r\n", ch );
         return;
      }
      else if( !str_cmp( arg, "race" ) )
      {
         int x = 0;

         if( !str_cmp( argument, "clear" ) || !str_cmp( argument, "none" ) )
         {
            nation->race = -1;
            send_to_char( "Done.\r\n", ch );
            return;
         }

         for( x = 0; x < MAX_PC_RACE; x++ )
            if( !str_cmp( race_table[x]->race_name, argument ) )
            {
               nation->race = x;
               send_to_char( "Done.\r\n", ch );
               return;
            }

         send_to_char( "No such race with that name.\r\n", ch );
         return;
      }
   }
   do_editnation( ch, "" );
   return;
}

void do_destroynation( CHAR_DATA * ch, char *argument )
{
   int i;

   if( IS_NPC( ch ) || !IS_IMMORTAL( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   for( i = 0; i < top_nation; i++ )
      if( !str_cmp( nation_list[i]->name, argument ) )
      {
         if( !NULLSTR( nation_list[i]->name ) )
            STRFREE( nation_list[i]->name );
         if( !NULLSTR( nation_list[i]->fname ) )
            STRFREE( nation_list[i]->fname );
         if( !NULLSTR( nation_list[i]->description ) )
            STRFREE( nation_list[i]->description );
         if( !NULLSTR( nation_list[i]->leader ) )
            STRFREE( nation_list[i]->leader );
         if( nation_list[i]->hometown )
         {
            nation_list[i]->hometown->nation = NULL;
            nation_list[i]->hometown = NULL;
         }
         DISPOSE( nation_list[i] );
         nation_list[i] = NULL;
         save_nations(  );
         save_hometowns(  );
         send_to_char( "Done.\r\n", ch );
         return;
      }
   send_to_char( "Syntax: destroynation <vnum/name>\r\n", ch );
   return;
}

void do_destroyhometown( CHAR_DATA * ch, char *argument )
{
   int i;

   if( IS_NPC( ch ) || !IS_IMMORTAL( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   for( i = 0; i < top_hometown; i++ )
      if( !str_cmp( hometown_list[i]->name, argument ) )
      {
         if( !NULLSTR( hometown_list[i]->name ) )
            STRFREE( hometown_list[i]->name );
         if( hometown_list[i]->nation )
         {
            hometown_list[i]->nation->hometown = NULL;
            hometown_list[i]->nation = NULL;
         }
         DISPOSE( hometown_list[i] );
         save_hometowns(  );
         save_nations(  );
         send_to_char( "Done.\r\n", ch );
         return;
      }
   send_to_char( "Syntax: destroyhometown <vnum/name>\r\n", ch );
   return;
}

/*
 * mobprog for making someone a citizen  -Nopey
 */
void do_mp_makecitizen( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim = NULL;
   HOMETOWN_DATA *htown = NULL;
   char vic_name[MAX_INPUT_LENGTH];

   if( !IS_NPC( ch ) || ch->desc || IS_AFFECTED( ch, AFF_CHARM ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   argument = one_argument( argument, vic_name );
   if( NULLSTR( vic_name ) )
   {
      progbug( "mpmakecitizen(): null victim name", ch );
      return;
   }

   if( ( victim = get_char_room( ch, vic_name ) ) == NULL )
   {
      progbug( "mpmakecitizen(): that victim isn't in the same room", ch );
      return;
   }

   if( ( htown = hometown_lookup( argument ) ) == NULL )
   {
      progbug( "mpmakecitizen(): hometown_lookup returned NULL hometown", ch );
      return;
   }

   if( ( htown->nation && htown->nation->race > -1 ) && ( victim->race != htown->nation->race ) )
   {
      pager_printf_color( victim, "You cannot become a citizen of %s because your race is not of calibur.\r\n",
                          htown->name );
      return;
   }

   /*
    * they have a previous hometown 
    */
   if( victim->pcdata->hometown )
   {
      pager_printf_color( victim,
                          "You cannot become a citizen of %s because you are already a citizen of %s.\r\nGo revoke your citizenship at %s.",
                          htown->name, victim->pcdata->hometown->name, victim->pcdata->hometown->name );
      return;
   }

   /*
    * set them up 
    */
   victim->pcdata->hometown = htown;
   victim->pcdata->htown_vnum = htown->vnum;

   if( htown->nation )
   {
      victim->pcdata->nation = htown->nation;
      victim->pcdata->nation_vnum = htown->nation->vnum;
   }
   htown->citizens++;

   pager_printf_color( victim, "You are now a citizen of %s!\r\n", htown->name );
   save_hometowns(  );
   return;
}

/*
 * mobprog for revoking a citizenship -Nopey
 */
void do_mp_revokecitizen( CHAR_DATA * ch, char *argument )
{
   char vic_name[MAX_INPUT_LENGTH];
   CHAR_DATA *victim = NULL;

   if( !IS_NPC( ch ) || ch->desc || IS_AFFECTED( ch, AFF_CHARM ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   argument = one_argument( argument, vic_name );
   if( NULLSTR( vic_name ) )
   {
      progbug( "mp_revokecitizen(): vic_name null", ch );
      return;
   }

   if( ( victim = get_char_room( ch, vic_name ) ) == NULL )
   {
      progbug( "mp_revokecitizen(): victim not in room", ch );
      return;
   }

   if( victim->pcdata->hometown == NULL || ( victim->pcdata->hometown &&
                                             str_cmp( victim->pcdata->hometown->name, argument ) ) )
   {
      pager_printf_color( victim, "Your not a citizen of %s.\r\n", argument );
      return;
   }

   if( victim->pcdata->hometown )
   {
      victim->pcdata->hometown->citizens--;
      victim->pcdata->hometown = NULL;
      victim->pcdata->htown_vnum = -1;
      if( victim->pcdata->nation )
      {
         victim->pcdata->nation = NULL;
         victim->pcdata->nation_vnum = -1;
      }
      pager_printf_color( victim, "You are no longer a citizen of %s.\r\n", argument );
   }
   save_hometowns(  );
   return;
}

/* update the loading character correctly -Nopey */
void update_char_hometown( CHAR_DATA * ch )
{
   char buf[MAX_INPUT_LENGTH];
   int i;

   if( ch->pcdata->htown_vnum > -1 )
   {
      for( i = 0; i < top_hometown; i++ )
         if( hometown_list[i]->vnum == ch->pcdata->htown_vnum )
         {
            ch->pcdata->hometown = hometown_list[i];
            if( hometown_list[i]->nation )
            {
               ch->pcdata->nation = hometown_list[i]->nation;
               ch->pcdata->nation_vnum = hometown_list[i]->nation->vnum;
            }
            return;
         }
      if( ch->pcdata->hometown == NULL )
      {
         sprintf( buf, "update_char_hometown(): (%s) invaild vnum for hometown (%d)", ch->name, ch->pcdata->htown_vnum );
         bug( buf, 0 );
         ch->pcdata->htown_vnum = -1;
      }
   }
   return;
}
