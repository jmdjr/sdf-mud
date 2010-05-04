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
/* * File: hometowns.h
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

#define NATION_DIR "../nations/"
#define MAX_HOMETOWNS             10
#define MAX_NATIONS               15

typedef struct hometown_data HOMETOWN_DATA;
typedef struct nation_data NATION_DATA;

struct hometown_data 
{
   NATION_DATA *nation;
   char *name;
   int vnum;
   int recall;
   int nation_vnum;
   int citizens;
   char *description;
};

struct nation_data 
{
   HOMETOWN_DATA *hometown;
   char *name;
   char *leader;
   char *fname;
   int vnum;
   int race;
   int htown_vnum;
   char *description;
};

HOMETOWN_DATA * hometown_list[MAX_HOMETOWNS];
NATION_DATA *nation_list[MAX_NATIONS];

DECLARE_DO_FUN( do_hometowns );
DECLARE_DO_FUN( do_nations );
DECLARE_DO_FUN( do_editnation );
DECLARE_DO_FUN( do_edithometown );
DECLARE_DO_FUN( do_makenation );
DECLARE_DO_FUN( do_makehometown );
DECLARE_DO_FUN( do_destroynation );
DECLARE_DO_FUN( do_destroyhometown );
DECLARE_DO_FUN( do_mp_makecitizen );
DECLARE_DO_FUN( do_mp_revokecitizen );
DECLARE_DO_FUN( do_setnation );

/* hometowns.c */ 
void save_hometowns( void );
void load_hometowns( void );
void save_nations( void );
void load_nations( void );
void update_char_hometown( CHAR_DATA * ch );

HOMETOWN_DATA *fread_hometown( FILE * fp );
NATION_DATA *fread_nation( char *filename );
HOMETOWN_DATA *hometown_lookup( char *name );
NATION_DATA *nation_lookup( char *name );
