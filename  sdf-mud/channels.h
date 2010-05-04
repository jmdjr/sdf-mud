/****************************************************************************
 *                   ^     +----- |  / ^     ^ |     | +-\                  *
 *                  / \    |      | /  |\   /| |     | |  \                 *
 *                 /   \   +---   |<   | \ / | |     | |  |                 *
 *                /-----\  |      | \  |  v  | |     | |  /                 *
 *               /       \ |      |  \ |     | +-----+ +-/                  *
 ****************************************************************************
 * AFKMud Copyright 1997-2002 Alsherok. Contributors: Samson, Dwip, Whir,   *
 * Cyberfox, Karangi, Rathian, Cam, Raine, and Tarl.                        *
 *                                                                          *
 * Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,        *
 * Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,        *
 * Grishnakh, Fireblade, and Nivek.                                         *
 *                                                                          *
 * Original MERC 2.1 code by Hatchet, Furey, and Kahn.                      *
 *                                                                          *
 * Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,      *
 * Michael Seifert, and Sebastian Hammer.                                   *
 ****************************************************************************
 *                          Dynamic Channel System                          *
 ****************************************************************************/

#define CHANNEL_FILE SYSTEM_DIR "channels.dat"

bool local_channel_hook( CHAR_DATA * ch, char *command, char *argument );
void load_mudchannels( void );

DECLARE_DO_FUN( do_makechannel );
DECLARE_DO_FUN( do_setchannel );
DECLARE_DO_FUN( do_destroychannel );
DECLARE_DO_FUN( do_showchannels );
DECLARE_DO_FUN( do_listen );

typedef struct mud_channel MUD_CHANNEL;

extern MUD_CHANNEL *first_channel;
extern MUD_CHANNEL *last_channel;

typedef enum
{
   CHAN_GLOBAL, CHAN_ZONE, CHAN_GUILD, CHAN_COUNCIL, CHAN_PK, CHAN_LOG
} channel_types;

struct mud_channel
{
   MUD_CHANNEL *next;
   MUD_CHANNEL *prev;
   char *name;
   char *history[20][2];   /* Not saved */
   int hlevel[20];   /* Not saved */
   int hinvis[20];   /* Not saved */
   int level;
   int type;
   bool keephistory;
   char *primary;
   char *secondary;
};
