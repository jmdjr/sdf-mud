/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/* ROM 2.4 Integrated Web Server - Version 1.0
 *
 * This is my first major snippet... Please be kind. ;-)
 * Copyright 1998 -- Defiant -- Rob Siemborski -- mud@towers.crusoe.net
 *
 * Many thanks to Russ and the rest of the developers of ROM for creating
 * such an excellent codebase to program on.
 *
 * If you use this code on your mud, I simply ask that you place my name
 * someplace in the credits.  You can put it where you feel it is
 * appropriate.
 *
 * I offer no guarantee that this will work on any mud except my own, and
 * if you can't get it to work, please don't bother me.  I wrote and tested
 * this only on a Linux 2.0.30 system.  Comments about bugs, are, however,
 * appreciated.
 *
 * Now... On to the installation!
 */

/*
 * Insanity v0.9a pre-release Modifications
 * By Chris Fewtrell (Trax) <C.J.Fewtrell@bcs.org.uk>
 *
 * - Added functionailiy for Secure Web server pages, using standard HTTP
 *   Basic authentication, comparing with pass list generated with command
 *   from within the MUD itself.
 * - Started work on web interface to help files, allowing them to be browsed
 *   from a web browser rather than being in MUD to read them.
 * - Seperated out the HTTP codes and content type to seperate functions
 *   (intending to allow more than HTML to be served via this)
 * - Adjusted the descriptor handling to prevent anyone from prematurely
 *   stopping a transfer causing a fd exception and the system to exit()
 * - Created a sorta "virtual" web directory for the webserver files to be
 *   actually served. This contains the usual images dir if any images are
 *   needed to be served from a central repository rather than generated.
 *   Be warned though! It WON'T follow any symlinks, I'll add that later
 *   with the stat function.. (maybe :)
 *
 * Future Possbile additions:
 * - Access to general boards though web interface, prolly prevent posting but
 *   being able to browse and read notes to 'all' would be allowed
 */

/*
 * Additional Modifications based upon with with Insanity Codebase
 * By Chris Fewtrell (Trax) <C.J.Fewtrell@bcs.org.uk>
 *
 * - Web server root directory created, all URLs that use internal code
 *   generated HTML intercept first, then path is handed off to webroot
 *   handler allowing a directory tree to be built up there. Should allow
 *   easier management of the file behind the internal web server.
 */

#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "mud.h"
#include "web.h"

struct type_data content_types[] = {
   {".html", CONTENT_HTML},
   {".htm", CONTENT_HTML},
   {".gif", CONTENT_GIF},
   {".txt", CONTENT_TEXT},
   {".text", CONTENT_TEXT},
   {".jpg", CONTENT_JPEG},
   {".jpeg", CONTENT_JPEG},
   {".gz", CONTENT_GZIP},
   {".gzip", CONTENT_GZIP},
   {".wav", CONTENT_WAV},
   {".wrl", CONTENT_VRML},
   {".class", CONTENT_CLASS},

   {"", CONTENT_TEXT}
};

/* The mark of the end of a HTTP/1.x request */
const char ENDREQUEST[5] = { 13, 10, 13, 10, 0 };  /* (CRLFCRLF) */

/* Externs */
int top_web_desc;

/* Locals */
WEB_DESCRIPTOR *web_descs;
int sockfd;

bool init_web( int wport )
{
   struct sockaddr_in my_addr;
   char buf[1024];

   web_descs = NULL;
   web_desc_free = NULL;

   sprintf( buf, "Attaching Internal Web Server to Port %d", wport );
   log_string( buf );

   if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
   {
      log_string( "----> Web Server: Error getting socket" );
      perror( "web-socket" );
      sprintf( buf, "Web server (%d) : Failed initialization - Error getting socket", wport );
      return FALSE;
   }

   my_addr.sin_family = AF_INET;
   my_addr.sin_port = htons( wport );
   my_addr.sin_addr.s_addr = htons( INADDR_ANY );
   bzero( &( my_addr.sin_zero ), 8 );

   if( ( bind( sockfd, ( struct sockaddr * )&my_addr, sizeof( struct sockaddr ) ) ) == -1 )
   {
      log_string( "----> Web Server: Error binding socket" );
      perror( "web-bind" );
      sprintf( buf, "Web server (%d) : Failed initialization - Error binding socket", wport );
      return FALSE;
   }

   /*
    * Only listen for 5 connects at once, do we really need more?
    */
   listen( sockfd, 5 );

   sprintf( buf, "Web server: Initalization complete. Servering on port %d.", wport );
//    sys_data.webup = TRUE;

   return TRUE;
}

struct timeval ZERO_TIME = { 0, 0 };

void handle_web( void )
{
   int max_fd;
   WEB_DESCRIPTOR *current, *prev = NULL, *next;
   fd_set readfds;

   FD_ZERO( &readfds );
   FD_SET( sockfd, &readfds );

   /*
    * it *will* be atleast sockfd
    */
   max_fd = sockfd;

   /*
    * add in all the current web descriptors
    */
   for( current = web_descs; current; current = current->next )
   {
      FD_SET( current->fd, &readfds );
      if( max_fd < current->fd )
         max_fd = current->fd;
   }

   /*
    * Wait for ONE descriptor to have activity
    */
   select( max_fd + 1, &readfds, NULL, NULL, &ZERO_TIME );

   if( FD_ISSET( sockfd, &readfds ) )
   {
      /*
       * NEW CONNECTION -- INIT & ADD TO LIST
       */

      current = new_web_desc(  );
      current->sin_size = sizeof( struct sockaddr_in );
      current->request[0] = '\0';

      if( ( current->fd = accept( sockfd, ( struct sockaddr * )&( current->their_addr ), &( current->sin_size ) ) ) == -1 )
      {
         log_string( "----> Web Server: Error accepting connection" );
         perror( "web-accept" );
         free_web_desc( current );
         FD_CLR( sockfd, &readfds );
         return;
      }

      current->next = web_descs;
      web_descs = current;

      /*
       * END ADDING NEW DESC
       */
   }

   /*
    * DATA IN!
    */
   for( prev = NULL, current = web_descs; current != NULL; current = next )
   {
      next = current->next;

      if( FD_ISSET( current->fd, &readfds ) )   /* We Got Data! */
      {
         char buf[MAXDATA];
         int numbytes;

         if( ( numbytes = read( current->fd, buf, sizeof( buf ) ) ) == -1 )
         {
            perror( "web-read" );
            if( prev )
               prev->next = next;
            else
               web_descs = next;

            free_web_desc( current );
            continue;
         }

         buf[numbytes] = '\0';

         strcat( current->request, buf );
      }

      prev = current;
   }  /* DONE WITH DATA IN */

   /*
    * DATA OUT
    */
   /*
    * Hmm we want to delay this if possible, to prevent it prematurely
    */
   for( prev = NULL, current = web_descs; current != NULL; current = next )
   {
      next = current->next;

      if( strstr( current->request, "HTTP/1." ) /* 1.x request (vernum on FIRST LINE) */
          && strstr( current->request, ENDREQUEST ) )
      {
         handle_web_request( current );
      }
      else if( !strstr( current->request, "HTTP/1." ) && strchr( current->request, '\n' ) )  /* HTTP/0.9 (no ver number) */
      {
         handle_web_request( current );
      }
      else
      {
         prev = current;
         continue;   /* Don't have full request yet! */
      }

      if( current->keepalive )
      {
         prev = current;
         continue;
      }

      close( current->fd );

      if( !prev )
      {
         web_descs = current->next;
      }
      else
      {
         prev->next = current->next;
      }

      free_web_desc( current );
   }  /* END DATA-OUT */
}

/* Generic Utility Function */

int send_buf( int fd, const char *buf )
{
   return send( fd, buf, strlen( buf ), 0 );
}

int determine_type( char *path )
{
   int i;

   for( i = 0; *content_types[i].suffix; i++ )
   {
      if( !str_suffix( content_types[i].suffix, path ) )
         return content_types[i].type;
   }

   /*
    * If we dunno, we'll use plain text then
    */
   return CONTENT_TEXT;
}

void send_200OK( WEB_DESCRIPTOR * wdesc )
{
   send_buf( wdesc->fd, "HTTP/1.1 200 OK\n" );
}

void send_404UNFOUND( WEB_DESCRIPTOR * wdesc )
{
   send_buf( wdesc->fd, "HTTP/1.1 404 Not Found\n" );
}

void send_401UNAUTHORISED( WEB_DESCRIPTOR * wdesc, char *realm )
{
   char buf[MAX_INPUT_LENGTH];

   sprintf( buf, "WWW-Authenticate: Basic realm=\"%s\"\n", realm );

   send_buf( wdesc->fd, "HTTP/1.1 401 Unauthorised\n" );
   send_buf( wdesc->fd, buf );
}

void send_content( WEB_DESCRIPTOR * wdesc, int type )
{
   switch ( type )
   {
      case CONTENT_HTML:
         send_buf( wdesc->fd, "Content-type: text/html\n\n" );
         break;
      default:
      case CONTENT_TEXT:
         send_buf( wdesc->fd, "Content-type: text/plain\n\n" );
         break;
      case CONTENT_GIF:
         send_buf( wdesc->fd, "Content-type: image/gif\n\n" );
         break;
      case CONTENT_WAV:
         send_buf( wdesc->fd, "Content-type: audio/x-wav\n\n" );
         break;
      case CONTENT_GZIP:
         send_buf( wdesc->fd, "Content-type: application/x-zip-compressed\n\n" );
         break;
      case CONTENT_VRML:
         send_buf( wdesc->fd, "Content-type: x-world/x-vrml\n\n" );
         break;
      case CONTENT_CLASS:
         send_buf( wdesc->fd, "Content-type: application/octect-stream\n\n" );
         break;

   }
}


void handle_web_request( WEB_DESCRIPTOR * wdesc )
{
   char buf[MAX_STRING_LENGTH];
   char path[MAX_STRING_LENGTH];
   char *stuff;
   int addr;

   stuff = one_argument( wdesc->request, path );
   one_argument( stuff, path );

   /*
    * process request
    */
   /*
    * are we using HTTP/1.x? If so, write out header stuff..
    */
   if( !strstr( wdesc->request, "GET" ) )
   {
      send_buf( wdesc->fd, "HTTP/1.1 501 Not Implemented" );
      return;
   }
   else if( strstr( wdesc->request, "HTTP/1." ) )
   {
      /*
       * Check for and handle secure area access
       */
      if( !str_prefix( SECURE_URL, path ) )
      {
         char *where;
         char encoded[MAX_INPUT_LENGTH];
         char username[MAX_INPUT_LENGTH];
         char *password = "";

         username[0] = '\0';
         encoded[0] = '\0';

         where = strstr( stuff, "Authorization: Basic" );

         if( !where )
            send_401UNAUTHORISED( wdesc, AUTH_DOMAIN );
         else
         {
            where += strlen( "Authorization: Basic" );

            where++;
            for( password = encoded; *where && !isspace( *where ); where++, password++ )
               *password = *where;

            *password = '\0';

            Base64Decode( encoded, (unsigned char*)username, MAX_INPUT_LENGTH );

            for( password = username; *password && *password != ':'; password++ );

            if( *password == ':' )
            {
               *password = '\0';
               password++;
            }
         }

         if( !check_web_pass( username, password ) )
         {
            handle_secure_web( wdesc, "", "", path + strlen( SECURE_URL ) );
            return;
         }
         else
         {
            handle_secure_web( wdesc, username, password, path + strlen( SECURE_URL ) );
            return;
         }
      }
   }

   addr = ntohl( wdesc->their_addr.sin_addr.s_addr );

   sprintf( buf, "Web - Request for %s recived from %d.%d.%d.%d",
            path, ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF, ( addr >> 8 ) & 0xFF, ( addr ) & 0xFF );
   //        wiznet(buf,NULL,NULL,WIZ_WEB,0,0);

   /*
    * Handle the actual request
    */
   if( !str_cmp( path, "/wholist" ) || !str_cmp( path, "/wholist.html" ) )
   {
      handle_web_who_request( wdesc );
      if( sysdata.webcounter == TRUE )
      {
         sprintf( buf, "Telnet Interface Hit: %s - WhoList", inet_ntoa( wdesc->their_addr.sin_addr ) );

         log_string( buf );
      }
   }
   else if( !str_cmp( path, "/" ) || !str_cmp( path, "/index.html" ) )
   {
      handle_web_main( wdesc );
      if( sysdata.webcounter == TRUE )
      {
         sprintf( buf, "Telnet Interface Hit: %s - Index", inet_ntoa( wdesc->their_addr.sin_addr ) );
         log_string( buf );
      }

   }
   else if( !str_cmp( path, "/wizlist" ) || !str_cmp( path, "/wizlist.html" ) )
   {
      handle_web_wizlist( wdesc );
      if( sysdata.webcounter == TRUE )
      {
         sprintf( buf, "Telnet Interface Hit: %s - WizList", inet_ntoa( wdesc->their_addr.sin_addr ) );
         log_string( buf );
      }
   }
   else if( !str_cmp( path, "/immlist" ) || !str_cmp( path, "/immlist.html" ) )
   {
      handle_web_immlist( wdesc );
      if( sysdata.webcounter == TRUE )
      {
         sprintf( buf, "Telnet Interface Hit: %s - IMMList", inet_ntoa( wdesc->their_addr.sin_addr ) );
         log_string( buf );
      }
   }

   /*
    * Ntanel said he did not want these portions, so me no give.  Hah!  Yakkov
    *

    else if(!str_cmp(path, "/about") || !str_cmp(path, "/about.html"))
    handle_web_about(wdesc);
    else if(!str_prefix(SECURE_URL, path))
    handle_secure_web(wdesc, "", "", path);
    else if(!str_prefix("/wwwlist", path))
    handle_wwwlist_request(wdesc);
    */
   else if( !str_prefix( "/help", path ) )
      handle_web_help( wdesc, path + strlen( "/help" ) );
   else if( !str_cmp( path, "/arealist" ) || !str_cmp( path, "/arealist.html" ) )
      handle_web_arealist( wdesc );
   else
      handle_web_root( wdesc, path );  /* Now pass to root dir handler */
}

void shutdown_web( void )
{
   WEB_DESCRIPTOR *current, *next;

   /*
    * Close All Current Connections
    */
   for( current = web_descs; current != NULL; current = next )
   {
      next = current->next;
      close( current->fd );
      free_web_desc( current );
   }

   /*
    * Stop Listening
    */
   close( sockfd );

//     sys_data.webup = FALSE;
}

/* point 1 */

void handle_web_main( WEB_DESCRIPTOR * wdesc )
{
   send_200OK( wdesc );
   send_content( wdesc, CONTENT_HTML );

   show_web_file( PUB_INDEX, wdesc );
}

/* point 2 */

void handle_web_unfound( WEB_DESCRIPTOR * wdesc )
{
   send_404UNFOUND( wdesc );
   send_content( wdesc, CONTENT_HTML );

   show_web_file( PUB_ERROR, wdesc );

   return;
}

/* point 3 */

void handle_web_wizlist( WEB_DESCRIPTOR * wdesc )
{
   char buf[MAX_STRING_LENGTH];
   char colbuf[2 * MAX_STRING_LENGTH];
   FILE *fp;
   int num = 0;
   char let;

   send_200OK( wdesc );
   send_content( wdesc, CONTENT_HTML );

   show_web_file( PUB_WIZLIST_H, wdesc );

   if( ( fp = fopen( WEBWIZ_FILE, "r" ) ) != NULL )
   {
      while( !feof( fp ) )
      {
         while( ( let = fgetc( fp ) ) != EOF && num < ( MAX_STRING_LENGTH - 2 ) )
         {
            if( let != '\r' )
               buf[num++] = let;
         }

      }
      buf[num] = '\0';
      fclose( fp );
   }
   else
      sprintf( buf, "Error opening Staff list Data file<br>\r\n" );

   web_colourconv( colbuf, buf );

   send_buf( wdesc->fd, colbuf );

   show_web_file( PUB_WIZLIST_F, wdesc );
}

void handle_web_immlist( WEB_DESCRIPTOR * wdesc )
{
   char buf[MAX_STRING_LENGTH];
   char colbuf[2 * MAX_STRING_LENGTH];
   FILE *fp;
   int num = 0;
   char let;

   send_200OK( wdesc );
   send_content( wdesc, CONTENT_HTML );

   show_web_file( PUB_IMMLIST_H, wdesc );

   if( ( fp = fopen( IMMWEB_FILE, "r" ) ) != NULL )
   {
      while( !feof( fp ) )
      {
         while( ( let = fgetc( fp ) ) != EOF && num < ( MAX_STRING_LENGTH - 2 ) )
         {
            if( let != '\r' )
               buf[num++] = let;
         }

      }
      buf[num] = '\0';
      fclose( fp );
   }
   else
      sprintf( buf, "Error opening Staff list Data file<br>\r\n" );

   web_colourconv( colbuf, buf );

   send_buf( wdesc->fd, colbuf );

   show_web_file( PUB_IMMLIST_F, wdesc );
}


/*
 * Many thanks to Altrag who contributed this function! --GW
 */
char *text2html( const char *ip )
{
   static struct
   {
      const char *text;
      const char *html;
      int tlen, hlen;
   } convert_table[] =
   {
      {
      "<", "&lt;"},
      {
      ">", "&gt;"},
      {
      "&", "&amp;"},
      {
      "\"", "&quot;"},
      {
      " ", "&nbsp;"},
      {
   NULL, NULL}};

   static char buf[MAX_STRING_LENGTH * 2];   /* Safety here .. --GW */
   char *bp = buf;
   int i;

   if( !convert_table[0].tlen )
   {
      for( i = 0; convert_table[i].text; ++i )
      {
         convert_table[i].tlen = strlen( convert_table[i].text );
         convert_table[i].hlen = strlen( convert_table[i].html );
      }
   }
   while( *ip )
   {
      for( i = 0; convert_table[i].text; ++i )
         if( !strncmp( ip, convert_table[i].text, convert_table[i].tlen ) )
            break;
      if( convert_table[i].text )
      {
         strcpy( bp, convert_table[i].html );
         bp += convert_table[i].hlen;
         ip += convert_table[i].tlen;
      }
      else
         *bp++ = *ip++;
   }
   *bp = '\0';
   return buf;
}

char *parse_quotes( char *arg )
{
   int str;

   for( str = 0; arg[str] != '\0'; str++ )
   {
      if( arg[str] == '*' )
         arg[str] = '"';
   }

   return arg;
}

/* point 4 */

void handle_web_who_request( WEB_DESCRIPTOR * wdesc )
{
   send_200OK( wdesc );
   send_content( wdesc, CONTENT_HTML );

   show_web_file( PUB_WHOLIST_H, wdesc );

   do_who( NULL, "" );
   handle_who_routine( wdesc );

   show_web_file( PUB_WHOLIST_F, wdesc );
}

/* point 5 */

void handle_wwwlist_request( WEB_DESCRIPTOR * wdesc )
{
   send_200OK( wdesc );
   send_content( wdesc, CONTENT_HTML );

   show_web_file( PUB_WWWLIST_H, wdesc );

   do_who( NULL, "www" );
   handle_who_routine( wdesc );

   show_web_file( PUB_WWWLIST_F, wdesc );
}

void handle_web_about( WEB_DESCRIPTOR * wdesc )
{
   send_200OK( wdesc );
   send_content( wdesc, CONTENT_HTML );

   show_web_file( PUB_ABOUT, wdesc );
}

WEB_DESCRIPTOR *new_web_desc( void )
{
   WEB_DESCRIPTOR *desc;

   if( web_desc_free == NULL )
   {
      CREATE( desc, WEB_DESCRIPTOR, 1 );
      top_web_desc++;
   }
   else
   {
      desc = web_desc_free;
      web_desc_free = web_desc_free->next;
   }

   desc->keepalive = FALSE;
   desc->next = NULL;

   return desc;
}

void free_web_desc( WEB_DESCRIPTOR * desc )
{
   desc->next = web_desc_free;
   web_desc_free = desc;
}

void handle_secure_web( WEB_DESCRIPTOR * wdesc, char *username, char *password, char *path )
{
   char file[MAX_INPUT_LENGTH];
   int type, fd;
   void *buffer;

   if( username[0] == '\0' || password[0] == '\0' )
   {
      send_401UNAUTHORISED( wdesc, "Staff Area" );
      return;
   }


   if( !str_cmp( path, "" ) || !str_cmp( path, "/" ) )
      sprintf( file, "%s%s", SECURE_WEB, "/index.html" );
   else
      sprintf( file, "%s%s", SECURE_WEB, path );

   if( file[strlen( file ) - 2] == '/' )
   {
      strcat( file, "index.html" );
   }

   /*
    * Work out the filetype so we know what we are doing
    */
   type = determine_type( file );

   if( ( fd = open( file, O_RDONLY | O_NONBLOCK ) ) == -1 )
   {
      send_404UNFOUND( wdesc );
      send_content( wdesc, CONTENT_HTML );

      show_web_file( STA_ERROR, wdesc );
   }
   else
   {
      int readlen = 0;

      buffer = malloc( 1024 );
      send_200OK( wdesc );
      send_content( wdesc, type );

      while( ( readlen = read( fd, buffer, 1024 ) ) > 0 )
         send( wdesc->fd, buffer, readlen, 0 );

      close( fd );
      free( buffer );
   }
   return;
}

bool check_web_pass( char *username, char *password )
{
   WEB_PASS *current;

   for( current = web_passwords; current; current = current->next )
      if( !str_cmp( current->username, username ) )
         if( !str_cmp( current->password, crypt( password, "yt" ) ) )
            return TRUE;

   return FALSE;
}

bool change_web_pass( char *username, char *newpass )
{
   WEB_PASS *current, *new;

   for( current = web_passwords; current; current = current->next )
      if( !str_cmp( current->username, username ) )
         break;

   if( !current )
   {
      log_string( "Creating new webpass entry..." );

      CREATE( new, WEB_PASS, 1 );
      new->username = str_dup( username );
      new->password = str_dup( "" );

      new->next = web_passwords;
      web_passwords = new;

   }
   else
      new = current;

   STRFREE( new->password );

   new->password = str_dup( crypt( newpass, "yt" ) );

   save_webpass(  );

   return TRUE;
}

void do_changewebpass( CHAR_DATA * ch, char *argument )
{

   char buf[MAX_INPUT_LENGTH];

   argument = one_argument( argument, buf );

   if( change_web_pass( ch->name, buf ) )
   {
      send_to_char( "Web password set.\r\n", ch );
   }
   else
   {
      send_to_char( "There was a problem setting the web password.\r\n", ch );
   }

   return;
}

void save_webpass(  )
{
   FILE *fpWebPass;
   WEB_PASS *current;

   if( ( fpWebPass = fopen( WEB_PASS_FILE, "w" ) ) )
   {
      log_string( "Saving web passes" );

      for( current = web_passwords; current; current = current->next )
         fprintf( fpWebPass, "WebPass %s~\n%s~\n", current->username, current->password );
      fprintf( fpWebPass, "$0\n" );

      fclose( fpWebPass );
   }
}

void load_webpass(  )
{
   FILE *fpWebPass;

   if( ( fpWebPass = fopen( WEB_PASS_FILE, "r" ) ) )
   {
      WEB_PASS *new;
      char *word;

      for( word = fread_word( fpWebPass ); str_cmp( word, "$0" ); )
      {
         CREATE( new, WEB_PASS, 1 );
         new->username = fread_string( fpWebPass );
         new->password = fread_string( fpWebPass );

         new->next = web_passwords;
         web_passwords = new;
         word = fread_word( fpWebPass );

      }

      fclose( fpWebPass );

   }
}


void release_web_desc( int desc )
{
   WEB_DESCRIPTOR *current;

   for( current = web_descs; current; current = current->next )
   {
      if( current->fd == desc )
      {
         current->keepalive = FALSE;
         return;
      }
   }
}

int web_colour( char type, char *string )
{
   char code[50];
   char *p = '\0';

   switch ( type )
   {
      default:
//     sprintf( code, "" );
         break;
      case '&':
         sprintf( code, "&" );
         break;
      case 'x':
         sprintf( code, "</font><font color=#000000>" );
         break;
      case 'b':
         sprintf( code, "</font><font color=#00007F>" );
         break;
      case 'c':
         sprintf( code, "</font><font color=#007F7F>" );
         break;
      case 'g':
         sprintf( code, "</font><font color=#007F00>" );
         break;
      case 'r':
         sprintf( code, "</font><font color=#7F0000>" );
         break;
      case 'w':
         sprintf( code, "</font><font color=#BFBFBF>" );
         break;
      case 'y':
         sprintf( code, "</font><font color=#FFFF00>" );
         break;
      case 'Y':
         sprintf( code, "</font><font color=#FFFF00>" );
         break;
      case 'B':
         sprintf( code, "</font><font color=#0000FF>" );
         break;
      case 'C':
         sprintf( code, "</font><font color=#00FFFF>" );
         break;
      case 'G':
         sprintf( code, "</font><font color=#00FF00>" );
         break;
      case 'R':
         sprintf( code, "</font><font color=#FF0000>" );
         break;
      case 'W':
         sprintf( code, "</font><font color=#FFFFFF>" );
         break;
      case 'z':
         sprintf( code, "</font><font color=#7F7F7F>" );
         break;
      case 'o':
         sprintf( code, "</font><font color=#FFFF00>" );
         break;
      case 'O':
         sprintf( code, "</font><font color=#7F7F00>" );
         break;
      case 'p':
         sprintf( code, "</font><font color=#7F007F>" );
         break;
      case 'P':
         sprintf( code, "</font><font color=#FF00FF>" );
         break;
      case '/':
         sprintf( code, "<br>" );
         break;
      case '{':
         sprintf( code, "%c", '{' );
         break;
      case '-':
         sprintf( code, "%c", '~' );
         break;
   }

   p = code;
   while( *p != '\0' )
   {
      *string = *p++;
      *++string = '\0';
   }

   return ( strlen( code ) );
}


void web_colourconv( char *buffer, const char *txt )
{
   const char *point;
   int skip = 0;

   if( txt )
   {
      for( point = txt; *point; point++ )
      {
         if( *point == '&' )
         {
            point++;
            skip = web_colour( *point, buffer );
            while( skip-- > 0 )
               ++buffer;
            continue;
         }
         *buffer = *point;
         *++buffer = '\0';
      }
      *buffer = '\0';
   }
   return;
}

/*
 * Added: 21 April 2001 (Trax)
 * Function to handle general requests for files which have no special handler
 * This maps the path requested onto the HTML root defined higher up and returns that
 * file if it exists (with correct http MIME header if found defined) Will work
 * with binary or text files
 */
void handle_web_root( WEB_DESCRIPTOR * wdesc, char *path )
{
   char buf[MAX_STRING_LENGTH];
   char file[MAX_INPUT_LENGTH];
   int type, fd;
   void *buffer;

   if( !str_cmp( path, "" ) || !str_cmp( path, "/" ) )
      sprintf( file, "%s%s", WEB_ROOT, "/index.html" );
   else
      sprintf( file, "%s%s", WEB_ROOT, path );

   if( file[strlen( file ) - 2] == '/' )
   {
      strcat( file, "index.html" );
   }

   /*
    * Work out the filetype so we know what we are doing
    */
   type = determine_type( file );

   if( ( fd = open( file, O_RDONLY | O_NONBLOCK ) ) == -1 )
   {
      send_404UNFOUND( wdesc );
      send_content( wdesc, CONTENT_HTML );
      sprintf( buf, "Unable to find file '%s'.", path );
      send_buf( wdesc->fd, buf );
   }
   else
   {
      int readlen = 0;

      buffer = malloc( 1024 );
      send_200OK( wdesc );
      send_content( wdesc, type );

      while( ( readlen = read( fd, buffer, 1024 ) ) > 0 )
         send( wdesc->fd, buffer, readlen, 0 );

      close( fd );
      free( buffer );
   }
   return;
}

/*
 * Added: 21 April 2001 (Trax)
 * Function for sending a simple basic index for accessing the helpfiles via
 * the web
 */
void send_help_index( WEB_DESCRIPTOR * wdesc )
{
   char let, buf[MAX_INPUT_LENGTH];

   send_buf( wdesc->fd,
             "<p><hr><center>\r\n"
             "<form action=/help method=get>\r\n"
             "Keyword Lookup:&nbsp;"
             "<input type=text size=15 name=keyword>&nbsp;\r\n" "<input type=submit value=Lookup>\r\n" "</form><p>[" );

   for( let = 'a'; let <= 'z'; let++ )
   {
      sprintf( buf, "&nbsp;<a href=/help/%c/>%c</a>&nbsp;%s", let, UPPER( let ), let == 'z' ? "" : "|" );
      send_buf( wdesc->fd, buf );
   }

   send_buf( wdesc->fd, "]<p></center><hr><p>\r\n" );
}

/*
 * Added: 21 April 2001 (Trax)
 * Function to display the list of helpfiles matching a single prefixed letter
 */
void send_help_list( WEB_DESCRIPTOR * wdesc, char let )
{
   char buf[1024], *left, word[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH], *ptr;
   HELP_DATA *pHelp;
   int col = 0, entries = 0;

   sprintf( buf, "%c", let );

   sprintf( buf2, "<title>Web Based Help: %c</title>"
            "<body bgcolor=000000 text=FFFFFF link=FF0000>\r\n"
            "<font size=+2><b><i>\r\n"
            "Web Based Help: Listing for letter '%c'\r\n"
            "</font></b></i>\r\n" "<p><hr>\r\n<table cols=5><tr>\r\n", let, let );
   send_buf( wdesc->fd, buf2 );

   for( pHelp = first_help; pHelp; pHelp = pHelp->next )
   {
      if( pHelp->level > MAX_WEBHELP_LEVEL )
         continue;

      for( left = one_argument( pHelp->keyword, word ); word[0] != '\0'; left = one_argument( left, word ) )
      {
         if( !str_prefix( buf, word ) )
         {

            char nospaces[MAX_INPUT_LENGTH];

            strcpy( nospaces, word );
            for( ptr = nospaces; *ptr != '\0'; ptr++ )
               if( *ptr == ' ' )
                  *ptr = '_';

            sprintf( buf2, "<td><a href=\"/help/%c/%s\">%s</a></td>", let, nospaces, word );

            send_buf( wdesc->fd, buf2 );

            entries++;

            if( ++col % 5 == 0 )
               send_buf( wdesc->fd, "</tr><tr>\r\n" );
         }
      }
   }

   sprintf( buf2, "</tr><tr>\r\n"
            "<td colspan = 5 align=center>\r\n"
            "<b>Total of %d entries found matching</b>\r\n" "</td></tr></table><p>\r\n", entries );
   send_buf( wdesc->fd, buf2 );
}

/*
 * Added: 21 April 2001 (Trax)
 * Function to display a particular helpfile via the web interface
 */
void display_web_help( WEB_DESCRIPTOR * wdesc, char *path )
{
   HELP_DATA *pHelp;
   char buf[MAX_STRING_LENGTH], *ptr;

   send_200OK( wdesc );
   send_content( wdesc, CONTENT_HTML );

   send_buf( wdesc->fd,
             "<title>Web Based Help Interface</title>\r\n"
             "<body bgcolor=000000 text=FFFFFF link=FF0000>\r\n"
             "<font size=+2><b>\r\n" "Web Based Help</b></font><br>\r\n" "<hr>\r\n" );

   strcpy( buf, path );

   for( ptr = buf; *ptr != '\0'; ptr++ )
   {
      if( *ptr == '_' )
         *ptr = ' ';
      if( *ptr == '+' )
         *ptr = ' ';
   }

   for( pHelp = first_help; pHelp; pHelp = pHelp->next )
   {
      if( pHelp->level > MAX_WEBHELP_LEVEL )
         continue;

      if( is_name( buf, pHelp->keyword ) )
      {
         char col_buf[MAX_STRING_LENGTH];

         web_colourconv( col_buf, pHelp->text );

         for( ptr = col_buf; *ptr != '\0'; ptr++ )
         {
            if( *ptr == '\r' )
               *ptr = ' ';
         }

         send_buf( wdesc->fd, "<pre><tt>\r\n" );
         send_buf( wdesc->fd, col_buf );
         send_buf( wdesc->fd, "\r\n</pre></tt>\r\n" );

         break;
      }
   }

   if( !pHelp )
   {
      send_buf( wdesc->fd, "Unable to locate help on " );
      send_buf( wdesc->fd, buf );
   }
   send_help_index( wdesc );
   return;
}

/*
 * Added: 21 April 2001 (Trax)
 * Function to handle the incoming requests for the web based help system
 * This parses checks and passes the information as required
 */
void handle_web_help( WEB_DESCRIPTOR * wdesc, char *path )
{
   char *ptr;

   if( *path == '?' )
   {
      /*
       * Okay we are dealing with the form query..
       */
      char keyword[MAX_INPUT_LENGTH], newpath[MAX_STRING_LENGTH];

      strcpy( keyword, path + 2 + strlen( "keyword" ) );

      sprintf( newpath, "/%c/%s", keyword[0], keyword );

      handle_web_help( wdesc, newpath );
      return;
   }

   if( path[0] == '\0' || !strcmp( path, "/" ) || !str_cmp( path, "/index.html" ) )
   {
      send_200OK( wdesc );
      send_content( wdesc, CONTENT_HTML );

      send_buf( wdesc->fd,
                "<title>Web Based Help System</title>\r\n"
                "<body bgcolor=000000 text=FFFFFF link=FF0000>\r\n"
                "<font size=+2><b>\r\n"
                "Web Based Help</b></font>\r\n"
                "<p>\r\n"
                "This section of the web server allows you to browse the helpfiles. "
                "<p>\r\n"
                "Please select a letter from the below list to start browsing the "
                "list of helpfiles or enter a keyword to look for in the lookup text box " "below.\r\n" );

      send_help_index( wdesc );

      return;
   }

   ptr = path + 1;

   if( *ptr >= 'a' && *ptr <= 'z' && *( ptr + 1 ) == '/' && *( ptr + 2 ) == '\0' )
   {
      send_200OK( wdesc );
      send_content( wdesc, CONTENT_HTML );
      send_help_list( wdesc, *ptr );
      send_help_index( wdesc );
      return;
   }

   display_web_help( wdesc, ptr + 2 );

   return;
}

void handle_web_arealist( WEB_DESCRIPTOR * wdesc )
{
   char *print_string = "<tr><td>%s</td><td>%s</td><td>%d - %d</td><td>%d - %d</td></tr>";

   AREA_DATA *pArea;
   char buf[MAX_STRING_LENGTH];

   send_200OK( wdesc );
   send_content( wdesc, CONTENT_HTML );

   send_buf( wdesc->fd, "<table><tr><td>Author</td><td>Area</td><td>Recommened</td><td>Enforced</td></tr>" );

   for( pArea = first_area_name; pArea; pArea = pArea->next_sort_name )
   {
      sprintf( buf, print_string,
               pArea->author, pArea->name,
               pArea->low_soft_range, pArea->hi_soft_range, pArea->low_hard_range, pArea->hi_hard_range );

      send_buf( wdesc->fd, buf );
   }
   send_buf( wdesc->fd, "</table>" );
   return;
}

/*
 *	This was added because of webmasters complaining on how they don't
 *	know how to code.  So web.h was added as well as extra directories
 *	in ../web (public_html and staff_html).     [readme.txt in ../web]
 *
 *	The file:  *.tih means 'Telnet Interface Header' (for beginning
 *	the html files) and *.tif 'Telnet Interface Footer' (for ending
 *	the html files).  The middle is filled in with generated code.
 *
 *	-- Christopher Aaron Haslage (Yakkov) -- 6/3/99 (No Help Needed)
 */

void show_web_file( char *filename, WEB_DESCRIPTOR * wdesc )
{
   char buf[MAX_STRING_LENGTH];
   FILE *fp;
   int num = 0;
   /*
    * char *word;
    */
   char let;


   if( ( fp = fopen( filename, "r" ) ) != NULL )
   {
      while( !feof( fp ) )
      {
/*******************
    for ( ; ; )
    {
        word   = feof( fp ) ? "End" : fread_word( fp );
        fMatch = FALSE;

        switch ( UPPER(word[0]) )
        {
	case '<':
*******************/
/*
    	if ( !strcmp( word, "<!-- CODE -->"))
 	{
 	word = feof ( fp ) ? "End" : fread_word( fp );

        sprintf(buf, "\r\n<P><font color=red>\n"
 		"CODE IS INSERTED HERE!\n"
		"</font><font color=white><P>\r\n");
 	send_buf(wdesc->fd, buf);
 	}
*/
         while( ( let = fgetc( fp ) ) != EOF && num < ( MAX_STRING_LENGTH - 2 ) )
         {
            if( let != '\r' )
               buf[num++] = let;
         }

      }
      buf[num] = '\0';
      fclose( fp );
   }
   else

      sprintf( buf, "\r\n<P><font color=red>\n"
               "ERROR: Missing or corrupted file in the Telnet Interface!\n" "</font><font color=white><P>\r\n" );

   send_buf( wdesc->fd, buf );

}

void handle_who_routine( WEB_DESCRIPTOR * wdesc )
{
   FILE *fp;
   char buf[MAX_STRING_LENGTH], col_buf[MAX_STRING_LENGTH];
   int c;
   int num = 0;

   if( ( fp = fopen( WHO_FILE, "r" ) ) != NULL )
   {
      while( !feof( fp ) )
      {
         while( ( buf[num] = fgetc( fp ) ) != EOF
                && buf[num] != '\n' && buf[num] != '\r' && num < ( MAX_STRING_LENGTH - 2 ) )
            num++;
         c = fgetc( fp );
         if( ( c != '\n' && c != '\r' ) || c == buf[num] )
            ungetc( c, fp );
         buf[num++] = '\n';
         buf[num] = '\0';

         if( strlen( buf ) > 32000 )
         {
            bug( "Strlen Greater then 32000: show_file", 0 );
            buf[32000] = '\0';
         }
         num = 0;
         web_colourconv( col_buf, buf );
         send_buf( wdesc->fd, col_buf );
//      send_buf(wdesc->fd,"<BR>");
      }
      fclose( fp );
   }
   return;
}
