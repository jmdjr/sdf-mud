/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

/*                           Marriage Code v1.3
                                  By Cid
                            eos.execnet.com:1234

Unlike my other codes this one does require the addition of another pcdata
field.  Like spouse.  If you add it spouse is a string not an integer.
So thats how it works.  Plus you'll have to modify db.c save.c to load up
the spouse string so this will work.   This snippet also includes a rings
command.  It'll create the rings for the married couple.  Which you'll have
to define in mud.h OBJ_VNUM_DIAMOND_RING and OBJ_VNUM_WEDDING_BAND to vnums
that you might have (ofcourse I created 2 rings for this then added in the
marriage code).  The rings command would be the easiest to add in cause of
just putting in 2 lines in mud.h...even though its a lot easier then the
marriage, and divorce code...but it needs the spouse field(checks to see if
they are married).   Well enjoy...btw dont forget to put somewhere in your
code to give me credits or if you just pop in this file...Keep the this header
intact.

Add do_divorce and do_marry to mud.h and tables.c - Ntanel StormBlade
*/
#include "mud.h"

void do_marry( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   CHAR_DATA *victim2;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   set_char_color( AT_WHITE, ch );
   if( !arg1 || arg1[0] == '\0' || !arg2 || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: marry <char1> <char2>\r\n", ch );
      return;
   }

   if( ( ( victim = get_char_world( ch, arg1 ) ) == NULL ) || ( ( victim2 = get_char_world( ch, arg2 ) ) == NULL ) )
   {
      send_to_char( "Both characters must be playing!\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) || IS_NPC( victim2 ) )
   {
      send_to_char( "Sorry! Mobs can't get married!\r\n", ch );
      return;
   }


   if( !victim->pcdata->wedto && !victim2->pcdata->wedto )
   {
      DESCRIPTOR_DATA *dtmp;

      send_to_char( "You pronounce them man and wife!\r\n", ch );
      send_to_char( "You say the big 'I do.'\r\n", victim );
      send_to_char( "You say the big 'I do.'\r\n", victim2 );

      act( AT_BLUE, "$n and $N are now declared married!\r\n", victim, NULL, victim2, TO_ROOM );

      victim->pcdata->wedto = STRALLOC( victim2->name );
      victim2->pcdata->wedto = STRALLOC( victim->name );

      xSET_BIT( victim->act, PLR_MARRIED );
      xSET_BIT( victim2->act, PLR_MARRIED );

      /*
       * Send message to all connected players. (Send to switched ones too) 
       */
      for( dtmp = first_descriptor; dtmp; dtmp = dtmp->next )
      {
         char buf[MAX_INPUT_LENGTH];
         CHAR_DATA *wch;

         if( dtmp->connected != CON_PLAYING )
            continue;

         wch = dtmp->original ? dtmp->original : dtmp->character;
         sprintf( buf, "&R[&WINFO&R] &c%s and %s have just gotten married!\r\n", PERS( victim, wch ), PERS( victim2, wch ) );

         send_to_char_color( buf, wch );
      }
      return;
   }
   else if( !victim->pcdata->wedto )
   {
      char buf[200];
      sprintf( buf, "But %s is already married!\r\n", victim2->name );
      send_to_char( buf, ch );
   }
   else if( !victim2->pcdata->wedto )
   {
      char buf[200];
      sprintf( buf, "But %s is already married!\r\n", victim->name );
      send_to_char( buf, ch );
   }
   else
   {
      send_to_char( "They are already married!\r\n", ch );
      return;
   }

   if( victim->level < 10 || victim2->level < 10 )
   {
      send_to_char( "They are not of the proper level to marry.\r\n", ch );
      return;
   }

   return;
}

void do_divorce( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   CHAR_DATA *victim2;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   set_char_color( AT_WHITE, ch );
   if( !arg1 || arg1[0] == '\0' || !arg2 || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: divorce <char1> <char2>\r\n", ch );
      return;
   }

   if( ( ( victim = get_char_world( ch, arg1 ) ) == NULL ) || ( ( victim2 = get_char_world( ch, arg2 ) ) == NULL ) )
   {
      send_to_char( "Both characters must be playing!\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) || IS_NPC( victim2 ) )
   {
      send_to_char( "I don't think they're Married to the Mob!\r\n", ch );
      return;
   }

   if( victim->pcdata->wedto && !str_cmp( victim->pcdata->wedto, victim2->name )
       && victim2->pcdata->wedto && !str_cmp( victim2->pcdata->wedto, victim->name ) )
      /*
       * if (victim->pcdata->spouse != victim2->name || victim2->pcdata->spouse != victim->name)
       * {
       * send_to_char(AT_WHITE, "They aren't even married!!\r\n",ch);
       * return;
       * } 
       */
   {
      DESCRIPTOR_DATA *dtmp;

      send_to_char( "You hand them their papers.\r\n", ch );
      send_to_char( "Your divorce is final.\r\n", victim );
      send_to_char( "Your divorce is final.\r\n", victim2 );

      act( AT_WHITE, "$n and $N swap divorce papers, they are no-longer married.", victim, NULL, victim2, TO_NOTVICT );

      STRFREE( victim->pcdata->wedto );
      STRFREE( victim2->pcdata->wedto );
      victim->pcdata->wedto = NULL;
      victim2->pcdata->wedto = NULL;

      xREMOVE_BIT( victim->act, PLR_MARRIED );
      xREMOVE_BIT( victim2->act, PLR_MARRIED );

      /*
       * Send message to all connected players. (Send to switched ones too) 
       */
      for( dtmp = first_descriptor; dtmp; dtmp = dtmp->next )
      {
         char buf[MAX_INPUT_LENGTH];
         CHAR_DATA *wch;

         if( dtmp->connected != CON_PLAYING )
            continue;

         wch = dtmp->original ? dtmp->original : dtmp->character;
         sprintf( buf, "&R[&WINFO&R] &c%s and %s have just gotten divorced!\r\n",
                  PERS( victim, wch ), PERS( victim2, wch ) );

         send_to_char_color( buf, wch );
      }

      return;
   }
   else
   {
      send_to_char( "They arent married!", ch );
      return;
   }
}

void do_rings( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   CHAR_DATA *victim2;
   char buf[MAX_STRING_LENGTH];
   OBJ_DATA *ring;
   EXTRA_DESCR_DATA *ed;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   set_char_color( AT_WHITE, ch );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: rings <char1> <char2>\r\n", ch );
      return;
   }

   if( ( ( victim = get_char_world( ch, arg1 ) ) == NULL ) || ( ( victim2 = get_char_world( ch, arg2 ) ) == NULL ) )
   {
      send_to_char( "Both characters must be playing!\r\n", ch );
      return;
   }
   /*
    * if ((victim->pcdata->spouse != victim2->name) || (victim2->pcdata->spouse != victim->name))
    * {
    * send_to_char(AT_WHITE, "They arent even married!!\r\n", ch);
    * return;
    * } 
    */

   switch ( victim2->sex )
   {
      case SEX_FEMALE:
      {
         ring = create_object( get_obj_index( OBJ_VNUM_DIAMOND_RING ), 0 );
         switch ( victim->sex )
         {
            case SEX_FEMALE:
            {
               sprintf( buf,
                        "This is the beautiful diamond ring given to you by your lovely\r\nwife %s at your wedding. It signifies your eternal love for eachother.\r\n",
                        victim->name );
               ring->description = STRALLOC( buf );
               break;
            }
            case SEX_MALE:
            {
               sprintf( buf,
                        "This is the beautiful diamond ring given to you by your handsome\r\nhusband %s at your wedding. It signifies your eternal love for eachother.\r\n",
                        victim->name );
               ring->description = STRALLOC( buf );
               break;
            }
            case SEX_NEUTRAL:
            default:
            {
               sprintf( buf,
                        "This is the beautiful diamond ring given to you by your\r\nspouse %s at your wedding. It signifies your eternal love for eachother.\r\n",
                        victim->name );
               ring->description = STRALLOC( buf );
               break;
            }
         }

         ed = SetOExtra( ring, "inscription" );
         sprintf( buf, "The inscription reads:\r\nTo my lovely wife, yours forever, %s\r\n", victim->name );
         ed->description = STRALLOC( buf );
         break;
      }
      case SEX_MALE:
      case SEX_NEUTRAL:
      default:
      {
         ring = create_object( get_obj_index( OBJ_VNUM_WEDDING_BAND ), 0 );
         switch ( victim->sex )
         {
            case SEX_FEMALE:
            {
               sprintf( buf,
                        "This is the ring given to you by your beautifull wife %s\r\nat your wedding. It signifies your eternal love for eachother.\r\n",
                        victim->name );
               ring->description = STRALLOC( buf );
               break;
            }
            case SEX_MALE:
            {
               sprintf( buf,
                        "This is the ring given to you by your handsome husband %s\r\nat your wedding. It signifies your eternal love for eachother.\r\n",
                        victim->name );
               ring->description = STRALLOC( buf );
               break;
            }
            case SEX_NEUTRAL:
            default:
            {
               sprintf( buf,
                        "This is the ring given to you by your spouse %s at\r\nyour wedding. It signifies your eternal love for eachother.\r\n",
                        victim->name );
               ring->description = STRALLOC( buf );
               break;
            }
         }

         ed = SetOExtra( ring, "inscription" );

         switch ( victim->sex )
         {
            default:
            case SEX_MALE:
            {
               sprintf( buf, "The inscription reads:\r\nTo my handsome husband... Forever yours, %s\r\n", victim->name );
               ed->description = STRALLOC( buf );
               break;
            }
            case SEX_NEUTRAL:
            {
               sprintf( buf, "The inscription reads:\r\nForever love, %s\r\n", victim->name );
               ed->description = STRALLOC( buf );
               break;
            }
         }
      }
   }

   obj_to_char( ring, victim );

   return;
}
