/*****************************************************************************
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
 * Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
 * Chastain, Michael Quan, and Mitchell Tse.                                *
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
 * Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
 * Win32 port by Nick Gammon                                                *
 * ------------------------------------------------------------------------ *
 *			    Main mud header file			    *
 ****************************************************************************/

#include <stdlib.h>
#include <sys/time.h>


#ifndef MUD_HFILE
#define MUD_HFILE

#ifdef WIN32
#include <winsock.h>
#include <sys/types.h>
#pragma warning( disable: 4018 4244 4761)
#define index strchr
#define rindex strrchr
#pragma warning( disable : 4550 )   /* NJG */
#define vsnprintf _vsnprintf  /* NJG */
#define snprintf _snprintf /* NJG */
#define lstat stat   /* NJG */
#pragma comment( lib, "ws2_32.lib" )   /* NJG */
#pragma comment( lib, "winmm.lib" ) /* NJG */
#endif

typedef int ch_ret;
typedef int obj_ret;

#define args( list )			list
#define DECLARE_DO_FUN( fun )		DO_FUN    fun
#define DECLARE_SPEC_FUN( fun )	SPEC_FUN  fun
#define DECLARE_SPELL_FUN( fun )	SPELL_FUN fun

/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
#if	!defined(FALSE)
#define FALSE	 0
#endif

#if	!defined(TRUE)
#define TRUE	 1
#endif

#if	!defined(BERR)
#define BERR	 255
#endif

#ifndef __cplusplus
typedef unsigned char bool;
#endif

#define KEY( literal, field, value ) \
if( !str_cmp( word, (literal) ) )    \
{                                    \
   (field) = (value);                \
   fMatch = TRUE;                    \
   break;                            \
}

#ifdef MCCP
/* MCCP */
#include <zlib.h>
#define TELOPT_COMPRESS 85
#define COMPRESS_BUF_SIZE 8192
#endif

/*
 * Structure types.
 */
typedef struct alias_data ALIAS_DATA;
typedef struct affect_data AFFECT_DATA;
typedef struct area_data AREA_DATA;
typedef struct auction_data AUCTION_DATA; /* auction data */
typedef struct watch_data WATCH_DATA;
typedef struct ban_data BAN_DATA;
typedef struct extracted_char_data EXTRACT_CHAR_DATA;
typedef struct char_data CHAR_DATA;
typedef struct hunt_hate_fear HHF_DATA;
typedef struct illegalpkill IPK_DATA;
typedef struct fighting_data FIGHT_DATA;
typedef struct descriptor_data DESCRIPTOR_DATA;
typedef struct exit_data EXIT_DATA;
typedef struct extra_descr_data EXTRA_DESCR_DATA;
typedef struct help_data HELP_DATA;
typedef struct mob_index_data MOB_INDEX_DATA;
typedef struct char_morph CHAR_MORPH;
typedef struct morph_data MORPH_DATA;
typedef struct nuisance_data NUISANCE_DATA;
typedef struct note_data NOTE_DATA;
typedef struct comment_data COMMENT_DATA;
typedef struct board_data BOARD_DATA;
typedef struct obj_data OBJ_DATA;
typedef struct obj_index_data OBJ_INDEX_DATA;
typedef struct pc_data PC_DATA;
typedef struct plane_data PLANE_DATA;
typedef struct reset_data RESET_DATA;
typedef struct room_index_data ROOM_INDEX_DATA;
typedef struct shop_data SHOP_DATA;
typedef struct race_type RACE_TYPE;
typedef struct repairshop_data REPAIR_DATA;
typedef struct reserve_data RESERVE_DATA;
typedef struct filter_data FILTER_DATA;
typedef struct time_info_data TIME_INFO_DATA;
typedef struct hour_min_sec HOUR_MIN_SEC;
typedef struct weather_data WEATHER_DATA;
typedef struct neighbor_data NEIGHBOR_DATA;  /* FB */
typedef struct clan_data CLAN_DATA;
typedef struct council_data COUNCIL_DATA;
typedef struct tourney_data TOURNEY_DATA;
typedef struct mob_prog_data MPROG_DATA;
typedef struct mob_prog_act_list MPROG_ACT_LIST;
typedef struct editor_data EDITOR_DATA;
typedef struct teleport_data TELEPORT_DATA;
typedef struct timer_data TIMER;
typedef struct system_data SYSTEM_DATA;
typedef struct smaug_affect SMAUG_AFF;
typedef struct who_data WHO_DATA;
typedef struct skill_type SKILLTYPE;
typedef struct social_type SOCIALTYPE;
typedef struct cmd_type CMDTYPE;
typedef struct killed_data KILLED_DATA;
typedef struct deity_data DEITY_DATA;
typedef struct wizent WIZENT;
typedef struct ignore_data IGNORE_DATA;
typedef struct immortal_host IMMORTAL_HOST;
typedef struct project_data PROJECT_DATA;
typedef struct extended_bitvector EXT_BV;
typedef struct lcnv_data LCNV_DATA;
typedef struct lang_data LANG_DATA;
typedef struct olc_data OLC_DATA;   /* Tagith */
typedef struct arena_data ARENA_DATA;  /* Kratas */
typedef struct bit_data BIT_DATA;
typedef struct room_to_be_decomped ROOM_DECOMP; /* Kratas */
typedef struct locker_data LOCKER_DATA;
typedef struct trivia_data TRIVIA_DATA;   /* Garinan */

/*
 * Function types.
 */
typedef void DO_RET( CHAR_DATA * ch );
typedef void DO_FUN( CHAR_DATA * ch, char *argument );
typedef ch_ret SPELL_FUN( int sn, int level, CHAR_DATA * ch, void *vo );
typedef bool SPEC_FUN( CHAR_DATA * ch );

#define DUR_CONV	23.333333333333333333333333
#define HIDDEN_TILDE	'*'

/* 32bit bitvector defines */
#define BV00		(1 <<  0)
#define BV01		(1 <<  1)
#define BV02		(1 <<  2)
#define BV03		(1 <<  3)
#define BV04		(1 <<  4)
#define BV05		(1 <<  5)
#define BV06		(1 <<  6)
#define BV07		(1 <<  7)
#define BV08		(1 <<  8)
#define BV09		(1 <<  9)
#define BV10		(1 << 10)
#define BV11		(1 << 11)
#define BV12		(1 << 12)
#define BV13		(1 << 13)
#define BV14		(1 << 14)
#define BV15		(1 << 15)
#define BV16		(1 << 16)
#define BV17		(1 << 17)
#define BV18		(1 << 18)
#define BV19		(1 << 19)
#define BV20		(1 << 20)
#define BV21		(1 << 21)
#define BV22		(1 << 22)
#define BV23		(1 << 23)
#define BV24		(1 << 24)
#define BV25		(1 << 25)
#define BV26		(1 << 26)
#define BV27		(1 << 27)
#define BV28		(1 << 28)
#define BV29		(1 << 29)
#define BV30		(1 << 30)
#define BV31		(1 << 31)
/* 32 USED! DO NOT ADD MORE! SB */

/*
 * String and memory management parameters.
 */
/* Desperation move on my part- the crashing started with the new 30+ areas,
   the links seem to be dropping on almost everything, so... what the hell
   ... maybe the btree cant support as many levels as are being created
       shogar
*/
#define MAX_KEY_HASH             16384 /* From 4096, attempt to fix problems
                                        * arising from different places. */
#define MAX_STRING_LENGTH	 4096 /* buf */
#define MSL                      MAX_STRING_LENGTH /* Easier to type -- Kratas */
#define MAX_INPUT_LENGTH	 1024 /* arg */
#define MIL                      MAX_INPUT_LENGTH  /* Easier to type -- Kratas */
#define MAX_INBUF_SIZE           2048
#define LAST_FILE_SIZE       500 //maximum entries in the last file

#define MAX_LAYERS		15 /* maximum clothing layers */
#define MAX_NEST	       10  /* maximum container nesting */

#define MAX_KILLTRACK		10 /* track mob vnums killed */

/*
 * Color Alignment Parameters
 */
#define ALIGN_LEFT	1
#define ALIGN_CENTER	2
#define ALIGN_RIGHT	3

/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 */
#define MAX_EXP_WORTH	       500000
#define MIN_EXP_WORTH		   20

#define MAX_BOARD         7   /* Global Boards */
#define MAX_REXITS		   20 /* Maximum exits allowed in 1 room */
#define MAX_SKILL		  500
#define MAX_CLASS           	  24
#define MAX_NPC_CLASS		   26
#define MAX_PC_CLASS		   26
#define MAX_RACE                   41  /* (was 19)  added more 3/25/99 Scion */
#define MAX_NPC_RACE		   91
#define MAX_LEVEL		   116
#define MAX_CLAN		   50
#define MAX_DEITY		   50
#define MAX_CPD			    4 /* Maximum council power level difference */
#define	MAX_HERB		   20
#define MAX_ALIAS                  20  /* alias - shogar */
#define	MAX_DISEASE		   20
#define MAX_PERSONAL		    5 /* Maximum personal skills */
#define MAX_WHERE_NAME             27
#define LEVEL_HERO		   (MAX_LEVEL - 16)
#define LEVEL_IMMORTAL		   (MAX_LEVEL - 15)
#define LEVEL_OWNER		   MAX_LEVEL
#define LEVEL_SUPREME		   (MAX_LEVEL - 1)
#define LEVEL_INFINITE		   (MAX_LEVEL - 2)
#define LEVEL_ETERNAL		   (MAX_LEVEL - 3)
#define LEVEL_IMPLEMENTOR	   (MAX_LEVEL - 4)
#define LEVEL_SUB_IMPLEM	   (MAX_LEVEL - 5)
#define LEVEL_ASCENDANT		   (MAX_LEVEL - 6)
#define LEVEL_GREATER		   (MAX_LEVEL - 7)
#define LEVEL_GOD		   (MAX_LEVEL - 8)
#define LEVEL_LESSER		   (MAX_LEVEL - 9)
#define LEVEL_TRUEIMM		   (MAX_LEVEL - 10)
#define LEVEL_DEMI		   (MAX_LEVEL - 11)
#define LEVEL_SAVIOR		   (MAX_LEVEL - 12)
#define LEVEL_CREATOR		   (MAX_LEVEL - 13)
#define LEVEL_ACOLYTE		   (MAX_LEVEL - 14)
#define LEVEL_NEOPHYTE		   (MAX_LEVEL - 15)
#define LEVEL_AVATAR		   (MAX_LEVEL - 16) /* 100 */
#define LEVEL_PCAVATAR		   50 /* Player Avs, avatar = demigod */

#define LEVEL_LOG		    LEVEL_LESSER
#define LEVEL_HIGOD		    LEVEL_GOD

#include "house.h"   /* For housing module */
#include "bank.h"
#include "slay.h"
#include "buffer.h"
#include "news.h" /* Extended News - 12/15/01 - Nopey */
#include "hometowns.h"  /* nations and hometowns module 7/2/02 -Nopey */
#include "pfiles.h"
#include "channels.h"   /* Dynamic channel system */

#define	SECONDS_PER_TICK			 70

#define PULSE_PER_SECOND			  4
#define PULSE_VIOLENCE				 (3 * PULSE_PER_SECOND)
#define PULSE_MOBILE				 (4 * PULSE_PER_SECOND)
#define PULSE_TICK		  (SECONDS_PER_TICK * PULSE_PER_SECOND)
#define PULSE_AREA				(60 * PULSE_PER_SECOND)
#define PULSE_AUCTION				 (9 * PULSE_PER_SECOND)

/*
 * Stuff for area versions --Shaddai
 */
int area_version;
#define HAS_SPELL_INDEX     -1
#define AREA_VERSION_WRITE 2

/* short cut crash bug fix provided by gfinello@mail.karmanet.it*/
typedef enum
{
   relMSET_ON, relOSET_ON
} relation_type;

typedef struct rel_data REL_DATA;

struct rel_data
{
   void *Actor;
   void *Subject;
   REL_DATA *next;
   REL_DATA *prev;
   relation_type Type;
};

/*
 * Command logging types.
 */
typedef enum
{
   LOG_NORMAL, LOG_ALWAYS, LOG_NEVER, LOG_BUILD, LOG_HIGH, LOG_COMM,
   LOG_WARN, LOG_ALL
} log_types;

/*
 * Return types for move_char, damage, greet_trigger, etc, etc
 * Added by Thoric to get rid of bugs
 */
typedef enum
{
   rNONE, rCHAR_DIED, rVICT_DIED, rBOTH_DIED, rCHAR_QUIT, rVICT_QUIT,
   rBOTH_QUIT, rSPELL_FAILED, rOBJ_SCRAPPED, rOBJ_EATEN, rOBJ_EXPIRED,
   rOBJ_TIMER, rOBJ_SACCED, rOBJ_QUAFFED, rOBJ_USED, rOBJ_EXTRACTED,
   rOBJ_DRUNK, rCHAR_IMMUNE, rVICT_IMMUNE, rCHAR_AND_OBJ_EXTRACTED = 128,
   rERROR = 255
} ret_types;

/* Echo types for echo_to_all */
#define ECHOTAR_ALL	0
#define ECHOTAR_PC	1
#define ECHOTAR_IMM	2

/* defines for new do_who */
#define WT_MORTAL	0
#define WT_DEADLY	1
#define WT_IMM		2
#define WT_GROUPED	3
#define WT_GROUPWHO	4
#define WT_GUEST	5
#define WT_STAFF	6
#define WT_ADMIN	7

/*
 * Defines for extended bitvectors
 */
#ifndef INTBITS
#define INTBITS	32
#endif
#define XBM		31 /* extended bitmask   ( INTBITS - 1 )  */
#define RSV		5  /* right-shift value  ( sqrt(XBM+1) )  */
#define XBI		4  /* integers in an extended bitvector   */
#define MAX_BITS	XBI * INTBITS
/*
 * Structure for extended bitvectors -- Thoric
 */
struct extended_bitvector
{
   unsigned int bits[XBI];
};

#ifdef I3
#include "i3.h"
#endif

#ifdef IMC
#include "imc.h"
#endif

#define MAX_xBITS 32000

/* abit and qbit struct */
struct bit_data
{
   int number;
   char desc[MAX_STRING_LENGTH];

   BIT_DATA *next;
   BIT_DATA *prev;
};

extern BIT_DATA *first_abit;
extern BIT_DATA *first_qbit;
extern BIT_DATA *last_abit;
extern BIT_DATA *last_qbit;

/*
 * Structure for a morph -- Shaddai
 */
/*
 *  Morph structs.
 */

#define ONLY_PKILL  	1
#define ONLY_PEACEFULL  2

struct char_morph
{
   MORPH_DATA *morph;
   EXT_BV affected_by;  /* New affected_by added */
   int immune; /* Immunities added */
   EXT_BV no_affected_by;  /* Prevents affects from being added */
   int no_immune; /* Prevents Immunities */
   int no_resistant; /* Prevents resistances */
   int no_suscept;   /* Prevents Susceptibilities */
   int resistant; /* Resistances added */
   int suscept;   /* Suscepts added */
   int timer;  /* How much time is left */
   short ac;
   short blood;
   short cha;
   short con;
   short damroll;
   short dex;
   short dodge;
   short hit;
   short hitroll;
   short inte;
   short lck;
   short mana;
   short move;
   short parry;
   short saving_breath;
   short saving_para_petri;
   short saving_poison_death;
   short saving_spell_staff;
   short saving_wand;
   short str;
   short tumble;
   short wis;
};

struct morph_data
{
   MORPH_DATA *next; /* Next morph file */
   MORPH_DATA *prev; /* Previous morph file */
   char *blood;   /* Blood added vamps only */
   char *damroll;
   char *deity;
   char *description;
   char *help; /* What player sees for info on morph */
   char *hit;  /* Hitpoints added */
   char *hitroll;
   char *key_words;  /* Keywords added to your name */
   char *long_desc;  /* New long_desc for player */
   char *mana; /* Mana added not for vamps */
   char *morph_other;   /* What others see when you morph */
   char *morph_self; /* What you see when you morph */
   char *move; /* Move added */
   char *name; /* Name used to polymorph into this */
   char *short_desc; /* New short desc for player */
   char *no_skills;  /* Prevented Skills */
   char *skills;
   char *unmorph_other; /* What others see when you unmorph */
   char *unmorph_self;  /* What you see when you unmorph */
   EXT_BV affected_by;  /* New affected_by added */
   int class;  /* Classes not allowed to use this */
   int defpos; /* Default position */
   int immune; /* Immunities added */
   EXT_BV no_affected_by;  /* Prevents affects from being added */
   int no_immune; /* Prevents Immunities */
   int no_resistant; /* Prevents resistances */
   int no_suscept;   /* Prevents Susceptibilities */
   int obj[3]; /* Object needed to morph you */
   int race;   /* Races not allowed to use this */
   int resistant; /* Resistances added */
   int suscept;   /* Suscepts added */
   int timer;  /* Timer for how long it lasts */
   int used;   /* How many times has this morph been used */
   int vnum;   /* Unique identifier */
   short ac;
   short bloodused;  /* Amount of blood morph requires Vamps only */
   short cha;  /* Amount Cha gained/Lost */
   short con;  /* Amount of Con gained/Lost */
   short dayfrom; /* Starting Day you can morph into this */
   short dayto;   /* Ending Day you can morph into this */
   short dex;  /* Amount of dex added */
   short dodge;   /* Percent of dodge added IE 1 = 1% */
   short favourused; /* Amount of favour to morph */
   short gloryused;  /* Amount of glory used to morph */
   short hpused;  /* Amount of hps used to morph */
   short inte; /* Amount of Int gained/lost */
   short lck;  /* Amount of Lck gained/lost */
   short level;   /* Minimum level to use this morph */
   short manaused;   /* Amount of mana used to morph */
   short moveused;   /* Amount of move used to morph */
   short parry;   /* Percent of parry added IE 1 = 1% */
   short pkill;   /* Pkill Only, Peacefull Only or Both */
   short saving_breath; /* Below are saving adjusted */
   short saving_para_petri;
   short saving_poison_death;
   short saving_spell_staff;
   short saving_wand;
   short sex;  /* The sex that can morph into this */
   short str;  /* Amount of str gained lost */
   short timefrom;   /* Hour starting you can morph */
   short timeto;  /* Hour ending that you can morph */
   short tumble;  /* Percent of tumble added IE 1 = 1% */
   short wis;  /* Amount of Wis gained/lost */
   bool no_cast;  /* Can you cast a spell to morph into it */
   bool objuse[3];   /* Objects needed to morph */
};

/*
 * Tongues / Languages structures
 */

struct lcnv_data
{
   LCNV_DATA *next;
   LCNV_DATA *prev;
   char *old;
   int olen;
   char *new;
   int nlen;
};

struct lang_data
{
   LANG_DATA *next;
   LANG_DATA *prev;
   char *name;
   LCNV_DATA *first_precnv;
   LCNV_DATA *last_precnv;
   char *alphabet;
   LCNV_DATA *first_cnv;
   LCNV_DATA *last_cnv;
};

/*
 * do_who output structure -- Narn
 */
struct who_data
{
   WHO_DATA *prev;
   WHO_DATA *next;
   char *text;
   int type;
};

/*
 * Player watch data structure  --Gorog
 */
struct watch_data
{
   WATCH_DATA *next;
   WATCH_DATA *prev;
   short imm_level;
   char *imm_name;   /* imm doing the watching */
   char *target_name;   /* player or command being watched   */
   char *player_site;   /* site being watched     */
};

/*
 * Nuisance structure
 */
#define MAX_NUISANCE_STAGE 10 /* How many nuisance stages */
struct nuisance_data
{
   time_t time; /* The time nuisance flag was set */
   long int max_time;   /* Time for max penalties */
   int flags;  /* Stage of nuisance */
   int power;  /* Power of nuisance */
};

/*
 * Ban Types --- Shaddai
 */
#define BAN_SITE        1
#define BAN_CLASS       2
#define BAN_RACE        3
#define BAN_WARN        -1

/*
 * Site ban structure.
 */
struct ban_data
{
   BAN_DATA *next;
   BAN_DATA *prev;
   char *name; /* Name of site/class/race banned */
   char *user; /* Name of user from site */
   char *note; /* Why it was banned */
   char *ban_by;  /* Who banned this site */
   char *ban_time;   /* Time it was banned */
   int flag;   /* Class or Race number */
   int unban_date;   /* When ban expires */
   short duration;   /* How long it is banned for */
   short level;   /* Level that is banned */
   bool warn;  /* Echo on warn channel */
   bool prefix;   /* Use of *site */
   bool suffix;   /* Use of site* */
};

/*
 * Yeesh.. remind us of the old MERC ban structure? :)
 */
struct reserve_data
{
   RESERVE_DATA *next;
   RESERVE_DATA *prev;
   char *name;
};

struct filter_data
{
   FILTER_DATA *next;
   FILTER_DATA *prev;
   char *name;
};

/*
 * Time and weather stuff.
 */
typedef enum
{
   SUN_DARK, SUN_RISE, SUN_LIGHT, SUN_SET
} sun_positions;

typedef enum
{
   SKY_CLOUDLESS, SKY_CLOUDY, SKY_RAINING, SKY_LIGHTNING
} sky_conditions;

struct time_info_data
{
   int hour;
   int day;
   int month;
   int year;
   int sunlight;
};

struct hour_min_sec
{
   int hour;
   int min;
   int sec;
   int manual;
};

/* Define maximum number of climate settings - FB */
#define MAX_CLIMATE 5

struct weather_data
{
/*    int			mmhg;
    int			change;
    int			sky;
    int			sunlight; */
   int temp;   /* temperature */
   int precip; /* precipitation */
   int wind;   /* umm... wind */
   int temp_vector;  /* vectors controlling */
   int precip_vector;   /* rate of change */
   int wind_vector;
   int climate_temp; /* climate of the area */
   int climate_precip;
   int climate_wind;
   NEIGHBOR_DATA *first_neighbor;   /* areas which affect weather sys */
   NEIGHBOR_DATA *last_neighbor;
   char *echo; /* echo string */
   int echo_color;   /* color for the echo */
};

struct neighbor_data
{
   NEIGHBOR_DATA *next;
   NEIGHBOR_DATA *prev;
   char *name;
   AREA_DATA *address;
};

/*
 * Structure used to build wizlist
 */
struct wizent
{
   WIZENT *next;
   WIZENT *last;
   char *name;
   short level;
};

/*
 * Structure to only allow immortals domains to access their chars.
 */
struct immortal_host
{
   IMMORTAL_HOST *next;
   IMMORTAL_HOST *prev;
   char *name;
   char *host;
   bool prefix;
   bool suffix;
};

struct project_data
{
   PROJECT_DATA *next;  /* Next project in list       */
   PROJECT_DATA *prev;  /* Previous project in list      */
   NOTE_DATA *first_log;   /* First log on project       */
   NOTE_DATA *last_log; /* Last log  on project       */
   char *name;
   char *owner;
   char *coder;
   char *status;
   char *date;
   char *description;
   bool taken; /* Has someone taken project?      */
};

/*
 * Connected state for a channel.
 */
typedef enum
{
   CON_GET_NAME = -100,
   CON_GET_OLD_PASSWORD,
   CON_CONFIRM_NEW_NAME,
   CON_GET_NEW_PASSWORD,
   CON_CONFIRM_NEW_PASSWORD,
   CON_GET_NEW_SEX,
   CON_GET_NEW_CLASS,
   CON_READ_MOTD,
   CON_GET_NEW_RACE,
   CON_GET_EMULATION,
   CON_GET_WANT_RIPANSI,
   CON_TITLE,
   CON_PRESS_ENTER,
   CON_WAIT_1,
   CON_WAIT_2,
   CON_WAIT_3,
   CON_ACCEPTED,
   CON_GET_PKILL,
   CON_READ_IMOTD,
   CON_COPYOVER_RECOVER,

   CON_PLAYING = 0,
   CON_EDITING,
   CON_OEDIT,
   CON_MEDIT,
   CON_REDIT,

   CON_MAINMENU,  /* Scion */
   CON_MAINMENU2,

   CON_ANSI,   /* Josh */
   CON_GET_HAIR_COLOR,  /* Josh */
   CON_GET_HAIR_LENGTH, /* Josh */
   CON_GET_EYE_COLOR,   /* Josh */
   CON_GET_SKIN_TONE,   /* Josh */
   CON_GET_BUILD, /* Josh */
   CON_GET_HEIGHT,   /* Josh */

   CON_NOTE_TO,   /* next 5 -- Kratas */
   CON_NOTE_SUBJECT,
   CON_NOTE_EXPIRE,
   CON_NOTE_TEXT,
   CON_NOTE_FINISH
} connection_types;

/*
 * Character substates
 */
typedef enum
{
   SUB_NONE, SUB_PAUSE, SUB_PERSONAL_DESC, SUB_BAN_DESC, SUB_OBJ_SHORT,
   SUB_OBJ_LONG, SUB_OBJ_EXTRA, SUB_MOB_LONG, SUB_MOB_DESC, SUB_ROOM_DESC,
   SUB_ROOM_EXTRA, SUB_ROOM_EXIT_DESC, SUB_WRITING_NOTE, SUB_MPROG_EDIT,
   SUB_HELP_EDIT, SUB_WRITING_MAP, SUB_PERSONAL_BIO, SUB_REPEATCMD,
   SUB_RESTRICTED, SUB_DEITYDESC, SUB_MORPH_DESC, SUB_MORPH_HELP,
   SUB_PROJ_DESC, SUB_SLAYCMSG, SUB_SLAYVMSG, SUB_SLAYRMSG, SUB_GBOARD_NOTE,
   SUB_NEWS_POST, SUB_NEWS_EDIT, SUB_HTOWN_DESC, SUB_NATION_DESC, SUB_WRITING_EMAIL,
   /*
    * timer types ONLY below this point
    */
   SUB_TIMER_DO_ABORT = 128, SUB_TIMER_CANT_ABORT
} char_substates;

/*
 * Descriptor (channel) structure.
 */
struct descriptor_data
{
   DESCRIPTOR_DATA *next;
   DESCRIPTOR_DATA *prev;
   DESCRIPTOR_DATA *snoop_by;
   CHAR_DATA *character;
   CHAR_DATA *original;
   char *host;
   int port;
   int descriptor;
   short connected;
   short idle;
   short lines;
   short scrlen;
   bool fcommand;
   char inbuf[MAX_INBUF_SIZE];
   char incomm[MAX_INPUT_LENGTH];
   char inlast[MAX_INPUT_LENGTH];
   int repeat;
   char *outbuf;
   unsigned long outsize;
   int outtop;
   char *pagebuf;
   unsigned long pagesize;
   int pagetop;
   char *pagepoint;
   char pagecmd;
   char pagecolor;
   char *user;
   int newstate;
   unsigned char prevcolor;
#ifdef MCCP
   z_stream *out_compress;
   unsigned char *out_compress_buf;
#endif
   OLC_DATA *olc; /* Tagith */
   bool ansi;  /* Josh */
};

/*
 * Attribute bonus structures.
 */
struct str_app_type
{
   short tohit;
   short todam;
   short carry;
   short wield;
};

struct int_app_type
{
   short learn;
};

struct wis_app_type
{
   short practice;
};

struct dex_app_type
{
   short defensive;
};

struct con_app_type
{
   short hitp;
   short shock;
};

struct cha_app_type
{
   short charm;
};

struct lck_app_type
{
   short luck;
};

/* the races */
typedef enum
{
   RACE_HUMAN, RACE_ELF, RACE_DWARF, RACE_HALFLING, RACE_PIXIE, RACE_VAMPIRE,
   RACE_HALF_OGRE, RACE_HALF_ORC, RACE_HALF_TROLL, RACE_HALF_ELF, RACE_GITH,
   RACE_DROW, RACE_SEA_ELF, RACE_LIZARDMAN, RACE_GNOME, RACE_MINOTAUR, RACE_SPRITE,
   RACE_LEPRECHAUN, RACE_KENDER, RACE_ARIEL, RACE_CENTAUR, RACE_SLITH, RACE_MRRSHAN,
   RACE_WRAITH, RACE_HARPY, RACE_GOBLIN, RACE_KOBOLD, RACE_NYMPH, RACE_TROGLODYTE,
   RACE_GARGOYLE
} race_types;

/* npc races */
/* #define	RACE_DRAGON	    31 */

/* Character Attribute stuff */
enum
{
   HAIR_COLOR, HAIR_LENGTH, EYE_COLOR, SKIN_TONE, RACE_BUILD, RACE_HEIGHT, MAX_TYPES
};
#define MAX_AMOUNT 16

/* Hair Colors */
#define HAIR_BLACK      BV00
#define HAIR_WHITE      BV01
#define HAIR_GRAY       BV02
#define HAIR_BROWN      BV03
#define HAIR_LBROWN     BV04
#define HAIR_DBROWN     BV05
#define HAIR_BLOND      BV06
#define HAIR_LBLOND     BV07
#define HAIR_DBLOND     BV08
#define HAIR_GBLOND     BV09
#define HAIR_BBLOND     BV10
#define HAIR_RED        BV11
#define HAIR_RBROWN     BV12
#define HAIR_RBLOND     BV13
#define HAIR_BRED       BV14
#define HAIR_ORED       BV15
#define HAIR_SILVER     BV16
#define HAIR_STRAWBERRY BV17
/* End Hair Colors */

/* Hair Lengths */
#define HAIR_BALD       BV00
#define HAIR_CREWCUT    BV01
#define HAIR_SHORT      BV03
#define HAIR_MEDIUM     BV04
#define HAIR_LONG       BV05
#define HAIR_WAIST      BV06
/* End Hair Lengths */

/* Eye Colors */
#define EYE_AQUA       BV00
#define EYE_BROWN      BV01
#define EYE_GREEN      BV02
#define EYE_TEAL       BV03
#define EYE_BLUEGREEN  BV04
#define EYE_HAZEL      BV05
#define EYE_BLUE       BV06
#define EYE_HBLUE      BV07
#define EYE_HGREEN     BV08
#define EYE_LBROWN     BV09
#define EYE_BBLUE      BV10
#define EYE_JADE       BV11
#define EYE_RED        BV12
#define EYE_BRED       BV13
#define EYE_SILVER     BV14
/* End Eye Colors */

/* Skin Tones */
#define SKIN_BROWN      BV00
#define SKIN_PALE       BV01
#define SKIN_RED        BV03
#define SKIN_WHITE      BV04
#define SKIN_YELLOW     BV05
#define SKIN_TAN        BV06
#define SKIN_GREEN      BV07
#define SKIN_BLUE       BV08
#define SKIN_GOLD       BV09
#define SKIN_SILVER     BV10
#define SKIN_BRONZE     BV11
#define SKIN_COPPER     BV12
#define SKIN_GRAY       BV13
/* End Skin Tones */

/* Builds */
#define BUILD_BONE      BV00
#define BUILD_THIN      BV01
#define BUILD_MEDIUM    BV02
#define BUILD_LARGE     BV03
#define BUILD_BUFF      BV04
#define BUILD_HARDY     BV05
/* End Builds */

/* Heights */
#define HEIGHT_MIDGET     BV00
#define HEIGHT_SHORT      BV01
#define HEIGHT_MEDIUM     BV02
#define HEIGHT_TALL       BV03
#define HEIGHT_TOWERING   BV04
/* End Heights */

/* End Character Attribute Stuff */

#define CLASS_NONE	   -1 /* For skill/spells according to guild */
#define CLASS_MAGE	    0
#define CLASS_CLERIC	    1
#define CLASS_THIEF	    2
#define CLASS_WARRIOR	    3
#define CLASS_VAMPIRE	    4
#define CLASS_DRUID	    5
#define CLASS_RANGER	    6
#define CLASS_AUGURER	    7 /* 7-7-96 SB */
#define CLASS_PALADIN	    8 /* 7-7-96 SB */
#define CLASS_DRAGON	    9 /* Added By Stoker 3/6/99 */
#define CLASS_DEMON        10 /* Added By Stoker 3/6/99 */
#define CLASS_WEREWOLF     11 /* 3/9/99 Scion */
#define CLASS_ILLUSIONIST  12 /* 3/9/99 Scion */
#define CLASS_NECROMANCER  13 /* 3/9/99 Scion */
#define CLASS_ASSASSIN     14 /* 3/9/99 Scion */
#define CLASS_WARLOCK      15 /* 3/9/99 Scion */
#define CLASS_BATTLEMAGE      16 /* 3/9/99 Scion */
#define CLASS_KNIGHT       17 /* 3/9/99 Scion */
#define CLASS_ANTIPALADIN  18 /* 3/9/99 Scion */
#define CLASS_HARPER       19 /* 3/9/99 Scion */
#define CLASS_MONK         20 /* 3/9/99 Scion */
#define CLASS_ROGUE        21 /* 3/18/99 Scion */
#define CLASS_ANGEL        22 /* Added By Stoker 3/22/99 */
#define CLASS_PSIONICIST   23 /* 3/22/99 Scion */

/*
 * Languages -- Altrag
 */
#define LANG_COMMON      BV00 /* 1 Human base language */
#define LANG_ELVEN       BV01 /* 2 Elven base language */
#define LANG_DWARVEN     BV02 /* 4 Dwarven base language */
#define LANG_PIXIE       BV03 /* 8 Pixie/Fairy base language */
#define LANG_OGRE        BV04 /* 16 Ogre base language */
#define LANG_ORCISH      BV05 /* 32 Orc base language */
#define LANG_TROLLISH    BV06 /* 64 Troll base language */
#define LANG_RODENT      BV07 /* 128 Small mammals */
#define LANG_INSECTOID   BV08 /* 256 Insects */
#define LANG_MAMMAL      BV09 /* 512 Larger mammals */
#define LANG_REPTILE     BV10 /* 1024 Small reptiles */
#define LANG_DRAGON      BV11 /* 2048 Large reptiles, Dragons */
#define LANG_SPIRITUAL   BV12 /* 4096 Necromancers or undeads/spectres */
#define LANG_MAGICAL     BV13 /* 8192 Spells maybe?  Magical creatures */
#define LANG_GOBLIN      BV14 /* Goblin base language */
#define LANG_GOD         BV15 /* Clerics possibly?  God creatures */
#define LANG_ANCIENT     BV16 /* Prelude to a glyph read skill? */
#define LANG_HALFLING    BV17 /* Halfling base language */
#define LANG_CLAN	 BV18 /* Clan language */
#define LANG_GITH	 BV19 /* Gith Language */
#define LANG_GNOME       BV20 /* Gnome Language */
#define LANG_UNKNOWN        0 /* Anything that doesnt fit a category */
#define VALID_LANGS    ( LANG_COMMON | LANG_ELVEN | LANG_DWARVEN | LANG_PIXIE  \
		       | LANG_OGRE | LANG_ORCISH | LANG_TROLLISH | LANG_GOBLIN \
		       | LANG_HALFLING | LANG_GITH | LANG_GNOME)
/* 18 Languages */

/*
 * TO types for act.
 */
typedef enum
{ TO_ROOM, TO_NOTVICT, TO_VICT, TO_CHAR, TO_CANSEE, TO_THIRD } to_types;

/*
 * Real action "TYPES" for act.
 */
#define AT_BLACK	    0
#define AT_BLOOD	    1
#define AT_DGREEN           2
#define AT_ORANGE	    3
#define AT_DBLUE	    4
#define AT_PURPLE	    5
#define AT_CYAN	  	    6
#define AT_GREY		    7
#define AT_DGREY	    8
#define AT_RED		    9
#define AT_GREEN	   10
#define AT_YELLOW	   11
#define AT_BLUE		   12
#define AT_PINK		   13
#define AT_LBLUE	   14
#define AT_WHITE	   15
#define AT_BLINK	   16
#define AT_PLAIN	   AT_GREY
#define AT_ACTION	   AT_GREY
#define AT_SAY		   AT_LBLUE
#define AT_SAYTO	   AT_LBLUE
#define AT_GOSSIP	   AT_GREEN
#define AT_YELL	           AT_GREEN
#define AT_TELL		   AT_RED
#define AT_WHISPER	   AT_RED
#define AT_HIT		   AT_WHITE
#define AT_HITME	   AT_YELLOW
#define AT_IMMORT	   AT_YELLOW
#define AT_HURT		   AT_RED
#define AT_FALLING	   AT_WHITE + AT_BLINK
#define AT_DANGER	   AT_RED + AT_BLINK
#define AT_MAGIC	   AT_BLUE
#define AT_CONSIDER	   AT_GREY
#define AT_REPORT	   AT_GREY
#define AT_POISON	   AT_GREEN
#define AT_SOCIAL	   AT_CYAN
#define AT_DYING	   AT_YELLOW
#define AT_DEAD		   AT_RED
#define AT_SKILL	   AT_GREEN
#define AT_CARNAGE	   AT_BLOOD
#define AT_DAMAGE	   AT_WHITE
#define AT_FLEE		   AT_YELLOW
#define AT_RMNAME	   AT_WHITE
/* Let's change colors at night, shall we? -- Scion */
#define AT_RMDESC	   ((time_info.hour>6 && time_info.hour<=18) ? \
				AT_YELLOW : AT_BLUE)
#define AT_OBJECT	   AT_GREEN
#define AT_PERSON	   AT_PINK
#define AT_LIST		   AT_BLUE
#define AT_BYE		   AT_GREEN
#define AT_GOLD		   AT_YELLOW
#define AT_GTELL	   AT_BLUE
#define AT_NOTE		   AT_GREEN
#define AT_HUNGRY	   AT_ORANGE
#define AT_THIRSTY	   AT_BLUE
#define	AT_FIRE		   AT_RED
#define AT_SOBER	   AT_WHITE
#define AT_WEAROFF	   AT_YELLOW
#define AT_EXITS	   AT_WHITE
#define AT_SCORE	   AT_LBLUE
#define AT_RESET	   AT_DGREEN
#define AT_LOG		   AT_PURPLE
#define AT_DIEMSG	   AT_WHITE
#define AT_WARTALK     AT_RED
#define AT_CLANTALK		AT_RED
#define AT_RACETALK	   AT_BLUE
#define AT_IGNORE	   AT_GREEN
#define AT_DIVIDER	   AT_PLAIN
#define AT_MORPH           AT_GREY
/* New channel defines -- Scion */
#define AT_AVTALK		AT_ORANGE
#define AT_MUSIC		AT_PINK
#define AT_DEMI	        	AT_LBLUE
#define AT_CHAN_DEFAULT AT_GREEN /* Default channel color -- Scion */

#define INIT_WEAPON_CONDITION    12
#define MAX_ITEM_IMPACT		 30

struct help_data
{
   HELP_DATA *next;
   HELP_DATA *prev;
   short level;
   char *keyword;
   char *text;
};

/*
 * Shop types.
 */
#define MAX_TRADE	 5

struct shop_data
{
   SHOP_DATA *next;  /* Next shop in list    */
   SHOP_DATA *prev;  /* Previous shop in list   */
   int keeper; /* Vnum of shop keeper mob */
   short buy_type[MAX_TRADE]; /* Item types shop will buy   */
   short profit_buy; /* Cost multiplier for buying */
   short profit_sell;   /* Cost multiplier for selling   */
   short open_hour;  /* First opening hour      */
   short close_hour; /* First closing hour      */
};

#define MAX_FIX		3
#define SHOP_FIX	1
#define SHOP_RECHARGE	2

struct repairshop_data
{
   REPAIR_DATA *next;   /* Next shop in list    */
   REPAIR_DATA *prev;   /* Previous shop in list   */
   int keeper; /* Vnum of shop keeper mob */
   short fix_type[MAX_FIX];   /* Item types shop will fix   */
   short profit_fix; /* Cost multiplier for fixing */
   short shop_type;  /* Repair shop type     */
   short open_hour;  /* First opening hour      */
   short close_hour; /* First closing hour      */
};

/* Mob program structures */
struct act_prog_data
{
   struct act_prog_data *next;
   void *vo;
};

struct mob_prog_act_list
{
   MPROG_ACT_LIST *next;
   char *buf;
   CHAR_DATA *ch;
   OBJ_DATA *obj;
   void *vo;
};

struct mob_prog_data
{
   MPROG_DATA *next;
   short type;
   bool triggered;
   int resetdelay;
   char *arglist;
   char *comlist;
   bool fileprog;
};

bool MOBtrigger;

/*
 * Per-class stuff.
 */
struct class_type
{
   char *who_name;   /* Name for 'who'    */
   short attr_prime; /* Prime attribute      */
   int alignment; /* Alignment -- Scion */
   int weapon; /* First weapon         */
   int guild;  /* Vnum of guild room      */
   short skill_adept;   /* Maximum skill level     */
   short thac0_00;   /* Thac0 for level  0      */
   short thac0_32;   /* Thac0 for level 32      */
   short hp_min;  /* Min hp gained on leveling  */
   short hp_max;  /* Max hp gained on leveling  */
   bool fMana; /* Class gains mana on level  */
   short exp_base;   /* Class base exp    */
};

/* race dedicated stuff */
struct race_type
{
   char race_name[16];  /* Race name         */
   EXT_BV affected;  /* Default affect bitvectors  */
   short str_plus;   /* Str bonus/penalty    */
   short dex_plus;   /* Dex      "        */
   short wis_plus;   /* Wis      "        */
   short int_plus;   /* Int      "        */
   short con_plus;   /* Con      "        */
   short cha_plus;   /* Cha      "        */
   short lck_plus;   /* Lck       "       */
   short hit;
   short mana;
   short resist;
   short suscept;
   int class_restriction;  /* Flags for illegal classes  */
   int language;  /* Default racial language      */
   short ac_plus;
   short alignment;
   EXT_BV attacks;
   EXT_BV defenses;
   short minalign;
   short maxalign;
   short exp_multiplier;
   short height;
   short weight;
   short hunger_mod;
   short thirst_mod;
   short saving_poison_death;
   short saving_wand;
   short saving_para_petri;
   short saving_breath;
   short saving_spell_staff;
   char *where_name[MAX_WHERE_NAME];
   short mana_regen;
   short hp_regen;
   short race_recall;
   short base_age;
   int attribs[MAX_TYPES]; /* For character attributes */
};

typedef enum
{
   CLAN_PLAIN, CLAN_VAMPIRE, CLAN_WARRIOR, CLAN_DRUID, CLAN_MAGE, CLAN_CELTIC,
   CLAN_THIEF, CLAN_CLERIC, CLAN_UNDEAD, CLAN_CHAOTIC, CLAN_NEUTRAL, CLAN_LAWFUL,
   CLAN_NOKILL, CLAN_ORDER, CLAN_GUILD, CLAN_COUNCIL
} clan_types;

typedef enum
{
   GROUP_CLAN, GROUP_COUNCIL, GROUP_GUILD
} group_types;


struct clan_data
{
   CLAN_DATA *next;  /* next clan in list       */
   CLAN_DATA *prev;  /* previous clan in list      */
   char *filename;   /* Clan filename        */
   char *name; /* Clan name            */
   char *motto;   /* Clan motto           */
   char *description;   /* A brief description of the clan  */
   char *deity;   /* Clan's deity            */
   char *leader;  /* Head clan leader        */
   char *number1; /* First officer        */
   char *number2; /* Second officer       */
   char *badge;   /* Clan badge on who/where/to_room      */
   char *members;
   int pkills[7]; /* Number of pkills on behalf of clan  */
   int pdeaths[7];   /* Number of pkills against clan */
   int mkills; /* Number of mkills on behalf of clan  */
   int mdeaths;   /* Number of clan deaths due to mobs   */
   int illegal_pk;   /* Number of illegal pk's by clan   */
   int score;  /* Overall score        */
   short clan_type;  /* See clan type defines      */
   short favour;  /* Deities favour upon the clan     */
   short strikes; /* Number of strikes against the clan  */
   short num_members;   /* Number of clan members     */
   short mem_limit;  /* Number of clan members allowed   */
   short alignment;  /* Clan's general alignment      */
   int board;  /* Vnum of clan board         */
   int clanobj1;  /* Vnum of first clan obj     */
   int clanobj2;  /* Vnum of second clan obj    */
   int clanobj3;  /* Vnum of third clan obj     */
   int clanobj4;  /* Vnum of fourth clan obj    */
   int clanobj5;  /* Vnum of fifth clan obj     */
   int recall; /* Vnum of clan's recall room    */
   int storeroom; /* Vnum of clan's store room     */
   int guard1; /* Vnum of clan guard type 1     */
   int guard2; /* Vnum of clan guard type 2     */
   int class;  /* For clubs            */
};

struct council_data
{
   COUNCIL_DATA *next;  /* next council in list       */
   COUNCIL_DATA *prev;  /* previous council in list      */
   char *filename;   /* Council filename        */
   char *name; /* Council name            */
   char *description;   /* A brief description of the council  */
   char *head; /* Council head         */
   char *head2;   /* Council co-head                      */
   char *powers;  /* Council powers       */
   char *members;
   short num_members;   /* Number of council members     */
   int board;  /* Vnum of council board      */
   int meeting;   /* Vnum of council's meeting room   */
};

struct deity_data
{
   DEITY_DATA *next;
   DEITY_DATA *prev;
   char *filename;
   char *name;
   char *description;
   short alignment;
   short worshippers;
   short scorpse;
   short sdeityobj;
   short savatar;
   short srecall;
   short suplift;
   short flee;
   short flee_npcrace;
   short flee_npcfoe;
   short kill;
   short kill_magic;
   short kill_npcrace;
   short kill_npcfoe;
   short sac;
   short bury_corpse;
   short aid_spell;
   short aid;
   short backstab;
   short steal;
   short die;
   short die_npcrace;
   short die_npcfoe;
   short spell_aid;
   short dig_corpse;
   int race;
   int race2;
   int class;
   int element;
   int sex;
   EXT_BV affected;
   int npcrace;
   int npcfoe;
   int suscept;
   int susceptnum;
   int elementnum;
   int affectednum;
   int objstat;
};

struct tourney_data
{
   int open;
   int low_level;
   int hi_level;
};

/*
 * Data structure for notes.
 */
struct note_data
{
   NOTE_DATA *next;
   NOTE_DATA *prev;
   char *sender;
   char *date;
   char *to_list;
   char *subject;
   int voting;
   char *yesvotes;
   char *novotes;
   char *abstentions;
   char *text;
   time_t expire;
   time_t date_stamp;
};

struct board_data
{
   BOARD_DATA *next; /* Next board in list         */
   BOARD_DATA *prev; /* Previous board in list     */
   NOTE_DATA *first_note;  /* First note on board        */
   NOTE_DATA *last_note;   /* Last note on board         */
   char *note_file;  /* Filename to save notes to     */
   char *read_group; /* Can restrict a board to a       */
   char *post_group; /* council, clan, guild etc        */
   char *extra_readers; /* Can give read rights to players */
   char *extra_removers;   /* Can give remove rights to players */
   int board_obj; /* Vnum of board object       */
   short num_posts;  /* Number of notes on this board   */
   short min_read_level;   /* Minimum level to read a note     */
   short min_post_level;   /* Minimum level to post a note    */
   short min_remove_level; /* Minimum level to remove a note  */
   short max_posts;  /* Maximum amount of notes allowed */
   int type;   /* Normal board or mail board? */
};

/*
 * An affect.
 *
 * So limited... so few fields... should we add more?
 */
struct affect_data
{
   AFFECT_DATA *next;
   AFFECT_DATA *prev;
   short type;
   int duration;
   short location;
   int modifier;
   EXT_BV bitvector;
};

/*
 * A SMAUG spell
 */
struct smaug_affect
{
   SMAUG_AFF *next;
   char *duration;
   short location;
   char *modifier;
   int bitvector; /* this is the bit number */
};

/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/

/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_CITYGUARD	   5331
#define MOB_VNUM_VAMPIRE	   18
#define MOB_VNUM_ANIMATED_CORPSE   5
#define MOB_VNUM_POLY_WOLF	   10
#define MOB_VNUM_POLY_MIST	   11
#define MOB_VNUM_POLY_BAT	   12
#define MOB_VNUM_POLY_HAWK	   13
#define MOB_VNUM_POLY_CAT	   14
#define MOB_VNUM_POLY_DOVE	   15
#define MOB_VNUM_POLY_FISH	   16
#define MOB_VNUM_DEITY		   17
#define MOB_VNUM_FAMILIAR	   26

/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
#define ACT_IS_NPC		  0   /* Auto set for mobs */
#define ACT_SENTINEL		  1   /* Stays in one room */
#define ACT_SCAVENGER		  2   /* Picks up objects  */
#define ACT_UNDERTAKER            3 /* corpse retriver      */
#define ACT_AGGRESSIVE		  5   /* Attacks PC's      */
#define ACT_STAY_AREA		  6   /* Won't leave area  */
#define ACT_WIMPY		  7   /* Flees when hurt   */
#define ACT_PET			  8   /* Auto set for pets */
#define ACT_TRAIN		  9   /* Can train PC's */
#define ACT_PRACTICE		 10   /* Can practice PC's */
#define ACT_IMMORTAL	         11 /* Cannot be killed  */
#define ACT_DEADLY	         12 /* Has a deadly poison  */
#define ACT_POLYSELF		 13
#define ACT_META_AGGR		 14   /* Attacks other mobs   */
#define ACT_GUARDIAN		 15   /* Protects master   */
#define ACT_RUNNING	         16 /* Hunts quickly  */
#define ACT_NOWANDER		 17   /* Doesn't wander */
#define ACT_MOUNTABLE		 18   /* Can be mounted */
#define ACT_MOUNTED              19 /* Is mounted     */
#define ACT_SCHOLAR              20 /* Can teach languages  */
#define ACT_SECRETIVE		 21   /* actions aren't seen  */
#define ACT_HEALER    	         22 /* Free                 */
#define ACT_MOBINVIS		 23   /* Like wizinvis  */
#define ACT_NOASSIST		 24   /* Doesn't assist mobs  */
#define ACT_AUTONOMOUS		 25   /* Doesn't auto switch tanks */
#define ACT_PACIFIST             26 /* Doesn't ever fight   */
#define ACT_NOATTACK		 27   /* No physical attacks  */
#define ACT_ANNOYING		 28   /* Other mobs will attack */
#define ACT_PROTOTYPE		 30   /* A prototype mob   */
#define ACT_BANKER               31 /* A bank teller mob    */
#define ACT_NOQUEST              32 /* Not questable        */
#define ACT_BOUNTY               33 /* Bounty Officer       */
#define ACT_GLORYSHOP            34 /* Glory Shop           */
/* 34 acts */

/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 *
 * hold and flaming are yet uncoded
 */
typedef enum
{
   AFF_BLIND, AFF_INVISIBLE, AFF_DETECT_EVIL, AFF_DETECT_INVIS,
   AFF_DETECT_MAGIC, AFF_DETECT_HIDDEN, AFF_HOLD, AFF_SANCTUARY,
   AFF_FAERIE_FIRE, AFF_INFRARED, AFF_CURSE, AFF_FLAMING, AFF_POISON,
   AFF_PROTECT, AFF_PARALYSIS, AFF_SNEAK, AFF_HIDE, AFF_SLEEP, AFF_CHARM,
   AFF_FLYING, AFF_PASS_DOOR, AFF_FLOATING, AFF_TRUESIGHT, AFF_DETECTTRAPS,
   AFF_SCRYING, AFF_FIRESHIELD, AFF_SHOCKSHIELD, AFF_HAUS1, AFF_ICESHIELD,
   AFF_POSSESS, AFF_BERSERK, AFF_AQUA_BREATH, AFF_RECURRINGSPELL,
   AFF_CONTAGIOUS, AFF_HASTE, AFF_SLOW, MAX_AFFECTED_BY
} affected_by_types;

/*
 * Resistant Immune Susceptible flags
 */
#define RIS_FIRE		  BV00
#define RIS_COLD		  BV01
#define RIS_ELECTRICITY		  BV02
#define RIS_ENERGY		  BV03
#define RIS_BLUNT		  BV04
#define RIS_PIERCE		  BV05
#define RIS_SLASH		  BV06
#define RIS_ACID		  BV07
#define RIS_POISON		  BV08
#define RIS_DRAIN		  BV09
#define RIS_SLEEP		  BV10
#define RIS_CHARM		  BV11
#define RIS_HOLD		  BV12
#define RIS_NONMAGIC		  BV13
#define RIS_PLUS1		  BV14
#define RIS_PLUS2		  BV15
#define RIS_PLUS3		  BV16
#define RIS_PLUS4		  BV17
#define RIS_PLUS5		  BV18
#define RIS_PLUS6		  BV19
#define RIS_MAGIC		  BV20
#define RIS_PARALYSIS		  BV21
/* 21 RIS's*/

/*
 * Attack types
 */
typedef enum
{
   ATCK_BITE, ATCK_CLAWS, ATCK_TAIL, ATCK_STING, ATCK_PUNCH, ATCK_KICK,
   ATCK_TRIP, ATCK_BASH, ATCK_STUN, ATCK_GOUGE, ATCK_BACKSTAB, ATCK_FEED,
   ATCK_DRAIN, ATCK_FIREBREATH, ATCK_FROSTBREATH, ATCK_ACIDBREATH,
   ATCK_LIGHTNBREATH, ATCK_GASBREATH, ATCK_POISON, ATCK_NASTYPOISON, ATCK_GAZE,
   ATCK_BLINDNESS, ATCK_CAUSESERIOUS, ATCK_EARTHQUAKE, ATCK_CAUSECRITICAL,
   ATCK_CURSE, ATCK_FLAMESTRIKE, ATCK_HARM, ATCK_FIREBALL, ATCK_COLORSPRAY,
   ATCK_WEAKEN, ATCK_SPIRALBLAST, MAX_ATTACK_TYPE
} attack_types;

/*
 * Defense types
 */
typedef enum
{
   DFND_PARRY, DFND_DODGE, DFND_HEAL, DFND_CURELIGHT, DFND_CURESERIOUS,
   DFND_CURECRITICAL, DFND_DISPELMAGIC, DFND_DISPELEVIL, DFND_SANCTUARY,
   DFND_FIRESHIELD, DFND_SHOCKSHIELD, DFND_SHIELD, DFND_BLESS, DFND_STONESKIN,
   DFND_TELEPORT, DFND_MONSUM1, DFND_MONSUM2, DFND_MONSUM3, DFND_MONSUM4,
   DFND_DISARM, DFND_ICESHIELD, DFND_GRIP, DFND_TRUESIGHT, MAX_DEFENSE_TYPE
} defense_types;

/*
 * Body parts
 */
#define PART_HEAD		  BV00
#define PART_ARMS		  BV01
#define PART_LEGS		  BV02
#define PART_HEART		  BV03
#define PART_BRAINS		  BV04
#define PART_GUTS		  BV05
#define PART_HANDS		  BV06
#define PART_FEET		  BV07
#define PART_FINGERS		  BV08
#define PART_EAR		  BV09
#define PART_EYE		  BV10
#define PART_LONG_TONGUE	  BV11
#define PART_EYESTALKS		  BV12
#define PART_TENTACLES		  BV13
#define PART_FINS		  BV14
#define PART_WINGS		  BV15
#define PART_TAIL		  BV16
#define PART_SCALES		  BV17
/* for combat */
#define PART_CLAWS		  BV18
#define PART_FANGS		  BV19
#define PART_HORNS		  BV20
#define PART_TUSKS		  BV21
#define PART_TAILATTACK		  BV22
#define PART_SHARPSCALES	  BV23
#define PART_BEAK		  BV24

#define PART_HAUNCH		  BV25
#define PART_HOOVES		  BV26
#define PART_PAWS		  BV27
#define PART_FORELEGS		  BV28
#define PART_FEATHERS		  BV29

/*
 * Autosave flags
 */
#define SV_DEATH		  BV00   /* Save on death */
#define SV_KILL			  BV01   /* Save when kill made */
#define SV_PASSCHG		  BV02   /* Save on password change */
#define SV_DROP			  BV03   /* Save on drop */
#define SV_PUT			  BV04   /* Save on put */
#define SV_GIVE			  BV05   /* Save on give */
#define SV_AUTO			  BV06   /* Auto save every x minutes (define in cset) */
#define SV_ZAPDROP		  BV07   /* Save when eq zaps */
#define SV_AUCTION		  BV08   /* Save on auction */
#define SV_GET			  BV09   /* Save on get */
#define SV_RECEIVE		  BV10   /* Save when receiving */
#define SV_IDLE			  BV11   /* Save when char goes idle */
#define SV_BACKUP		  BV12   /* Make backup of pfile on save */
#define SV_QUITBACKUP		  BV13   /* Backup on quit only --Blod */

/*
 * Pipe flags
 */
#define PIPE_TAMPED		  BV01
#define PIPE_LIT		  BV02
#define PIPE_HOT		  BV03
#define PIPE_DIRTY		  BV04
#define PIPE_FILTHY		  BV05
#define PIPE_GOINGOUT		  BV06
#define PIPE_BURNT		  BV07
#define PIPE_FULLOFASH		  BV08

/*
 * Flags for act_string -- Shaddai
 */
#define STRING_NONE               0
#define STRING_IMM                BV01

/*
 * old flags for conversion purposes -- will not conflict with the flags below
 */
#define OLD_SF_SAVE_HALF_DAMAGE	  BV18   /* old save for half damage */
#define OLD_SF_SAVE_NEGATES	  BV19   /* old save negates affect  */

/*
 * Skill/Spell flags	The minimum BV *MUST* be 11!
 */
#define SF_WATER		  BV00
#define SF_EARTH		  BV01
#define SF_AIR			  BV02
#define SF_ASTRAL		  BV03
#define SF_AREA			  BV04   /* is an area spell      */
#define SF_DISTANT		  BV05   /* affects something far away  */
#define SF_REVERSE		  BV06
#define SF_NOSELF		  BV07   /* Can't target yourself!   */
#define SF_UNUSED2		  BV08   /* free for use!      */
#define SF_ACCUMULATIVE		  BV09   /* is accumulative    */
#define SF_RECASTABLE		  BV10   /* can be refreshed      */
#define SF_NOSCRIBE		  BV11   /* cannot be scribed     */
#define SF_NOBREW		  BV12   /* cannot be brewed      */
#define SF_GROUPSPELL		  BV13   /* only affects group members  */
#define SF_OBJECT		  BV14   /* directed at an object   */
#define SF_CHARACTER		  BV15   /* directed at a character  */
#define SF_SECRETSKILL		  BV16   /* hidden unless learned   */
#define SF_PKSENSITIVE		  BV17   /* much harder for plr vs. plr   */
#define SF_STOPONFAIL		  BV18   /* stops spell on first failure */
#define SF_NOFIGHT		  BV19   /* stops if char fighting       */
#define SF_NODISPEL               BV20 /* stops spell from being dispelled */
typedef enum
{ SS_NONE, SS_POISON_DEATH, SS_ROD_WANDS, SS_PARA_PETRI,
   SS_BREATH, SS_SPELL_STAFF
} save_types;

#define ALL_BITS		INT_MAX
#define SDAM_MASK		ALL_BITS & ~(BV00 | BV01 | BV02)
#define SACT_MASK		ALL_BITS & ~(BV03 | BV04 | BV05)
#define SCLA_MASK		ALL_BITS & ~(BV06 | BV07 | BV08)
#define SPOW_MASK		ALL_BITS & ~(BV09 | BV10)
#define SSAV_MASK		ALL_BITS & ~(BV11 | BV12 | BV13)

typedef enum
{ SD_NONE, SD_FIRE, SD_COLD, SD_ELECTRICITY, SD_ENERGY, SD_ACID,
   SD_POISON, SD_DRAIN
} spell_dam_types;

typedef enum
{ SA_NONE, SA_CREATE, SA_DESTROY, SA_RESIST, SA_SUSCEPT,
   SA_DIVINATE, SA_OBSCURE, SA_CHANGE
} spell_act_types;

typedef enum
{ SP_NONE, SP_MINOR, SP_GREATER, SP_MAJOR } spell_power_types;

typedef enum
{ SC_NONE, SC_LUNAR, SC_SOLAR, SC_TRAVEL, SC_SUMMON,
   SC_LIFE, SC_DEATH, SC_ILLUSION
} spell_class_types;

typedef enum
{ SE_NONE, SE_NEGATE, SE_EIGHTHDAM, SE_QUARTERDAM, SE_HALFDAM,
   SE_3QTRDAM, SE_REFLECT, SE_ABSORB
} spell_save_effects;

/*
 * Sex.
 * Used in #MOBILES.
 */
typedef enum
{ SEX_NEUTRAL, SEX_MALE, SEX_FEMALE } sex_types;

typedef enum
{
   TRAP_TYPE_POISON_GAS = 1, TRAP_TYPE_POISON_DART, TRAP_TYPE_POISON_NEEDLE,
   TRAP_TYPE_POISON_DAGGER, TRAP_TYPE_POISON_ARROW, TRAP_TYPE_BLINDNESS_GAS,
   TRAP_TYPE_SLEEPING_GAS, TRAP_TYPE_FLAME, TRAP_TYPE_EXPLOSION,
   TRAP_TYPE_ACID_SPRAY, TRAP_TYPE_ELECTRIC_SHOCK, TRAP_TYPE_BLADE,
   TRAP_TYPE_SEX_CHANGE
} trap_types;

#define MAX_TRAPTYPE		   TRAP_TYPE_SEX_CHANGE

#define TRAP_ROOM      		   BV00
#define TRAP_OBJ	      	   BV01
#define TRAP_ENTER_ROOM		   BV02
#define TRAP_LEAVE_ROOM		   BV03
#define TRAP_OPEN		   BV04
#define TRAP_CLOSE		   BV05
#define TRAP_GET		   BV06
#define TRAP_PUT		   BV07
#define TRAP_PICK		   BV08
#define TRAP_UNLOCK		   BV09
#define TRAP_N			   BV10
#define TRAP_S			   BV11
#define TRAP_E	      		   BV12
#define TRAP_W	      		   BV13
#define TRAP_U	      		   BV14
#define TRAP_D	      		   BV15
#define TRAP_EXAMINE		   BV16
#define TRAP_NE			   BV17
#define TRAP_NW			   BV18
#define TRAP_SE			   BV19
#define TRAP_SW			   BV20

/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
#define OBJ_VNUM_GOLD_ONE	      2
#define OBJ_VNUM_GOLD_SOME	      3

/* Rings */
#define OBJ_VNUM_DIAMOND_RING        47
#define OBJ_VNUM_WEDDING_BAND        48

#define OBJ_VNUM_CORPSE_NPC	     10
#define OBJ_VNUM_CORPSE_PC	     11
#define OBJ_VNUM_SEVERED_HEAD	     12
#define OBJ_VNUM_TORN_HEART	     13
#define OBJ_VNUM_SLICED_ARM	     14
#define OBJ_VNUM_SLICED_LEG	     15
#define OBJ_VNUM_SPILLED_GUTS	     16
#define OBJ_VNUM_BLOOD		     17
#define OBJ_VNUM_BLOODSTAIN	     18
#define OBJ_VNUM_SCRAPS		     19

#define OBJ_VNUM_MUSHROOM	     20
#define OBJ_VNUM_LIGHT_BALL	     21
#define OBJ_VNUM_SPRING		     22

#define OBJ_VNUM_SKIN		     23
#define OBJ_VNUM_SLICE		     24
#define OBJ_VNUM_SHOPPING_BAG	     25

#define OBJ_VNUM_BLOODLET	     26

#define OBJ_VNUM_FIRE		     30
#define OBJ_VNUM_TRAP		     31
#define OBJ_VNUM_PORTAL		     32

#define OBJ_VNUM_BLACK_POWDER	     33
#define OBJ_VNUM_SCROLL_SCRIBING     34
#define OBJ_VNUM_FLASK_BREWING       35
#define OBJ_VNUM_NOTE		     36

#define OBJ_VNUM_SILVER_ONE          76
#define OBJ_VNUM_SILVER_SOME	     77
#define OBJ_VNUM_COPPER_ONE	     78
#define OBJ_VNUM_COPPER_SOME	     79


/* missing body parts put in system.are cause no room was left for
limbo.are to expand. Eventually I will remove all system important items
and objects from limbo.are and gods.are and put them in this range - shogar*/

/* p.s. Im not even close to being done with this all these have to be made.
these defines are for a more intelligent definition of part_vnums[] in
fight.c
*/

#define OBJ_VNUM_BRAINS              50
#define OBJ_VNUM_HANDS               51
#define OBJ_VNUM_FEET                52
#define OBJ_VNUM_FINGERS             53
#define OBJ_VNUM_EAR                 54
#define OBJ_VNUM_EYE                 55
#define OBJ_VNUM_LONG_TONGUE         56
#define OBJ_VNUM_EYESTALKS           57
#define OBJ_VNUM_TENTACLES           58
#define OBJ_VNUM_FINS                59
#define OBJ_VNUM_WINGS               60
#define OBJ_VNUM_TAIL                61
#define OBJ_VNUM_SCALES              62
#define OBJ_VNUM_TUSKS               63
#define OBJ_VNUM_HORNS               64
#define OBJ_VNUM_CLAWS               65
#define OBJ_VNUM_FEATHERS            66
#define OBJ_VNUM_FORELEGS            67
#define OBJ_VNUM_PAWS                68
#define OBJ_VNUM_HOOVES              69
#define OBJ_VNUM_BEAK                70
#define OBJ_VNUM_SHARPSCALES         71
#define OBJ_VNUM_HAUNCHES            72
#define OBJ_VNUM_FANGS               73
/* end missing body parts - shogar */

#define OBJ_VNUM_DEITY		     46

/* Academy eq */
#define OBJ_VNUM_SCHOOL_MACE	  10315
#define OBJ_VNUM_SCHOOL_DAGGER	  10312
#define OBJ_VNUM_SCHOOL_SWORD	  10313
#define OBJ_VNUM_SCHOOL_VEST	  10308
#define OBJ_VNUM_SCHOOL_SHIELD	  10310
#define OBJ_VNUM_SCHOOL_BANNER    10311
/* Sharpening stone */
#define OBJ_VNUM_SHARPEN	  75

/*
 * Item types.
 * Used in #OBJECTS.
 */
typedef enum
{
   ITEM_NONE, ITEM_LIGHT, ITEM_SCROLL, ITEM_WAND, ITEM_STAFF, ITEM_WEAPON,
   ITEM_FIREWEAPON, ITEM_MISSILE, ITEM_TREASURE, ITEM_ARMOR, ITEM_POTION,
   ITEM_WORN, ITEM_FURNITURE, ITEM_TRASH, ITEM_OLDTRAP, ITEM_CONTAINER,
   ITEM_NOTE, ITEM_DRINK_CON, ITEM_KEY, ITEM_FOOD, ITEM_GOLD, ITEM_PEN,
   ITEM_BOAT, ITEM_CORPSE_NPC, ITEM_CORPSE_PC, ITEM_FOUNTAIN, ITEM_PILL,
   ITEM_BLOOD, ITEM_BLOODSTAIN, ITEM_SCRAPS, ITEM_PIPE, ITEM_HERB_CON,
   ITEM_HERB, ITEM_INCENSE, ITEM_FIRE, ITEM_BOOK, ITEM_SWITCH, ITEM_LEVER,
   ITEM_PULLCHAIN, ITEM_BUTTON, ITEM_DIAL, ITEM_RUNE, ITEM_RUNEPOUCH,
   ITEM_MATCH, ITEM_TRAP, ITEM_MAP, ITEM_PORTAL, ITEM_PAPER,
   ITEM_TINDER, ITEM_LOCKPICK, ITEM_SPIKE, ITEM_DISEASE, ITEM_OIL, ITEM_FUEL,
   ITEM_EMPTY1, ITEM_EMPTY2, ITEM_MISSILE_WEAPON, ITEM_PROJECTILE,
   ITEM_QUIVER, ITEM_SHOVEL, ITEM_SALVE, ITEM_COOK, ITEM_KEYRING,
   ITEM_PIECE, ITEM_ODOR, ITEM_SILVER, ITEM_COPPER
} item_types;

#define MAX_ITEM_TYPE		     ITEM_COPPER

/*
 * Extra flags.
 * Used in #OBJECTS.
 */
typedef enum
{
   ITEM_GLOW, ITEM_HUM, ITEM_DARK, ITEM_LOYAL, ITEM_EVIL, ITEM_INVIS, ITEM_MAGIC,
   ITEM_NODROP, ITEM_BLESS, ITEM_ANTI_GOOD, ITEM_ANTI_EVIL, ITEM_ANTI_NEUTRAL,
   ITEM_NOREMOVE, ITEM_INVENTORY, ITEM_ANTI_MAGE, ITEM_ANTI_THIEF,
   ITEM_ANTI_WARRIOR, ITEM_ANTI_CLERIC, ITEM_ORGANIC, ITEM_METAL, ITEM_DONATION,
   ITEM_CLANOBJECT, ITEM_CLANCORPSE, ITEM_ANTI_VAMPIRE, ITEM_ANTI_DRUID,
   ITEM_HIDDEN, ITEM_POISONED, ITEM_COVERING, ITEM_DEATHROT, ITEM_BURIED,
   ITEM_PROTOTYPE, ITEM_NOLOCATE, ITEM_GROUNDROT, ITEM_ARTIFACT, ITEM_PLRBLD,
   ITEM_NOSCRAP,
   MAX_ITEM_FLAG
} item_extra_flags;

/* Magic flags - extra extra_flags for objects that are used in spells */
#define ITEM_RETURNING		BV00
#define ITEM_BACKSTABBER  	BV01
#define ITEM_BANE		BV02
#define ITEM_MAGIC_LOYAL	BV03
#define ITEM_HASTE		BV04
#define ITEM_DRAIN		BV05
#define ITEM_LIGHTNING_BLADE  	BV06
#define ITEM_PKDISARMED		BV07  /* Maybe temporary, not a perma flag */

/* Lever/dial/switch/button/pullchain flags */
#define TRIG_UP			BV00
#define TRIG_UNLOCK		BV01
#define TRIG_LOCK		BV02
#define TRIG_D_NORTH		BV03
#define TRIG_D_SOUTH		BV04
#define TRIG_D_EAST		BV05
#define TRIG_D_WEST		BV06
#define TRIG_D_UP		BV07
#define TRIG_D_DOWN		BV08
#define TRIG_DOOR		BV09
#define TRIG_CONTAINER		BV10
#define TRIG_OPEN		BV11
#define TRIG_CLOSE		BV12
#define TRIG_PASSAGE		BV13
#define TRIG_OLOAD		BV14
#define TRIG_MLOAD		BV15
#define TRIG_TELEPORT		BV16
#define TRIG_TELEPORTALL	BV17
#define TRIG_TELEPORTPLUS	BV18
#define TRIG_DEATH		BV19
#define TRIG_CAST		BV20
#define TRIG_FAKEBLADE		BV21
#define TRIG_RAND4		BV22
#define TRIG_RAND6		BV23
#define TRIG_TRAPDOOR		BV24
#define TRIG_ANOTHEROOM		BV25
#define TRIG_USEDIAL		BV26
#define TRIG_ABSOLUTEVNUM	BV27
#define TRIG_SHOWROOMDESC	BV28
#define TRIG_AUTORETURN		BV29

#define TELE_SHOWDESC		BV00
#define TELE_TRANSALL		BV01
#define TELE_TRANSALLPLUS	BV02

/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE		BV00
#define ITEM_WEAR_FINGER	BV01
#define ITEM_WEAR_NECK		BV02
#define ITEM_WEAR_BODY		BV03
#define ITEM_WEAR_HEAD		BV04
#define ITEM_WEAR_LEGS		BV05
#define ITEM_WEAR_FEET		BV06
#define ITEM_WEAR_HANDS		BV07
#define ITEM_WEAR_ARMS		BV08
#define ITEM_WEAR_SHIELD	BV09
#define ITEM_WEAR_ABOUT		BV10
#define ITEM_WEAR_WAIST		BV11
#define ITEM_WEAR_WRIST		BV12
#define ITEM_WIELD		BV13
#define ITEM_HOLD		BV14
#define ITEM_DUAL_WIELD		BV15
#define ITEM_WEAR_EARS		BV16
#define ITEM_WEAR_EYES		BV17
#define ITEM_MISSILE_WIELD	BV18
#define ITEM_WEAR_BACK		BV19
#define ITEM_WEAR_FACE		BV20
#define ITEM_WEAR_ANKLE		BV21
#define ITEM_WEAR_FLOAT		BV22
#define ITEM_WEAR_MAX		23

/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
typedef enum
{
   APPLY_NONE, APPLY_STR, APPLY_DEX, APPLY_INT, APPLY_WIS, APPLY_CON,
   APPLY_SEX, APPLY_CLASS, APPLY_LEVEL, APPLY_AGE, APPLY_HEIGHT, APPLY_WEIGHT,
   APPLY_MANA, APPLY_HIT, APPLY_MOVE, APPLY_GOLD, APPLY_EXP, APPLY_AC,
   APPLY_HITROLL, APPLY_DAMROLL, APPLY_SAVING_POISON, APPLY_SAVING_ROD,
   APPLY_SAVING_PARA, APPLY_SAVING_BREATH, APPLY_SAVING_SPELL, APPLY_CHA,
   APPLY_AFFECT, APPLY_RESISTANT, APPLY_IMMUNE, APPLY_SUSCEPTIBLE,
   APPLY_WEAPONSPELL, APPLY_LCK, APPLY_BACKSTAB, APPLY_PICK, APPLY_TRACK,
   APPLY_STEAL, APPLY_SNEAK, APPLY_HIDE, APPLY_PALM, APPLY_DETRAP, APPLY_DODGE,
   APPLY_PEEK, APPLY_SCAN, APPLY_GOUGE, APPLY_SEARCH, APPLY_MOUNT, APPLY_DISARM,
   APPLY_KICK, APPLY_PARRY, APPLY_BASH, APPLY_STUN, APPLY_PUNCH, APPLY_CLIMB,
   APPLY_GRIP, APPLY_SCRIBE, APPLY_BREW, APPLY_WEARSPELL, APPLY_REMOVESPELL,
   APPLY_EMOTION, APPLY_MENTALSTATE, APPLY_STRIPSN, APPLY_REMOVE, APPLY_DIG,
   APPLY_FULL, APPLY_THIRST, APPLY_DRUNK, APPLY_BLOOD, APPLY_COOK,
   APPLY_RECURRINGSPELL, APPLY_CONTAGIOUS, APPLY_EXT_AFFECT, APPLY_ODOR,
   APPLY_ROOMFLAG, APPLY_SECTORTYPE, APPLY_ROOMLIGHT, APPLY_TELEVNUM,
   APPLY_TELEDELAY, MAX_APPLY_TYPE, APPLY_SILVER, APPLY_COPPER
} apply_types;

#define REVERSE_APPLY		   1000

/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE		   BV00
#define CONT_PICKPROOF		   BV01
#define CONT_CLOSED		   BV02
#define CONT_LOCKED		   BV03
#define CONT_EATKEY		   BV04

/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_LIMBO		      2
#define ROOM_VNUM_POLY		      3
#define ROOM_VNUM_HELL                6   /* for hell fix - shogar */
#define ROOM_VNUM_CHAT		   1200

/*
 * Either ResortMUD or ResortMUD2 can be commented out, never both!
 *
 * Defined in #ROOMS for ResortMUD using dhresort.are.
            *//*
        * #define ROOM_VNUM_TEMPLE     9240
        * #define ROOM_VNUM_ALTAR         9248
        * #define ROOM_VNUM_SCHOOL        2
        * #define ROOM_AUTH_START            1
        * #define ROOM_VNUM_HALLOFFALLEN     9247
        * #define ROOM_VNUM_DONATE     9249
        * #define ROOM_VNUM_MORGUE           9245
        */

/*
 * Defined in #ROOMS for ResortMUD2 using newdark.are.
 */
#define ROOM_VNUM_TEMPLE	  21001
#define ROOM_VNUM_ALTAR		  21194
#define ROOM_VNUM_SCHOOL	  10430
#define ROOM_AUTH_START		    100
#define ROOM_VNUM_HALLOFFALLEN    21195
#define ROOM_VNUM_DONATE	  21193
#define ROOM_VNUM_MORGUE          21538

/*
 * Room flags.           Holy cow!  Talked about stripped away..
 * Used in #ROOMS.       Those merc guys know how to strip code down.
 *			 Lets put it all back... ;)
 */

#define ROOM_DARK		BV00
#define ROOM_DEATH		BV01
#define ROOM_NO_MOB		BV02
#define ROOM_INDOORS		BV03
#define ROOM_LAWFUL		BV04
#define ROOM_NEUTRAL		BV05
#define ROOM_CHAOTIC		BV06
#define ROOM_NO_MAGIC		BV07
#define ROOM_TUNNEL		BV08
#define ROOM_PRIVATE		BV09
#define ROOM_SAFE		BV10
#define ROOM_SOLITARY		BV11
#define ROOM_PET_SHOP		BV12
#define ROOM_NO_RECALL		BV13
#define ROOM_DONATION		BV14
#define ROOM_NODROPALL		BV15
#define ROOM_SILENCE		BV16
#define ROOM_LOGSPEECH		BV17
#define ROOM_NODROP		BV18
#define ROOM_CLANSTOREROOM	BV19
#define ROOM_NO_SUMMON		BV20
#define ROOM_NO_ASTRAL		BV21
#define ROOM_TELEPORT		BV22
#define ROOM_TELESHOWDESC	BV23
#define ROOM_NOFLOOR		BV24
#define ROOM_NOSUPPLICATE       BV25
#define ROOM_ARENA		BV26
#define ROOM_NOMISSILE		BV27
#define ROOM_AMPLIFY		BV28
#define ROOM_LOCKER             BV29
#define ROOM_PROTOTYPE	     	BV30
#define ROOM_WILDERNESS         BV31

/*
 * Directions.
 * Used in #ROOMS.
 */
typedef enum
{
   DIR_NORTH, DIR_EAST, DIR_SOUTH, DIR_WEST, DIR_UP, DIR_DOWN,
   DIR_NORTHEAST, DIR_NORTHWEST, DIR_SOUTHEAST, DIR_SOUTHWEST, DIR_SOMEWHERE
} dir_types;

#define PT_WATER	100
#define PT_AIR		200
#define PT_EARTH	300
#define PT_FIRE		400

/*
 * Push/pull types for exits					-Thoric
 * To differentiate between the current of a river, or a strong gust of wind
 */
typedef enum
{
   PULL_UNDEFINED, PULL_VORTEX, PULL_VACUUM, PULL_SLIP, PULL_ICE, PULL_MYSTERIOUS,
   PULL_CURRENT = PT_WATER, PULL_WAVE, PULL_WHIRLPOOL, PULL_GEYSER,
   PULL_WIND = PT_AIR, PULL_STORM, PULL_COLDWIND, PULL_BREEZE,
   PULL_LANDSLIDE = PT_EARTH, PULL_SINKHOLE, PULL_QUICKSAND, PULL_EARTHQUAKE,
   PULL_LAVA = PT_FIRE, PULL_HOTAIR
} dir_pulltypes;

#define MAX_DIR			DIR_SOUTHWEST  /* max for normal walking */
#define DIR_PORTAL		DIR_SOMEWHERE  /* portal direction    */

/*
 * Exit flags.			EX_RES# are reserved for use by the
 * Used in #ROOMS.		SMAUG development team
 */
#define EX_ISDOOR		  BV00
#define EX_CLOSED		  BV01
#define EX_LOCKED		  BV02
#define EX_SECRET		  BV03
#define EX_SWIM			  BV04
#define EX_PICKPROOF		  BV05
#define EX_FLY			  BV06
#define EX_CLIMB		  BV07
#define EX_DIG			  BV08
#define EX_EATKEY		  BV09
#define EX_NOPASSDOOR		  BV10
#define EX_HIDDEN		  BV11
#define EX_PASSAGE		  BV12
#define EX_PORTAL 		  BV13
#define EX_RES1			  BV14
#define EX_RES2			  BV15
#define EX_xCLIMB		  BV16
#define EX_xENTER		  BV17
#define EX_xLEAVE		  BV18
#define EX_xAUTO		  BV19
#define EX_NOFLEE	  	  BV20
#define EX_xSEARCHABLE		  BV21
#define EX_BASHED                 BV22
#define EX_BASHPROOF              BV23
#define EX_NOMOB		  BV24
#define EX_WINDOW		  BV25
#define EX_xLOOK		  BV26
#define MAX_EXFLAG		  26

/*
 * Sector types.
 * Used in #ROOMS.
 */
typedef enum
{
   SECT_INSIDE, SECT_CITY, SECT_FIELD, SECT_FOREST, SECT_HILLS, SECT_MOUNTAIN,
   SECT_WATER_SWIM, SECT_WATER_NOSWIM, SECT_UNDERWATER, SECT_AIR, SECT_DESERT,
   SECT_DUNNO, SECT_OCEANFLOOR, SECT_UNDERGROUND,
   /*
    * Wilderness Sectors
    */
   SECT_ROAD, SECT_RUINS, SECT_JUNGLE, SECT_SWAMP, SECT_ENTER, SECT_ROCK_MOUNTAIN,
   SECT_SNOW_MOUNTAIN, SECT_UNUSED, SECT_LAVA,
   SECT_MAX
} sector_types;

/* For automapper */
const char *sector_translation[26][2];
extern short map[215][215];
void prep_map( ROOM_INDEX_DATA * location, int min, int max );
char *make_map_line( int lnum, int min, int max );

/*
 * Equpiment wear locations.
 * Used in #RESETS.
 */
typedef enum
{
   WEAR_NONE = -1, WEAR_LIGHT = 0, WEAR_FINGER_L, WEAR_FINGER_R, WEAR_NECK_1,
   WEAR_NECK_2, WEAR_BODY, WEAR_HEAD, WEAR_LEGS, WEAR_FEET, WEAR_HANDS,
   WEAR_ARMS, WEAR_SHIELD, WEAR_ABOUT, WEAR_WAIST, WEAR_WRIST_L, WEAR_WRIST_R,
   WEAR_WIELD, WEAR_HOLD, WEAR_DUAL_WIELD, WEAR_EARS, WEAR_EYES,
   WEAR_MISSILE_WIELD, WEAR_BACK, WEAR_FACE, WEAR_ANKLE_L, WEAR_ANKLE_R,
   WEAR_FLOAT, MAX_WEAR
} wear_locations;

/* Board Types */
typedef enum
{ BOARD_NOTE, BOARD_MAIL } board_types;

/* Auth Flags */
#define FLAG_WRAUTH		      1
#define FLAG_AUTH		      2

/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
typedef enum
{
   COND_DRUNK, COND_FULL, COND_THIRST, COND_BLOODTHIRST, COND_BLEEDING, MAX_CONDS
} conditions;

/*
 * Positions.
 */
typedef enum
{
   POS_DEAD, POS_MORTAL, POS_INCAP, POS_STUNNED, POS_SLEEPING, POS_BERSERK,
   POS_RESTING, POS_AGGRESSIVE, POS_SITTING, POS_FIGHTING, POS_DEFENSIVE,
   POS_EVASIVE, POS_STANDING, POS_MOUNTED, POS_SHOVE, POS_DRAG, POS_RECOVER
} positions;

/*
 * Styles.
 */
typedef enum
{
   STYLE_BERSERK, STYLE_AGGRESSIVE, STYLE_FIGHTING, STYLE_DEFENSIVE,
   STYLE_EVASIVE,
} styles;

/*
 * ACT bits for players.
 */
typedef enum
{
   PLR_IS_NPC, PLR_BOUGHT_PET, PLR_SHOVEDRAG, PLR_AUTOEXIT, PLR_AUTOLOOT,
   PLR_AUTOSAC, PLR_BLANK, PLR_OUTCAST, PLR_BRIEF, PLR_COMBINE, PLR_PROMPT,
   PLR_TELNET_GA, PLR_HOLYLIGHT, PLR_WIZINVIS, PLR_ROOMVNUM, PLR_SILENCE,
   PLR_NO_EMOTE, PLR_ATTACKER, PLR_NO_TELL, PLR_LOG, PLR_DENY, PLR_FREEZE,
   PLR_THIEF, PLR_KILLER, PLR_LITTERBUG, PLR_ANSI, PLR_RIP, PLR_NICE, PLR_FLEE,
   PLR_AUTOGOLD, PLR_AUTOMAP, PLR_AFK, PLR_INVISPROMPT, PLR_MARRIED, PLR_NOMENU,
   PLR_NEEDHELP, PLR_QUESTOR, PLR_NEWMONEY, PLR_COWARD, PLR_EXEMPT
} player_flags;

/* Bits for pc_data->flags. */
#define PCFLAG_R1                  BV00
#define PCFLAG_DEADLY              BV01
#define PCFLAG_UNAUTHED		   BV02
#define PCFLAG_NORECALL            BV03
#define PCFLAG_NOINTRO             BV04
#define PCFLAG_GAG		   BV05
#define PCFLAG_RETIRED             BV06
#define PCFLAG_GUEST               BV07
#define PCFLAG_NOSUMMON		   BV08
#define PCFLAG_PAGERON		   BV09
#define PCFLAG_NOTITLE             BV10
#define PCFLAG_GROUPWHO		   BV11
#define PCFLAG_DIAGNOSE		   BV12
#define PCFLAG_HIGHGAG		   BV13
#define PCFLAG_WATCH		   BV14  /* see function "do_watch" */
#define PCFLAG_HELPSTART	   BV15  /* Force new players to help start */
#define PCFLAG_IMMPROOF		   BV16  /* stop imms misbehaving-shogar */
#define PCFLAG_NOBEEP		   BV17  /* stop people from beeping you -- Scion */
#define PCFLAG_ANONYMOUS	   BV18  /* Stop mortals' ranks and levels from showing up -- Scion */
#define PCFLAG_INDUCTOR		   BV19  /* Can induct people into clan/club/order -- Scion */
#define PCFLAG_NOSTUN              BV20
#define PCFLAG_NOTELL              BV21
typedef enum
{
   TIMER_NONE, TIMER_RECENTFIGHT, TIMER_SHOVEDRAG, TIMER_DO_FUN,
   TIMER_APPLIED, TIMER_PKILLED, TIMER_ASUPRESSED, TIMER_NUISANCE,
   TIMER_FEARSPELL
} timer_types;

struct timer_data
{
   TIMER *prev;
   TIMER *next;
   DO_FUN *do_fun;
   int value;
   short type;
   int count;
};

/*
 * Channel bits.
 */
#define	CHANNEL_AUCTION		   BV00
#define	CHANNEL_CHAT		   BV01
#define	CHANNEL_QUEST		   BV02
#define	CHANNEL_IMMTALK		   BV03
#define	CHANNEL_MUSIC		   BV04
#define	CHANNEL_ASK		   BV05
#define	CHANNEL_SHOUT		   BV06
#define	CHANNEL_YELL		   BV07
#define CHANNEL_MONITOR		   BV08
#define CHANNEL_LOG		   BV09
#define CHANNEL_HIGHGOD		   BV10
#define CHANNEL_CLAN		   BV11
#define CHANNEL_BUILD		   BV12
#define CHANNEL_HIGH		   BV13
#define CHANNEL_AVTALK		   BV14
#define CHANNEL_PRAY		   BV15
#define CHANNEL_COUNCIL 	   BV16
#define CHANNEL_GUILD              BV17
#define CHANNEL_COMM		   BV18
#define CHANNEL_TELLS		   BV19
#define CHANNEL_ORDER              BV20
#define CHANNEL_NEWBIE             BV21
#define CHANNEL_WARTALK            BV22
#define CHANNEL_RACETALK           BV23
#define CHANNEL_WARN               BV24
#define CHANNEL_WHISPER		   BV25
#define CHANNEL_AUTH		   BV26
#define	CHANNEL_DEMI	           BV27
#define CHANNEL_THINK		   BV28
#define CHANNEL_COUNCIL2	   BV29
#define CHANNEL_NEWBIE2            BV30
#define CHANNEL_HELP               BV31

/* Area defines - Scryn 8/11
 *
 */
#define AREA_DELETED		   BV00
#define AREA_LOADED                BV01

/* Area flags - Narn Mar/96 */
#define AFLAG_NOPKILL               BV00
#define AFLAG_FREEKILL		    BV01
#define AFLAG_NOTELEPORT	    BV02
#define AFLAG_RESET_ON_BOOT         BV03
#define AFLAG_PLR_LOADED            BV04
#define AFLAG_EXIT_NAMES            BV05
#define AFLAG_CLOSED                BV06
#define AFLAG_PROTOTYPE             BV07
/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct mob_index_data
{
   MOB_INDEX_DATA *next;
   MOB_INDEX_DATA *next_sort;
   SPEC_FUN *spec_fun;
   SHOP_DATA *pShop;
   REPAIR_DATA *rShop;
   MPROG_DATA *mudprogs;
   EXT_BV progtypes;
   char *player_name;
   char *short_descr;
   char *long_descr;
   char *description;
   int vnum;
   short count;
   short killed;
   short sex;
   short level;
   EXT_BV act;
   EXT_BV affected_by;
   short alignment;
   short mobthac0;   /* Unused */
   short ac;
   short hitnodice;
   short hitsizedice;
   short hitplus;
   short damnodice;
   short damsizedice;
   short damplus;
   short numattacks;
   long int gold;
   int exp;
   int xflags;
   int resistant;
   int immune;
   int susceptible;
   EXT_BV attacks;
   EXT_BV defenses;
   int speaks;
   int speaking;
   short position;
   short defposition;
   short height;
   short weight;
   short race;
   short class;
   short hitroll;
   short damroll;
   short perm_str;
   short perm_int;
   short perm_wis;
   short perm_dex;
   short perm_con;
   short perm_cha;
   short perm_lck;
   short saving_poison_death;
   short saving_wand;
   short saving_para_petri;
   short saving_breath;
   short saving_spell_staff;
   long int silver;
   long int copper;
};


struct hunt_hate_fear
{
   char *name;
   CHAR_DATA *who;
};

struct fighting_data
{
   CHAR_DATA *who;
   int xp;
   short align;
   short duration;
   short timeskilled;
};

struct editor_data
{
   short numlines;
   short on_line;
   short size;
   char line[49][81];
};

struct extracted_char_data
{
   EXTRACT_CHAR_DATA *next;
   CHAR_DATA *ch;
   ROOM_INDEX_DATA *room;
   ch_ret retcode;
   bool extract;
};

/* Moved arena data to its own struct, should save some mem -- Kratas */
struct arena_data
{
   int status;
   int bet;
   int num_bets;
   CHAR_DATA *bet_on;
   CHAR_DATA *challenged_by;
};

/*
 * One character (PC or NPC).
 */
struct char_data
{
   CHAR_DATA *next;
   CHAR_DATA *prev;
   CHAR_DATA *next_in_room;
   CHAR_DATA *prev_in_room;
   CHAR_DATA *master;
   CHAR_DATA *leader;
   FIGHT_DATA *fighting;
   CHAR_DATA *reply;
   CHAR_DATA *retell;
   CHAR_DATA *switched;
   CHAR_DATA *mount;
   HHF_DATA *hunting;
   HHF_DATA *fearing;
   HHF_DATA *hating;
   SPEC_FUN *spec_fun;
   MPROG_ACT_LIST *mpact;
   int mpactnum;
   size_t mpscriptpos;
   MOB_INDEX_DATA *pIndexData;
   DESCRIPTOR_DATA *desc;
   AFFECT_DATA *first_affect;
   AFFECT_DATA *last_affect;
   NOTE_DATA *pnote;
   NOTE_DATA *comments;
   OBJ_DATA *first_carrying;
   OBJ_DATA *last_carrying;
   ROOM_INDEX_DATA *in_room;
   BIT_DATA *first_abit;   /* Abit/Qbit stuff */
   BIT_DATA *last_abit;
   OBJ_DATA *on;  /* furniture, stand on objects */
   ROOM_INDEX_DATA *was_in_room;
   PC_DATA *pcdata;
   DO_FUN *last_cmd;
   DO_FUN *prev_cmd; /* mapping */
   void *dest_buf;   /* This one is to assign to differen things */
   char *alloc_ptr;  /* Must str_dup and free this one */
   void *spare_ptr;
   int tempnum;
   EDITOR_DATA *editor;
   TIMER *first_timer;
   TIMER *last_timer;
   CHAR_MORPH *morph;
   CHAR_DATA *questgiver;  /* Vassago */
   short nextquest;  /* Vassago */
   short countdown;  /* Vassago */
   short questobj;   /* Vassago */
   short questmob;   /* Vassago */
   char *name;
   char *short_descr;
   char *long_descr;
   char *description;
   short ghost_level;   /* Froboz */
   short incog_level;   /* Froboz */
   short num_fighting;
   short substate;
   short sex;
   short class;
   short class2;
   short class3;
   short race;
   short level;
   short level2;
   short level3;
   short trust;
   int played;
   time_t logon;
   time_t save_time;
   short timer;
   short wait;
   short hit;
   short max_hit;
   short mana;
   short max_mana;
   short move;
   short max_move;
   short practice;
   short numattacks;
   long int gold;
   int exp;
   int exp2;
   EXT_BV act;
   EXT_BV affected_by;
   EXT_BV no_affected_by;
   int carry_weight;
   int carry_number;
   int xflags;
   int resistant;
   int no_resistant;
   int immune;
   int no_immune;
   int susceptible;
   int no_susceptible;
   EXT_BV attacks;
   EXT_BV defenses;
   int speaks;
   int speaking;
   short saving_poison_death;
   short saving_wand;
   short saving_para_petri;
   short saving_breath;
   short saving_spell_staff;
   short alignment;
   short barenumdie;
   short baresizedie;
   short mobthac0;
   short hitroll;
   short damroll;
   short hitplus;
   short damplus;
   short position;
   short defposition;
   short style;
   short height;
   short weight;
   short armor;
   short wimpy;
   int deaf;
   short perm_str;
   short perm_int;
   short perm_wis;
   short perm_dex;
   short perm_con;
   short perm_cha;
   short perm_lck;
   short mod_str;
   short mod_int;
   short mod_wis;
   short mod_dex;
   short mod_con;
   short mod_cha;
   short mod_lck;
   short mental_state;  /* simplified */
   short emotional_state;  /* simplified */
   int retran;
   int regoto;
   short mobinvis;   /* Mobinvis level SB */
   short age_bonus;
   short age;
   short day;
   short month;
   short year;
   int fur_pos;   /* furniture pos. */
   long int silver;
   long int copper;

/* Stuff for Bounty - bounty, bounty owed, bounty eligible, and bounty kills (for hiscore) - Garinan */
   short bounty;
   short bowed;
   short belig;
   short bkills;

#if 0
   /*
    * All these are going in a struct when I get around to changing them. -- Scion
    */
   int arena;  /* Scion arenacode */
   int arenabet;  /* Scion arenacode */
   int num_bets;  /* Scion arenacode */
   CHAR_DATA *arena_bet_on;   /* Scion arenacode */
#endif
};

/* Trivia Structure -Garinan */
struct trivia_data
{
   TRIVIA_DATA *next;
   TRIVIA_DATA *prev;
   char *question;
   char *answer;
};

struct killed_data
{
   int vnum;
   char count;
};

/* Structure for link list of ignored players */
struct ignore_data
{
   IGNORE_DATA *next;
   IGNORE_DATA *prev;
   char *name;
};

/* Max number of people you can ignore at once */
#define MAX_IGN		6

typedef struct alias_queue ALIAS_QUEUE;

struct alias_queue
{
   ALIAS_QUEUE *next;
   char *cmd;
};

/*
 * Data which only PC's have.
 */
struct pc_data
{
   ALIAS_DATA *first_alias;
   ALIAS_DATA *last_alias;
   ALIAS_QUEUE *alias_queue;
   int alias_used;   /* Alias used x times -- Scion */
   CHAR_DATA *pet;
   CLAN_DATA *clan;
   COUNCIL_DATA *council;
   COUNCIL_DATA *council2;
   AREA_DATA *area;
   DEITY_DATA *deity;
   ARENA_DATA *arena;   /* Kratas */
   BIT_DATA *first_qbit;   /* Abit/QBit stuff */
   BIT_DATA *last_qbit;
   char *homepage;
   char *email;
   char *clan_name;
   char *council_name;
   char *council2_name;
   char *deity_name;
   char *pwd;
   char *bamfin;
   char *bamfout;
   char *filename;   /* For the safe mset name -Shaddai */
   char *rank;
   char *afkmsg;
   char *title;
   char *lastname;   /* last name -- Scion */
   char *extraname;  /* extraname before name on wholist */
   char *bestowments;   /* Special bestowed commands     */
   char *say_history[10];  /* Say history -- Kratas */
   char *tell_histories[10];  /* Tell History -- Kratas */
   /*
    * Character Attributes
    */
   char *hair_color;
   char *eye_color;
   char *skin_tone;
   char *ch_build;
   char *hair_length;
   char *ch_height;
   char *chan_listen;
   int flags;  /* Whether the player is deadly and whatever else we add.      */
   int pkills; /* Number of pkills on behalf of clan */
   int pdeaths;   /* Number of times pkilled (legally)  */
   int mkills; /* Number of mobs killed         */
   int mdeaths;   /* Number of deaths due to mobs       */
   int illegal_pk;   /* Number of illegal pk's committed   */
   long int balance;
   unsigned int smites;
   unsigned int smited;
   int arena_wins;   /* Arena Code -- Kratas */
   int arena_kills;
   int arena_deaths;

   long int outcast_time;  /* The time at which the char was outcast */
   NUISANCE_DATA *nuisance;   /* New Nuisance structure */

   long int restore_time;  /* The last time the char did a restore all */
   int r_range_lo;   /* room range */
   int r_range_hi;
   int m_range_lo;   /* mob range  */
   int m_range_hi;
   int o_range_lo;   /* obj range  */
   int o_range_hi;
   short wizinvis;   /* wizinvis level */
   short min_snoop;  /* minimum snoop level */
   short condition[MAX_CONDS];
   short learned[MAX_SKILL];
   KILLED_DATA killed[MAX_KILLTRACK];
   short quest_number;  /* current *QUEST BEING DONE* DON'T REMOVE! */
   short quest_curr; /* current number of quest points */
   int quest_accum;  /* quest points accumulated in players life */
   short favor;   /* deity favor */
   int auth_state;
   time_t release_date; /* Auto-helling.. Altrag */
   char *helled_by;
   char *bio;  /* Personal Bio */
   char *authed_by;  /* what crazy imm authed this name ;) */
   SKILLTYPE *special_skills[MAX_PERSONAL];  /* personalized skills/spells */
   char *prompt;  /* User config prompts */
   char *fprompt; /* Fight prompts */
   char *subprompt;  /* Substate prompt */
   short pagerlen;   /* For pager (NOT menus) */
   bool openedtourney;
   IGNORE_DATA *first_ignored;   /* keep track of who to ignore */
   IGNORE_DATA *last_ignored;
   char **tell_history; /* for immortal only command lasttell */
   short lt_index;   /* last_tell index */
   HOMETOWN_DATA *hometown;
   NATION_DATA *nation;
   int htown_vnum;
   int nation_vnum;
   char *wedto;   /* Who character is married to */
   char *help_message;  /* Stores assist message */
   char *alias[MAX_ALIAS]; /* alias - shogar */
   char *alias_sub[MAX_ALIAS];
   char *sendmail;   //buffer used to send an email

   struct global_board_data *board; /* The current board */
   time_t last_note[MAX_BOARD];  /* last note for the boards */
   NOTE_DATA *in_progress;

   char *outputprefix;
   char *outputsuffix;

   LOCKER_DATA *locker;
   ROOM_INDEX_DATA *locker_room; /* Pointer to virtual room */
   int locker_vnum;

   int memorize[8];  /* Locations that can be memorized with the memorize command -- Scion */

#ifdef I3
   I3_CHARDATA *i3chardata;
#endif

#ifdef IMC
   IMC_CHARDATA *imcchardata;
#endif

   long int sbalance;
   long int gbalance;
};

/*
 * Liquids.
 */
#define LIQ_WATER        0
#define LIQ_MAX		18

struct liq_type
{
   char *liq_name;
   char *liq_color;
   short liq_affect[3];
};

struct illegalpkill
{
   IPK_DATA *next;
   IPK_DATA *prev;
   char *vict;
   char *name;
   char *date;
   bool commited;
};

struct locker_data
{
   short capacity;
   short holding;
   int flags;
   int room;
};

/*
 * Damage types from the attack_table[]
 */
typedef enum
{
   DAM_HIT, DAM_SLICE, DAM_STAB, DAM_SLASH, DAM_WHIP, DAM_CLAW,
   DAM_BLAST, DAM_POUND, DAM_CRUSH, DAM_GREP, DAM_BITE, DAM_PIERCE,
   DAM_SUCTION, DAM_BOLT, DAM_ARROW, DAM_DART, DAM_STONE, DAM_PEA
} damage_types;


/*
 * Extra description data for a room or object.
 */
struct extra_descr_data
{
   EXTRA_DESCR_DATA *next; /* Next in list                     */
   EXTRA_DESCR_DATA *prev; /* Previous in list                 */
   char *keyword; /* Keyword in look/examine          */
   char *description;   /* What to see                      */
};



/*
 * Prototype for an object.
 */
struct obj_index_data
{
   OBJ_INDEX_DATA *next;
   OBJ_INDEX_DATA *next_sort;
   EXTRA_DESCR_DATA *first_extradesc;
   EXTRA_DESCR_DATA *last_extradesc;
   AFFECT_DATA *first_affect;
   AFFECT_DATA *last_affect;
   MPROG_DATA *mudprogs;   /* objprogs */
   EXT_BV progtypes; /* objprogs */
   char *name;
   char *short_descr;
   char *description;
   char *action_desc;
   int vnum;
   short level;
   short item_type;
   EXT_BV extra_flags;
   int magic_flags;  /*Need more bitvectors for spells - Scryn */
   int wear_flags;
   short count;
   short weight;
   long int gold_cost;
   long int silver_cost;
   long int copper_cost;
   int value[6];
   int serial;
   short layers;
   int rent;   /* Unused */
};


/*
 * One object.
 */
struct obj_data
{
   OBJ_DATA *next;
   OBJ_DATA *prev;
   OBJ_DATA *next_content;
   OBJ_DATA *prev_content;
   OBJ_DATA *first_content;
   OBJ_DATA *last_content;
   OBJ_DATA *in_obj;
   CHAR_DATA *carried_by;
   EXTRA_DESCR_DATA *first_extradesc;
   EXTRA_DESCR_DATA *last_extradesc;
   AFFECT_DATA *first_affect;
   AFFECT_DATA *last_affect;
   OBJ_INDEX_DATA *pIndexData;
   ROOM_INDEX_DATA *in_room;
   char *name;
   char *short_descr;
   char *description;
   char *action_desc;
   short item_type;
   size_t mpscriptpos;
   EXT_BV extra_flags;
   int magic_flags;  /*Need more bitvectors for spells - Scryn */
   int wear_flags;
   MPROG_ACT_LIST *mpact;  /* mudprogs */
   int mpactnum;  /* mudprogs */
   short wear_loc;
   short weight;
   long int gold_cost;
   long int silver_cost;
   long int copper_cost;
   short level;
   short timer;
   int value[6];
   short count;   /* support for object grouping */
   int serial; /* serial number         */
};

/*
 * Exit data.
 */
struct exit_data
{
   EXIT_DATA *prev;  /* previous exit in linked list  */
   EXIT_DATA *next;  /* next exit in linked list   */
   EXIT_DATA *rexit; /* Reverse exit pointer    */
   ROOM_INDEX_DATA *to_room;  /* Pointer to destination room   */
   char *keyword; /* Keywords for exit or door  */
   char *description;   /* Description of exit     */
   int vnum;   /* Vnum of room exit leads to */
   int rvnum;  /* Vnum of room in opposite dir  */
   int exit_info; /* door states & other flags  */
   int key; /* Key vnum       */
   short vdir; /* Physical "direction"    */
   short distance;   /* how far to the next room   */
   short pull; /* pull of direction (current)   */
   short pulltype;   /* type of pull (current, wind)  */
};



/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'H': hide an object
 *   'B': set a bitvector
 *   'T': trap an object
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct reset_data
{
   RESET_DATA *next;
   RESET_DATA *prev;
   RESET_DATA *first_reset;
   RESET_DATA *last_reset;
   RESET_DATA *next_reset;
   RESET_DATA *prev_reset;
   char command;
   int extra;
   int arg1;
   int arg2;
   int arg3;
};

/* Constants for arg2 of 'B' resets. */
#define	BIT_RESET_DOOR			0
#define BIT_RESET_OBJECT		1
#define BIT_RESET_MOBILE		2
#define BIT_RESET_ROOM			3
#define BIT_RESET_TYPE_MASK		0xFF  /* 256 should be enough */
#define BIT_RESET_DOOR_THRESHOLD	8
#define BIT_RESET_DOOR_MASK		0xFF00   /* 256 should be enough */
#define BIT_RESET_SET			BV30
#define BIT_RESET_TOGGLE		BV31
#define BIT_RESET_FREEBITS	  0x3FFF0000   /* For reference */

/*
 * Area definition.
 */
struct area_data
{
   AREA_DATA *next;
   AREA_DATA *prev;
   AREA_DATA *next_sort;
   AREA_DATA *prev_sort;
   AREA_DATA *next_sort_name; /* Used for alphanum. sort */
   AREA_DATA *prev_sort_name; /* Ditto, Fireblade */
   ROOM_INDEX_DATA *first_room;
   ROOM_INDEX_DATA *last_room;
   char *name;
   char *filename;
   int flags;
   short status;  /* h, 8/11 */
   short age;
   short nplayer;
   short reset_frequency;
   short ttl;
   int low_r_vnum;
   int hi_r_vnum;
   int low_o_vnum;
   int hi_o_vnum;
   int low_m_vnum;
   int hi_m_vnum;
   int low_soft_range;
   int hi_soft_range;
   int low_hard_range;
   int hi_hard_range;
   char *author;  /* Scryn */
   char *resetmsg;   /* Rennard */
   short max_players;
   int mkills;
   int mdeaths;
   int pkills;
   int pdeaths;
   long int gold_looted;
   int illegal_pk;
   int high_economy;
   int low_economy;
   WEATHER_DATA *weather;  /* FB */
   long int silver_looted;
   long int copper_looted;
};

/*
 * Used to keep track of system settings and statistics		-Thoric
 */
struct system_data
{
   int alias_wait;   /* Command wait on aliases */
   int maxplayers;   /* Maximum players this boot   */
   int totalplayers; /* Total players this boot */
   int alltimemax;   /* Maximum players ever   */
   long int global_gold_looted; /* Gold looted this boot */
   int upill_val; /* Used pill value */
   int upotion_val;  /* Used potion value */
   int brewed_used;  /* Brewed potions used */
   int scribed_used; /* Scribed scrolls used */
   char *time_of_max;   /* Time of max ever */
   char *mud_name;   /* Name of mud */
   bool NO_NAME_RESOLVING; /* Hostnames are not resolved  */
   bool DENY_NEW_PLAYERS;  /* New players cannot connect  */
   bool WAIT_FOR_AUTH;  /* New players must be auth'ed */
   short read_all_mail; /* Read all player mail(was 54) */
   short read_mail_free;   /* Read mail for free (was 51) */
   short write_mail_free;  /* Write mail for free(was 51) */
   short take_others_mail; /* Take others mail (was 54)   */
   short muse_level; /* Level of muse channel */
   short think_level;   /* Level of think channel LEVEL_HIGOD */
   short build_level;   /* Level of build channel LEVEL_BUILD */
   short log_level;  /* Level of log channel LEVEL LOG */
   short level_modify_proto;  /* Level to modify prototype stuff LEVEL_LESSER */
   short level_override_private; /* override private flag */
   short level_mset_player;   /* Level to mset a player */
   short bash_plr_vs_plr;  /* Bash mod player vs. player */
   short bash_nontank;  /* Bash mod basher != primary attacker */
   short gouge_plr_vs_plr; /* Gouge mod player vs. player */
   short gouge_nontank; /* Gouge mod player != primary attacker */
   short stun_plr_vs_plr;  /* Stun mod player vs. player */
   short stun_regular;  /* Stun difficult */
   short dodge_mod;  /* Divide dodge chance by */
   short parry_mod;  /* Divide parry chance by */
   short tumble_mod; /* Divide tumble chance by */
   short dam_plr_vs_plr;   /* Damage mod player vs. player */
   short dam_plr_vs_mob;   /* Damage mod player vs. mobile */
   short dam_mob_vs_plr;   /* Damage mod mobile vs. player */
   short dam_mob_vs_mob;   /* Damage mod mobile vs. mobile */
   short level_getobjnotake;  /* Get objects without take flag */
   short level_forcepc; /* The level at which you can use force on players. */
   short bestow_dif; /* Max # of levels between trust and command level for a bestow to work --Blodkai */
   short max_sn;  /* Max skills */
   char *guild_overseer;   /* Pointer to char containing the name of the */
   char *guild_advisor; /* guild overseer and advisor. */
   int save_flags;   /* Toggles for saving conditions */
   short save_frequency;   /* How old to autosave someone */
   short check_imm_host;   /* Do we check immortal's hosts? */
   short morph_opt;  /* Do we optimize morph's? */
   short save_pets;  /* Do pets save? */
   short ban_site_level;   /* Level to ban sites */
   short ban_class_level;  /* Level to ban classes */
   short ban_race_level;   /* Level to ban races */
   short ident_retries; /* Number of times to retry broken pipes. */
   long int global_silver_looted;  /* Global Silver taken */
   long int global_copper_looted;  /* Global copper taken */
   int arena;  /* Scion arenacode */
   bool imm_arena;   /* Scion arenacode */
   bool double_exp;
   bool webtoggle;
   int webport;
   bool webcounter;
   bool wizlock;  /* Kratas */
   bool all_pkill_mode; /* Cronel pkill mode */
   char *homepage;
   int ccount;
   int scount;
   bool neverempty;  /* Altrag */
   short newbie_purge;  /* Level to auto-purge newbies at - Samson 12-27-98 */
   short regular_purge; /* Level to purge normal players at - Samson 12-27-98 */
   bool CLEANPFILES; /* Should the mud clean up pfiles daily? - Samson 12-27-98 */
   int mplimit;
   int pklimit;
};



struct plane_data
{
   PLANE_DATA *next;
   PLANE_DATA *prev;
   char *name;
};

struct room_to_be_decomped
{
   ROOM_DECOMP *next;
   ROOM_DECOMP *prev;
   ROOM_INDEX_DATA *room;
   unsigned int ttl_decomp;
};

/*
 * Room type.
 */
struct room_index_data
{
   ROOM_INDEX_DATA *next;
   ROOM_INDEX_DATA *next_sort;
   RESET_DATA *first_reset;
   RESET_DATA *last_reset;
   RESET_DATA *last_mob_reset;
   RESET_DATA *last_obj_reset;
   ROOM_INDEX_DATA *next_aroom;  /* Rooms within an area */
   ROOM_INDEX_DATA *prev_aroom;
   CHAR_DATA *first_person;   /* people in the room  */
   CHAR_DATA *last_person; /*      ..    */
   OBJ_DATA *first_content;   /* objects on floor    */
   OBJ_DATA *last_content; /*      ..    */
   EXTRA_DESCR_DATA *first_extradesc;  /* extra descriptions */
   EXTRA_DESCR_DATA *last_extradesc;   /*      ..    */
   AREA_DATA *area;
   EXIT_DATA *first_exit;  /* exits from the room */
   EXIT_DATA *last_exit;   /*      ..    */
   AFFECT_DATA *first_affect; /* effects on the room */
   AFFECT_DATA *last_affect;  /*      ..    */
   PLANE_DATA *plane;   /* do it by room rather than area */
   MPROG_ACT_LIST *mpact;  /* mudprogs */
   int mpactnum;  /* mudprogs */
   MPROG_DATA *mudprogs;   /* mudprogs */
   size_t mpscriptpos;
   char *name;

   /*
    * wilderness
    */
   char *legend;
   char *color;
#ifdef COMPRESSION_TEST
   /*
    * Description Compression
    */
   z_stream *compress;
   unsigned char *compress_desc;
   unsigned int desc_alloc;
   unsigned int desc_size;
   ROOM_DECOMP *rdecomp;
   /*
    * End Compression
    */
#endif
   char *description;
   int desc_length;
   int vnum;
   int room_flags;
   EXT_BV progtypes; /* mudprogs */
   short light;   /* amount of light in the room */
   short sector_type;
   int tele_vnum;
   short tele_delay;
   short tunnel;  /* max people that will fit */
   short virtual; /* neccessary for bilvnums shogar */
};

/*
 * Delayed teleport type.
 */
struct teleport_data
{
   TELEPORT_DATA *next;
   TELEPORT_DATA *prev;
   ROOM_INDEX_DATA *room;
   short timer;
};


/*
 * Types of skill numbers.  Used to keep separate lists of sn's
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_UNDEFINED               -1
#define TYPE_HIT                     1000 /* allows for 1000 skills/spells */
#define TYPE_HERB		     2000   /* allows for 1000 attack types  */
#define TYPE_PERSONAL		     3000   /* allows for 1000 herb types    */
#define TYPE_RACIAL		     4000   /* allows for 1000 personal types */
#define TYPE_DISEASE		     5000   /* allows for 1000 racial types  */

/*
 *  Target types.
 */
typedef enum
{
   TAR_IGNORE, TAR_CHAR_OFFENSIVE, TAR_CHAR_DEFENSIVE, TAR_CHAR_SELF,
   TAR_OBJ_INV
} target_types;

typedef enum
{
   SKILL_UNKNOWN, SKILL_SPELL, SKILL_SKILL, SKILL_WEAPON, SKILL_TONGUE,
   SKILL_HERB, SKILL_RACIAL, SKILL_DISEASE
} skill_types;



struct timerset
{
   int num_uses;
   struct timeval total_time;
   struct timeval min_time;
   struct timeval max_time;
};



/*
 * Skills include spells as a particular case.
 */
struct skill_type
{
   char *name; /* Name of skill     */
   short skill_level[MAX_CLASS]; /* Level needed by class   */
   short skill_adept[MAX_CLASS]; /* Max attainable % in this skill */
   short race_level[MAX_RACE];   /* Racial abilities: level      */
   short race_adept[MAX_RACE];   /* Racial abilities: adept      */
   SPELL_FUN *spell_fun;   /* Spell pointer (for spells) */
   DO_FUN *skill_fun;   /* Skill pointer (for skills) */
   short target;  /* Legal targets     */
   short minimum_position; /* Position for caster / user */
   short slot; /* Slot for #OBJECT loading   */
   short min_mana;   /* Minimum mana used    */
   short beats;   /* Rounds required to use skill  */
   char *noun_damage;   /* Damage message    */
   char *msg_off; /* Wear off message     */
   short guild;   /* Which guild the skill belongs to */
   short min_level;  /* Minimum level to be able to cast */
   short type; /* Spell/Skill/Weapon/Tongue  */
   short range;   /* Range of spell (rooms)  */
   int info;   /* Spell action/class/etc  */
   int flags;  /* Flags       */
   char *hit_char;   /* Success message to caster  */
   char *hit_vict;   /* Success message to victim  */
   char *hit_room;   /* Success message to room */
   char *hit_dest;   /* Success message to dest room  */
   char *miss_char;  /* Failure message to caster  */
   char *miss_vict;  /* Failure message to victim  */
   char *miss_room;  /* Failure message to room */
   char *die_char;   /* Victim death msg to caster */
   char *die_vict;   /* Victim death msg to victim */
   char *die_room;   /* Victim death msg to room   */
   char *imm_char;   /* Victim immune msg to caster   */
   char *imm_vict;   /* Victim immune msg to victim   */
   char *imm_room;   /* Victim immune msg to room  */
   char *dice; /* Dice roll         */
   int value;  /* Misc value        */
   char saves; /* What saving spell applies  */
   char difficulty;  /* Difficulty of casting/learning */
   SMAUG_AFF *affects;  /* Spell affects, if any   */
   char *components; /* Spell components, if any   */
   char *teachers;   /* Skill requires a special teacher */
   char participants;   /* # of required participants */
   struct timerset userec; /* Usage record         */
};


/* how many items to track.... prevent repeat auctions */
#define AUCTION_MEM 3

struct auction_data
{
   OBJ_DATA *item;   /* a pointer to the item */
   CHAR_DATA *seller;   /* a pointer to the seller - which may NOT quit */
   CHAR_DATA *buyer; /* a pointer to the buyer - which may NOT quit */
   int bet; /* last bet - or 0 if noone has bet anything */
   short going;   /* 1,2, sold */
   short pulse;   /* how many pulses (.25 sec) until another call-out ? */
   int starting;
   OBJ_INDEX_DATA *history[AUCTION_MEM];  /* store auction history */
   short hist_timer; /* clear out history buffer if auction is idle */
};

/*
 * So we can have different configs for different ports -- Shaddai
 */
extern int port;

/*
 * These are skill_lookup return values for common skills and spells.
 */
extern short gsn_style_evasive;
extern short gsn_style_defensive;
extern short gsn_style_standard;
extern short gsn_style_aggressive;
extern short gsn_style_berserk;

extern short gsn_trance;
extern short gsn_ruminate;
extern short gsn_meditate;
extern short gsn_detrap;
extern short gsn_backstab;
extern short gsn_circle;
extern short gsn_cook;
extern short gsn_study;
extern short gsn_dodge;
extern short gsn_hide;
extern short gsn_peek;
extern short gsn_pick_lock;
extern short gsn_scan;
extern short gsn_sneak;
extern short gsn_steal;
extern short gsn_gouge;
extern short gsn_track;
extern short gsn_search;
extern short gsn_dig;
extern short gsn_mount;
extern short gsn_bashdoor;
extern short gsn_berserk;
extern short gsn_hitall;

extern short gsn_disarm;
extern short gsn_enhanced_damage;
extern short gsn_kick;
extern short gsn_parry;
extern short gsn_rescue;
extern short gsn_second_attack;
extern short gsn_third_attack;
extern short gsn_fourth_attack;
extern short gsn_fifth_attack;
extern short gsn_sixth_attack;
extern short gsn_seventh_attack;
extern short gsn_eighth_attack;
extern short gsn_dual_wield;

extern short gsn_feed;
extern short gsn_bloodlet;
extern short gsn_broach;
extern short gsn_mistwalk;

extern short gsn_bleeding;
extern short gsn_aid;
extern short gsn_assassinate;

/* used to do specific lookups */
extern short gsn_first_spell;
extern short gsn_first_skill;
extern short gsn_first_weapon;
extern short gsn_first_tongue;
extern short gsn_top_sn;

/* casting skills -- Scion */
extern short gsn_second_cast;
extern short gsn_third_cast;
extern short gsn_fourth_cast;
extern short gsn_fifth_cast;
extern short gsn_sixth_cast;
extern short gsn_seventh_cast;
extern short gsn_eighth_cast;

/* spells */
extern short gsn_blindness;
extern short gsn_charm_person;
extern short gsn_aqua_breath;
extern short gsn_curse;
extern short gsn_invis;
extern short gsn_mass_invis;
extern short gsn_poison;
extern short gsn_sleep;
extern short gsn_possess;
extern short gsn_fireball; /* for fireshield  */
extern short gsn_chill_touch; /* for iceshield   */
extern short gsn_lightning_bolt; /* for shockshield */
extern short gsn_haste;
extern short gsn_slow;

/* newer attack skills */
extern short gsn_punch;
extern short gsn_bash;
extern short gsn_stun;
extern short gsn_bite;
extern short gsn_claw;
extern short gsn_sting;
extern short gsn_tail;

extern short gsn_poison_weapon;
extern short gsn_scribe;
extern short gsn_brew;
extern short gsn_climb;

extern short gsn_pugilism;
extern short gsn_long_blades;
extern short gsn_short_blades;
extern short gsn_flexible_arms;
extern short gsn_talonous_arms;
extern short gsn_maces;
extern short gsn_missile_weapons;
extern short gsn_shieldwork;

extern short gsn_grip;
extern short gsn_slice;

extern short gsn_tumble;

/* Language gsns. -- Altrag */
extern short gsn_common;
extern short gsn_elven;
extern short gsn_dwarven;
extern short gsn_pixie;
extern short gsn_ogre;
extern short gsn_orcish;
extern short gsn_trollish;
extern short gsn_goblin;
extern short gsn_halfling;
extern short gsn_sharpen;  /* Sharpen skill --Cronel */
extern short gsn_listen;
extern short gsn_hunt;

/*
 * Cmd flag names --Shaddai
 */
extern char *const cmd_flags[];

/*
 * Utility macros.
 */
#define UMIN(a, b)		((a) < (b) ? (a) : (b))
#define UMAX(a, b)		((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)		((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)		((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))


/*
 * Old-style Bit manipulation macros
 *
 * The bit passed is the actual value of the bit (Use the BV## defines)
 */
#define IS_SET(flag, bit)	((flag) & (bit))
#define SET_BIT(var, bit)	((var) |= (bit))
#define REMOVE_BIT(var, bit)	((var) &= ~(bit))
#define TOGGLE_BIT(var, bit)	((var) ^= (bit))
#define CH(d)			((d)->original ? (d)->original : (d)->character)

/*
 * Macros for accessing virtually unlimited bitvectors.		-Thoric
 *
 * Note that these macros use the bit number rather than the bit value
 * itself -- which means that you can only access _one_ bit at a time
 *
 * This code uses an array of integers
 */

/*
 * The functions for these prototypes can be found in misc.c
 * They are up here because they are used by the macros below
 */
bool ext_is_empty args( ( EXT_BV * bits ) );
void ext_clear_bits args( ( EXT_BV * bits ) );
int ext_has_bits args( ( EXT_BV * var, EXT_BV * bits ) );
bool ext_same_bits args( ( EXT_BV * var, EXT_BV * bits ) );
void ext_set_bits args( ( EXT_BV * var, EXT_BV * bits ) );
void ext_remove_bits args( ( EXT_BV * var, EXT_BV * bits ) );
void ext_toggle_bits args( ( EXT_BV * var, EXT_BV * bits ) );
int get_value args( (long int gval,long int sval,long int cval ) );
void conv_currency args( ( CHAR_DATA * ch, long int tmpvalue ) );


/*
 * Here are the extended bitvector macros:
 */
#define xIS_SET(var, bit)	((var).bits[(bit) >> RSV] & 1 << ((bit) & XBM))
#define xSET_BIT(var, bit)	((var).bits[(bit) >> RSV] |= 1 << ((bit) & XBM))
#define xSET_BITS(var, bit)	(ext_set_bits(&(var), &(bit)))
#define xREMOVE_BIT(var, bit)	((var).bits[(bit) >> RSV] &= ~(1 << ((bit) & XBM)))
#define xREMOVE_BITS(var, bit)	(ext_remove_bits(&(var), &(bit)))
#define xTOGGLE_BIT(var, bit)	((var).bits[(bit) >> RSV] ^= 1 << ((bit) & XBM))
#define xTOGGLE_BITS(var, bit)	(ext_toggle_bits(&(var), &(bit)))
#define xCLEAR_BITS(var)	(ext_clear_bits(&(var)))
#define xIS_EMPTY(var)		(ext_is_empty(&(var)))
#define xHAS_BITS(var, bit)	(ext_has_bits(&(var), &(bit)))
#define xSAME_BITS(var, bit)	(ext_same_bits(&(var), &(bit)))

/*
 * Memory allocation macros.
 */

/* Added the number+1 thing because we were going over our spending limit
   on memory -every- time CREATE got used. It was only by one, but once we
   got up over about 110 areas and started to have regular players, things
   were crashing us quite often. -- Scion */

#define CREATE(result, type, number)				\
do								\
{								\
    if (!((result) = (type *) calloc ((number), sizeof(type))))	\
    {								\
	perror("calloc failure.");				\
	fprintf(stderr, "Calloc failure @ %s:%d\n", __FILE__, __LINE__); \
	abort();						\
    }								\
} while(0)

#define RECREATE(result,type,number)				\
do								\
{								\
    if (!((result) = (type *) realloc ((result), sizeof(type) * (number))))\
    {								\
	perror("realloc failure");				\
	fprintf(stderr, "Realloc failure @ %s:%d\n", __FILE__, __LINE__); \
	abort();						\
    }								\
} while(0)

#define DISPOSE(point)                      \
do                                          \
{                                           \
   if( (point) )                            \
   {                                        \
      free( (point) );                      \
      (point) = NULL;                       \
   }                                        \
} while(0)

#define STRALLOC(point)		str_alloc((point))
#define QUICKLINK(point)	quick_link((point))
#define STRFREE(point)                          \
do                                              \
{                                               \
   if((point))                                  \
   {                                            \
      if( str_free((point)) == -1 )             \
         bug( "&RSTRFREEing bad pointer: %s, line %d", __FILE__, __LINE__ ); \
      (point) = NULL;                           \
   }                                            \
} while(0)

/* double-linked list handling macros -Thoric */
/* Updated by Scion 8/6/1999 */
#define LINK(link, first, last, next, prev)                     \
do                                                              \
{                                                               \
    if ( !(first) ) {                                           \
      (first)                   = (link);                       \
      (last)					= (link);	\
    } else                                                      \
      (last)->next              = (link);                       \
    (link)->next                = NULL;                         \
    if (first == link)						\
      (link)->prev 				= NULL;		\
    else							\
      (link)->prev              = (last);                       \
    (last)                      = (link);                       \
} while(0)

#define INSERT(link, insert, first, next, prev)                 \
do                                                              \
{                                                               \
    (link)->prev                = (insert)->prev;               \
    if ( !(insert)->prev )                                      \
      (first)                   = (link);                       \
    else                                                        \
      (insert)->prev->next      = (link);                       \
    (insert)->prev              = (link);                       \
    (link)->next                = (insert);                     \
} while(0)

#define UNLINK(link, first, last, next, prev)                   \
do                                                              \
{                                                               \
	if ( !(link)->prev ) {                                  \
      (first)                   = (link)->next;                 \
	  if ((first))						\
	  (first)->prev				= NULL;		\
	} else {                                                \
      (link)->prev->next        = (link)->next;                 \
	}							\
	if ( !(link)->next ) {                                  \
      (last)                    = (link)->prev;                 \
	  if ((last))						\
	  (last)->next				= NULL;		\
	} else {                                                \
      (link)->next->prev        = (link)->prev;                 \
	}							\
} while(0)

#define CHECK_LINKS(first, last, next, prev, type)		\
do {								\
  type *ptr, *pptr = NULL;					\
  if ( !(first) && !(last) )					\
    break;							\
  if ( !(first) )						\
  {								\
    bug( "CHECK_LINKS: last with NULL first!  %s.",		\
        __STRING(first) );					\
    for ( ptr = (last); ptr->prev; ptr = ptr->prev );		\
    (first) = ptr;						\
  }								\
  else if ( !(last) )						\
  {								\
    bug( "CHECK_LINKS: first with NULL last!  %s.",		\
        __STRING(first) );					\
    for ( ptr = (first); ptr->next; ptr = ptr->next );		\
    (last) = ptr;						\
  }								\
  if ( (first) )						\
  {								\
    for ( ptr = (first); ptr; ptr = ptr->next )			\
    {								\
      if ( ptr->prev != pptr )					\
      {								\
        bug( "CHECK_LINKS(%s): %p:->prev != %p.  Fixing.",	\
            __STRING(first), ptr, pptr );			\
        ptr->prev = pptr;					\
      }								\
      if ( ptr->prev && ptr->prev->next != ptr )		\
      {								\
        bug( "CHECK_LINKS(%s): %p:->prev->next != %p.  Fixing.",\
            __STRING(first), ptr, ptr );			\
        ptr->prev->next = ptr;					\
      }								\
      pptr = ptr;						\
    }								\
    pptr = NULL;						\
  }								\
  if ( (last) )							\
  {								\
    for ( ptr = (last); ptr; ptr = ptr->prev )			\
    {								\
      if ( ptr->next != pptr )					\
      {								\
        bug( "CHECK_LINKS (%s): %p:->next != %p.  Fixing.",	\
            __STRING(first), ptr, pptr );			\
        ptr->next = pptr;					\
      }								\
      if ( ptr->next && ptr->next->prev != ptr )		\
      {								\
        bug( "CHECK_LINKS(%s): %p:->next->prev != %p.  Fixing.",\
            __STRING(first), ptr, ptr );			\
        ptr->next->prev = ptr;					\
      }								\
      pptr = ptr;						\
    }								\
  }								\
} while(0)

#define ASSIGN_GSN(gsn, skill)					\
do								\
{								\
    if ( ((gsn) = skill_lookup((skill))) == -1 )		\
	fprintf( stderr, "ASSIGN_GSN: Skill %s not found.\n",	\
		(skill) );					\
} while(0)

#define CHECK_SUBRESTRICTED(ch)					\
do								\
{								\
    if ( (ch)->substate == SUB_RESTRICTED )			\
    {								\
	send_to_char( "You cannot use this command from within another command.\r\n", ch );	\
	return;							\
    }								\
} while(0)


/*
 * Character macros.
 */
#define IS_NEEDHELP(ch)         (xIS_SET((ch)->act, PLR_NEEDHELP))
#define CLASS_NAME(ch)          (((ch)->level2 == -1) ? class_table[ch->class]->who_name \
                                : CLASS_NAME2(ch))
#define SKILL_CLASS(ch,sn)      (((ch)->level2 == -1) ? skill_table[sn]->skill_level[ch->class] : \
                                (SKILL_CLASS2(ch, sn) || \
                                skill_table[sn]->skill_level[ch->class] ))
#define SKILL_CLASS2(ch,sn)     (((ch)->level3 == -1) ? skill_table[sn]->skill_level[ch->class2] : \
                                (skill_table[sn]->skill_level[ch->class3] || \
                                skill_table[sn]->skill_level[ch->class2] ))
#define CLASS_NAME2(ch)		(((ch)->level3 == -1) ? class_table[ch->class2]->who_name \
				: class_table[ch->class3]->who_name)
#define IS_NPC(ch)		(xIS_SET((ch)->act, ACT_IS_NPC) && !(ch)->pcdata)
#define IS_WRITING(ch)          (((ch)->desc && (ch)->desc->connected) ? TRUE : FALSE)
#define IS_IMMORTAL(ch)		(get_trust((ch)) >= LEVEL_IMMORTAL || ch->level2 >= LEVEL_IMMORTAL || ch->level3 >= LEVEL_IMMORTAL )
#define IS_HERO(ch)		(get_trust((ch)) >= LEVEL_HERO)
#define IS_AFFECTED(ch, sn)	(xIS_SET((ch)->affected_by, (sn)))
#define HAS_BODYPART(ch, part)	((ch)->xflags == 0 || IS_SET((ch)->xflags, (part)))

#define CAN_CAST(ch)		((ch)->class != 2 && (ch)->class != 3)

#define IS_VAMPIRE(ch)		(!IS_NPC(ch)				    \
				&& ((ch)->class==CLASS_DEMON		    \
				||  (ch)->race==RACE_VAMPIRE                \
                                ||  (ch)->class==CLASS_VAMPIRE))
#define IS_GOOD(ch)		((ch)->alignment >= 350)
#define IS_EVIL(ch)		((ch)->alignment <= -350)
#define IS_NEUTRAL(ch)		(!IS_GOOD(ch) && !IS_EVIL(ch))

#define IS_AWAKE(ch)		((ch)->position > POS_SLEEPING)
#define GET_AC(ch)		((ch)->armor				    \
				    + ( IS_AWAKE(ch)			    \
				    ? dex_app[get_curr_dex(ch)].defensive   \
				    : 0 )				    \
				    + VAMP_AC(ch))
#if 0
#define GET_HITROLL(ch)		((ch)->hitroll				    \
				    +str_app[get_curr_str(ch)].tohit	    \
				    +(2-(abs((ch)->mental_state)/10)))

/* Thanks to Chriss Baeke for noticing damplus was unused */
#define GET_DAMROLL(ch)		((ch)->damroll                              \
				    +(ch)->damplus			    \
				    +str_app[get_curr_str(ch)].todam	    \
				    +(((ch)->mental_state > 5		    \
				    &&(ch)->mental_state < 15) ? 1 : 0) )
#endif
#define IS_OUTSIDE(ch)		(!IS_SET(				    \
				    (ch)->in_room->room_flags,		    \
				    ROOM_INDOORS) && !IS_SET(               \
				    (ch)->in_room->room_flags,              \
				    ROOM_TUNNEL))

#define NO_WEATHER_SECT(sect)  (  sect == SECT_INSIDE || 	           \
				  sect == SECT_UNDERWATER ||               \
                                  sect == SECT_OCEANFLOOR ||               \
                                  sect == SECT_UNDERGROUND )

#define IS_DRUNK(ch, drunk)     (number_percent() < \
			        ( (ch)->pcdata->condition[COND_DRUNK] \
				* 2 / (drunk) ) )

#define IS_CLANNED(ch)		(!IS_NPC((ch))				    \
				&& (ch)->pcdata->clan			    \
				&& (ch)->pcdata->clan->clan_type != CLAN_ORDER  \
				&& (ch)->pcdata->clan->clan_type != CLAN_GUILD)

#define IS_ORDERED(ch)		(!IS_NPC((ch))				    \
				&& (ch)->pcdata->clan			    \
				&& (ch)->pcdata->clan->clan_type == CLAN_ORDER)

#define IS_GUILDED(ch)		(!IS_NPC((ch))				    \
				&& (ch)->pcdata->clan			    \
				&& (ch)->pcdata->clan->clan_type == CLAN_GUILD)

#define IS_DEADLYCLAN(ch)	(!IS_NPC((ch))				    \
				&& (ch)->pcdata->clan			    \
				&& (ch)->pcdata->clan->clan_type != CLAN_NOKILL) \
				&& (ch)->pcdata->clan->clan_type != CLAN_ORDER)  \
				&& (ch)->pcdata->clan->clan_type != CLAN_GUILD)

#define IS_DEVOTED(ch)		(!IS_NPC((ch))				    \
				&& (ch)->pcdata->deity)

#define IS_PKILL(ch)            (ch->pcdata && IS_SET( ch->pcdata->flags, PCFLAG_DEADLY ))
                                /*someone changed IS_PKILL to (TRUE) and commented out test
                                 *no idea why.  reinstated old macro so hitall works
                                 *Mordred January 2 2009
                                */
#define CAN_PKILL(ch)           ((IS_PKILL(ch) && ch->level >= 10) || sysdata.all_pkill_mode)
/* Cronel, pkill mode */

/* Addition to make people with nuisance flag have more wait */

#define WAIT_STATE(ch, npulse) ((ch)->wait=(!IS_NPC(ch)&&ch->pcdata->nuisance&&\
			      (ch->pcdata->nuisance->flags>4))?UMAX((ch)->wait,\
			      (npulse+((ch)->pcdata->nuisance->flags-4)+ \
               		      ch->pcdata->nuisance->power)): \
			      UMAX((ch)->wait, (IS_IMMORTAL(ch) ? 0 :(npulse))))


#define EXIT(ch, door)		( get_exit( (ch)->in_room, door ) )

#define CAN_GO(ch, door)	(EXIT((ch),(door))			 \
				&& (EXIT((ch),(door))->to_room != NULL)  \
                          	&& !IS_SET(EXIT((ch), (door))->exit_info, EX_CLOSED))

#define IS_FLOATING(ch)		( IS_AFFECTED((ch), AFF_FLYING) || IS_AFFECTED((ch), AFF_FLOATING) )

#define IS_VALID_SN(sn)		( (sn) >=0 && (sn) < MAX_SKILL		     \
				&& skill_table[(sn)]			     \
				&& skill_table[(sn)]->name )

#define IS_VALID_HERB(sn)	( (sn) >=0 && (sn) < MAX_HERB		     \
				&& herb_table[(sn)]			     \
				&& herb_table[(sn)]->name )

#define IS_VALID_DISEASE(sn)	( (sn) >=0 && (sn) < MAX_DISEASE	     \
				&& disease_table[(sn)]			     \
				&& disease_table[(sn)]->name )

#define IS_PACIFIST(ch)		(IS_NPC(ch) && xIS_SET(ch->act, ACT_PACIFIST))

#define SPELL_FLAG(skill, flag)	( IS_SET((skill)->flags, (flag)) )
#define SPELL_DAMAGE(skill)	( ((skill)->info      ) & 7 )
#define SPELL_ACTION(skill)	( ((skill)->info >>  3) & 7 )
#define SPELL_CLASS(skill)	( ((skill)->info >>  6) & 7 )
#define SPELL_POWER(skill)	( ((skill)->info >>  9) & 3 )
#define SPELL_SAVE(skill)	( ((skill)->info >> 11) & 7 )
#define SET_SDAM(skill, val)	( (skill)->info =  ((skill)->info & SDAM_MASK) + ((val) & 7) )
#define SET_SACT(skill, val)	( (skill)->info =  ((skill)->info & SACT_MASK) + (((val) & 7) << 3) )
#define SET_SCLA(skill, val)	( (skill)->info =  ((skill)->info & SCLA_MASK) + (((val) & 7) << 6) )
#define SET_SPOW(skill, val)	( (skill)->info =  ((skill)->info & SPOW_MASK) + (((val) & 3) << 9) )
#define SET_SSAV(skill, val)	( (skill)->info =  ((skill)->info & SSAV_MASK) + (((val) & 7) << 11) )

/* Retired and guest imms. */
#define IS_RETIRED(ch) (ch->pcdata && IS_SET(ch->pcdata->flags,PCFLAG_RETIRED))
#define IS_GUEST(ch) (ch->pcdata && IS_SET(ch->pcdata->flags,PCFLAG_GUEST))

/* RIS by gsn lookups. -- Altrag.
   Will need to add some || stuff for spells that need a special GSN. */

#define IS_FIRE(dt)		( IS_VALID_SN(dt) &&			     \
				SPELL_DAMAGE(skill_table[(dt)]) == SD_FIRE )
#define IS_COLD(dt)		( IS_VALID_SN(dt) &&			     \
				SPELL_DAMAGE(skill_table[(dt)]) == SD_COLD )
#define IS_ACID(dt)		( IS_VALID_SN(dt) &&			     \
				SPELL_DAMAGE(skill_table[(dt)]) == SD_ACID )
#define IS_ELECTRICITY(dt)	( IS_VALID_SN(dt) &&			     \
				SPELL_DAMAGE(skill_table[(dt)]) == SD_ELECTRICITY )
#define IS_ENERGY(dt)		( IS_VALID_SN(dt) &&			     \
				SPELL_DAMAGE(skill_table[(dt)]) == SD_ENERGY )

#define IS_DRAIN(dt)		( IS_VALID_SN(dt) &&			     \
				SPELL_DAMAGE(skill_table[(dt)]) == SD_DRAIN )

#define IS_POISON(dt)		( IS_VALID_SN(dt) &&			     \
				SPELL_DAMAGE(skill_table[(dt)]) == SD_POISON )


#define NOT_AUTHED(ch)		(!IS_NPC(ch) && ch->pcdata->auth_state <= 3  \
			      && IS_SET(ch->pcdata->flags, PCFLAG_UNAUTHED) )

#define IS_WAITING_FOR_AUTH(ch) (!IS_NPC(ch) && ch->desc		     \
			      && ch->pcdata->auth_state == 1		     \
			      && IS_SET(ch->pcdata->flags, PCFLAG_UNAUTHED) )

/*
 * Object macros.
 */
#define CAN_WEAR(obj, part)	(IS_SET((obj)->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)	(xIS_SET((obj)->extra_flags, (stat)))

/*
 * MudProg macros.						-Thoric
 */
#define HAS_PROG(what, prog)	(xIS_SET((what)->progtypes, (prog)))

/*
 * Description macros.
 */
#define PERS(ch, looker)	( can_see( (looker), (ch) ) ?		\
				( IS_NPC(ch) ? (ch)->short_descr	\
				: (ch)->name ) : "someone" )

#define MORPHPERS(ch, looker)   ( can_see( (looker), (ch) ) ?           \
                                (ch)->morph->morph->short_desc       \
                                : "someone" )


#define log_string(txt)		( log_string_plus( (txt), LOG_NORMAL, LEVEL_LOG ) )
#define dam_message(ch, victim, dam, dt)	( new_dam_message((ch), (victim), (dam), (dt), NULL) )

/*
 *  Defines for the command flags. --Shaddai
 */
#define	CMD_FLAG_POSSESS	BV00
#define CMD_FLAG_POLYMORPHED	BV01
#define CMD_WATCH		BV02  /* FB */

/*
 * Structure for a command in the command lookup table.
 */
struct cmd_type
{
   CMDTYPE *next;
   char *name;
   DO_FUN *do_fun;
   int flags;  /* Added for Checking interpret stuff -Shaddai */
   short position;
   short level;
   short log;
   struct timerset userec;
   int lag_count; /* count lag flags for this cmd - FB */
   short cshow;   /* if command is shown in command list - Zarius */
};

struct alias_data
{
   ALIAS_DATA *next;
   ALIAS_DATA *prev;
   char *name;
   char *alias;
};

/*
 * Structure for a social in the socials table.
 */
struct social_type
{
   SOCIALTYPE *next;
   char *name;
   char *char_no_arg;
   char *others_no_arg;
   char *char_found;
   char *others_found;
   char *vict_found;
   char *char_auto;
   char *others_auto;
};



/*
 * Global constants.
 */
extern time_t last_restore_all_time;
extern time_t boot_time;   /* this should be moved down */
extern HOUR_MIN_SEC *set_boot_time;
extern struct tm *new_boot_time;
extern time_t new_boot_time_t;

extern const struct str_app_type str_app[26];
extern const struct int_app_type int_app[26];
extern const struct wis_app_type wis_app[26];
extern const struct dex_app_type dex_app[26];
extern const struct con_app_type con_app[26];
extern const struct cha_app_type cha_app[26];
extern const struct lck_app_type lck_app[26];

extern const struct race_type _race_table[MAX_RACE];
extern struct race_type *race_table[MAX_RACE];
extern const struct liq_type liq_table[LIQ_MAX];
extern char *const attack_table[18];

extern char **const s_message_table[18];
extern char **const p_message_table[18];

extern char *const skill_tname[];
extern short const movement_loss[SECT_MAX];
extern char *const dir_name[];
extern char *const where_name[];
extern const short rev_dir[];
extern const int trap_door[];
extern char *const r_flags[];
extern char *const w_flags[];
extern char *const o_flags[];
extern char *const a_flags[];
extern char *const o_types[];
extern char *const a_types[];
extern char *const act_flags[];
extern char *const plr_flags[];
extern char *const pc_flags[];
extern char *const trap_flags[];
extern char *const ris_flags[];
extern char *const trig_flags[];
extern char *const part_flags[];
extern char *const npc_race[];
extern char *const npc_class[];
extern char *const defense_flags[];
extern char *const attack_flags[];
extern char *const area_flags[];
extern char *const container_flags[];  /* Tagith */
extern char *const ex_pmisc[];
extern char *const ex_pwater[];
extern char *const ex_pair[];
extern char *const ex_pearth[];
extern char *const ex_pfire[];
extern char *const wear_locs[];
extern int const lang_array[];
extern char *const lang_names[];

extern char *const temp_settings[]; /* FB */
extern char *const precip_settings[];
extern char *const wind_settings[];
extern char *const preciptemp_msg[6][6];
extern char *const windtemp_msg[6][6];
extern char *const precip_msg[];
extern char *const wind_msg[];

/*
 * Global variables.
 */
extern bool mud_down;
extern char *target_name;
extern char *ranged_target_name;
extern int numobjsloaded;
extern int nummobsloaded;
extern int physicalobjects;
extern int last_pkroom;
extern int num_descriptors;
extern struct system_data sysdata;
extern int top_sn;
extern int top_vroom;
extern int top_herb;
extern char str_boot_time[];
extern char reboot_time[];

extern CMDTYPE *command_hash[126];

extern struct class_type *class_table[MAX_CLASS];
extern char *title_table[MAX_CLASS][MAX_LEVEL + 1][2];

extern SKILLTYPE *skill_table[MAX_SKILL];
extern SOCIALTYPE *social_index[27];
extern CHAR_DATA *cur_char;
extern ROOM_INDEX_DATA *cur_room;
extern bool cur_char_died;
extern ch_ret global_retcode;
extern SKILLTYPE *herb_table[MAX_HERB];
extern SKILLTYPE *disease_table[MAX_DISEASE];

extern int cur_obj;
extern int cur_obj_serial;
extern bool cur_obj_extracted;
extern obj_ret global_objcode;

extern HELP_DATA *first_help;
extern HELP_DATA *last_help;
extern SHOP_DATA *first_shop;
extern SHOP_DATA *last_shop;
extern REPAIR_DATA *first_repair;
extern REPAIR_DATA *last_repair;

extern WATCH_DATA *first_watch;
extern WATCH_DATA *last_watch;
extern BAN_DATA *first_ban;
extern BAN_DATA *last_ban;
extern BAN_DATA *first_ban_class;
extern BAN_DATA *last_ban_class;
extern BAN_DATA *first_ban_race;
extern BAN_DATA *last_ban_race;
extern RESERVE_DATA *first_reserved;
extern RESERVE_DATA *last_reserved;
extern FILTER_DATA *first_filtered;
extern FILTER_DATA *last_filtered;
extern CHAR_DATA *first_char;
extern CHAR_DATA *last_char;
extern DESCRIPTOR_DATA *first_descriptor;
extern DESCRIPTOR_DATA *last_descriptor;
extern BOARD_DATA *first_board;
extern BOARD_DATA *last_board;
extern PLANE_DATA *first_plane;
extern PLANE_DATA *last_plane;
extern PROJECT_DATA *first_project;
extern PROJECT_DATA *last_project;
extern OBJ_DATA *first_object;
extern OBJ_DATA *last_object;
extern CLAN_DATA *first_clan;
extern CLAN_DATA *last_clan;
extern IPK_DATA *first_pk;
extern IPK_DATA *last_pk;
extern COUNCIL_DATA *first_council;
extern COUNCIL_DATA *last_council;
extern DEITY_DATA *first_deity;
extern DEITY_DATA *last_deity;
extern AREA_DATA *first_area;
extern AREA_DATA *last_area;
extern AREA_DATA *first_build;
extern AREA_DATA *last_build;
extern AREA_DATA *first_asort;
extern AREA_DATA *last_asort;
extern AREA_DATA *first_bsort;
extern AREA_DATA *last_bsort;
extern AREA_DATA *first_area_name;  /*alphanum. sort */
extern AREA_DATA *last_area_name;   /* Fireblade */
extern LANG_DATA *first_lang;
extern LANG_DATA *last_lang;
extern TRIVIA_DATA *first_question;
extern TRIVIA_DATA *last_question;

extern TELEPORT_DATA *first_teleport;
extern TELEPORT_DATA *last_teleport;
extern OBJ_DATA *extracted_obj_queue;
extern EXTRACT_CHAR_DATA *extracted_char_queue;
extern OBJ_DATA *save_equipment[MAX_WEAR][MAX_LAYERS];
extern CHAR_DATA *quitting_char;
extern CHAR_DATA *loading_char;
extern CHAR_DATA *saving_char;
extern OBJ_DATA *all_obj;

extern time_t current_time;
extern bool fLogAll;
extern char log_buf[];
extern TIME_INFO_DATA time_info;
extern WEATHER_DATA weather_info;
extern IMMORTAL_HOST *immortal_host_start;
extern IMMORTAL_HOST *immortal_host_end;
extern int weath_unit;
extern int rand_factor;
extern int climate_factor;
extern int neigh_factor;
extern int max_vector;

extern AUCTION_DATA *auction;
extern struct act_prog_data *mob_act_list;

#include "do.h"
/*
 * Spell functions.
 * Defined in magic.c.
 */
DECLARE_SPELL_FUN( spell_null );
DECLARE_SPELL_FUN( spell_notfound );
DECLARE_SPELL_FUN( spell_acid_blast );
DECLARE_SPELL_FUN( spell_animate_dead );
DECLARE_SPELL_FUN( spell_astral_walk );
DECLARE_SPELL_FUN( spell_blindness );
DECLARE_SPELL_FUN( spell_burning_hands );
DECLARE_SPELL_FUN( spell_call_lightning );
DECLARE_SPELL_FUN( spell_custos_angelus );
DECLARE_SPELL_FUN( spell_cause_critical );
DECLARE_SPELL_FUN( spell_cause_light );
DECLARE_SPELL_FUN( spell_cause_serious );
DECLARE_SPELL_FUN( spell_change_sex );
DECLARE_SPELL_FUN( spell_charm_person );
DECLARE_SPELL_FUN( spell_chill_touch );
DECLARE_SPELL_FUN( spell_colour_spray );
DECLARE_SPELL_FUN( spell_control_weather );
DECLARE_SPELL_FUN( spell_create_food );
DECLARE_SPELL_FUN( spell_create_water );
DECLARE_SPELL_FUN( spell_cure_blindness );
DECLARE_SPELL_FUN( spell_cure_poison );
DECLARE_SPELL_FUN( spell_curse );
DECLARE_SPELL_FUN( spell_detect_poison );
DECLARE_SPELL_FUN( spell_dispel_evil );
DECLARE_SPELL_FUN( spell_dispel_magic );
DECLARE_SPELL_FUN( spell_disenchant_weapon );
DECLARE_SPELL_FUN( spell_dream );
DECLARE_SPELL_FUN( spell_earthquake );
DECLARE_SPELL_FUN( spell_enchant_armor );
DECLARE_SPELL_FUN( spell_enchant_weapon );
DECLARE_SPELL_FUN( spell_energy_drain );
DECLARE_SPELL_FUN( spell_familiar );
DECLARE_SPELL_FUN( spell_faerie_fire );
DECLARE_SPELL_FUN( spell_faerie_fog );
DECLARE_SPELL_FUN( spell_fear ); /* fear.c */
DECLARE_SPELL_FUN( spell_farsight );
DECLARE_SPELL_FUN( spell_fireball );
DECLARE_SPELL_FUN( spell_flamestrike );
DECLARE_SPELL_FUN( spell_gate );
DECLARE_SPELL_FUN( spell_knock );
DECLARE_SPELL_FUN( spell_harm );
DECLARE_SPELL_FUN( spell_identify );
DECLARE_SPELL_FUN( spell_inferno );
DECLARE_SPELL_FUN( spell_invis );
DECLARE_SPELL_FUN( spell_know_alignment );
DECLARE_SPELL_FUN( spell_lightning_bolt );
DECLARE_SPELL_FUN( spell_locate_object );
DECLARE_SPELL_FUN( spell_magic_missile );
DECLARE_SPELL_FUN( spell_mist_walk );
DECLARE_SPELL_FUN( spell_pass_door );
DECLARE_SPELL_FUN( spell_plant_pass );
DECLARE_SPELL_FUN( spell_poison );
DECLARE_SPELL_FUN( spell_polymorph );
DECLARE_SPELL_FUN( spell_possess );
DECLARE_SPELL_FUN( spell_recharge );
DECLARE_SPELL_FUN( spell_remove_curse );
DECLARE_SPELL_FUN( spell_remove_invis );
DECLARE_SPELL_FUN( spell_remove_trap );
DECLARE_SPELL_FUN( spell_shocking_grasp );
DECLARE_SPELL_FUN( spell_sleep );
DECLARE_SPELL_FUN( spell_smaug );
DECLARE_SPELL_FUN( spell_solar_flight );
DECLARE_SPELL_FUN( spell_summon );
DECLARE_SPELL_FUN( spell_teleport );
DECLARE_SPELL_FUN( spell_teleport_superior );
DECLARE_SPELL_FUN( spell_ventriloquate );
DECLARE_SPELL_FUN( spell_weaken );
DECLARE_SPELL_FUN( spell_word_of_recall );
DECLARE_SPELL_FUN( spell_acid_breath );
DECLARE_SPELL_FUN( spell_fire_breath );
DECLARE_SPELL_FUN( spell_frost_breath );
DECLARE_SPELL_FUN( spell_gas_breath );
DECLARE_SPELL_FUN( spell_lightning_breath );
DECLARE_SPELL_FUN( spell_spiral_blast );
DECLARE_SPELL_FUN( spell_scorching_surge );
DECLARE_SPELL_FUN( spell_helical_flow );
DECLARE_SPELL_FUN( spell_transport );
DECLARE_SPELL_FUN( spell_portal );
DECLARE_SPELL_FUN( spell_ethereal_fist );
DECLARE_SPELL_FUN( spell_spectral_furor );
DECLARE_SPELL_FUN( spell_hand_of_chaos );
DECLARE_SPELL_FUN( spell_disruption );
DECLARE_SPELL_FUN( spell_sonic_resonance );
DECLARE_SPELL_FUN( spell_mind_wrack );
DECLARE_SPELL_FUN( spell_mind_wrench );
DECLARE_SPELL_FUN( spell_revive );
DECLARE_SPELL_FUN( spell_sulfurous_spray );
DECLARE_SPELL_FUN( spell_caustic_fount );
DECLARE_SPELL_FUN( spell_acetum_primus );
DECLARE_SPELL_FUN( spell_galvanic_whip );
DECLARE_SPELL_FUN( spell_magnetic_thrust );
DECLARE_SPELL_FUN( spell_quantum_spike );
DECLARE_SPELL_FUN( spell_black_hand );
DECLARE_SPELL_FUN( spell_black_fist );
DECLARE_SPELL_FUN( spell_black_lightning );
DECLARE_SPELL_FUN( spell_midas_touch );
DECLARE_SPELL_FUN( spell_bethsaidean_touch );
DECLARE_SPELL_FUN( spell_expurgation );
DECLARE_SPELL_FUN( spell_sacral_divinity );
DECLARE_SPELL_FUN( spell_life_transfer );

/*
char *crypt ( const char *key, const char *salt );
*/

/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so players can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */
#define PLAYER_DIR	"../player/"   /* Player files         */
#define BACKUP_DIR	"../backup/"   /* Backup Player files    */
#define GOD_DIR		"../gods/"  /* God Info Dir         */
#define BOARD_DIR	"../boards/"   /* Board data dir    */
#define CLAN_DIR	"../clans/" /* Clan data dir     */
#define COUNCIL_DIR  	"../councils/" /* Council data dir    */
#define GUILD_DIR       "../guilds/"   /* Guild data dir               */
#define DEITY_DIR	"../deity/" /* Deity data dir    */
#define BUILD_DIR       "../building/" /* Online building save dir     */
#define SYSTEM_DIR	"../system/"   /* Main system files    */
#define PROG_DIR	"../mudprogs/" /* MUDProg files     */
#define CORPSE_DIR	"../corpses/"  /* Corpses        */
#define	CLASS_DIR	"../classes/"  /* Classes        */
#define WATCH_DIR	"../watch/" /* Imm watch files --Gorog      */
#define LOCKER_DIR	"../lockers/"  /* Player Lockers    */

/*
 * The watch directory contains a maximum of one file for each immortal
 * that contains output from "player watches". The name of each file
 * in this directory is the name of the immortal who requested the watch
 */
// #define NULL_FILE	"../system/TEMP.file";
#define AREA_LIST	"area.lst"  /* List of areas     */
#define WATCH_LIST      "watch.lst" /* List of watches              */
#define BAN_LIST        "ban.lst"   /* List of bans                 */
#define RESERVED_LIST	"reserved.lst" /* List of reserved names  */
#define FILTERED_LIST   "filtered.lst" /* List of filtered words */
#define CLAN_LIST	"clan.lst"  /* List of clans     */
#define COUNCIL_LIST	"council.lst"  /* List of councils     */
#define GUILD_LIST      "guild.lst" /* List of guilds               */
#define DEITY_LIST	"deity.lst" /* List of deities      */
#define EMAIL_FILE      SYSTEM_DIR "email.dat"  //Email file for sending out email
#define	CLASS_LIST	"class.lst" /* List of classes      */
#define	RACE_LIST	"race.lst"  /* List of races     */

#define TRIVIA_FILE     "trivia.dat"   /* You've got questions, we've got answers */
#define MORPH_FILE      "morph.dat" /* For morph data */
#define BOARD_FILE	"boards.txt"   /* For bulletin boards   */
#define SHUTDOWN_FILE	"shutdown.txt" /* For 'shutdown'  */
#define IMM_HOST_FILE   SYSTEM_DIR "immortal.host" /* For stoping hackers */
#define LAST_LIST       SYSTEM_DIR "last.lst"   //last list

#define OBJ_FILE		"objects.dat"  /* For storing objects across reboots */
#define MOB_FILE		"mobs.dat"  /* For storing mobs across reboots */
#define FILTER_FILE	"filter.dat"   /* Filters by Garinan      */

#define RIPSCREEN_FILE	SYSTEM_DIR "mudrip.rip"
#define RIPTITLE_FILE	SYSTEM_DIR "mudtitle.rip"
#define ANSITITLE_FILE	SYSTEM_DIR "mudtitle.ans"
#define ASCTITLE_FILE	SYSTEM_DIR "mudtitle.asc"
#define NOHELP_FILE     SYSTEM_DIR "nohelp.txt"
#define BOOTLOG_FILE	SYSTEM_DIR "boot.txt"   /* Boot up error file  */
#define PBUG_FILE	SYSTEM_DIR "pbugs.txt"  /* For 'bug' command   */
#define IDEA_FILE	SYSTEM_DIR "ideas.txt"  /* For 'idea'       */
#define TYPO_FILE	SYSTEM_DIR "typos.txt"  /* For 'typo'       */
#define FIXED_FILE	SYSTEM_DIR "fixed.txt"  /* For 'fixed' command */
#define LOG_FILE	SYSTEM_DIR "log.txt" /* For talking in logged rooms */
#define MOBLOG_FILE	SYSTEM_DIR "moblog.txt" /* For mplog messages  */
#define IMMLIST_FILE	SYSTEM_DIR "IMMLIST" /* Immlist       */
#define IMMWEB_FILE	SYSTEM_DIR "IMMWEB"  /* Immlist       */
#define WIZLIST_FILE	SYSTEM_DIR "WIZLIST" /* Wizlist       */
#define GUESTLIST_FILE  SYSTEM_DIR "GUESTLIST"  /* Guestlist           */
#define WEBWIZ_FILE     SYSTEM_DIR "WEBWIZ"  /* Web Wizlist         */
#define WHO_FILE	SYSTEM_DIR "WHO"  /* Who output file  */
#define WEBWHO_FILE	SYSTEM_DIR "WEBWHO"  /* WWW Who output file */
#define REQUEST_PIPE	SYSTEM_DIR "REQUESTS"   /* Request FIFO  */
#define SKILL_FILE	SYSTEM_DIR "skills.dat" /* Skill table   */
#define HERB_FILE	SYSTEM_DIR "herbs.dat"  /* Herb table       */
#define TONGUE_FILE	SYSTEM_DIR "tongues.dat"   /* Tongue tables    */
#define SOCIAL_FILE	SYSTEM_DIR "socials.dat"   /* Socials       */
#define PKILL_FILE	SYSTEM_DIR "pkills.dat" /* Illegal pkill data  */
#define COMMAND_FILE	SYSTEM_DIR "commands.dat"  /* Commands      */
#define USAGE_FILE	SYSTEM_DIR "usage.txt"  /* How many people are on every half hour - trying to determine best reboot time */
#define ECONOMY_FILE	SYSTEM_DIR "economy.txt"   /* Gold looted, value of used potions/pills  */
#define PROJECTS_FILE	SYSTEM_DIR "projects.txt"  /* For projects  */
#define PLANE_FILE	SYSTEM_DIR "planes.dat" /* For planes       */
#define CLASSDIR	"../classes/"
#define RACEDIR 	"../races/"
#define COPYOVER_FILE	SYSTEM_DIR "copyover.dat"  /* for warm reboots    */
#define EXE_FILE	"../src/rmexe" /* executable path */
#define DT_FILE		SYSTEM_DIR "deathtrap.log" /* Record who died, and what they were wearing -Sadiq */
#define CHANGES_FILE   SYSTEM_DIR "changes.dat"

/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD	CHAR_DATA
#define MID	MOB_INDEX_DATA
#define OD	OBJ_DATA
#define OID	OBJ_INDEX_DATA
#define RID	ROOM_INDEX_DATA
#define SF	SPEC_FUN
#define BD	BOARD_DATA
#define CL	CLAN_DATA
#define EDD	EXTRA_DESCR_DATA
#define RD	RESET_DATA
#define ED	EXIT_DATA
#define	ST	SOCIALTYPE
#define	CO	COUNCIL_DATA
#define DE	DEITY_DATA
#define SK	SKILLTYPE

/* hiscore stuff -- Scion */
#define HISCORE_CODE
char *is_hiscore_obj args( ( OBJ_DATA * obj ) );
void show_hiscore args( ( char *keyword, CHAR_DATA * ch ) );
void adjust_hiscore args( ( char *keyword, CHAR_DATA * ch, int score ) );
void save_hiscores args( ( void ) );
void load_hiscores args( ( void ) );
void register_hskelter_pkill( CHAR_DATA * ch );

/* act_comm.c */
bool circle_follow args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void add_follower args( ( CHAR_DATA * ch, CHAR_DATA * master ) );
void stop_follower args( ( CHAR_DATA * ch ) );
void die_follower args( ( CHAR_DATA * ch ) );
bool is_same_group args( ( CHAR_DATA * ach, CHAR_DATA * bch ) );
void send_rip_screen args( ( CHAR_DATA * ch ) );
void send_rip_title args( ( CHAR_DATA * ch ) );
void send_ansi_title args( ( CHAR_DATA * ch ) );
void send_ascii_title args( ( CHAR_DATA * ch ) );
void to_channel( const char *argument, char *xchannel, int level );
void talk_auction args( ( char *argument ) );
int knows_language( CHAR_DATA * ch, int language );
bool can_learn_lang args( ( CHAR_DATA * ch, int language ) );
int countlangs args( ( int languages ) );
char *translate args( ( int percent, const char *in, const char *name ) );
char *obj_short args( ( OBJ_DATA * obj ) );

/* act_info.c */
char *num_punct args( ( int foo ) );
int get_door args( ( char *arg ) );
char *format_obj_to_char args( ( OBJ_DATA * obj, CHAR_DATA * ch, bool fShort ) );
void show_list_to_char args( ( OBJ_DATA * list, CHAR_DATA * ch, bool fShort, bool fShowNothing ) );
bool is_ignoring args( ( CHAR_DATA * ch, CHAR_DATA * ign_ch ) );
void show_race_line args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void record_death_trap args( ( CHAR_DATA * ch ) );

/* act_move.c */
void clear_vrooms args( ( void ) );
ED *find_door args( ( CHAR_DATA * ch, char *arg, bool quiet ) );
ED *get_exit args( ( ROOM_INDEX_DATA * room, short dir ) );
ED *get_exit_to args( ( ROOM_INDEX_DATA * room, short dir, int vnum ) );
ED *get_exit_num args( ( ROOM_INDEX_DATA * room, short count ) );
ch_ret move_char args( ( CHAR_DATA * ch, EXIT_DATA * pexit, int fall ) );
void teleport args( ( CHAR_DATA * ch, int room, int flags ) );
short encumbrance args( ( CHAR_DATA * ch, short move ) );
bool will_fall args( ( CHAR_DATA * ch, int fall ) );
ch_ret pullcheck args( ( CHAR_DATA * ch, int pulse ) );
char *rev_exit args( ( short vdir ) );

/* act_obj.c */
obj_ret damage_obj args( ( OBJ_DATA * obj ) );
short get_obj_resistance args( ( OBJ_DATA * obj ) );
void save_clan_storeroom args( ( CHAR_DATA * ch, CLAN_DATA * clan ) );
void obj_fall args( ( OBJ_DATA * obj, bool through ) );
bool is_class args( ( CHAR_DATA * ch, int class ) );

/* act_wiz.c */
RID *find_location args( ( CHAR_DATA * ch, char *arg ) );
void echo_to_all args( ( short AT_COLOR, char *argument, short tar ) );
void get_reboot_string args( ( void ) );
struct tm *update_time args( ( struct tm * old_time ) );
void free_social args( ( SOCIALTYPE * social ) );
void add_social args( ( SOCIALTYPE * social ) );
void free_command args( ( CMDTYPE * command ) );
void unlink_command args( ( CMDTYPE * command ) );
void add_command args( ( CMDTYPE * command ) );

/* bits.c */
BIT_DATA *find_abit( int number );
BIT_DATA *find_qbit( int number );
BIT_DATA *get_abit( CHAR_DATA * ch, int number );
BIT_DATA *get_qbit( CHAR_DATA * ch, int number );
void load_bits( void );
void save_bits( void );
void set_abit( CHAR_DATA * ch, int number );
void set_qbit( CHAR_DATA * ch, int number );
void remove_abit( CHAR_DATA * ch, int number );
void remove_qbit( CHAR_DATA * ch, int number );

/* boards.c */
void load_boards args( ( void ) );
BD *get_board args( ( OBJ_DATA * obj ) );
void free_note args( ( NOTE_DATA * pnote ) );

/* gboards.c */
void free_global_note args( ( NOTE_DATA * note ) );

/* build.c */
void RelCreate( relation_type, void *, void * );
void RelDestroy( relation_type, void *, void * );
int get_cmdflag args( ( char *flag ) );
char *flag_string args( ( int bitvector, char *const flagarray[] ) );
char *ext_flag_string args( ( EXT_BV * bitvector, char *const flagarray[] ) );
int get_mpflag args( ( char *flag ) );
int get_dir args( ( char *txt ) );
char *strip_cr args( ( char *str ) );

/* clans.c */
FILE *fpReserve;
CL *get_clan args( ( char *name ) );
void load_clans args( ( void ) );
void save_clan args( ( CLAN_DATA * clan ) );

CO *get_council args( ( char *name ) );
void load_councils args( ( void ) );
void save_council args( ( COUNCIL_DATA * council ) );

/* deity.c */
DE *get_deity args( ( char *name ) );
void load_deity args( ( void ) );
void save_deity args( ( DEITY_DATA * deity ) );

/* comm.c */
bool check_parse_name( char *name, bool newchar );
char *color_align( char *argument, int size, int align );
const char *const_color_align( const char *argument, int size, int align );
int color_str_len( char *argument );
int const_color_str_len( const char *argument );
int color_strnlen( char *argument, int maxlength );
int const_color_strnlen( const char *argument, int maxlength );
void close_socket args( ( DESCRIPTOR_DATA * dclose, bool force ) );
void write_to_buffer( DESCRIPTOR_DATA * d, const char *txt, size_t length );
void write_to_pager( DESCRIPTOR_DATA * d, const char *txt, size_t length );
void send_to_char args( ( const char *txt, CHAR_DATA * ch ) );
void send_to_char_color args( ( const char *txt, CHAR_DATA * ch ) );
void send_to_pager args( ( const char *txt, CHAR_DATA * ch ) );
void send_to_pager_color args( ( const char *txt, CHAR_DATA * ch ) );
void set_char_color args( ( short AType, CHAR_DATA * ch ) );
void set_pager_color args( ( short AType, CHAR_DATA * ch ) );
void ch_printf( CHAR_DATA * ch, const char *fmt, ... ) __attribute__ ( ( format( printf, 2, 3 ) ) );
void pager_printf( CHAR_DATA * ch, const char *fmt, ... ) __attribute__ ( ( format( printf, 2, 3 ) ) );
void ch_printf_color( CHAR_DATA * ch, const char *fmt, ... ) __attribute__ ( ( format( printf, 2, 3 ) ) );
void pager_printf_color( CHAR_DATA * ch, const char *fmt, ... ) __attribute__ ( ( format( printf, 2, 3 ) ) );
void act args( ( short AType, const char *format, CHAR_DATA * ch, const void *arg1, const void *arg2, int type ) );
void log_printf( const char *fmt, ... ) __attribute__ ( ( format( printf, 1, 2 ) ) );
void copyover_recover args( ( void ) );

char *myobj args( ( OBJ_DATA * obj ) );

/* reset.c */
RD *make_reset args( ( char letter, int extra, int arg1, int arg2, int arg3 ) );
RD *add_reset( ROOM_INDEX_DATA * room, char letter, int extra, int arg1, int arg2, int arg3 );
void reset_area args( ( AREA_DATA * pArea ) );

/* db.c */
void show_file args( ( CHAR_DATA * ch, char *filename ) );
char *str_dup args( ( char const *str ) );
void boot_db args( ( bool fCopyOver ) );
void area_update args( ( void ) );
void add_char args( ( CHAR_DATA * ch ) );
CD *create_mobile args( ( MOB_INDEX_DATA * pMobIndex ) );
OD *create_object args( ( OBJ_INDEX_DATA * pObjIndex, int level ) );
void clear_char args( ( CHAR_DATA * ch ) );
void free_char args( ( CHAR_DATA * ch ) );
char *get_extra_descr args( ( const char *name, EXTRA_DESCR_DATA * ed ) );
MID *get_mob_index args( ( int vnum ) );
OID *get_obj_index args( ( int vnum ) );
RID *get_room_index args( ( int vnum ) );
char fread_letter args( ( FILE * fp ) );
int fread_number args( ( FILE * fp ) );
EXT_BV fread_bitvector args( ( FILE * fp ) );
void fwrite_bitvector args( ( EXT_BV * bits, FILE * fp ) );
char *print_bitvector args( ( EXT_BV * bits ) );
char *fread_string args( ( FILE * fp ) );
char *fread_flagstring( FILE * fp );
char *fread_string_nohash args( ( FILE * fp ) );
void fread_to_eol args( ( FILE * fp ) );
char *fread_word args( ( FILE * fp ) );
char *fread_line args( ( FILE * fp ) );
int number_fuzzy args( ( int number ) );
int number_range args( ( int from, int to ) );
int number_percent args( ( void ) );
int number_door args( ( void ) );
int number_bits args( ( int width ) );
int number_mm args( ( void ) );
int dice args( ( int number, int size ) );
int interpolate args( ( int level, int value_00, int value_32 ) );
void smash_tilde args( ( char *str ) );
void hide_tilde args( ( char *str ) );
char *show_tilde args( ( char *str ) );
bool str_cmp args( ( const char *astr, const char *bstr ) );
bool str_prefix args( ( const char *astr, const char *bstr ) );
bool str_infix args( ( const char *astr, const char *bstr ) );
bool str_suffix args( ( const char *astr, const char *bstr ) );
char *capitalize args( ( const char *str ) );
char *strlower args( ( const char *str ) );
char *strupper args( ( const char *str ) );
char *aoran args( ( const char *str ) );
void append_file args( ( CHAR_DATA * ch, char *file, char *str ) );
void append_to_file args( ( char *file, char *str ) );
void bug args( ( const char *str, ... ) );
void log_string_plus args( ( const char *str, short log_type, short level ) );
RID *make_room( int vnum, AREA_DATA * area );
OID *make_object args( ( int vnum, int cvnum, char *name ) );
MID *make_mobile args( ( int vnum, int cvnum, char *name ) );
ED *make_exit args( ( ROOM_INDEX_DATA * pRoomIndex, ROOM_INDEX_DATA * to_room, short door ) );
void add_help args( ( HELP_DATA * pHelp ) );
void fix_area_exits args( ( AREA_DATA * tarea ) );
void load_area_file args( ( AREA_DATA * tarea, char *filename ) );
void randomize_exits args( ( ROOM_INDEX_DATA * room, short maxdir ) );
void make_webwiz args( ( void ) );
void make_wizlist args( ( void ) );
void make_guestlist args( ( void ) );
void tail_chain args( ( void ) );
void delete_room( ROOM_INDEX_DATA * room );
void delete_obj( OBJ_INDEX_DATA * obj );
void delete_mob( MOB_INDEX_DATA * mob );
void sort_area args( ( AREA_DATA * pArea, bool proto ) );
void sort_area_by_name args( ( AREA_DATA * pArea ) ); /* Fireblade */
void write_projects args( ( void ) );

/* build.c */
void start_editing args( ( CHAR_DATA * ch, char *data ) );
void stop_editing args( ( CHAR_DATA * ch ) );
void edit_buffer args( ( CHAR_DATA * ch, char *argument ) );
char *copy_buffer args( ( CHAR_DATA * ch ) );
char *copy_buffer_nohash( CHAR_DATA * ch );
bool can_rmodify args( ( CHAR_DATA * ch, ROOM_INDEX_DATA * room ) );
bool can_omodify args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );
bool can_mmodify args( ( CHAR_DATA * ch, CHAR_DATA * mob ) );
bool can_medit args( ( CHAR_DATA * ch, MOB_INDEX_DATA * mob ) );
void free_reset args( ( AREA_DATA * are, RESET_DATA * res ) );
void free_area args( ( AREA_DATA * are ) );
void assign_area args( ( CHAR_DATA * ch ) );
EDD *SetRExtra args( ( ROOM_INDEX_DATA * room, char *keywords ) );
bool DelRExtra args( ( ROOM_INDEX_DATA * room, char *keywords ) );
EDD *SetOExtra args( ( OBJ_DATA * obj, char *keywords ) );
bool DelOExtra args( ( OBJ_DATA * obj, char *keywords ) );
EDD *SetOExtraProto args( ( OBJ_INDEX_DATA * obj, char *keywords ) );
bool DelOExtraProto args( ( OBJ_INDEX_DATA * obj, char *keywords ) );
void fold_area args( ( AREA_DATA * tarea, char *filename, bool install ) );
int get_otype args( ( char *type ) );
int get_atype args( ( char *type ) );
int get_aflag args( ( char *flag ) );
int get_oflag args( ( char *flag ) );
int get_wflag args( ( char *flag ) );
void init_area_weather args( ( void ) );
void save_weatherdata args( ( void ) );

/* fight.c */
void free_pkdata args( ( IPK_DATA * ipk ) );
void save_pkdata args( ( void ) );
void load_pkdata args( ( void ) );
void fread_pkdata args( ( FILE * fp ) );
int max_fight( void );
void violence_update args( ( void ) );
ch_ret multi_hit args( ( CHAR_DATA * ch, CHAR_DATA * victim, int dt ) );
ch_ret projectile_hit args( ( CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * wield, OBJ_DATA * projectile, short dist ) );
short ris_damage args( ( CHAR_DATA * ch, short dam, int ris ) );
ch_ret damage args( ( CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt ) );
void update_pos args( ( CHAR_DATA * victim ) );
void set_fighting args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void stop_fighting args( ( CHAR_DATA * ch, bool fBoth ) );
void free_fight args( ( CHAR_DATA * ch ) );
CD *who_fighting args( ( CHAR_DATA * ch ) );
void check_killer args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void check_attacker args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void death_cry args( ( CHAR_DATA * ch ) );
void stop_hunting args( ( CHAR_DATA * ch ) );
void stop_hating args( ( CHAR_DATA * ch ) );
void stop_fearing args( ( CHAR_DATA * ch ) );
void start_hunting args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void start_hating args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void start_fearing args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool is_hunting args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool is_hating args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool is_fearing args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool is_safe args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool legal_loot args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
short VAMP_AC args( ( CHAR_DATA * ch ) );
bool check_illegal_pk args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void raw_kill args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool in_arena args( ( CHAR_DATA * ch ) );
bool can_astral args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
int get_hitroll args( ( CHAR_DATA * ch, bool real ) );
int get_damroll args( ( CHAR_DATA * ch, bool real ) );

/* makeobjs.c */
void make_corpse args( ( CHAR_DATA * ch, CHAR_DATA * killer ) );
void make_blood args( ( CHAR_DATA * ch ) );
void make_bloodstain args( ( CHAR_DATA * ch ) );
void make_scraps args( ( OBJ_DATA * obj ) );
void make_fire args( ( ROOM_INDEX_DATA * in_room, short timer ) );
OD *make_trap args( ( int v0, int v1, int v2, int v3 ) );
OD *create_money args( ( int amount, int type ) );

/* misc.c */
void actiondesc( CHAR_DATA * ch, OBJ_DATA * obj );
EXT_BV meb( int bit );
EXT_BV multimeb( int bit, ... );

/* deity.c */
void adjust_favor args( ( CHAR_DATA * ch, int field, int mod ) );

/* mud_comm.c */
char *mprog_type_to_name args( ( int type ) );

/* mud_prog.c */
void mprog_wordlist_check args( ( char *arg, CHAR_DATA * mob, CHAR_DATA * actor, OBJ_DATA * object, void *vo, int type ) );
void mprog_percent_check args( ( CHAR_DATA * mob, CHAR_DATA * actor, OBJ_DATA * object, void *vo, int type ) );
void mprog_act_trigger args( ( char *buf, CHAR_DATA * mob, CHAR_DATA * ch, OBJ_DATA * obj, void *vo ) );
void mprog_bribe_trigger args( ( CHAR_DATA * mob, CHAR_DATA * ch, int amount ) );
void mprog_bribe_trigger_silver args( ( CHAR_DATA * mob, CHAR_DATA * ch, int amount ) );
void mprog_bribe_trigger_copper args( ( CHAR_DATA * mob, CHAR_DATA * ch, int amount ) );
void mprog_entry_trigger args( ( CHAR_DATA * mob ) );
void mprog_give_trigger args( ( CHAR_DATA * mob, CHAR_DATA * ch, OBJ_DATA * obj ) );
void mprog_greet_trigger args( ( CHAR_DATA * mob ) );
void mprog_fight_trigger args( ( CHAR_DATA * mob, CHAR_DATA * ch ) );
void mprog_hitprcnt_trigger args( ( CHAR_DATA * mob, CHAR_DATA * ch ) );
void mprog_death_trigger args( ( CHAR_DATA * killer, CHAR_DATA * mob ) );
void mprog_random_trigger args( ( CHAR_DATA * mob ) );
void mprog_speech_trigger args( ( char *txt, CHAR_DATA * mob ) );
void mprog_script_trigger args( ( CHAR_DATA * mob ) );
void mprog_hour_trigger args( ( CHAR_DATA * mob ) );
void mprog_time_trigger args( ( CHAR_DATA * mob ) );
void progbug args( ( char *str, CHAR_DATA * mob ) );
void rset_supermob args( ( ROOM_INDEX_DATA * room ) );
void release_supermob args( (  ) );

/* planes.c */
PLANE_DATA *plane_lookup args( ( const char *name ) );
void load_planes args( ( void ) );
void save_planes args( ( void ) );
void check_planes args( ( PLANE_DATA * p ) );

/* player.c */
void set_title args( ( CHAR_DATA * ch, char *title ) );

/* polymorph.c */
void fwrite_morph_data args( ( CHAR_DATA * ch, FILE * fp ) );
void fread_morph_data args( ( CHAR_DATA * ch, FILE * fp ) );
void clear_char_morph args( ( CHAR_MORPH * morph ) );
CHAR_MORPH *make_char_morph args( ( MORPH_DATA * morph ) );
void free_char_morph args( ( CHAR_MORPH * morph ) );
char *race_string args( ( int bitvector ) );
char *class_string args( ( int bitvector ) );
void setup_morph_vnum args( ( void ) );
void unmorph_all args( ( MORPH_DATA * morph ) );
MORPH_DATA *get_morph args( ( char *arg ) );
MORPH_DATA *get_morph_vnum args( ( int arg ) );
int do_morph_char args( ( CHAR_DATA * ch, MORPH_DATA * morph ) );
MORPH_DATA *find_morph args( ( CHAR_DATA * ch, char *target, bool is_cast ) );
void do_unmorph_char args( ( CHAR_DATA * ch ) );
void send_morph_message args( ( CHAR_DATA * ch, MORPH_DATA * morph, bool is_morph ) );
bool can_morph args( ( CHAR_DATA * ch, MORPH_DATA * morph, bool is_cast ) );
void do_morph args( ( CHAR_DATA * ch, MORPH_DATA * morph ) );
void do_unmorph args( ( CHAR_DATA * ch ) );
void save_morphs args( ( void ) );
void fwrite_morph args( ( FILE * fp, MORPH_DATA * morph ) );
void load_morphs args( ( void ) );
MORPH_DATA *fread_morph args( ( FILE * fp ) );
void free_morph args( ( MORPH_DATA * morph ) );
void morph_defaults args( ( MORPH_DATA * morph ) );
void sort_morphs args( ( void ) );

/* skills.c */
bool can_use_skill args( ( CHAR_DATA * ch, int percent, int gsn ) );
bool check_skill args( ( CHAR_DATA * ch, char *command, char *argument ) );
void learn_from_success args( ( CHAR_DATA * ch, int sn ) );
void learn_from_failure args( ( CHAR_DATA * ch, int sn ) );
bool check_parry args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool check_dodge args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool check_tumble args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool check_grip args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void disarm args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void trip args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool mob_fire args( ( CHAR_DATA * ch, char *name ) );
CD *scan_for_victim args( ( CHAR_DATA * ch, EXIT_DATA * pexit, char *name ) );

void imud_recv_message(  );

/* ban.c */
int add_ban args( ( CHAR_DATA * ch, char *arg1, char *arg2, int btime, int type ) );
void show_bans args( ( CHAR_DATA * ch, int type ) );
void save_banlist args( ( void ) );
void load_banlist args( ( void ) );
bool check_total_bans args( ( DESCRIPTOR_DATA * d ) );
bool check_bans args( ( CHAR_DATA * ch, int type ) );

/* imm_host.c */
bool check_immortal_domain args( ( CHAR_DATA * ch, char *host ) );
int load_imm_host args( ( void ) );
int fread_imm_host args( ( FILE * fp, IMMORTAL_HOST * data ) );
void do_write_imm_host args( ( void ) );

/* handler.c */
int get_exp args( ( CHAR_DATA * ch ) );
int get_exp_worth args( ( CHAR_DATA * ch ) );
int exp_level args( ( CHAR_DATA * ch, short level ) );
short get_trust args( ( CHAR_DATA * ch ) );
short get_age args( ( CHAR_DATA * ch ) );
short get_curr_str args( ( CHAR_DATA * ch ) );
short get_curr_int args( ( CHAR_DATA * ch ) );
short get_curr_wis args( ( CHAR_DATA * ch ) );
short get_curr_dex args( ( CHAR_DATA * ch ) );
short get_curr_con args( ( CHAR_DATA * ch ) );
short get_curr_cha args( ( CHAR_DATA * ch ) );
short get_curr_lck args( ( CHAR_DATA * ch ) );
bool can_take_proto args( ( CHAR_DATA * ch ) );
int can_carry_n args( ( CHAR_DATA * ch ) );
int can_carry_w args( ( CHAR_DATA * ch ) );
bool is_name args( ( const char *str, char *namelist ) );
bool is_name_prefix args( ( const char *str, char *namelist ) );
bool nifty_is_name args( ( char *str, char *namelist ) );
bool nifty_is_name_prefix args( ( char *str, char *namelist ) );
void affect_modify args( ( CHAR_DATA * ch, AFFECT_DATA * paf, bool fAdd ) );
void affect_to_char args( ( CHAR_DATA * ch, AFFECT_DATA * paf ) );
void affect_remove args( ( CHAR_DATA * ch, AFFECT_DATA * paf ) );
void affect_strip args( ( CHAR_DATA * ch, int sn ) );
bool is_affected args( ( CHAR_DATA * ch, int sn ) );
void affect_join args( ( CHAR_DATA * ch, AFFECT_DATA * paf ) );
void char_from_room args( ( CHAR_DATA * ch ) );
void char_to_room args( ( CHAR_DATA * ch, ROOM_INDEX_DATA * pRoomIndex ) );
OD *obj_to_char args( ( OBJ_DATA * obj, CHAR_DATA * ch ) );
void obj_from_char args( ( OBJ_DATA * obj ) );
int apply_ac args( ( OBJ_DATA * obj, int iWear ) );
OD *get_eq_char args( ( CHAR_DATA * ch, int iWear ) );
void equip_char args( ( CHAR_DATA * ch, OBJ_DATA * obj, int iWear ) );
void unequip_char args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );
int count_obj_list args( ( OBJ_INDEX_DATA * obj, OBJ_DATA * list ) );
void obj_from_room args( ( OBJ_DATA * obj ) );
OD *obj_to_room args( ( OBJ_DATA * obj, ROOM_INDEX_DATA * pRoomIndex ) );
OD *obj_to_obj args( ( OBJ_DATA * obj, OBJ_DATA * obj_to ) );
void obj_from_obj args( ( OBJ_DATA * obj ) );
void extract_obj args( ( OBJ_DATA * obj ) );
void extract_exit args( ( ROOM_INDEX_DATA * room, EXIT_DATA * pexit ) );
void clean_room args( ( ROOM_INDEX_DATA * room ) );
void clean_obj args( ( OBJ_INDEX_DATA * obj ) );
void clean_mob args( ( MOB_INDEX_DATA * mob ) );
void clean_resets( ROOM_INDEX_DATA * room );
void extract_char args( ( CHAR_DATA * ch, bool fPull ) );
char *fur_pos_string args( ( int pos ) ); /* furniture */
char *pos_string args( ( CHAR_DATA * victim ) );   /* furniture */
int count_users args( ( OBJ_DATA * obj ) );  /* furniture */
CD *get_char_room args( ( CHAR_DATA * ch, char *argument ) );
CD *get_char_world args( ( CHAR_DATA * ch, char *argument ) );
OD *get_obj_type args( ( OBJ_INDEX_DATA * pObjIndexData ) );
OD *get_obj_list args( ( CHAR_DATA * ch, char *argument, OBJ_DATA * list ) );
OD *get_obj_list_rev args( ( CHAR_DATA * ch, char *argument, OBJ_DATA * list ) );
OD *get_obj_carry args( ( CHAR_DATA * ch, char *argument ) );
OD *get_obj_wear args( ( CHAR_DATA * ch, char *argument ) );
OD *get_obj_vnum args( ( CHAR_DATA * ch, int vnum ) );
OD *get_obj_here args( ( CHAR_DATA * ch, char *argument ) );
OD *get_obj_world args( ( CHAR_DATA * ch, char *argument ) );
int get_obj_number args( ( OBJ_DATA * obj ) );
int get_obj_weight args( ( OBJ_DATA * obj ) );
int get_real_obj_weight args( ( OBJ_DATA * obj ) );
bool room_is_dark args( ( ROOM_INDEX_DATA * pRoomIndex ) );
bool room_is_private args( ( ROOM_INDEX_DATA * pRoomIndex ) );
bool can_see args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool can_see_obj args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );
bool can_drop_obj args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );
char *item_type_name args( ( OBJ_DATA * obj ) );
char *affect_loc_name args( ( int location ) );
char *affect_bit_name args( ( EXT_BV * vector ) );
char *extra_bit_name args( ( EXT_BV * extra_flags ) );
char *magic_bit_name args( ( int magic_flags ) );
char *pull_type_name args( ( int pulltype ) );
ch_ret check_for_trap args( ( CHAR_DATA * ch, OBJ_DATA * obj, int flag ) );
ch_ret check_room_for_traps args( ( CHAR_DATA * ch, int flag ) );
bool is_trapped args( ( OBJ_DATA * obj ) );
OD *get_trap args( ( OBJ_DATA * obj ) );
ch_ret spring_trap args( ( CHAR_DATA * ch, OBJ_DATA * obj ) );
void name_stamp_stats args( ( CHAR_DATA * ch ) );
void fix_char args( ( CHAR_DATA * ch ) );
void showaffect args( ( CHAR_DATA * ch, AFFECT_DATA * paf ) );
void set_cur_obj args( ( OBJ_DATA * obj ) );
bool obj_extracted args( ( OBJ_DATA * obj ) );
void queue_extracted_obj args( ( OBJ_DATA * obj ) );
void clean_obj_queue args( ( void ) );
void set_cur_char args( ( CHAR_DATA * ch ) );
bool char_died args( ( CHAR_DATA * ch ) );
void queue_extracted_char args( ( CHAR_DATA * ch, bool extract ) );
void clean_char_queue args( ( void ) );
void add_timer args( ( CHAR_DATA * ch, short type, short count, DO_FUN * fun, int value ) );
TIMER *get_timerptr args( ( CHAR_DATA * ch, short type ) );
short get_timer args( ( CHAR_DATA * ch, short type ) );
void extract_timer args( ( CHAR_DATA * ch, TIMER * timer ) );
void remove_timer args( ( CHAR_DATA * ch, short type ) );
bool in_soft_range args( ( CHAR_DATA * ch, AREA_DATA * tarea ) );
bool in_hard_range args( ( CHAR_DATA * ch, AREA_DATA * tarea ) );
bool chance args( ( CHAR_DATA * ch, short percent ) );
bool chance_attrib args( ( CHAR_DATA * ch, short percent, short attrib ) );
OD *clone_object args( ( OBJ_DATA * obj ) );
void split_obj args( ( OBJ_DATA * obj, int num ) );
void separate_obj args( ( OBJ_DATA * obj ) );
bool empty_obj args( ( OBJ_DATA * obj, OBJ_DATA * destobj, ROOM_INDEX_DATA * destroom ) );
OD *find_obj args( ( CHAR_DATA * ch, char *argument, bool carryonly ) );
bool ms_find_obj args( ( CHAR_DATA * ch ) );
void worsen_mental_state args( ( CHAR_DATA * ch, int mod ) );
void better_mental_state args( ( CHAR_DATA * ch, int mod ) );
void boost_economy args( ( AREA_DATA * tarea, long int gold ) );
void lower_economy args( ( AREA_DATA * tarea, long int gold ) );
void economize_mobgold args( ( CHAR_DATA * mob ) );
bool economy_has args( ( AREA_DATA * tarea, long int gold ) );
void add_kill args( ( CHAR_DATA * ch, CHAR_DATA * mob ) );
int times_killed args( ( CHAR_DATA * ch, CHAR_DATA * mob ) );
void update_aris args( ( CHAR_DATA * ch ) );
AREA_DATA *get_area args( ( char *name ) );  /* FB */
OD *get_objtype args( ( CHAR_DATA * ch, short type ) );

/* interp.c */
bool check_pos args( ( CHAR_DATA * ch, short position ) );
void interpret args( ( CHAR_DATA * ch, char *argument ) );
bool is_number args( ( char *arg ) );
int number_argument args( ( char *argument, char *arg ) );
char *one_argument args( ( char *argument, char *arg_first ) );
char *one_argument2 args( ( char *argument, char *arg_first ) );
ST *find_social args( ( char *command ) );
CMDTYPE *find_command args( ( char *command ) );
void hash_commands args( (  ) );
void start_timer args( ( struct timeval * sttime ) );
time_t end_timer args( ( struct timeval * sttime ) );
void send_timer args( ( struct timerset * vtime, CHAR_DATA * ch ) );
void update_userec args( ( struct timeval * time_used, struct timerset * userec ) );
void no_find args( ( CHAR_DATA * ch ) );

/* magic.c */
bool process_spell_components args( ( CHAR_DATA * ch, int sn ) );
int ch_slookup args( ( CHAR_DATA * ch, const char *name ) );
int find_spell args( ( CHAR_DATA * ch, const char *name, bool know ) );
int find_skill args( ( CHAR_DATA * ch, const char *name, bool know ) );
int find_weapon args( ( CHAR_DATA * ch, const char *name, bool know ) );
int find_tongue args( ( CHAR_DATA * ch, const char *name, bool know ) );
int skill_lookup args( ( const char *name ) );
int herb_lookup args( ( const char *name ) );
int personal_lookup args( ( CHAR_DATA * ch, const char *name ) );
int slot_lookup args( ( int slot ) );
int bsearch_skill args( ( const char *name, int first, int top ) );
int bsearch_skill_exact args( ( const char *name, int first, int top ) );
int bsearch_skill_prefix args( ( const char *name, int first, int top ) );
bool saves_poison_death args( ( int level, CHAR_DATA * victim ) );
bool saves_wand args( ( int level, CHAR_DATA * victim ) );
bool saves_para_petri args( ( int level, CHAR_DATA * victim ) );
bool saves_breath args( ( int level, CHAR_DATA * victim ) );
bool saves_spell_staff args( ( int level, CHAR_DATA * victim ) );
ch_ret obj_cast_spell args( ( int sn, int level, CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj ) );
int dice_parse args( ( CHAR_DATA * ch, int level, char *exp2 ) );
SK *get_skilltype args( ( int sn ) );

/* request.c */
void init_request_pipe args( ( void ) );
void check_requests args( ( void ) );

/* save.c */
/* object saving defines for fread/write_obj. -- Altrag */
#define OS_CARRY	0
#define OS_CORPSE	1
#define OS_GROUND	2
#define OS_LOCKER	3

void save_char_obj args( ( CHAR_DATA * ch ) );
bool load_char_obj args( ( DESCRIPTOR_DATA * d, char *name, bool preload ) );
void set_alarm args( ( long seconds ) );
void requip_char args( ( CHAR_DATA * ch ) );
void fwrite_obj args( ( CHAR_DATA * ch, OBJ_DATA * obj, FILE * fp, int iNest, short os_type ) );
void fread_obj args( ( CHAR_DATA * ch, FILE * fp, short os_type ) );
void de_equip_char args( ( CHAR_DATA * ch ) );
void re_equip_char args( ( CHAR_DATA * ch ) );
void read_char_mobile args( ( char *argument ) );
void write_char_mobile args( ( CHAR_DATA * ch, char *argument ) );
CHAR_DATA *fread_mobile args( ( FILE * fp ) );
void fwrite_mobile args( ( FILE * fp, CHAR_DATA * mob ) );

/* shops.c */

/* special.c */
SF *spec_lookup args( ( const char *name ) );
char *lookup_spec args( ( SPEC_FUN * special ) );

/* tables.c */
int get_skill args( ( char *skilltype ) );
char *spell_name args( ( SPELL_FUN * spell ) );
char *skill_name args( ( DO_FUN * skill ) );
void load_skill_table args( ( void ) );
void save_skill_table args( ( void ) );
void sort_skill_table args( ( void ) );
void remap_slot_numbers args( ( void ) );
void load_socials args( ( void ) );
void save_socials args( ( void ) );
void load_commands args( ( void ) );
void save_commands args( ( void ) );
SPELL_FUN *spell_function args( ( char *name ) );
DO_FUN *skill_function args( ( char *name ) );
void write_class_file args( ( int cl ) );
void save_classes args( ( void ) );
void load_classes args( ( void ) );
void load_herb_table args( ( void ) );
void save_herb_table args( ( void ) );
void load_races args( ( void ) );
void load_tongues args( ( void ) );
int MAX_PC_RACE;
void read_last_file args( ( CHAR_DATA * ch, int count, char *name ) );
void write_last_file args( ( char *entry ) );

/* track.c */
void found_prey args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void hunt_victim args( ( CHAR_DATA * ch ) );

/* update.c */
void advance_level args( ( CHAR_DATA * ch, bool level ) );
void gain_exp args( ( CHAR_DATA * ch, int gain ) );
void gain_condition args( ( CHAR_DATA * ch, int iCond, int value ) );
void update_handler args( ( void ) );
void copyover_check args( ( time_t reset ) );
void reboot_check args( ( time_t reset ) );
void auction_update args( ( void ) );
void remove_portal args( ( OBJ_DATA * portal ) );
void weather_update args( ( void ) );
void bet_update args( ( void ) );

#ifdef MCCP
/* mccp.c */
bool compressStart args( ( DESCRIPTOR_DATA * desc ) );
bool compressEnd args( ( DESCRIPTOR_DATA * desc ) );
bool processCompressed args( ( DESCRIPTOR_DATA * desc ) );
bool writeCompressed args( ( DESCRIPTOR_DATA * desc, char *txt, int length ) );
#endif

/* hashstr.c */
char *str_alloc args( ( char *str ) );
char *quick_link args( ( char *str ) );
int str_free args( ( char *str ) );
void show_hash args( ( int count ) );
char *hash_stats args( ( void ) );
char *check_hash args( ( char *str ) );
void hash_dump args( ( int hash ) );
void show_high_hash args( ( int top ) );

/* newscore.c */
char *get_class args( ( CHAR_DATA * ch ) );
char *get_class2 args( ( CHAR_DATA * ch ) );
char *get_class3 args( ( CHAR_DATA * ch ) );
char *get_race args( ( CHAR_DATA * ch ) );

/* olc stuff (o/m/redit.c) */
bool is_inolc args( ( DESCRIPTOR_DATA * d ) );

#undef	SK
#undef	CO
#undef	ST
#undef	CD
#undef	MID
#undef	OD
#undef	OID
#undef	RID
#undef	SF
#undef	BD
#undef	CL
#undef	EDD
#undef	RD
#undef	ED

/*
 * defines for use with this get_affect function
 */

#define RIS_000		BV00
#define RIS_R00		BV01
#define RIS_0I0		BV02
#define RIS_RI0		BV03
#define RIS_00S		BV04
#define RIS_R0S		BV05
#define RIS_0IS		BV06
#define RIS_RIS		BV07

#define GA_AFFECTED	BV09
#define GA_RESISTANT	BV10
#define GA_IMMUNE	BV11
#define GA_SUSCEPTIBLE	BV12
#define GA_RIS          BV30

/* furniture flags */
#define STAND_AT		BV00
#define STAND_ON		BV01
#define STAND_IN		BV02
#define SIT_AT			BV03
#define SIT_ON			BV04
#define SIT_IN			BV05
#define REST_AT			BV06
#define REST_ON			BV07
#define REST_IN			BV08
#define SLEEP_AT		BV09
#define SLEEP_ON		BV10
#define SLEEP_IN		BV11
#define PUT_AT			BV12
#define PUT_ON			BV13
#define PUT_IN			BV14
#define PUT_INSIDE		BV15

#define FURNITURE_UNUSED	-1
#define FURNITURE_NONE		0
#define ST_AT			BV00  /* Stand at */
#define ST_ON			BV01  /* Stand on */
#define ST_IN			BV02  /* Stand in */
#define SI_AT			BV03  /* Sit at      */
#define SI_ON			BV04  /* Sit on      */
#define SI_IN			BV05  /* Sit in      */
#define RE_AT			BV06  /* Rest at     */
#define RE_ON			BV07  /* Rest on     */
#define RE_IN			BV08  /* Rest in     */
#define SL_AT			BV09  /* Sleep at */
#define SL_ON			BV10  /* Sleep on */
#define SL_IN			BV11  /* Sleep in */
#define PT_AT			BV12  /* Put at      */
#define PT_ON			BV13  /* Put on      */
#define PT_IN			BV14  /* Put in      */
#define PT_INSIDE		BV15  /* Put inside  */

/*
 * mudprograms stuff
 */
extern CHAR_DATA *supermob;

/*
 * MUD_PROGS START HERE
 */
void oprog_speech_trigger( char *txt, CHAR_DATA * ch );
void oprog_random_trigger( OBJ_DATA * obj );
void oprog_wear_trigger( CHAR_DATA * ch, OBJ_DATA * obj );
bool oprog_use_trigger( CHAR_DATA * ch, OBJ_DATA * obj, CHAR_DATA * vict, OBJ_DATA * targ );
void oprog_remove_trigger( CHAR_DATA * ch, OBJ_DATA * obj );
void oprog_sac_trigger( CHAR_DATA * ch, OBJ_DATA * obj );
void oprog_damage_trigger( CHAR_DATA * ch, OBJ_DATA * obj );
void oprog_repair_trigger( CHAR_DATA * ch, OBJ_DATA * obj );
void oprog_drop_trigger( CHAR_DATA * ch, OBJ_DATA * obj );
void oprog_zap_trigger( CHAR_DATA * ch, OBJ_DATA * obj );
char *oprog_type_to_name( int type );
void oprog_greet_trigger( CHAR_DATA * ch );
void oprog_get_trigger( CHAR_DATA * ch, OBJ_DATA * obj );
void oprog_examine_trigger( CHAR_DATA * ch, OBJ_DATA * obj );
void oprog_pull_trigger( CHAR_DATA * ch, OBJ_DATA * obj );
void oprog_push_trigger( CHAR_DATA * ch, OBJ_DATA * obj );

/* mud prog defines */

#define ERROR_PROG        -1
#define IN_FILE_PROG      -2

typedef enum
{
   ACT_PROG, SPEECH_PROG, RAND_PROG, FIGHT_PROG, DEATH_PROG, HITPRCNT_PROG,
   ENTRY_PROG, GREET_PROG, ALL_GREET_PROG, GIVE_PROG, BRIBE_PROG, HOUR_PROG,
   TIME_PROG, WEAR_PROG, REMOVE_PROG, SAC_PROG, LOOK_PROG, EXA_PROG, ZAP_PROG,
   GET_PROG, DROP_PROG, DAMAGE_PROG, REPAIR_PROG, RANDIW_PROG, SPEECHIW_PROG,
   PULL_PROG, PUSH_PROG, SLEEP_PROG, REST_PROG, LEAVE_PROG, SCRIPT_PROG,
   USE_PROG, BRIBE_COPPER_PROG, BRIBE_SILVER_PROG
} prog_types;

/*
 * For backwards compatability
 */
#define RDEATH_PROG DEATH_PROG
#define ENTER_PROG  ENTRY_PROG
#define RFIGHT_PROG FIGHT_PROG
#define RGREET_PROG GREET_PROG
#define OGREET_PROG GREET_PROG

void rprog_leave_trigger( CHAR_DATA * ch );
void rprog_enter_trigger( CHAR_DATA * ch );
void rprog_sleep_trigger( CHAR_DATA * ch );
void rprog_rest_trigger( CHAR_DATA * ch );
void rprog_rfight_trigger( CHAR_DATA * ch );
void rprog_death_trigger( CHAR_DATA * ch );
void rprog_speech_trigger( char *txt, CHAR_DATA * ch );
void rprog_random_trigger( CHAR_DATA * ch );
void rprog_time_trigger( CHAR_DATA * ch );
void rprog_hour_trigger( CHAR_DATA * ch );
char *rprog_type_to_name( int type );

#define OPROG_ACT_TRIGGER
#ifdef OPROG_ACT_TRIGGER
void oprog_act_trigger( char *buf, OBJ_DATA * mobj, CHAR_DATA * ch, OBJ_DATA * obj, void *vo );
#endif
#define RPROG_ACT_TRIGGER
#ifdef RPROG_ACT_TRIGGER
void rprog_act_trigger( char *buf, ROOM_INDEX_DATA * room, CHAR_DATA * ch, OBJ_DATA * obj, void *vo );
#endif

#define GET_ADEPT(ch,sn)    (  skill_table[(sn)]->skill_adept[(ch)->class])
#define LEARNED(ch,sn)	    (IS_NPC(ch) ? 80 : URANGE(0, ch->pcdata->learned[sn], 101))

#ifdef WIN32
void gettimeofday( struct timeval *tv, struct timezone *tz );
void kill_timer(  );

/* directory scanning stuff */

typedef struct dirent
{
   char *d_name;
};

typedef struct
{
   HANDLE hDirectory;
   WIN32_FIND_DATA Win32FindData;
   struct dirent dirinfo;
   char sDirName[MAX_PATH];
} DIR;


DIR *opendir( char *sDirName );
struct dirent *readdir( DIR * dp );
void closedir( DIR * dp );

/* --------------- Stuff for Win32 services ------------------ */
/*

   NJG:

   When "exit" is called to handle an error condition, we really want to
   terminate the game thread, not the whole process.

 */

#define exit(arg) Win32_Exit(arg)
void Win32_Exit( int exit_code );

#endif

#define send_to_char send_to_char_color
#define send_to_pager send_to_pager_color

#endif

#define MANA UMAX(skill->min_mana,100/(2+ch->level-skill->skill_level[ch->class]))
#define BLOOD UMAX(1,(MANA+4)/8)

#define IS_QUESTOR(ch)     (xIS_SET((ch)->act, PLR_QUESTOR))

void look_sky args( ( CHAR_DATA * ch ) );
