/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*
 *  Inferno spell
 *  Author: Rantic (supfly@geocities.com)
 *  of FrozenMUD (empire.digiunix.net 4000)
 *
 *  Permission to use and distribute this code is granted provided
 *  this header is retained and unaltered, and the distribution
 *  package contains all the original files unmodified.
 *  If you modify this code and use/distribute modified versions
 *  you must give credit to the original author(s).
 */


ch_ret spell_inferno( int sn, int level, CHAR_DATA *ch, void *vo)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	bool ch_died;
	int dam;
	int hpch;
	ch_ret retcode;
		
	ch_died = FALSE;
	retcode = rNONE;
	
	if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
	{
		set_char_color( AT_MAGIC, ch );
		send_to_char( "Magical forces extinguish your inferno.\n\r", ch );
		return rNONE;
	}
	
	hpch = UMAX( 10, ch->hit );
	dam = number_range( hpch/15, hpch/6 );

	act( AT_MAGIC, "A hellish inferno rises at your command!", ch, NULL, NULL, TO_CHAR );
	act( AT_MAGIC, "A hellish inferno rises at $n's command!", ch, NULL, NULL, TO_ROOM );
	
	for ( vch = ch->in_room->first_person; vch; vch = vch_next )
	{
		vch_next = vch->next_in_room;
		if ( !vch->in_room )
			continue;

		if ( !IS_NPC( vch ) &&  xIS_SET( vch->act, PLR_WIZINVIS ) && vch->pcdata->wizinvis >= LEVEL_IMMORTAL )
			continue;
		  
		if ( vch != ch && ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) ) )
		{
			if ( saves_spell_staff( level, vch ) )  /* small fix, --Cronel */
				retcode = damage( ch, vch, dam/2, sn );
			else
				retcode = damage( ch, vch, dam, sn );
		}

		if ( retcode == rCHAR_DIED || char_died(ch) )
		{
			ch_died = TRUE;
			continue;
		}
		
		if ( char_died(vch) )
		{	
			act( AT_BLOOD, "$N is engulfed by the inferno's scorching flame, burning $M to a crisp.", ch, NULL, vch, TO_CHAR );
			act( AT_BLOOD, "$N is engulfed by the inferno's scorching flame, burning $M to a crisp.", ch, NULL, vch, TO_ROOM );
			continue;
		}			

		if ( !ch_died )
		{
		    set_char_color( AT_MAGIC, vch );
			send_to_char( "The inferno scorches everything it touches.\n\r", vch );
		}
	}

	if ( ch_died )
		return rCHAR_DIED;
	else
		return rNONE;
}
