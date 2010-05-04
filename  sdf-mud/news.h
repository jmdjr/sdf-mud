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
 * File: news.h
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

#define NEWS_VERSION "v3.03"
#define NEWS_FILE "news.dat"
#define NEWS_TOP "&G&Y[&W&uRm2  News&g&Y]&g\r\n"
#define NEWS_HEADER "&G&Y[ &W#&Y] [&WPoster&Y]  [&WDate&Y]       [&WSubject&Y]&g\r\n"
#define NEWS_VIEW               15
#define NEWS_MAX_TYPES          10
#define NEWS_HTML_DIR           "/home/badas/public_html/"
#define MAX_POSTERS             20

DECLARE_DO_FUN( do_editnews );

typedef struct news_data NEWS;
struct news_data
{
   NEWS *next;
   NEWS *prev;
   void *temp;
   int number;
   int type;
   char *title;
   char *name;
   char *post;
   char *date;
};

typedef struct news_type NEWS_TYPE;
struct news_type
{
   NEWS *first_news;
   NEWS *last_news;
   NEWS_TYPE *next;
   NEWS_TYPE *prev;
   int vnum;
   int org_type;
   int post_level;
   int view_level;
   int html_max_view;
   char *owner;
   char *header;
   char *cmd_name;
   char *name;
   char *posters[MAX_POSTERS];
   char *org_name;
   char *html_news_path;
   char *html_txt_fname;
   bool locked;
};

NEWS_TYPE *first_news_type;
NEWS_TYPE *last_news_type;

/* news.c */
NEWS *grab_news args( ( NEWS_TYPE * type, char *str ) );
NEWS_TYPE *figure_type args( ( char *str ) );
void display_news args( ( CHAR_DATA * ch, NEWS * news, NEWS_TYPE * type ) );
void renumber_news args( ( void ) );
void save_news args( ( void ) );
void load_news args( ( void ) );
void fread_news args( ( NEWS * news, FILE * fp ) );
char *stamp_time args( ( void ) );
void html_news args( ( void ) );
void snarf_news args( ( FILE * fp, NEWS_TYPE * type ) );
void display_news_type args( ( CHAR_DATA * ch, NEWS_TYPE * type, char *argument ) );
void fread_news_type args( ( NEWS_TYPE * type, FILE * fp ) );
bool news_cmd_hook args( ( CHAR_DATA * ch, char *cmd, char *argument ) );
void link_news_to_type args( ( NEWS * news ) );
bool can_post_news args( ( CHAR_DATA * ch, NEWS_TYPE * type ) );
bool can_read_news args( ( CHAR_DATA * ch, NEWS_TYPE * type ) );
