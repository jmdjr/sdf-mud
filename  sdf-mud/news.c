/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ResortMud Version 5.0b programed By Badastaz and Garinan
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/
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
 * File: news.c
 * Name: Extended News (v3.03)
 * Author: John 'Noplex' Bellone (jbellone@comcast.net)
 * Terms:
 * If this file is to be re-disributed; you must send an email
 * to the author. All headers above the #include calls must be
 * kept intact. All license requirements must be met. License
 * can be found in the included license.txt document or on the
 * website.
 * Description:
 * This is Crimson Blade's extended news module. It allows for news
 * cataglories to be created; each having it's own different filesystem
 * where news can be posted. It also allows for each to be specified a
 * specific path and filename where the news-textdump of HTML can be
 * sent to.
 * The news commands are tied directly into the command interpreter; so
 * news-types are instantaniously added; removed; and edited.
 */
#include <time.h>
#include "mud.h"

#ifndef FCLOSE
#define FCLOSE(fp)  fclose(fp); fp=NULL;
#endif

#ifndef NULLSTR
#define NULLSTR(str)        (!str || str[0] == '\0')
#endif

/* locals */
static char local_buf[MAX_INPUT_LENGTH];
static char *news_file_version;
static int top_news_type;

/* the lovely; and useful; command table */
char *news_command_table[NEWS_MAX_TYPES];

/* olc editnews command */
void do_editnews( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

   if( IS_NPC( ch ) || !IS_IMMORTAL( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   switch ( ch->substate )
   {
      default:
         break;

      case SUB_NEWS_POST:
      {
         NEWS *news = NULL;
         NEWS_TYPE *type = NULL;

         news = ch->dest_buf;
         type = news->temp;
         STRFREE( news->post );
         news->post = copy_buffer( ch );
         stop_editing( ch );
         LINK( news, type->first_news, type->last_news, next, prev );
         ch->substate = ch->tempnum;
         news->temp = NULL;
         renumber_news(  );
         save_news(  );
         return;
      }
         break;

      case SUB_NEWS_EDIT:
      {
         NEWS *news = NULL;

         news = ch->dest_buf;
         STRFREE( news->post );
         news->post = copy_buffer( ch );
         stop_editing( ch );
         ch->substate = ch->tempnum;
         renumber_news(  );
         save_news(  );
         return;
      }
         break;
   }

   argument = one_argument( argument, arg );

   if( NULLSTR( arg ) )
   {
      send_to_char( "Syntax: editnews addtype <name>\r\n"
                    "        editnews addnews <type> <subject>\r\n"
                    "        editnews removetype <number>\r\n"
                    "        editnews removenews <type> <number>\r\n"
                    "        editnews edittype <field> <value>\r\n"
                    "        editnews editnews <type> <number> <new subject [optional]>\r\n"
                    " Fields being one of the following:\r\n"
                    "name header cmd_name lock org_name org_type\r\n"
                    "post_level view_level owner addposter remposter\r\n"
                    "html_max_view html_news_path html_txt_filename\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "addtype" ) )
   {
      NEWS_TYPE *type = NULL;
      int i;

      if( NULLSTR( argument ) )
      {
         send_to_char( "Syntax: editnews addtype <name>\r\n", ch );
         return;
      }

      if( top_news_type >= NEWS_MAX_TYPES )
      {
         send_to_char( "There are too many news types.\r\n", ch );
         return;
      }

      CREATE( type, NEWS_TYPE, 1 );
      type->name = STRALLOC( argument );
      type->cmd_name = STRALLOC( argument );
      type->owner = STRALLOC( "" );
      type->org_name = STRALLOC( "" );
      for( i = 0; i < MAX_POSTERS; i++ )
         type->posters[i] = NULL;
      type->html_news_path = STRALLOC( "" );
      type->html_txt_fname = STRALLOC( "" );
      type->vnum = top_news_type++;
      type->html_max_view = -1;
      news_command_table[type->vnum] = STRALLOC( type->cmd_name );

      LINK( type, first_news_type, last_news_type, next, prev );
      send_to_char( "Done.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "removetype" ) )
   {
      NEWS_TYPE *type = NULL;
      int i;

      if( NULLSTR( argument ) )
      {
         send_to_char( "Syntax: editnews removetype <name>\r\n", ch );
         return;
      }

      if( ( type = figure_type( argument ) ) == NULL )
      {
         send_to_char( "Invaild news-type.\r\n", ch );
         return;
      }

      /*
       * forgot about this... 00ps! 
       */
      if( !NULLSTR( type->owner ) && ( str_cmp( type->owner, ch->name ) && get_trust( ch ) < LEVEL_IMPLEMENTOR ) )
      {
         send_to_char( "You don't have ownership of this news-type.\r\n", ch );
         return;
      }

      UNLINK( type, first_news_type, last_news_type, next, prev );
      if( !NULLSTR( type->name ) )
         STRFREE( type->name );
      if( !NULLSTR( type->header ) )
         STRFREE( type->header );
      if( !NULLSTR( type->cmd_name ) )
         STRFREE( type->cmd_name );
      if( !NULLSTR( type->org_name ) )
         STRFREE( type->org_name );
      if( !NULLSTR( type->owner ) )
         STRFREE( type->owner );
      if( !NULLSTR( type->html_news_path ) )
         STRFREE( type->html_news_path );
      if( !NULLSTR( type->html_txt_fname ) )
         STRFREE( type->html_txt_fname );
      if( !NULLSTR( news_command_table[type->vnum] ) )
         STRFREE( news_command_table[type->vnum] );
      {
         NEWS *news = NULL;
         void *next;

         for( news = type->first_news; news; news = next )
         {
            next = news->next;
            UNLINK( news, type->first_news, type->last_news, next, prev );
            if( news->name )
               STRFREE( news->name );
            if( news->title )
               STRFREE( news->title );
            if( news->date )
               STRFREE( news->date );
            if( news->post )
               STRFREE( news->post );
            DISPOSE( news );
         }
      }
      for( i = 0; i < MAX_POSTERS; i++ )
         STRFREE( type->posters[i] );
      DISPOSE( type );
      top_news_type--;
      renumber_news(  );
      save_news(  );
      send_to_char( "Done.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "edittype" ) )
   {
      char arg2[MAX_INPUT_LENGTH];
      char arg3[MAX_INPUT_LENGTH];
      NEWS_TYPE *type = NULL;

      argument = one_argument( argument, arg2 );
      argument = one_argument( argument, arg3 );
      if( NULLSTR( arg2 ) || NULLSTR( arg3 ) )
      {
         send_to_char( "Syntax: editnews edittype <type> <field> <value>\r\n", ch );
         send_to_char( "Fields being one of the following:\r\n"
                       "name header cmd_name lock org_name org_type\r\n"
                       "post_level view_level owner addposter remposter\r\n"
                       "html_max_view html_news_path html_txt_filename\r\n", ch );
         return;
      }

      if( ( type = figure_type( arg2 ) ) == NULL )
      {
         send_to_char( "Invaild news-type.\r\n", ch );
         return;
      }

      if( !NULLSTR( type->owner ) && ( str_cmp( type->owner, ch->name ) && get_trust( ch ) < LEVEL_IMPLEMENTOR ) )
      {
         send_to_char( "You don't have ownership of this news-type.\r\n", ch );
         return;
      }

      if( !str_cmp( arg3, "cmd_name" ) )
      {
         type->cmd_name = STRALLOC( argument );
         news_command_table[type->vnum] = STRALLOC( type->cmd_name );
         send_to_char( "Done.\r\n", ch );
         save_news(  );
         return;
      }
      else if( !str_cmp( arg3, "name" ) )
      {
         type->name = STRALLOC( argument );
         send_to_char( "Done.\r\n", ch );
         save_news(  );
         return;
      }
      else if( !str_cmp( arg3, "header" ) )
      {
         sprintf( local_buf, "%s\r\n", argument );
         type->header = STRALLOC( local_buf );
         send_to_char( "Done.\r\n", ch );
         save_news(  );
         return;
      }
      else if( !str_cmp( arg3, "owner" ) )
      {
         if( NULLSTR( argument ) )
         {
            send_to_char( "Syntax: editnews edittype <type> owner <clear/name>\r\n", ch );
            return;
         }

         if( !str_cmp( argument, "none" ) || !str_cmp( argument, "clear" ) )
            type->owner = STRALLOC( "" );
         else
            type->owner = STRALLOC( argument );
         send_to_char( "Done.\r\n", ch );
         save_news(  );
         return;
      }
      else if( !str_cmp( arg3, "post_level" ) )
      {
         if( !str_cmp( argument, "clear" ) || !str_cmp( argument, "none" ) )
            type->post_level = -1;
         else
         {
            if( type->post_level > get_trust( ch ) )
            {
               send_to_char( "You cannot make an post-level higher than your own.\r\n", ch );
               return;
            }

            if( type->post_level > MAX_LEVEL )
            {
               pager_printf_color( ch, "The post-level must be between 0 - %d.", MAX_LEVEL );
               return;
            }
            type->post_level = atoi( argument );
            send_to_char( "Done.\r\n", ch );
            save_news(  );
            return;
         }
      }
      else if( !str_cmp( arg3, "view_level" ) )
      {
         if( !str_cmp( argument, "clear" ) || !str_cmp( argument, "none" ) )
            type->view_level = -1;
         else
         {
            if( type->view_level > get_trust( ch ) )
            {
               send_to_char( "You cannot make an view-level higher than your own.\r\n", ch );
               return;
            }

            if( type->view_level > MAX_LEVEL )
            {
               pager_printf_color( ch, "The view-level must be between 0 - %d.", MAX_LEVEL );
               return;
            }
            type->view_level = atoi( argument );
            send_to_char( "Done.\r\n", ch );
            save_news(  );
            return;
         }
      }
      else if( !str_cmp( arg3, "remposter" ) )
      {
         int i;

         if( !type->locked )
         {
            send_to_char( "This news-type isn't locked.\r\n", ch );
            send_to_char( "Use 'editnews <type> lock' to toggle lock mode.\r\n", ch );
            return;
         }

         for( i = 0; i < MAX_POSTERS; i++ )
            if( !str_cmp( type->posters[i], argument ) )
            {
               STRFREE( type->posters[i] );
               send_to_char( "Done.\r\n", ch );
               save_news(  );
               return;
            }

         send_to_char( "No such poster.\r\n", ch );
         return;
      }
      else if( !str_cmp( arg3, "addposter" ) )
      {
         int i;

         if( !type->locked )
         {
            send_to_char( "This news-type isn't locked.\r\n", ch );
            send_to_char( "Use 'editnews <type> lock' to toggle lock mode.\r\n", ch );
            return;
         }

         for( i = 0; i < MAX_POSTERS; i++ )
            if( type->posters[i] == '\0' )
            {
               type->posters[i] = STRALLOC( argument );
               send_to_char( "Done.\r\n", ch );
               save_news(  );
               return;
            }

         send_to_char( "Poster slots are full.\r\n", ch );
         return;
      }
      else if( !str_cmp( arg3, "lock" ) )
      {
         type->locked = !type->locked;

         if( !type->locked )
         {
            int i;

            for( i = 0; type->posters[i] != '\0'; i++ )
               STRFREE( type->posters[i] );
         }
         send_to_char( "Done.\r\n", ch );
         save_news(  );
         return;
      }
      else if( !str_cmp( arg3, "org_type" ) )
      {
         if( NULLSTR( argument ) )
         {
            send_to_char( "Organization types are:\r\n" "clan guild order\r\n", ch );
            return;
         }

         if( !str_cmp( argument, "clan" ) )
         {
            type->org_type = CLAN_PLAIN;
            send_to_char( "Done.\r\n", ch );
         }
         else if( !str_cmp( argument, "guild" ) )
         {
            type->org_type = CLAN_GUILD;
            send_to_char( "Done.\r\n", ch );
         }
         else if( !str_cmp( argument, "order" ) )
         {
            type->org_type = CLAN_ORDER;
            send_to_char( "Done.\r\n", ch );
         }
         else if( !str_cmp( argument, "council" ) )
         {
            type->org_type = CLAN_COUNCIL;
            send_to_char( "Done.\r\n", ch );
         }
         save_news(  );
         return;
      }
      else if( !str_cmp( arg3, "org_name" ) )
      {
         if( type->org_type == -1 )
         {
            send_to_char( "You must first sent an organization type.\r\n", ch );
            return;
         }

         if( NULLSTR( argument ) || !str_cmp( argument, "clear" ) || !str_cmp( argument, "none" ) )
            type->org_name[0] = '\0';
         else
            type->org_name = STRALLOC( argument );
         send_to_char( "Done.\r\n", ch );
         return;
      }
      else
      {
         send_to_char( "Syntax: editnews edittype <type> <field> <value>\r\n", ch );
         send_to_char( "Fields being one of the following:\r\n"
                       "name header cmd_name lock org_name org_type\r\n"
                       "post_level view_level owner addposter remposter\r\n", ch );
         return;
      }
   }

   if( !str_cmp( arg, "addnews" ) )
   {
      char arg2[MAX_INPUT_LENGTH];
      NEWS_TYPE *type = NULL;
      NEWS *news = NULL;

      argument = one_argument( argument, arg2 );

      if( NULLSTR( arg2 ) || NULLSTR( argument ) )
      {
         send_to_char( "Syntax: editnews addnews <type> <subject>\r\n", ch );
         return;
      }

      if( ( type = figure_type( arg2 ) ) == NULL )
      {
         send_to_char( "Invaild news-type. Use 'newstypes' to get a vaild listing.\r\n", ch );
         return;
      }

      if( !can_post_news( ch, type ) )
      {
         send_to_char( "You cannot add-news to this news-type.\r\n", ch );
         return;
      }

      CREATE( news, NEWS, 1 );
      news->title = STRALLOC( argument );
      news->name = STRALLOC( ch->name );
      news->date = STRALLOC( stamp_time(  ) );
      news->post = STRALLOC( "" );

      /*
       * pop character into a writing buffer 
       */
      if( ch->substate == SUB_REPEATCMD )
         ch->tempnum = SUB_REPEATCMD;
      else
         ch->tempnum = SUB_NONE;

      ch->substate = SUB_NEWS_POST;
      news->temp = type;
      ch->dest_buf = news;
      start_editing( ch, news->post );
      return;
   }

   if( !str_cmp( arg, "editnews" ) )
   {
      char arg2[MAX_INPUT_LENGTH];
      char arg3[MAX_INPUT_LENGTH];
      NEWS *news = NULL;
      NEWS_TYPE *type = NULL;

      argument = one_argument( argument, arg2 );
      argument = one_argument( argument, arg3 );
      if( NULLSTR( arg2 ) )
      {
         send_to_char( "Syntax: editnews editnews <type> <number> <new subject [optional]>\r\n", ch );
         return;
      }

      /*
       * changed for new -newstype- indexing - 5/5/02 
       */
      if( ( type = figure_type( arg2 ) ) == NULL )
      {
         send_to_char( "Invaild news-type. Use 'newstypes' to get a vaild listing.\r\n", ch );
         return;
      }

      if( ( news = grab_news( type, arg3 ) ) == NULL )
      {
         pager_printf_color( ch, "That's not a vaild news number.\r\nUse '%s' to view the vaild numbers.\r\n",
                             type->cmd_name );
         return;
      }

      /*
       * a changed title 
       */
      if( !NULLSTR( argument ) )
         news->title = STRALLOC( argument );

      /*
       * new date news was edited 
       */
      news->date = STRALLOC( stamp_time(  ) );
      /*
       * pop character into a writing buffer 
       */
      if( ch->substate == SUB_REPEATCMD )
         ch->tempnum = SUB_REPEATCMD;
      else
         ch->tempnum = SUB_NONE;

      ch->substate = SUB_NEWS_EDIT;
      ch->dest_buf = news;
      start_editing( ch, news->post );
      return;
   }

   if( !str_cmp( arg, "removenews" ) )
   {
      char arg2[MAX_INPUT_LENGTH];
      NEWS *news = NULL;
      NEWS_TYPE *type = NULL;

      argument = one_argument( argument, arg2 );
      if( NULLSTR( argument ) || NULLSTR( arg2 ) )
      {
         send_to_char( "Syntax: editnews remove <number>\r\n", ch );
         return;
      }

      /*
       * changed for new -newstype- indexing - 5/5/02 
       */
      if( ( type = figure_type( arg2 ) ) == NULL )
      {
         send_to_char( "Invaild news-type. Use 'newstypes' to get a vaild listing.\r\n", ch );
         return;
      }

      if( ( news = grab_news( type, argument ) ) == NULL )
      {
         send_to_char( "Type 'news' to gain a list of the news numbers.\r\n", ch );
         return;
      }

      UNLINK( news, type->first_news, type->last_news, next, prev );
      if( !NULLSTR( news->name ) )
         STRFREE( news->name );
      if( !NULLSTR( news->title ) )
         STRFREE( news->title );
      if( !NULLSTR( news->date ) )
         STRFREE( news->date );
      if( !NULLSTR( news->post ) )
         STRFREE( news->post );
      DISPOSE( news );
      renumber_news(  );
      save_news(  );
      send_to_char( "Done.\r\n", ch );
      return;
   }
}

/* figure the type of a newstype by the vnum
 * or by the cmd_name off the news_cmd_table -Nopey */
NEWS_TYPE *figure_type( char *str )
{
   if( is_number( str ) )
   {
      NEWS_TYPE *type = NULL;
      int number = atoi( str );

      /*
       * poll the list for the vnum 
       */
      for( type = first_news_type; type; type = type->next )
         if( type->vnum == number )
            return type;
   }
   else  /* a cmd name */
   {
      NEWS_TYPE *type = NULL;
      int i;

      /*
       * poll the cmd_name array for word 
       */
      for( i = 0; i < top_news_type; i++ )
         if( !str_cmp( str, news_command_table[i] ) )
         {
            for( type = first_news_type; type; type = type->next )
               if( type->vnum == i )
                  return type;
         }
   }
   return NULL;
}

/* Snatch news up from the linked list */
NEWS *grab_news( NEWS_TYPE * type, char *str )
{
   NEWS *news = NULL;

   for( news = type->first_news; news; news = news->next )
   {
      if( news->number == atoi( str ) )
         return news;
   }
   return NULL;
}

/* display a full news to the character */
/* updated for the new display type 5/1/02 */
void display_news( CHAR_DATA * ch, NEWS * news, NEWS_TYPE * type )
{
   send_to_pager_color( type->header, ch );
   send_to_pager_color( NEWS_HEADER, ch );
   pager_printf_color( ch, "&G&Y[&W%2d&Y]  &W%-8s &Y@&W%-11s &Y%s&g\r\n", news->number, news->name, news->date,
                       news->title );
   send_to_pager_color( news->post, ch );
   return;
}

/* renumber the news */
/* changed for new indexing - 5/5/02 */
void renumber_news( void )
{
   NEWS_TYPE *type = NULL;
   NEWS *news = NULL;
   int i, j;

   for( i = 0; i < top_news_type; i++ )
      if( news_command_table[i] )
         STRFREE( news_command_table[i] );

   top_news_type = 0;

   for( type = first_news_type; type; type = type->next )
   {
      type->vnum = top_news_type++;
      news_command_table[type->vnum] = STRALLOC( type->cmd_name );

      for( j = 0, news = type->first_news; news; news = news->next )
      {
         j++;
         news->number = j;
         news->type = type->vnum;
      }
   }
   return;
}

/* save the linked list */
/* changed for new indexing - 5/5/02 */
void save_news( void )
{
   NEWS *news = NULL;
   NEWS_TYPE *type = NULL;
   FILE *fp = NULL;
   char filename[256];
   int i;

   sprintf( filename, "%s%s", SYSTEM_DIR, NEWS_FILE );
   if( ( fp = fopen( filename, "w" ) ) == NULL )
   {
      perror( "save_news(): cannot open file" );
      return;
   }

   fprintf( fp, "#VERSION         %s~\n", NEWS_VERSION );
   for( type = first_news_type; type; type = type->next )
   {
      fprintf( fp, "#NEWSTYPE\n" );
      fprintf( fp, "Name          %s~\n", type->name );
      fprintf( fp, "Cmd_Name      %s~\n", type->cmd_name );
      fprintf( fp, "Header        %s~\n", type->header );
      if( !NULLSTR( type->org_name ) )
         fprintf( fp, "Org_Name      %s~\n", type->org_name );
      if( !NULLSTR( type->owner ) )
         fprintf( fp, "Owner         %s~\n", type->owner );
/*   if(!NULLSTR(type->html_news_path))
    fprintf(fp, "HtmlPath      %s~\n", type->html_news_path);
   if(!NULLSTR(type->html_txt_fname))
    fprintf(fp, "HtmlFname     %s~\n", type->html_txt_fname);
    fprintf(fp, "HtmlMaxView   %d\n", type->html_max_view); */
      fprintf( fp, "Org_Type      %d\n", type->org_type );
      fprintf( fp, "Locked        %d\n", type->locked );
      fprintf( fp, "Vnum          %d\n", type->vnum );
      fprintf( fp, "Post_Level    %d\n", type->post_level );
      fprintf( fp, "View_Level    %d\n", type->view_level );
      for( i = 0; i < MAX_POSTERS; i++ )
         if( !NULLSTR( type->posters[i] ) )
            fprintf( fp, "Poster          %s~\n", type->posters[i] );
      fprintf( fp, "End\n" );
      for( news = type->first_news; news; news = news->next )
      {
         fprintf( fp, "#NEWS\n" );
         fprintf( fp, "Title    %s~\n", news->title );
         fprintf( fp, "Name     %s~\n", news->name );
         fprintf( fp, "Date     %s~\n", news->date );
         fprintf( fp, "Type     %d\n", news->type );
         fprintf( fp, "POST     %s~\n", news->post );
         fprintf( fp, "End\n" );
      }
   }
   FCLOSE( fp );
   /*
    * html_news(); 
    */
   return;
}

/* load the linked list from disk */
void load_news( void )
{
   FILE *fp = NULL;
   char filename[256];

   sprintf( filename, "%s%s", SYSTEM_DIR, NEWS_FILE );
   if( ( fp = fopen( filename, "r" ) ) == NULL )
   {
      perror( "load_news(): cannot open file" );
      return;
   }

   for( ;; )
   {
      NEWS_TYPE *type = NULL;
      NEWS *news = NULL;
      char *word;
      char letter = fread_letter( fp );

      if( letter == '*' )
      {
         fread_to_eol( fp );
         continue;
      }

      if( letter != '#' )
      {
         bug( "load_news(): # not found (%c)", letter );
         break;
      }

      word = fread_word( fp );

      /*
       * added for new indexing - 5/5/02 
       */
      /*
       * not used yet -Nopey 
       */
      if( !str_cmp( word, "VERSION" ) )
      {
         news_file_version = fread_string( fp );
         continue;
      }
      else if( !str_cmp( word, "NEWSTYPE" ) )
      {
         CREATE( type, NEWS_TYPE, 1 );
         type->org_type = -1;
         type->post_level = -1;
         type->view_level = -1;
         type->html_max_view = -1;
         type->locked = 0;

         fread_news_type( type, fp );
         LINK( type, first_news_type, last_news_type, next, prev );
         continue;
      }
      else if( !str_cmp( word, "NEWS" ) )
      {
         CREATE( news, NEWS, 1 );
         news->type = -1;

         fread_news( news, fp );
         link_news_to_type( news );
         continue;
      }
      else if( !str_cmp( word, "END" ) )
         break;
      else
      {
         bug( "load_news(): unknown section %s", word );
         continue;
      }
   }
   FCLOSE( fp );
   renumber_news(  );
   return;
}

/* added for new indexing - 5/5/02 - Nopey */
/* adds the news to to the correct newstype */
void link_news_to_type( NEWS * news )
{
   NEWS_TYPE *type = NULL;

   sprintf( local_buf, "%d", news->type );
   if( ( type = figure_type( local_buf ) ) == NULL )
   {
      bug( "link_news_to_type(): invaild news->type %d", news->type );
      return;
   }
   LINK( news, type->first_news, type->last_news, next, prev );
   return;
}

void fread_news( NEWS * news, FILE * fp )
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
            fread_to_eol( fp );
            break;

         case 'D':
            KEY( "Date", news->date, fread_string( fp ) );
            break;

         case 'E':
            if( !str_cmp( word, "END" ) )
            {
               if( !news->name )
                  news->name = STRALLOC( "Unknown" );

               if( !news->date )
               {
                  news->date = STRALLOC( stamp_time(  ) );
               }

               if( !news->title )
                  news->title = STRALLOC( "News Post" );

               if( news->type <= -1 )
                  news->type = 0;
               return;
            }
            break;

         case 'N':
            KEY( "Name", news->name, fread_string( fp ) );
            break;

         case 'P':
            if( !str_cmp( word, "POST" ) )
            {
               fMatch = TRUE;
               news->post = fread_string( fp );
               break;
            }
            break;

         case 'T':
            KEY( "Title", news->title, fread_string( fp ) );
            KEY( "Type", news->type, fread_number( fp ) );
            break;
      }

      if( !fMatch )
         bug( "fread_news(): no match: %s", word );
   }
}

/* added for new index - 5/5/02 - Nopey */
void fread_news_type( NEWS_TYPE * type, FILE * fp )
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
            fread_to_eol( fp );
            break;

         case 'C':
            KEY( "Cmd_Name", type->cmd_name, fread_string( fp ) );
            break;

         case 'E':
            if( !str_cmp( word, "END" ) )
            {
               if( !NULLSTR( type->name ) )
                  type->name = STRALLOC( "Unknown" );

               if( !NULLSTR( type->owner ) )
                  type->owner = STRALLOC( "" );

               if( !NULLSTR( type->org_name ) )
                  type->org_name = STRALLOC( "" );

               return;
            }
            break;

         case 'H':
            KEY( "Header", type->header, fread_string( fp ) );
            KEY( "HtmlFname", type->html_txt_fname, fread_string( fp ) );
            KEY( "HtmlMaxView", type->html_max_view, fread_number( fp ) );
            KEY( "HtmlPath", type->html_news_path, fread_string( fp ) );
            break;

         case 'L':
            KEY( "Locked", type->locked, fread_number( fp ) );
            break;

         case 'N':
            KEY( "Name", type->name, fread_string( fp ) );
            break;

         case 'O':
            KEY( "Org_Name", type->org_name, fread_string( fp ) );
            KEY( "Org_Type", type->org_type, fread_number( fp ) );
            KEY( "Owner", type->owner, fread_string( fp ) );
            break;

         case 'P':
            if( !str_cmp( word, "Poster" ) )
            {
               int i;

               for( i = 0; type->posters[i] != NULL; i++ )
                  type->posters[i] = fread_string( fp );
               break;
            }
            KEY( "Post_Level", type->post_level, fread_number( fp ) );
            break;

         case 'V':
            KEY( "View_Level", type->view_level, fread_number( fp ) );
            KEY( "Vnum", type->vnum, fread_number( fp ) );
            break;
      }

      if( !fMatch )
         bug( "fread_news_type(): no match: %s", word );
   }
}

/* stamp date in mm/dd/yy. return string */
char *stamp_time( void )
{
   static char buf[128];
   struct tm *ntime = localtime( &current_time );

   sprintf( buf, "%d/%d/%2.2d", ntime->tm_mon + 1, ntime->tm_mday, ntime->tm_year + 1900 );
   return buf;
}

/* write html for a specific news-type        -Nopey */
void html_news( void )
{
   NEWS_TYPE *type = NULL;
   FILE *fp = NULL;

   for( type = first_news_type; type; type = type->next )
      if( !NULLSTR( type->html_news_path ) && !NULLSTR( type->html_txt_fname ) && type->html_max_view > 0 )
      {
         char filename[256];

         sprintf( filename, "%s%s", NEWS_HTML_DIR, type->html_txt_fname );
         if( ( fp = fopen( filename, "w" ) ) == NULL )
         {
            bug( "html_news(): cannot open %s for writing", filename );
            return;
         }
         snarf_news( fp, type );
         FCLOSE( fp );
      }
   return;
}

/*
 * rip it apart! =\ -Nopey
 */
void snarf_news( FILE * fp, NEWS_TYPE * type )
{
   NEWS *news = NULL;
   int i = 0;

   for( news = type->last_news; i < type->html_max_view; news = news->prev, i++ )
   {
      fprintf( fp, "<table align='center' width='399' border='1'><tr><td bgcolor='#990000'>\n" );
      fprintf( fp, "<font face='verdana' color='#FFFFFF' size='2'>%s</font></tr></td>\n", news->title );
      fprintf( fp, "<tr><td bgcolor='#000000'><font face='verdana' color='#990000' size='1'>\n" );
      fprintf( fp, "<div align='center'>&raquo; %s posted on %s</font></div></td></tr>\n", news->name, news->date );
      fprintf( fp, "<tr><td valign='top' bgcolor='#000000'><font size='2' color='#FFFFFF' face='verdana'>\n" );
      fprintf( fp, "%s</font></td></tr></table>\n", news->post );
   }
   /*
    * this must stay here -- line below 
    */
   fprintf( fp, "<table align='center'><tr><td bgcolor='#990000'><font size='2' color='#FFFFFF' face='verdana'>\n" );
   fprintf( fp,
            "Extended News is copyright 2001-2002 <a href='mailto:noplex@crimsonblade.org?subject=Extended News'>John 'Noplex' Bellone</a>; <a href='http://www.crimsonblade.org/snippets/' target='new'>download your copy here.</a></font></td></tr></table>\n" );
   fprintf( fp,
            "<div align='center'><font face='verdana' size='1' color='#FFFFFF'>Extended News v%s. Last updated: %s</font></div>\n",
            NEWS_VERSION, ctime( &current_time ) );
   return;
}

/* news command hook; interp.c -Nopey */
bool news_cmd_hook( CHAR_DATA * ch, char *cmd, char *argument )
{
   int i;

   for( i = 0; i < top_news_type; i++ )
      if( !str_cmp( cmd, news_command_table[i] ) )
      {
         NEWS_TYPE *type = NULL;

         sprintf( local_buf, "%d", i );
         if( ( type = figure_type( local_buf ) ) == NULL )
         {
            bug( "news_cmd_hook(): cannot find type for cmd %s", cmd );
            return FALSE;
         }
         display_news_type( ch, type, argument );
         return TRUE;
      }
   return FALSE;
}

/*
 * display the news entry from the command hook -Nopey
 */
void display_news_type( CHAR_DATA * ch, NEWS_TYPE * type, char *argument )
{
   /*
    * 5/8/02 
    */
   /*
    * moved - 6/30/02 
    */
   if( !can_read_news( ch, type ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   /*
    * required to stay here; do not remove you may change the
    * * colors accordingly to suit your news-types if you wish 
    */
   if( !str_cmp( argument, "version" ) )
   {
      pager_printf_color( ch, "&G&YExtended News&W %s&Y. Written by &WNoplex&Y.&g\r\n", NEWS_VERSION );
      return;
   }

   if( !type->first_news )
   {
      send_to_char( "There is currently no news for this news-type.\r\n", ch );
      return;
   }

   if( NULLSTR( argument ) || !str_cmp( argument, "all" ) )
   {
      bool all_news = FALSE;
      NEWS *news = NULL;
      int x = type->last_news->number, y = NEWS_VIEW;
      int skipper = ( x - y );

      if( !str_cmp( argument, "all" ) )
         all_news = TRUE;

      send_to_pager_color( NEWS_TOP, ch );
      if( type->header )
         send_to_pager_color( type->header, ch );
      send_to_pager_color( NEWS_HEADER, ch );

      for( news = type->first_news; news; news = news->next )
      {
         if( !all_news )
         {
            if( skipper > -1 )
            {
               skipper--;
               continue;
            }
         }
         pager_printf_color( ch, "&G&Y[&W%2d&Y]  &W%-8s &Y@&W%-11s &Y%s&g\r\n", news->number, news->name, news->date,
                             news->title );
      }

      if( !all_news )
         pager_printf_color( ch,
                             "&YThere is currently &W%d&Y news items loaded.\r\nUse '&W%s all&Y' to view all of them.&g\r\n",
                             type->last_news->number, type->cmd_name );
      pager_printf_color( ch, "\r\n&YTo read indiviual news posts type '&W%s <number>&Y'\r\n", type->cmd_name );
      return;
   }

   {
      NEWS *news = NULL;

      if( ( news = grab_news( type, argument ) ) == NULL )
      {
         pager_printf_color( ch, "That's not a news post number.\r\nUse '%s' to view them.\r\n", type->cmd_name );
         return;
      }
      display_news( ch, news, type );
      return;
   }
}

/* decide whether or not the character can read the news -Nopey */
/* most of these features were implemented about ideas spoken
 * with Blodkai; or ideas and implemenations that RoD has used with
 * this snippet */
bool can_read_news( CHAR_DATA * ch, NEWS_TYPE * type )
{
   if( type->org_type > -1 && !NULLSTR( type->org_name ) )
      switch ( type->org_type )
      {
         case CLAN_PLAIN:
            if( str_cmp( type->org_name, ch->pcdata->clan_name ) )
               return FALSE;
            break;

         case CLAN_GUILD:
            if( str_cmp( type->org_name, class_table[ch->class]->who_name ) )
               return FALSE;
            break;

         case CLAN_ORDER:
            if( str_cmp( type->org_name, ch->pcdata->clan_name ) )
               return FALSE;
            break;

            /*
             * not sure if this is stock; might have to add it in mud.h 
             */
         case CLAN_COUNCIL:
            if( str_cmp( type->org_name, ch->pcdata->council_name ) )
               return FALSE;
            break;
      }

   /*
    * minimum view level? 
    */
   if( type->view_level > get_trust( ch ) )
      return FALSE;

   return TRUE;
}

/* various checks against not posting news               -Nopey */
bool can_post_news( CHAR_DATA * ch, NEWS_TYPE * type )
{
   int i;

   /*
    * posting level; minimum 
    */
   if( type->view_level > get_trust( ch ) )
      return FALSE;

   /*
    * locked news-types 
    */
   if( type->locked && ( !NULLSTR( type->owner ) && str_cmp( ch->name, type->owner ) ) )
   {
      /*
       * fixed - 9/13/02 
       */
      for( i = 0; i < MAX_POSTERS; i++ )
         if( !str_cmp( type->posters[i], ch->name ) )
            return TRUE;
   }
   else
      return TRUE;

   return FALSE;
}
