/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

void do_saveall( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA   *vch;
    AREA_DATA   *tarea;
    char         filename[256];
    for ( vch = first_char; vch; vch = vch->next )
    {
        if ( !IS_NPC( vch ) )
        {
           save_char_obj( vch );
           set_pager_color( AT_PLAIN, ch );
           pager_printf( ch, "Saving %-s...\r\n", vch->name);
        }
    }
    for ( tarea = first_build; tarea; tarea = tarea->next )
    {
       if ( !IS_SET(tarea->status, AREA_LOADED ) )
       {
          continue;
       }
       sprintf( filename, "%s%s", BUILD_DIR, tarea->filename );
       fold_area( tarea, filename, FALSE );
    }

}
