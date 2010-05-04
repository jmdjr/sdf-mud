/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*	
 *	Web.h was made because some webmasters don't know the first thing
 *	about code.  So I saw it as a chance to dip my quill into a little
 *	more coding.
 *	
 *	What this is for is use with 'show_web_file' in websvr.c, it will
 *	do about the exact same as 'show_file' but for the WWW.  What this
 *	will do is show HTML files from a directory rather from code.
 *	
 *	The file types go like this:
 *
 *	*.ti	These are files that are just one solid page with no code
 *		before or after it.  Such as an index page or error page.
 *
 *	*.tih	This will be a header (Telnet Interface Header), that will
 *	start out a HTML file with tags or what not, before the code.
 *	Placing graphics for headers is not uncommon and leave a nice touch.
 *	
 *	*.tif	This will be a footer (Telnet Interface Footer), it will clean
 *	up the mess made by the code, and finish the HTML, providing links
 *	and so on and so forth.  This is your interface, be creative :)
 *
 *	This should make those rudy poo webmasters happy, somewhat :)
 *	Just explain that they will have to account for the middle of the html
 *	to be code generated and to format accordingly before and after.
 *
 *	-- Christopher Aaron Haslage (Yakkov) 6/3/99 (No Help)
 */

#define PUBLIC_WEB		"../web/public_html/"
#define STAFF_WEB		"../web/staff_html/"

#define PUB_INDEX		PUBLIC_WEB "index.ti"
#define PUB_ERROR		PUBLIC_WEB "error.ti"
#define PUB_ABOUT		PUBLIC_WEB "about.ti"
#define PUB_IMMLIST_H		PUBLIC_WEB "immlist.tih"
#define PUB_IMMLIST_F		PUBLIC_WEB "immlist.tif"
#define PUB_WIZLIST_H		PUBLIC_WEB "wizlist.tih"
#define PUB_WIZLIST_F		PUBLIC_WEB "wizlist.tif"
#define PUB_WHOLIST_H		PUBLIC_WEB "wholist.tih"
#define PUB_WHOLIST_F		PUBLIC_WEB "wholist.tif"
#define PUB_WWWLIST_H		PUBLIC_WEB "wwwlist.tih"
#define PUB_WWWLIST_F		PUBLIC_WEB "wwwlist.tif"
#define PUB_HELP_H		PUBLIC_WEB "help.tih"
#define PUB_HELP_F		PUBLIC_WEB "help.tif"

#define STA_ERROR		STAFF_WEB "error.ti"

/*
 * Added: 21 April 2001 (Trax)
 * Moved to a .h file as some of this info is gonna be useful elsewhere
 * (and I don't like bloating merc.h/mud.h, its bad enough as it is)
 *
 * Define various places where web pages will reside for the server to serve
 * as well as any other misc files req.
 */
#define SECURE_WEB      STAFF_WEB   /* The physical dir containing the 401 secure tree */
#define SECURE_URL      "/staffarea"   /* The secure URL. http://mud.is.here:5502SECURE_URL */
#define AUTH_DOMAIN     "Staff Area"   /* Secure Area Description (tell me where this is used) */
#define MAXDATA 1024
#define WEB_PASS_FILE   SYSTEM_DIR "webpass.dat"   /* Web Password Storage */
#define WEB_ROOT	PUBLIC_WEB  /* General file storage if not caught by a special case */
#define MAX_WEBHELP_LEVEL 100 /* The level that webhelps are displayed for (inclusive) */

/* HTTP Basic Auth req. Base64 encoded/decode */
void Base64Decode args( ( char *bufcoded, unsigned char *bufplain, int outbufsize ) );

typedef struct web_descriptor WEB_DESCRIPTOR;

struct web_descriptor
{
   int fd;
   char request[MAXDATA * 2];
   struct sockaddr_in their_addr;
   socklen_t sin_size;
   WEB_DESCRIPTOR *next;
   bool valid;
   bool keepalive;
};

typedef struct web_password WEB_PASS;

struct web_password
{
   WEB_PASS *next;
   char *username;
   char *password;
};


WEB_PASS *web_passwords;
WEB_DESCRIPTOR *web_desc_free;

void web_colourconv( char *buffer, const char *txt );

/*
 * Content type stuff
 * This should let us use multiple filetypes
 * behind the server (graphics, html, text etc..)
 * all based on suffix matching   
 */
#define CONTENT_HTML    1
#define CONTENT_TEXT    2
#define CONTENT_GIF     3
#define CONTENT_JPEG    4
#define CONTENT_GZIP    5
#define CONTENT_WAV     6
#define CONTENT_VRML    7
#define CONTENT_CLASS   8

struct type_data
{
   char *suffix;
   int type;
};

/* FUNCTION DEFS */
int send_buf( int fd, const char *buf );
void handle_web_request( WEB_DESCRIPTOR * wdesc );
void handle_web_who_request( WEB_DESCRIPTOR * wdesc );
void handle_wwwlist_request( WEB_DESCRIPTOR * wdesc );
void handle_web_main( WEB_DESCRIPTOR * wdesc );
void handle_web_unfound( WEB_DESCRIPTOR * wdesc );
void handle_web_wizlist( WEB_DESCRIPTOR * wdesc );
void handle_web_immlist( WEB_DESCRIPTOR * wdesc );
void handle_images( WEB_DESCRIPTOR * wdesc, char *path );
void handle_web_about( WEB_DESCRIPTOR * wdesc );
void handle_who_routine( WEB_DESCRIPTOR * wdesc );
void show_web_file( char *filename, WEB_DESCRIPTOR * wdesc );
void handle_web_root( WEB_DESCRIPTOR * wdesc, char *path );
void handle_web_imc( WEB_DESCRIPTOR * wdesc, char *path );
void handle_web_i3( WEB_DESCRIPTOR * wdesc, char *path );
void handle_web_help( WEB_DESCRIPTOR * wdesc, char *path );
void handle_web_arealist( WEB_DESCRIPTOR * wdesc );

bool check_web_pass( char *username, char *password );
void handle_secure_web( WEB_DESCRIPTOR * wdesc, char *username, char *password, char *path );
void save_webpass( void );

WEB_DESCRIPTOR *new_web_desc( void );
void free_web_desc( WEB_DESCRIPTOR * desc );

/* The IMC interface functions (web-imc.c) */
#ifdef IMC2WLIST
const char *web_imc_list( void );
#endif

#ifdef I3
#ifdef I3WLIST
void web_I3_mudlist( WEB_DESCRIPTOR * wdesc );
#endif
#endif
