/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/* mobchat.c
 * MOB Talking module by Scion of ResortMUD 4/1999
 * Use of this code is given as long as appropriate credit
 * for the original author remains intact. You may modify,
 * distribute, and use this code. I would appreciate a short
 * email if you do use this code in a MUD not based on ResortMUD's
 * codebase, but it is by no means necessary. -- Scion */

#include "mud.h"

/* Internal functions */
void do_mobchat( CHAR_DATA * vict, CHAR_DATA * ch, char *message );
char *mob_reply( char *message );

void do_mobchat( CHAR_DATA * vict, CHAR_DATA * ch, char *message )
{
   char buf[MAX_STRING_LENGTH];

   if( !IS_NPC( ch ) )
      return;
   if( !HAS_PROG( ch->pIndexData, SPEECH_PROG ) )
   {
      sprintf( buf, "%s %s", vict->name, mob_reply( message ) );
      do_tell( ch, buf );
   }
}

char *mob_reply( char *message )
{

   /*
    * Look for keywords in speech and reply.
    * * This will eventually be a file that gets parsed
    * * because that is a much more flexible and usable
    * * way of doing things, but this is good enough for
    * * testing stage stuff. -- Scion 
    */

   return "Wow! That\'s nifty!";
}
