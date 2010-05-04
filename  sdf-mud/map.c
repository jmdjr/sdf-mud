#include <string.h>
#include "mud.h"

const char *sector_translation[26][2] = {
   {"&C&w#", "Inside"},
   {"&C&B.", "City"},
   {"&C&g.", "Field"},
   {"&C&G@", "Forest"},
   {"&C&g^^", "Hill"},
   {"&C&p^^", "Mountain"},
   {"&C&B~", "Swimmable Water"},
   {"&C&B!", "Nonswimmable Water"},
   {"&C&B+", "Underwater"},
   {"&C&C-", "Air"},
   {"&C&O:", "Desert"},
   {"&C&C?", "Dunno"},
   {"&C&B_", "Oceanfloor"},
   {"&C&Y=", "Underground"},
   /*
    * Wilderness Sectors 
    */
   {"&C&p.", "Road"},
   {"&C&W#", "Ruins"},
   {"&C&g@", "Jungle"},
   {"&C&G~", "Swamp"},
   {"&C&R!", "Entrance"},
   {"&C&O^^", "Rock Mountain"},
   {"&C&w^^", "Snow Mountain"},
   {"&C&P?", "Unused(Reserved for Future Use)"},
   {"&C&R~", "Lava"},
   /*
    * End of Wilderness Sectors 
    */
   {"&C&x ", "Nonexistant Room"},   /* SECT_MAX */
   {"&C&R*", "Center of Map"}
};

short map[215][215];

void map_room( ROOM_INDEX_DATA * room, int min, int max, int x, int y )
{
   EXIT_DATA *pexit = NULL;
   int tx, ty;

   map[x][y] = room->sector_type;

   for( pexit = room->first_exit; pexit; pexit = pexit->next )
   {
      if( pexit->to_room != NULL && !IS_SET( pexit->exit_info, EX_CLOSED )
          && !IS_SET( pexit->exit_info, EX_HIDDEN ) && !IS_SET( pexit->exit_info, EX_SECRET )
          && pexit->vdir != DIR_UP && pexit->vdir != DIR_DOWN && pexit->vdir != DIR_SOMEWHERE )
      {

         if( x < min || y < min || x > max || y > max )
            return;

         tx = x;
         ty = y;

         if( pexit->vdir == DIR_NORTH || pexit->vdir == DIR_NORTHEAST || pexit->vdir == DIR_NORTHWEST )
            tx--;
         if( pexit->vdir == DIR_SOUTH || pexit->vdir == DIR_SOUTHWEST || pexit->vdir == DIR_SOUTHEAST )
            tx++;
         if( pexit->vdir == DIR_EAST || pexit->vdir == DIR_SOUTHEAST || pexit->vdir == DIR_NORTHEAST )
            ty++;
         if( pexit->vdir == DIR_WEST || pexit->vdir == DIR_SOUTHWEST || pexit->vdir == DIR_NORTHWEST )
            ty--;

         if( map[tx][ty] == SECT_MAX )
            map_room( pexit->to_room, min, max, tx, ty );
      }
   }

   return;
}

void prep_map( ROOM_INDEX_DATA * location, int min, int max )
{
   int x, y, center;

   for( x = 0; x < 215; x++ )
      for( y = 0; y < 215; y++ )
         map[x][y] = SECT_MAX;

   center = ( 215 / 2 ) - 1;

   map_room( location, min - 5, max + 5, center, center );

   map[center][center] = SECT_MAX + 1;

   return;
}

char *make_map_line( int lnum, int min, int max )
{
   static char line[215];
   int counter;

   strcpy( line, "&C&c|" );

   for( counter = min; counter < max; counter++ )
   {
      strcat( line, sector_translation[map[lnum][counter]][0] );
   }

   strcat( line, "&C&c|" );

   return line;
}

void do_map( CHAR_DATA * ch, char *argument )
{
   int counter, size, min, max;
   char mline[430];

   if( !sector_translation[SECT_MAX + 1][0] )
   {
      send_to_char_color( "&C&wYour sector translation table needs to be updated to match your current"
                          " sector types.\r\n", ch );
      return;
   }

   if( argument && !str_cmp( argument, "legend" ) )
   {
      send_to_char_color( "&C&G[MAP LEGEND]\r\n", ch );
      for( counter = 0; counter < SECT_MAX + 2; ++counter )
         if( counter != SECT_MAX )
            ch_printf_color( ch, "   %s - %s\r\n", sector_translation[counter][0], sector_translation[counter][1] );
      return;
   }

   if( argument && argument[0] != '\0' )
   {
      send_to_char_color( "&C&wSyntax: \r\n", ch );
      send_to_char_color( "&C&wmap legend &G- &wMap Symbol Explanation\r\n", ch );
      send_to_char_color( "&C&wOtherwise just try map with no arguments.\r\n", ch );
      return;
   }

   size = 6;
   min = ( 215 / 2 ) - size / 2;
   max = ( 215 / 2 ) + size / 2;

   prep_map( ch->in_room, min, max );

   strcpy( mline, "&C&c." );

   for( counter = 0; counter < size; counter++ )
      strcat( mline, "&C&c-" );

   strcat( mline, "&C&c.\r\n" );


   for( counter = min; counter < max; counter++ )
   {
      strcat( mline, make_map_line( counter, min, max ) );
      strcat( mline, "\r\n" );
   }

   strcat( mline, "&C&c'" );

   for( counter = 0; counter < size; counter++ )
      strcat( mline, "&C&c-" );

   strcat( mline, "&C&c'\r\n" );

   send_to_char_color( mline, ch );

   return;
}
