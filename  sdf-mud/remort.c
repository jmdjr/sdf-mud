/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Kratas, Scion, Shogar and Tagith.                    *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*
 * Remort Code
 * Allows players to switch classes
 * Kratas
 */
#include <string.h>
#include "mud.h"

void do_remort( CHAR_DATA * ch, char *argument )
{
   int x;
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   bool found = FALSE;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Sorry, but mobs just can't remort!\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      buf[0] = '\0';
      send_to_char( "Classes you can choose from:\r\n", ch );
      for( x = 0; x < MAX_CLASS; x++ )
      {
         if( IS_SET( race_table[ch->race]->class_restriction, 1 << x ) )
            continue;

         if( x != ch->class && x != ch->class2 && str_cmp( class_table[x]->who_name, "Dragon" ) )
         {
            if( x == 4 )
               x++;

            if( strlen( buf ) + strlen( class_table[x]->who_name ) > 77 )
            {
               strcat( buf, "\r\n" );
               send_to_char( buf, ch );
               buf[0] = '\0';
            }
            else
               strcat( buf, " " );

            strcat( buf, class_table[x]->who_name );
         }
      }
      strcat( buf, "\r\n" );
      send_to_char( buf, ch );
      return;
   }

   if( ch->level < LEVEL_PCAVATAR )
   {
      send_to_char( "You can't remort yet.\r\n", ch );
      return;
   }

   if( ch->level3 != -1 )
   {
      send_to_char( "You can't remort anymore!\r\n", ch );
      return;
   }

   argument = one_argument( argument, arg );

   for( x = 0; x < MAX_CLASS; x++ )
   {
      if( IS_SET( race_table[ch->race]->class_restriction, 1 << x ) )
         continue;

      if( !str_cmp( class_table[x]->who_name, arg ) )
      {
         found = TRUE;
         break;
      }
   }

   if( !found )
   {
      ch_printf( ch, "There is no such class %s.\r\n", arg );
      return;
   }

   if( ch->class == x || ( ch->class2 == x && ch->level2 >= 0 ) )
   {
      send_to_char( "But you're already that class.\r\n", ch );
      return;
   }

   if( ch->level2 == -1 )
   {
      ch->level2 = ch->level;
      ch->level = 1;
      ch->class2 = ch->class;
      ch->class = x;
   }
   else
   {
      if( ch->level != 100 && ch->level2 != 100 )
      {
         send_to_char( "Both of your classes must be level 100 to do this.\r\n", ch );
         return;
      }
      ch->level3 = ch->level2;
      ch->level2 = ch->level;
      ch->level = 1;
      ch->class3 = ch->class2;
      ch->class2 = ch->class;
      ch->class = x;

   }
   de_equip_char( ch );

   /*
    * Now setup stats 
    */
   ch->max_hit = 20;
   ch->hit = ch->max_hit + race_table[ch->race]->hit;

   ch->max_mana = 100;
   ch->mana = ch->max_mana + race_table[ch->race]->mana;

   ch->max_move = 100;
   ch->move = ch->max_move;

   ch->exp = 0;

//    re_equip_char( ch );

   save_char_obj( ch );

   DISPOSE( ch->pcdata->rank );
   ch->pcdata->rank = str_dup( "" );
   ch_printf( ch, "You are now a %s %s.\r\n", race_table[ch->race]->race_name, class_table[x]->who_name );
   sprintf( buf, "%s has just remorted!", ch->name );

   do_info( NULL, buf );

   char_from_room( ch );
   char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
   do_look( ch, "auto" );
}

int get_adept_remort(CHAR_DATA * ch, int sn)
{
   if(skill_table[sn]->skill_adept[ch->class] >= skill_table[sn]->skill_adept[ch->class2]
   && skill_table[sn]->skill_adept[ch->class] >= skill_table[sn]->skill_adept[ch->class3])
   {
      return skill_table[sn]->skill_adept[ch->class];
   }
   else
   {
      if(skill_table[sn]->skill_adept[ch->class2] >= skill_table[sn]->skill_adept[ch->class]
      && skill_table[sn]->skill_adept[ch->class2] >= skill_table[sn]->skill_adept[ch->class3])
      {
         return skill_table[sn]->skill_adept[ch->class2];
      }
      else 
      {
         if(skill_table[sn]->skill_adept[ch->class3] >= skill_table[sn]->skill_adept[ch->class]
         && skill_table[sn]->skill_adept[ch->class3] >= skill_table[sn]->skill_adept[ch->class2])
         {
            return skill_table[sn]->skill_adept[ch->class3];
         }
      }
   }
  return -1;
}
