/*******************************************************		Crimson Blade Codebase	Copyright 2000-2002 Noplex (John Bellone)	      http://www.crimsonblade.org		admin@crimsonblade.org		Coders: Noplex, Krowe		 Based on Smaug 1.4a*******************************************************/
/*======================Advanced Immortal Host======================By Noplex with help from Senir and Samson*/
#define MAX_DOMAIN     10typedef struct immortal_host_log IMMORTAL_HOST_LOG;
 struct immortal_host_log 
{
   IMMORTAL_HOST_LOG *next;
   IMMORTAL_HOST_LOG *prev;
   char *host;
   char *date;
};
typedef struct immortal_host IMMORTAL_HOST;
struct immortal_host 
{
   IMMORTAL_HOST *next;
   IMMORTAL_HOST *prev;
   char *name;
   char *domain[MAX_DOMAIN];
   IMMORTAL_HOST_LOG *first_log;
   IMMORTAL_HOST_LOG *last_log;
};
IMMORTAL_HOST * first_imm_host;
IMMORTAL_HOST *last_imm_host;
void load_imm_host( void );
IMMORTAL_HOST *fread_imm_host( FILE * fp );
IMMORTAL_HOST_LOG *fread_imm_host_log( FILE * fp );
void save_imm_host( void );
bool check_immortal_domain( CHAR_DATA * ch, char *host );
DECLARE_DO_FUN( do_immhost );
