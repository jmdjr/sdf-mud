/***************************************************************************
*	ROM 2.4 is copyright 1993-1998 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@hypercube.org)				   *
*	    Gabrielle Taylor (gtaylor@hypercube.org)			   *
*	    Brian Moore (zump@rom.org)					   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

/***************************************************************************
*    Ported to Smaug 1.4 by Desden, el Chaman Tibetano in November 1998    *
*           ( Jose Luis Sogorb ) Email: jlalbatros@retemail.es          *
*                                                                          *
***************************************************************************/

#include "mud.h"

void do_heal( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *mob;
   char arg[MAX_INPUT_LENGTH];
   int cost, sn;
   SPELL_FUN *spell;
   char *words;

   /*
    * Search for an act_healer 
    */
   for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   {
      if( IS_NPC( mob ) && xIS_SET( mob->act, ACT_HEALER ) )
         break;
   }

   if( mob == NULL )
   {
      send_to_char( "You can't do that here.\r\n", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      /*
       * display price list 
       */
      act( AT_PLAIN, "$N says 'I offer the following spells:'", ch, NULL, mob, TO_CHAR );
      send_to_char( "  light: cure light wounds          25 silver\r\n", ch );
      send_to_char( "  serious: cure serious wounds      30 silver\r\n", ch );
      send_to_char( "  critical: cure critical wounds    35 silver\r\n", ch );
      send_to_char( "  heal: healing spell               40 silver\r\n", ch );
      send_to_char( "  blind: cure blindness             20 silver\r\n", ch );
      send_to_char( "  poison:  cure poison              20 silver\r\n", ch );
      send_to_char( "  curse: remove curse               20 silver\r\n", ch );
      send_to_char( "  refresh: restore movement         30 silver\r\n", ch );
      send_to_char( "  mana:  restore mana	          35 silver\r\n", ch );
      send_to_char( "  life:  life force                 60 silver\r\n", ch );
      send_to_char( "  armor: armor                      20 silver\r\n", ch );
      send_to_char( "  bless: bless                      20 silver\r\n", ch );
      send_to_char( "  blur:  blur                       20 silver\r\n", ch );
      send_to_char( "  devil: devils might               25 silver\r\n", ch );
      send_to_char( "  dragon: dragon wit                30 silver\r\n", ch );
      send_to_char( "  elven: elven beauty               30 silver\r\n", ch );
      send_to_char( "  fire: fireshield                  32 silver\r\n", ch );
      send_to_char( "  flight: flight                    10 silver\r\n", ch );
      send_to_char( "  float: float                      10 silver\r\n", ch );
      send_to_char( "  haste: haste                      35 silver\r\n", ch );
      send_to_char( "  ices: iceshield                   32 silver\r\n", ch );
      send_to_char( "  invis: invis                      15 silver\r\n", ch );
      send_to_char( "  kindr: kindred strength           20 silver\r\n", ch );
      send_to_char( "  lepre: leprechaun fortune         25 silver\r\n", ch );
      send_to_char( "  sagac: sagacity                   30 silver\r\n", ch );
      send_to_char( "  sanc: sanctuary                   90 silver\r\n", ch );
      send_to_char( "  shield: shield                    32 silver\r\n", ch );
      send_to_char( "  slink:  slink                     30 silver\r\n", ch );
      send_to_char( "  troll: trollish vigor             30 silver\r\n", ch );
      send_to_char( "  vali: valiance                    30 silver\r\n", ch );
      send_to_char( " Type heal <type> to be healed.\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "light" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "cure light" );
      words = "judicandus dies";
      cost = 25;
   }

   else if( !str_cmp( arg, "serious" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "cure serious" );
      words = "judicandus gzfuajg";
      cost = 30;
   }

   else if( !str_cmp( arg, "critical" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "cure critical" );
      words = "judicandus qfuhuqar";
      cost = 35;
   }

   else if( !str_cmp( arg, "heal" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "heal" );
      words = "pzar";
      cost = 40;
   }

   else if( !str_cmp( arg, "blind" ) )
   {
      spell = spell_cure_blindness;
      sn = skill_lookup( "cure blindness" );
      words = "judicandus noselacri";
      cost = 20;
   }

   else if( !str_cmp( arg, "poison" ) )
   {
      spell = spell_cure_poison;
      sn = skill_lookup( "cure poison" );
      words = "judicandus sausabru";
      cost = 20;
   }

   else if( !str_cmp( arg, "curse" ) )
   {
      spell = spell_remove_curse;
      sn = skill_lookup( "remove curse" );
      words = "candussido judifgz";
      cost = 20;
   }

   else if( !str_cmp( arg, "mana" ) )
   {
      spell = NULL;
      sn = -1;
      words = "energizer";
      cost = 35;
   }

   else if( !str_cmp( arg, "refresh" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "refresh" );
      words = "candusima";
      cost = 30;
   }

   else if( !str_cmp( arg, "life" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "life force" );
      words = "woedsajus";
      cost = 60;
   }

   else if( !str_cmp( arg, "armor" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "armor" );
      words = "woedsajuse";
      cost = 20;
   }

   else if( !str_cmp( arg, "bless" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "bless" );
      words = "woedsajusw";
      cost = 20;
   }

   else if( !str_cmp( arg, "blur" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "blur" );
      words = "woedsajusq";
      cost = 20;
   }

   else if( !str_cmp( arg, "devil" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "devils might" );
      words = "woedsajustr";
      cost = 25;
   }

   else if( !str_cmp( arg, "dragon" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "dragon wit" );
      words = "woedsajuste";
      cost = 30;
   }

   else if( !str_cmp( arg, "elven" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "elven beauty" );
      words = "woedsajustw";
      cost = 30;
   }

   else if( !str_cmp( arg, "fire" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "fireshield" );
      words = "woedsajustq";
      cost = 32;
   }

   else if( !str_cmp( arg, "flight" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "flight" );
      words = "woedsajusta";
      cost = 10;
   }

   else if( !str_cmp( arg, "float" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "float" );
      words = "woedsajustz";
      cost = 10;
   }

   else if( !str_cmp( arg, "haste" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "haste" );
      words = "woedsajustc";
      cost = 35;
   }

   else if( !str_cmp( arg, "ices" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "iceshield" );
      words = "woedsajustx";
      cost = 35;
   }

   else if( !str_cmp( arg, "invis" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "invis" );
      words = "woedsajustn";
      cost = 15;
   }

   else if( !str_cmp( arg, "kindr" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "kindred strength" );
      words = "woedsajustl";
      cost = 20;
   }

   else if( !str_cmp( arg, "lepre" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "leprechaun fortune" );
      words = "woedsajustm";
      cost = 25;
   }

   else if( !str_cmp( arg, "sagac" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "sagacity" );
      words = "woedsajustmz";
      cost = 30;
   }

   else if( !str_cmp( arg, "sanc" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "sanctuary" );
      words = "woedsajustmx";
      cost = 90;
   }

   else if( !str_cmp( arg, "shield" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "shield" );
      words = "woedsajustmb";
      cost = 32;
   }

   else if( !str_cmp( arg, "slink" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "slink" );
      words = "woedsajustmw";
      cost = 30;
   }

   else if( !str_cmp( arg, "troll" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "trollish vigor" );
      words = "woedsajustme";
      cost = 30;
   }

   else if( !str_cmp( arg, "vali" ) )
   {
      spell = spell_smaug;
      sn = skill_lookup( "valiance" );
      words = "woedsajustmd";
      cost = 30;
   }

   else
   {
      act( AT_PLAIN, "$N says ' Type 'heal' for a list of spells.'", ch, NULL, mob, TO_CHAR );
      return;
   }

   if( cost > ch->silver )
   {
      act( AT_PLAIN, "$N says 'You do not have enough money for my services.'", ch, NULL, mob, TO_CHAR );
      return;
   }

   WAIT_STATE( ch, PULSE_VIOLENCE );
   ch->silver -= cost;
   act( AT_PLAIN, "$n utters the words '$T'.", mob, NULL, words, TO_ROOM );

   if( spell == NULL )
   {
      ch->mana += dice( 2, 8 ) + ch->level / 3;
      ch->mana += UMIN( ch->level, ch->max_mana - ch->mana );
      send_to_char( "A warm glow passes through you.\r\n", ch );
      return;
   }

   if( sn == -1 )
      return;

   spell( sn, ch->level, mob, ch );
}

/*********   End of code *****************************************/
