/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*Include the next file as starmap.c into your /src directory and add starmap.o
#include <string.h>
#define NUM_DAYS 35
/* Match this to the number of days per month; this is the moon cycle */
#define NUM_MONTHS 17
/* Match this to the number of months defined in month_name[].  */  
#define MAP_WIDTH 72
/* Should be the string length and number of the constants below.*/ 
   
   "    O:       R*        G*    G.  W* W. W.          C. C.    Y* Y. Y.    ",
   "  O*.                c.          W.W.     W.            C.       Y..Y.  ",
   "O.O. O.              c.  G..G.           W:      B*                   Y.",
   "     O.    c.     c.                     W. W.                  r*    Y.",
   "     O.c.     c.      G.             P..     W.        p.      Y.   Y:  ",
   "        c.                    G*    P.  P.           p.  p:     Y.   Y. ",
   "                 b*             P.: P*                 p.p:             " 
};

/****************** CONSTELLATIONS and STARS *****************************
const char *sun_map[] = 
};

};

{
   static char buf[MAX_STRING_LENGTH];
   static char buf2[4];
   int starpos, sunpos, moonpos, moonphase, i, linenum, precip;
   
   precip = ( ch->in_room->area->weather->precip + 3 * weath_unit - 1 ) / weath_unit;
   
   {
      send_to_char( "There are some clouds in the sky so you cannot see anything else.\r\n", ch );
      return;
   }
   
   moonpos = ( sunpos + time_info.day * MAP_WIDTH / NUM_DAYS ) % MAP_WIDTH;
   
      moonphase -= 8;
   
   /*
    * The left end of the star_map will be straight overhead at midnight during 
    */ 
   {
      if( ( time_info.hour >= 6 && time_info.hour <= 18 ) && 
         continue;
      
      /*
       * for ( i = MAP_WIDTH/4; i <= 3*MAP_WIDTH/4; i++)
       */ 
      {
         /*
          * plot moon on top of anything else...unless new moon & no eclipse 
          */ 
                  && ( moonpos <= 3 * MAP_WIDTH / 4 + 2 ) /* in sky? */  
                  && ( i <= moonpos + 2 ) /* is this pixel near moon? */  
                                                                                || moonphase !=
                                                                                0 ) /*no eclipse */  
                                                                                                                    3][i +
                                                                                                                       2 -
                                                                                                                       moonpos]
                                                                                                           == '@' ) )
         {
            if( ( moonphase < 0 && i - 2 - moonpos >= moonphase ) || 
               
            else
               strcat( buf, " " );
         }
         else
            if( ( linenum >= 3 ) && ( linenum < 6 ) && /* nighttime */ 
                && ( moonpos <= 3 * MAP_WIDTH / 4 + 2 ) /* in sky? */  
                && ( i <= moonpos + 2 ) /* is this pixel near moon? */  
                                                                              '@' ) )
         {
            if( ( moonphase < 0 && i - 2 - moonpos >= moonphase ) || 
               strcat( buf, "&W@" );
            else
               strcat( buf, " " );
         }
         else  /* plot sun or stars */
         {
            if( time_info.hour >= 6 && time_info.hour <= 18 )  /* daytime */
            {
               if( i >= sunpos - 2 && i <= sunpos + 2 )
               {
                  sprintf( buf2, "&Y%c", sun_map[linenum - 3][i + 2 - sunpos] );
                  strcat( buf, buf2 );
               }
               else
                  strcat( buf, " " );
            }
            else
            {
               switch ( star_map[linenum][( MAP_WIDTH + i - starpos ) % MAP_WIDTH] )
               {
                  default:
                     strcat( buf, " " );
                     break;
                  
                     strcat( buf, ":" );
                     break;
                  
                     strcat( buf, "." );
                     break;
                  
                     strcat( buf, "*" );
                     break;
                  
                     strcat( buf, "&G " );
                     break;
                  
                     strcat( buf, "&g " );
                     break;
                  
                     strcat( buf, "&R " );
                     break;
                  
                     strcat( buf, "&r " );
                     break;
                  
                     strcat( buf, "&C " );
                     break;
                  
                     strcat( buf, "&O " );
                     break;
                  
                     strcat( buf, "&B " );
                     break;
                  
                     strcat( buf, "&P " );
                     break;
                  
                     strcat( buf, "&W " );
                     break;
                  
                     strcat( buf, "&b " );
                     break;
                  
                     strcat( buf, "&p " );
                     break;
                  
                     strcat( buf, "&Y " );
                     break;
                  
                     strcat( buf, "&c " );
                     break;
               }
            }
         }
      }
      strcat( buf, "\r\n" );
      send_to_char_color( buf, ch );
   }
}