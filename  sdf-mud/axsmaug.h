/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

#include "axcfgsmaug.h"
#ifdef SMAUG102
char *version="AEX-1.4.4 Smaug1.02a\n\r";
#endif
#ifdef SMAUG14
char *version="AEX-1.4.4 Smaug1.14\n\r";
#endif
#ifdef DRAGONBANE
#define SMAUG102
char *version="AEX-2.4.4 DragonBane - Smaug1.02a\n\r";
#endif

int get_cont_flag(char *str)
{
	int index=0;

	while(axcontainer_flags[index])
        {
		if(!strcmp(axcontainer_flags[index],str))
			return index;
		else
			index++;
	}
	return -1;
}

#ifndef DRAGONBANE
int get_npc_class( char *type )
{
    int x;

    for ( x = 0; x < MAX_NPC_CLASS; x++ )
      if ( !str_cmp( type, npc_class[x] ) )
        return x;
    return -1;
}
int get_mflag( char *flag )
{
    int x;

    for ( x = 0; x < (sizeof(mag_flags) / sizeof(mag_flags[0])); x++ )
      if ( !str_cmp( flag, mag_flags[x] ) )
        return x;
    return -1;
}
#endif

void ax_exit( FILE *fp,AREA_DATA *tarea,int lrange, int hrange )
{
    ROOM_INDEX_DATA	*room;
    int			 i,vnum;
    EXIT_DATA *pexit;

    for ( vnum = lrange; vnum <= hrange; vnum++ )
    {
	if ( (room = get_room_index( vnum )) == NULL )
	  continue;
        for (i=0;i<MAX_DIR+1;i++)
        {
	    if ( (pexit = get_exit(room, i)) == NULL)
			continue;
		if(pexit->to_room->area != tarea)
                {
			fprintf(fp, "@roomislinktoexternalarea EXTERNAL_EXIT %15d %-12.12s To: %-20.20s %s\n",
				OUT_VNUM(vnum),
				dir_name[i], 
				pexit->to_room->area->filename,
				pexit->to_room->area->name);
		}
	}
    }
}

void ax_entrance( FILE *fp,AREA_DATA *tarea,int lrange, int hrange )
{
    ROOM_INDEX_DATA	*room;
    int			 i,vnum,thrange,tlrange;
    AREA_DATA		*otherarea;
    EXIT_DATA *pexit;

    for ( otherarea=first_area; otherarea; otherarea=otherarea->next)
    {
	if (tarea == otherarea)
		continue;
    	thrange = otherarea->hi_r_vnum;
    	tlrange = otherarea->low_r_vnum;
    	for ( vnum = tlrange; vnum <= thrange; vnum++ )
    	{

		if ( (room = get_room_index( vnum )) == NULL )
	  		continue;
        	for (i=0;i<MAX_DIR+1;i++)
        	{
			if ( (pexit = get_exit(room, i)) == NULL)
				continue;
			if(pexit->to_room->area == tarea)
			{
			     fprintf(fp, "@roomhaslinkfromexternalarea EXTERNAL_ENTRANCE %15d From: %-12.12s %-20.20s %s\n",
				OUT_VNUM(pexit->vnum),
				dir_name[rev_dir[i]], 
				otherarea->filename,
				otherarea->name);
			}
		}
	}
    }
}
