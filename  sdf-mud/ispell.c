/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

#define Stringify(x) Str(x)
#define Str(x) #x

#include <string.h>
#include <ctype.h>
//#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "mud.h"

static FILE *ispell_out;
static int ispell_pid = -1;
static int to[2], from[2];

#define ISPELL_BUF_SIZE 1024

void ispell_init(  )
{
   char ignore_buf[1024];

   pipe( to );
   pipe( from );

   ispell_pid = fork(  );
   if( ispell_pid < 0 )
   {
      char *errmsg = '\0';
      sprintf( errmsg, "ispell_init: fork: %s", strerror( errno ) );
      bug( errmsg, 0 );
   }
   else if( ispell_pid == 0 ) /* child */
   {
      int i;
      dup2( to[0], 0 ); /* this is where we read commands from - make
                         * it stdin */
      close( to[0] );
      close( to[1] );

      dup2( from[1], 1 );  /* this is where we write stuff to */
      close( from[0] );
      close( from[1] );

      /*
       * Close all the other files 
       */
      for( i = 2; i < 255; i++ )
         close( i );

      execlp( "ispell", "ispell", "-a", ( char * )NULL );
      exit( 1 );
   }
   else
   {  /* ok ! */
      close( to[0] );
      close( from[1] );
      ispell_out = fdopen( to[1], "w" );
      setbuf( ispell_out, NULL );
#if !defined( sun )  /* that ispell on sun gives no (c) msg */
      read( from[0], ignore_buf, 1024 );
#endif
   }
}

void ispell_done(  )
{
   if( ispell_pid != -1 )
   {
      fprintf( ispell_out, "#\n" );
      fclose( ispell_out );
      close( from[0] );
      waitpid( ispell_pid, NULL, 0 );
      ispell_pid = -1;
   }
}

char *get_ispell_line( char *word )
{
   static char buf[ISPELL_BUF_SIZE];
   char buf2[MAX_STRING_LENGTH];
   int len;

   if( ispell_pid == -1 )
      return NULL;

   if( word )
   {
      fprintf( ispell_out, "^%s\n", word );
      fflush( ispell_out );
   }

   len = read( from[0], buf2, ISPELL_BUF_SIZE );
   buf2[len] = '\0';

   /*
    * Read up to max 1024 characters here 
    */
   if( sscanf( buf2, "%" Stringify( ISPELL_BUF_SIZE ) "[^\n]\n\n", buf ) != 1 )
      return NULL;

   return buf;
}

void do_ispell( CHAR_DATA * ch, char *argument )
{
   char *pc, buf[MAX_STRING_LENGTH];

   if( ispell_pid <= 0 )
   {
      send_to_char( "ISpell is not running.\r\n", ch );
      return;
   }

   if( !argument[0] || strchr( argument, ' ' ) )
   {
      send_to_char_color( "You &Wmight&w want to specify a word?\r\n", ch );
      return;
   }

   if( argument[0] == '+' )
   {
      for( pc = argument + 1; *pc; pc++ )
         if( !isalpha( *pc ) )
         {
            sprintf( buf, "'%c' is not a letter.\r\n", *pc );
            send_to_char( buf, ch );
            return;
         }
      fprintf( ispell_out, "*%s\n", argument + 1 );
      fflush( ispell_out );
      return;  /* we assume everything is OK.. better be so! */
   }

   pc = get_ispell_line( argument );
   if( !pc )
   {
      send_to_char( "ISpell: failed.\r\n", ch );
      return;
   }

   switch ( pc[0] )
   {
      case '*':
      case '+':  /* root */
      case '-':  /* compound */
         send_to_char_color( "&cCorrect.&w\r\n", ch );
         break;
      case '&':  /* miss */
         sprintf( buf, "&WNot found&w. &cPossible words&W:&w %s\r\n", strchr( pc, ':' ) + 1 );
         send_to_char_color( buf, ch );
         break;

      case '?':  /* guess */
         sprintf( buf, "&WNot found&w. &cPossible words&W:&w %s\r\n", strchr( pc, ':' ) + 1 );
         send_to_char_color( buf, ch );
         break;

      case '#':  /* none */
         send_to_char_color( "&cUnable to find anything that matches.&w\r\n", ch );
         break;

      default:
         sprintf( buf, "Weird output from ispell: %s\r\n", pc );
         send_to_char( buf, ch );
   }
}
