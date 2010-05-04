/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

void do_aexit( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA	*room;
    int			 i,vnum;
    AREA_DATA		*tarea;
    EXIT_DATA *pexit;
    int lrange;
    int trange;

    tarea = ch->in_room->area;
    trange = tarea->hi_r_vnum;
    lrange = tarea->low_r_vnum;
    for ( vnum = lrange; vnum <= trange; vnum++ )
    {
	if ( (room = get_room_index( vnum )) == NULL )
	  continue;
        for (i=0;i<MAX_DIR+1;i++)
        {
	    if ( (pexit = get_exit(room, i)) == NULL)
			continue;
		if(pexit->to_room->area != tarea)
                {
			pager_printf(ch, "To: %-20.20s %s\r\n",pexit->to_room->area->filename,pexit->to_room->area->name);
 			pager_printf( ch,"%15d %-30.30s -> %15d (%s)\r\n", vnum, room->name,pexit->vnum,dir_name[i] );
		}
	}
    }
}

void do_aentrance( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA	*room;
    int			 i,vnum;
    AREA_DATA		*tarea;
    AREA_DATA		*otherarea;

    EXIT_DATA *pexit;
    int lrange;
    int trange;

    tarea = ch->in_room->area;
    for ( otherarea=first_area; otherarea; otherarea=otherarea->next)
    {
	if (tarea == otherarea)
		continue;
    	trange = otherarea->hi_r_vnum;
    	lrange = otherarea->low_r_vnum;
    	for ( vnum = lrange; vnum <= trange; vnum++ )
    	{

		if ( (room = get_room_index( vnum )) == NULL )
	  		continue;
        	for (i=0;i<MAX_DIR+1;i++)
        	{
			if ( (pexit = get_exit(room, i)) == NULL)
				continue;
			if(pexit->to_room->area == tarea)
			{
			        pager_printf(ch, "From: %-20.20s %s\r\n",otherarea->filename,otherarea->name);
 				pager_printf(ch, "%15d %-30.30s -> %15d (%s)\r\n", vnum, room->name,pexit->vnum,dir_name[i] );
			}
		}
	}
    }
}
