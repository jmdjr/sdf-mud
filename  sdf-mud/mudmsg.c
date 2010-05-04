/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*
 * Original concept by Garil, modified by Senir.
 *
 * This code currently does not function, below is the inserted bits which exist in
 * other files and is commented out.  Do not uncomment the below bits in the
 * other files until the code is made fully funtional.
 *
 * Uncomment in do.h
 * DECLARE_DO_FUN( do_intermud     );
 * void imud_recv_message();
 *
 * Uncomment in comm.c
 * imud_recv_message();
 *
 * Add to the Makefile - C_FILES
 * mudmsg.c
 */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include "mud.h"

#define INTERMUDCHAN_FORMAT "&C&W[&RIntermud&W] &w%s@%s&W: &R%s\r\n"
#define INTERMUDCHAN_LEVEL 2

/* Increment MAX_MUD_PORTS by one and add a new port for each new mud added */
#define MAX_MUD_PORTS 2
int mudports[MAX_MUD_PORTS];

struct mud_msgbuf
{
   long mtype;
   char mtext[2048];
};

short clearq = 0;

void do_intermud( CHAR_DATA * ch, char *argument )
{
   struct mud_msgbuf qbuf[MAX_MUD_PORTS];
   int qid = -1;
   short i = 1;

   mudports[0] = 4000;
   mudports[1] = 7000;

   if( !argument || argument[0] == '\0' )
   {
      send_to_char_color( "&C&wWhat do you want to communicate?\r\n", ch );
      return;
   }

   if( ( qid = msgget( 89798, IPC_CREAT | 0660 ) ) == -1 )
   {
      bug( "Unable to msgget id for intermud chan sending.", 0 );
      return;
   }

   for( i = 0; i < MAX_MUD_PORTS; i++ )
   {
      if( mudports[i] == 0 )
         continue;

      qbuf[i].mtype = mudports[i];

      qbuf[i].mtext[0] = '\0';

      sprintf( qbuf[i].mtext, "\"%s\" \"%s\" %s", sysdata.mud_name, capitalize( ch->name ), argument );

      if( ( msgsnd( qid, ( struct msgbuf * )&qbuf[i], sizeof( struct mud_msgbuf ), 0 ) ) == -1 )
         bug( "Unable to queue intermud chan msg for sending to mud at port %d.", mudports[i] );
   }

   clearq = 3;

}

void recv_text_handler( char *str )
{
   DESCRIPTOR_DATA *d = NULL;
   CHAR_DATA *och = NULL;
   char mname[MAX_INPUT_LENGTH];
   char pname[MAX_INPUT_LENGTH];

   str = one_argument( str, mname );
   str = one_argument( str, pname );

   for( d = first_descriptor; d; d = d->next )
   {
      och = d->original ? d->original : d->character;

      if( !och || d->connected != CON_PLAYING
          // || IS_SET(och->deaf, CHANNEL_INTERMUD)
          || get_trust( och ) < INTERMUDCHAN_LEVEL )
         continue;

      ch_printf( och, INTERMUDCHAN_FORMAT, pname, mname, str );
   }

   return;
}

void imud_recv_message(  )
{
   struct mud_msgbuf qbuf;
   int ret;
   int qid = -1;

   if( ( qid = msgget( 89798, IPC_CREAT | 0660 ) ) == -1 )
   {
      //bug("Unable to msgget id for intermud chan receiving.", 0);
      return;
   }

   while( ( ret = msgrcv( qid, ( struct msgbuf * )&qbuf, sizeof( struct mud_msgbuf ), port, IPC_NOWAIT ) ) > 0 )
      recv_text_handler( qbuf.mtext );

   if( ret == -1 && errno != ENOMSG )
      bug( "Error in receiving intermud msg from queue.", 0 );

   if( clearq > 0 )
   {
      if( --clearq > 0 )
         return;

      msgctl( qid, IPC_RMID, 0 );
   }

}
