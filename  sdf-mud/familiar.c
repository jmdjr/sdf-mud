/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ResortMud Version 5.0b programed By Badastaz and Garinan
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

#include	"mud.h"

/* from magic.c */
void failed_casting( struct skill_type *skill, CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj );

ch_ret spell_familiar( int sn, int level, CHAR_DATA * ch, void *vo )
{
   MOB_INDEX_DATA *pMobIndex;
   CHAR_DATA *mount;
   SKILLTYPE *skill = get_skilltype( sn );

   if( ch->pcdata->pet != NULL )
   {
      send_to_char( "You already have a companion.\r\n", ch );
      failed_casting( skill, ch, NULL, NULL );
      return rSPELL_FAILED;
   }
   if( ch->position == POS_FIGHTING )
   {
      send_to_char( "You can't study the ritual while in combat!\r\n", ch );
      failed_casting( skill, ch, NULL, NULL );
      return rSPELL_FAILED;
   }

   if( ( pMobIndex = get_mob_index( MOB_VNUM_FAMILIAR ) ) == NULL )
   {
      send_to_char( "The familiar mob doesn't exist.\r\n", ch );
      return rSPELL_FAILED;
   }
   /*
    * can't cast the spell in these sectors 
    */
   if( ch->in_room->sector_type == SECT_INSIDE
       || ch->in_room->sector_type == SECT_WATER_SWIM
       || ch->in_room->sector_type == SECT_WATER_NOSWIM || ch->in_room->sector_type == SECT_AIR )
   {
      send_to_char( "There is not enough magical energy here.\r\n", ch );
      failed_casting( skill, ch, NULL, NULL );
      return rSPELL_FAILED;
   }

   mount = create_mobile( pMobIndex );

   mount->level = number_fuzzy( ( ch->level + ch->mod_cha ) / 2 );   // charisma bonus added by G
   mount->mana = mount->max_mana = 0;
   mount->hit = mount->max_hit = number_fuzzy( ( ch->max_hit / 2 ) + ch->mod_cha );
   mount->armor = number_fuzzy( ch->armor - 10 );
   mount->hitroll = number_fuzzy( ch->level / 30 );
   mount->damroll = number_fuzzy( ch->level / 30 );

   /*
    * free up the old mob names 
    */
   STRFREE( mount->description );
   STRFREE( mount->name );
   STRFREE( mount->short_descr );
   STRFREE( mount->long_descr );

   /*
    * terrain 
    */
   switch ( ch->in_room->sector_type )
   {
      case ( SECT_CITY ): /* rat */
      case ( SECT_FIELD ):
         mount->description =
            str_dup( "You see a large furry rat.  Long whiskers hang down from it's nose.\r\n"
                     "You can feel the dirt and disease crawling off this beast.\r\n" );
         mount->short_descr = str_dup( "large rat" );
         mount->long_descr = str_dup( "A large furry rodent is here.\r\n" );
         mount->name = str_dup( "familiar rat" );
         xSET_BIT( mount->attacks, DAM_BITE );
         break;
      case ( SECT_FOREST ):  /* falcon */
      case ( SECT_HILLS ):
         mount->description =
            str_dup( "You see a large falcon.  Golden brown feathers frame powerful\r\n"
                     "wings.  Long talons grasp at nothingness in vain attempts at\r\n"
                     "getting some rabbit or rodent for dinner.\r\n" );
         mount->short_descr = str_dup( "large falcon" );
         mount->long_descr = str_dup( "A large falcon screams here.\r\n" );
         mount->name = str_dup( "familiar falcon" );
         xSET_BIT( mount->attacks, DAM_CLAW );
         break;
      case ( SECT_MOUNTAIN ):   /* mountain lion */
         mount->description =
            str_dup( "You see a very large mountain lion.  One wrong look and it could\r\n"
                     "have your head lying at your feet.  You should think better than\r\n"
                     "cross this beast especial if you have a weapon in your hand.\r\n" );
         mount->short_descr = str_dup( "large mountain lion" );
         mount->long_descr = str_dup( "A large mountain lion claws the ground here.\r\n" );
         mount->name = str_dup( "familiar mountain lion" );
         xSET_BIT( mount->attacks, DAM_BITE );
         break;
      case ( SECT_DESERT ):  /* sandworm */
         mount->description =
            str_dup( "You see a large white sandworm wiggling in the light.\r\n"
                     "A red spot on one end makes you guess it is a mouth.\r\n"
                     "A loud moan comes from the direction of that red spot.\r\n" );
         mount->short_descr = str_dup( "sandworm" );
         mount->long_descr = str_dup( "A white sandworm wiggles on the ground here.\r\n" );
         mount->name = str_dup( "familiar sandworm" );
         xSET_BIT( mount->attacks, DAM_SUCTION );
         break;
   }
   /*
    * player seen stuff here 
    */
   do_sit( ch, "" );
   char_to_room( mount, ch->in_room );
   act( AT_MAGIC, "You begin to chant and call to a $N!.", ch, NULL, mount, TO_CHAR );
   act( AT_MAGIC, "$n begins to chant and calls to a $N!", ch, NULL, mount, TO_ROOM );
   WAIT_STATE( ch, 2 * PULSE_MOBILE );
   add_follower( mount, ch );
   mount->leader = ch;
   ch->pcdata->pet = mount;
   do_stand( ch, "" );

   xSET_BIT( mount->act, ACT_PET );
   xSET_BIT( mount->affected_by, AFF_CHARM );
   ch->move -= ( mount->level / 2 );   /* physically draining lose of move */
   return rNONE;
}
