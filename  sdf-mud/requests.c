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
 *			    Special requests module			    *
 * ------------------------------------------------------------------------ *
 * - Only handles who requests currently, but will hopefully support much   *
 *   more in the future.  Including: reboot/shutdown etc.		    *
 ****************************************************************************/

#include <string.h>
#include <fcntl.h>
#include "mud.h"

int REQ;

void init_request_pipe(  )
{
#ifdef REQUESTS
   if( ( REQ = open( REQUEST_PIPE, O_RDONLY | O_NONBLOCK ) ) == -1 )
   {
      bug( "REQUEST pipe not found", 0 );
      exit( 1 );
   }
#endif
}

void check_requests(  )
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   int c;
#ifdef REQUESTS
   if( read( REQ, buf, sizeof( buf ) ) > 0 )
   {
      close( REQ );
      init_request_pipe(  );
      for( c = 0; c < MAX_STRING_LENGTH; c++ )
         if( buf[c] == '\n' || buf[c] == '\r' )
         {
            buf[c] = '\0';
            break;
         }
      sprintf( buf2, "REQUEST: %s", buf );
      log_string( buf2 );
      if( strcmp( buf, "who" ) == 0 )
         do_who( NULL, "" );
      else if( strcmp( buf, "webwho" ) == 0 )
         do_who( NULL, "www" );
   }
#endif
}