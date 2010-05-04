/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

#include "mud.h"
#include "gboards.h"

DO_FUN *skill_function( char *name )
{
   switch ( name[3] )
   {
      case 'a':
         if( !str_cmp( name, "do_aassign" ) )
            return do_aassign;
         if( !str_cmp( name, "do_accessories" ) )
            return do_accessories;
         if( !str_cmp( name, "do_abit" ) )
            return do_abit;
         if( !str_cmp( name, "do_advance" ) )
            return do_advance;
         if( !str_cmp( name, "do_aentrance" ) )
            return do_aentrance;
         if( !str_cmp( name, "do_aexit" ) )
            return do_aexit;
         if( !str_cmp( name, "do_addchange" ) )
            return do_addchange;
         if( !str_cmp( name, "do_add_imm_host" ) )
            return do_add_imm_host;
         if( !str_cmp( name, "do_affected" ) )
            return do_affected;
         if( !str_cmp( name, "do_afk" ) )
            return do_afk;
         if( !str_cmp( name, "do_aid" ) )
            return do_aid;
         if( !str_cmp( name, "do_alias" ) )
            return do_alias;
         if( !str_cmp( name, "do_allow" ) )
            return do_allow;
         if( !str_cmp( name, "do_ansi" ) )
            return do_ansi;
         if( !str_cmp( name, "do_apply" ) )
            return do_apply;
         if( !str_cmp( name, "do_appraise" ) )
            return do_appraise;
         if( !str_cmp( name, "do_aquest" ) )
            return do_aquest;
         if( !str_cmp( name, "do_areas" ) )
            return do_areas;
         /*
          * Scion arenacode 
          */
         if( !str_cmp( name, "do_arena" ) )
            return do_arena;
         if( !str_cmp( name, "do_aset" ) )
            return do_aset;
         if( !str_cmp( name, "do_assassinate" ) )
            return do_assassinate;
         if( !str_cmp( name, "do_astat" ) )
            return do_astat;
         if( !str_cmp( name, "do_at" ) )
            return do_at;
         if( !str_cmp( name, "do_atmob" ) )
            return do_atmob;
         if( !str_cmp( name, "do_atobj" ) )
            return do_atobj;
         if( !str_cmp( name, "do_auction" ) )
            return do_auction;
         if( !str_cmp( name, "do_authorize" ) )
            return do_authorize;
         if( !str_cmp( name, "do_avator" ) )
            return do_avator;
         if( !str_cmp( name, "do_avhelp" ) )
            return do_avhelp;
         break;
      case 'b':
         if( !str_cmp( name, "do_backstab" ) )
            return do_backstab;
         if( !str_cmp( name, "do_backup" ) )
            return do_backup;
         if( !str_cmp( name, "do_balance" ) )
            return do_balance;
         if( !str_cmp( name, "do_balzhur" ) )
            return do_balzhur;
         if( !str_cmp( name, "do_bamfin" ) )
            return do_bamfin;
         if( !str_cmp( name, "do_bamfout" ) )
            return do_bamfout;
         if( !str_cmp( name, "do_ban" ) )
            return do_ban;
         if( !str_cmp( name, "do_bandage" ) )
            return do_bandage;
         if( !str_cmp( name, "do_bash" ) )
            return do_bash;
         if( !str_cmp( name, "do_bashdoor" ) )
            return do_bashdoor;
         if( !str_cmp( name, "do_beep" ) )
            return do_beep;
         if( !str_cmp( name, "do_berserk" ) )
            return do_berserk;
         if( !str_cmp( name, "do_bestow" ) )
            return do_bestow;
         if( !str_cmp( name, "do_bestowarea" ) )
            return do_bestowarea;
         if( !str_cmp( name, "do_bio" ) )
            return do_bio;
         if( !str_cmp( name, "do_bite" ) )
            return do_bite;
         if( !str_cmp( name, "do_bloodlet" ) )
            return do_bloodlet;
         if( !str_cmp( name, "do_boards" ) )
            return do_boards;
         if( !str_cmp( name, "do_bodybag" ) )
            return do_bodybag;
         if( !str_cmp( name, "do_bounty" ) )
            return do_bounty;
         if( !str_cmp( name, "do_brandish" ) )
            return do_brandish;
         if( !str_cmp( name, "do_brew" ) )
            return do_brew;
         if( !str_cmp( name, "do_broach" ) )
            return do_broach;
         if( !str_cmp( name, "do_bset" ) )
            return do_bset;
         if( !str_cmp( name, "do_bstat" ) )
            return do_bstat;
         if( !str_cmp( name, "do_bug" ) )
            return do_bug;
         if( !str_cmp( name, "do_bury" ) )
            return do_bury;
         if( !str_cmp( name, "do_buy" ) )
            return do_buy;
         if( !str_cmp( name, "do_buylocker" ) )
            return do_buylocker;
         break;
      case 'c':
         if( !str_cmp( name, "do_cast" ) )
            return do_cast;
         if( !str_cmp( name, "do_cbackup" ) )
            return do_cbackup;
         if( !str_cmp( name, "do_cedit" ) )
            return do_cedit;
         if( !str_cmp( name, "do_changes" ) )
            return do_changes;
         if( !str_cmp( name, "do_channels" ) )
            return do_channels;
         if( !str_cmp( name, "do_chsave" ) )
            return do_chsave;
         if( !str_cmp( name, "do_check_vnums" ) )
            return do_check_vnums;
         if( !str_cmp( name, "do_check_links" ) )
            return do_check_links;
         if( !str_cmp( name, "do_circle" ) )
            return do_circle;
         if( !str_cmp( name, "do_clans" ) )
            return do_clans;
         if( !str_cmp( name, "do_classes" ) )
            return do_classes;
         if( !str_cmp( name, "do_cleararea" ) )
            return do_cleararea;
         if( !str_cmp( name, "do_cleanse" ) )
            return do_cleanse;
         if( !str_cmp( name, "do_climate" ) )
            return do_climate;
         if( !str_cmp( name, "do_climb" ) )
            return do_climb;
         if( !str_cmp( name, "do_close" ) )
            return do_close;
         if( !str_cmp( name, "do_cmdtable" ) )
            return do_cmdtable;
         if( !str_cmp( name, "do_commands" ) )
            return do_commands;
         if( !str_cmp( name, "do_comment" ) )
            return do_comment;
/*	if ( !str_cmp( name, "do_compare" ))		return do_compare; */
#ifdef MCCP
         if( !str_cmp( name, "do_compress" ) )
            return do_compress;
#endif
         if( !str_cmp( name, "do_config" ) )
            return do_config;
         if( !str_cmp( name, "do_connect" ) )
            return do_connect;
         if( !str_cmp( name, "do_consider" ) )
            return do_consider;
         if( !str_cmp( name, "do_copyover" ) )
            return do_copyover;
         if( !str_cmp( name, "do_corpse" ) )
            return do_corpse;
         if( !str_cmp( name, "do_cook" ) )
            return do_cook;
         if( !str_cmp( name, "do_council_induct" ) )
            return do_council_induct;
         if( !str_cmp( name, "do_council_outcast" ) )
            return do_council_outcast;
         if( !str_cmp( name, "do_councils" ) )
            return do_councils;
         if( !str_cmp( name, "do_credits" ) )
            return do_credits;
         if( !str_cmp( name, "do_cset" ) )
            return do_cset;
         break;
      case 'd':
         if( !str_cmp( name, "do_deities" ) )
            return do_deities;
         if( !str_cmp( name, "do_delay" ) )
            return do_delay;
         if( !str_cmp( name, "do_deny" ) )
            return do_deny;
         if( !str_cmp( name, "do_deposit" ) )
            return do_deposit;
         if( !str_cmp( name, "do_description" ) )
            return do_description;
         if( !str_cmp( name, "do_destro" ) )
            return do_destro;
         if( !str_cmp( name, "do_destroy" ) )
            return do_destroy;
	 if( !str_cmp( name, "do_destroychannel" ) )
	    return do_destroychannel;
         if( !str_cmp( name, "do_destroyslay" ) )
            return do_destroyslay;
         if( !str_cmp( name, "do_destroynation" ) )
            return do_destroynation;
         if( !str_cmp( name, "do_destroyhometown" ) )
            return do_destroyhometown;
         if( !str_cmp( name, "do_detrap" ) )
            return do_detrap;
         if( !str_cmp( name, "do_devote" ) )
            return do_devote;
         if( !str_cmp( name, "do_dig" ) )
            return do_dig;
         if( !str_cmp( name, "do_disable" ) )
            return do_disable;
         if( !str_cmp( name, "do_disarm" ) )
            return do_disarm;
         if( !str_cmp( name, "do_disconnect" ) )
            return do_disconnect;
         if( !str_cmp( name, "do_dismiss" ) )
            return do_dismiss;
         if( !str_cmp( name, "do_dismount" ) )
            return do_dismount;
         if( !str_cmp( name, "do_divorce" ) )
            return do_divorce;
         if( !str_cmp( name, "do_dmesg" ) )
            return do_dmesg;
         if( !str_cmp( name, "do_donate" ) )
            return do_donate;
         if( !str_cmp( name, "do_down" ) )
            return do_down;
         if( !str_cmp( name, "do_drag" ) )
            return do_drag;
         if( !str_cmp( name, "do_drink" ) )
            return do_drink;
         if( !str_cmp( name, "do_drop" ) )
            return do_drop;
         if( !str_cmp( name, "do_diagnose" ) )
            return do_diagnose;
         break;
      case 'e':
         if( !str_cmp( name, "do_east" ) )
            return do_east;
         if( !str_cmp( name, "do_eat" ) )
            return do_eat;
         if( !str_cmp( name, "do_echo" ) )
            return do_echo;
         if( !str_cmp( name, "do_editnation" ) )
            return do_editnation;
         if( !str_cmp( name, "do_edithometown" ) )
            return do_edithometown;
         if( !str_cmp( name, "do_editnews" ) )
            return do_editnews;
         if( !str_cmp( name, "do_elevate" ) )
            return do_elevate;
         if( !str_cmp( name, "do_email" ) )
            return do_email;
         if( !str_cmp( name, "do_emote" ) )
            return do_emote;
         if( !str_cmp( name, "do_empty" ) )
            return do_empty;
         if( !str_cmp( name, "do_enter" ) )
            return do_enter;
         if( !str_cmp( name, "do_equipment" ) )
            return do_equipment;
         if( !str_cmp( name, "do_examine" ) )
            return do_examine;
         if( !str_cmp( name, "do_extraname" ) )
            return do_extraname;
         if( !str_cmp( name, "do_exits" ) )
            return do_exits;
         break;
      case 'f':
         if( !str_cmp( name, "do_feed" ) )
            return do_feed;
         if( !str_cmp( name, "do_fill" ) )
            return do_fill;
         if( !str_cmp( name, "do_findnote" ) )
            return do_findnote;
         if( !str_cmp( name, "do_fire" ) )
            return do_fire;
         if( !str_cmp( name, "do_fixchar" ) )
            return do_fixchar;
         if( !str_cmp( name, "do_fixed" ) )
            return do_fixed;
         if( !str_cmp( name, "do_flee" ) )
            return do_flee;
         if( !str_cmp( name, "do_foldarea" ) )
            return do_foldarea;
         if( !str_cmp( name, "do_follow" ) )
            return do_follow;
         if( !str_cmp( name, "do_for" ) )
            return do_for;
         if( !str_cmp( name, "do_force" ) )
            return do_force;
         if( !str_cmp( name, "do_forceclose" ) )
            return do_forceclose;
         if( !str_cmp( name, "do_form_password" ) )
            return do_form_password;
         if( !str_cmp( name, "do_fprompt" ) )
            return do_fprompt;
         if( !str_cmp( name, "do_fquit" ) )
            return do_fquit;
         if( !str_cmp( name, "do_free_vnums" ) )
            return do_free_vnums;
         if( !str_cmp( name, "do_freeze" ) )
            return do_freeze;
         if( !str_cmp( name, "do_fclear" ) )
            return do_fclear;
         if( !str_cmp( name, "do_fshow" ) )
            return do_fshow;
         break;
      case 'g':
         if( !str_cmp( name, "do_gaso" ) )
            return do_gaso;
         if( !str_cmp( name, "do_get" ) )
            return do_get;
         if( !str_cmp( name, "do_gfighting" ) )
            return do_gfighting;
         if( !str_cmp( name, "do_ghost" ) )
            return do_ghost;
         if( !str_cmp( name, "do_give" ) )
            return do_give;
         if( !str_cmp( name, "do_glance" ) )
            return do_glance;
         if( !str_cmp( name, "do_global_boards" ) )
            return do_global_boards;
         if( !str_cmp( name, "do_global_note" ) )
            return do_global_note;
         if( !str_cmp( name, "do_gold" ) )
            return do_gold;
         if( !str_cmp( name, "do_gohome" ) )
            return do_gohome;
         if( !str_cmp( name, "do_goto" ) )
            return do_goto;
         if( !str_cmp( name, "do_gouge" ) )
            return do_gouge;
         if( !str_cmp( name, "do_group" ) )
            return do_group;
         if( !str_cmp( name, "do_grub" ) )
            return do_grub;
         if( !str_cmp( name, "do_gtell" ) )
            return do_gtell;
         if( !str_cmp( name, "do_guestlist" ) )
            return do_guestlist;
         if( !str_cmp( name, "do_guilds" ) )
            return do_guilds;
         if( !str_cmp( name, "do_gwhere" ) )
            return do_gwhere;
         break;
      case 'h':
         if( !str_cmp( name, "do_heal" ) )
            return do_heal;
         if( !str_cmp( name, "do_hedit" ) )
            return do_hedit;
         if( !str_cmp( name, "do_hell" ) )
            return do_hell;
         if( !str_cmp( name, "do_help" ) )
            return do_help;
         if( !str_cmp( name, "do_hide" ) )
            return do_hide;
         if( !str_cmp( name, "do_hitall" ) )
            return do_hitall;
         if( !str_cmp( name, "do_hiscoset" ) )
            return do_hiscoset;
         if( !str_cmp( name, "do_hiscore" ) )
            return do_hiscore;
         if( !str_cmp( name, "do_hl" ) )
            return do_hl;
         if( !str_cmp( name, "do_hlist" ) )
            return do_hlist;
         if( !str_cmp( name, "do_holylight" ) )
            return do_holylight;
         if( !str_cmp( name, "do_homebuy" ) )
            return do_homebuy;
         if( !str_cmp( name, "do_house" ) )
            return do_house;
         if( !str_cmp( name, "do_homepage" ) )
            return do_homepage;
         if( !str_cmp( name, "do_hometowns" ) )
            return do_hometowns;
         if( !str_cmp( name, "do_hunt" ) )
            return do_hunt;
         if( !str_cmp( name, "do_hset" ) )
            return do_hset;
         if( !str_cmp( name, "do_hskelter" ) )
            return do_hskelter;
         break;
      case 'i':
         if( !str_cmp( name, "do_illegal_pk" ) )
            return do_illegal_pk;
         if( !str_cmp( name, "do_ide" ) )
            return do_ide;
         if( !str_cmp( name, "do_idea" ) )
            return do_idea;
         if( !str_cmp( name, "do_ignore" ) )
            return do_ignore;
         if( !str_cmp( name, "do_immlist" ) )
            return do_immlist;
         if( !str_cmp( name, "do_immortalize" ) )
            return do_immortalize;
         if( !str_cmp( name, "do_imm_exam" ) )
            return do_imm_exam;
         if( !str_cmp( name, "do_imm_morph" ) )
            return do_imm_morph;
         if( !str_cmp( name, "do_imm_unmorph" ) )
            return do_imm_unmorph;
         if( !str_cmp( name, "do_incognito" ) )
            return do_incognito;
         if( !str_cmp( name, "do_induct" ) )
            return do_induct;
         if( !str_cmp( name, "do_info" ) )
            return do_info;
         if( !str_cmp( name, "do_installarea" ) )
            return do_installarea;
         if( !str_cmp( name, "do_instaroom" ) )
            return do_instaroom;
         if( !str_cmp( name, "do_instazone" ) )
            return do_instazone;
/*        if ( !str_cmp( name, "do_intermud"))            return do_intermud; */
         if( !str_cmp( name, "do_invade" ) )
            return do_invade;
         if( !str_cmp( name, "do_inventory" ) )
            return do_inventory;
         if( !str_cmp( name, "do_invis" ) )
            return do_invis;
         if( !str_cmp( name, "do_ipcompare" ) )
            return do_ipcompare;
         if( !str_cmp( name, "do_iscore" ) )
            return do_iscore;
         if( !str_cmp( name, "do_ispell" ) )
            return do_ispell;
         break;
      case 'j':
         if( !str_cmp( name, "do_junk" ) )
            return do_junk;
         break;
      case 'k':
         if( !str_cmp( name, "do_khistory" ) )
            return do_khistory;
         if( !str_cmp( name, "do_kick" ) )
            return do_kick;
         if( !str_cmp( name, "do_kill" ) )
            return do_kill;
         if( !str_cmp( name, "do_kills" ) )
            return do_kills;
         break;
      case 'l':
         if( !str_cmp( name, "do_languages" ) )
            return do_languages;
         if( !str_cmp( name, "do_last" ) )
            return do_last;

         if( !str_cmp( name, "do_lastname" ) )
            return do_lastname;
         if( !str_cmp( name, "do_laws" ) )
            return do_laws;
         if( !str_cmp( name, "do_leave" ) )
            return do_leave;
         if( !str_cmp( name, "do_level" ) )
            return do_level;
         if( !str_cmp( name, "do_light" ) )
            return do_light;
         if( !str_cmp( name, "do_list" ) )
            return do_list;
         if( !str_cmp( name, "do_listen" ) )
            return do_listen;
         if( !str_cmp( name, "do_litterbug" ) )
            return do_litterbug;
         if( !str_cmp( name, "do_loadarea" ) )
            return do_loadarea;
         if( !str_cmp( name, "do_loadup" ) )
            return do_loadup;
         if( !str_cmp( name, "do_lock" ) )
            return do_lock;
         if( !str_cmp( name, "do_locker" ) )
            return do_locker;
         if( !str_cmp( name, "do_log" ) )
            return do_log;
         if( !str_cmp( name, "do_look" ) )
            return do_look;
/*	if ( !str_cmp( name, "do_lookmap" ))		return do_lookmap; */
         if( !str_cmp( name, "do_low_purge" ) )
            return do_low_purge;
         break;
      case 'm':
         if( !str_cmp( name, "do_mailroom" ) )
            return do_mailroom;
         if( !str_cmp( name, "do_make" ) )
            return do_make;
         if( !str_cmp( name, "do_makeboard" ) )
            return do_makeboard;
         if( !str_cmp( name, "do_makechannel" ) )
            return do_makechannel;
         if( !str_cmp( name, "do_makeclan" ) )
            return do_makeclan;
         if( !str_cmp( name, "do_makecouncil" ) )
            return do_makecouncil;
         if( !str_cmp( name, "do_makedeity" ) )
            return do_makedeity;
         if( !str_cmp( name, "do_makelocker" ) )
            return do_makelocker;
         if( !str_cmp( name, "do_makerepair" ) )
            return do_makerepair;
         if( !str_cmp( name, "do_makenation" ) )
            return do_makenation;
         if( !str_cmp( name, "do_makehometown" ) )
            return do_makehometown;
         if( !str_cmp( name, "do_makeshop" ) )
            return do_makeshop;
         if( !str_cmp( name, "do_makeslay" ) )
            return do_makeslay;
         if( !str_cmp( name, "do_makewizlist" ) )
            return do_makewizlist;
         if( !str_cmp( name, "do_mana" ) )
            return do_mana;
         if( !str_cmp( name, "do_map" ) )
            return do_map;
/*	if ( !str_cmp( name, "do_mapout" ))		return do_mapout; */
         if( !str_cmp( name, "do_massign" ) )
            return do_massign;
         if( !str_cmp( name, "do_marry" ) )
            return do_marry;
         if( !str_cmp( name, "do_mcreate" ) )
            return do_mcreate;
         if( !str_cmp( name, "do_mdelete" ) )
            return do_mdelete;
         if( !str_cmp( name, "do_memorize" ) )
            return do_memorize;
         if( !str_cmp( name, "do_memory" ) )
            return do_memory;
         if( !str_cmp( name, "do_mfind" ) )
            return do_mfind;
         if( !str_cmp( name, "do_minvoke" ) )
            return do_minvoke;
	 if( !str_cmp( name, "do_meditate" ) )
            return do_meditate;
         if( !str_cmp( name, "do_mistwalk" ) )
            return do_mistwalk;
         if( !str_cmp( name, "do_mlist" ) )
            return do_mlist;
         if( !str_cmp( name, "do_moblog" ) )
            return do_moblog;
         if( !str_cmp( name, "do_morphcreate" ) )
            return do_morphcreate;
         if( !str_cmp( name, "do_morphdestroy" ) )
            return do_morphdestroy;
         if( !str_cmp( name, "do_morphlist" ) )
            return do_morphlist;
         if( !str_cmp( name, "do_morphset" ) )
            return do_morphset;
         if( !str_cmp( name, "do_morphstat" ) )
            return do_morphstat;

         if( !str_cmp( name, "do_mortalize" ) )
            return do_mortalize;
         if( !str_cmp( name, "do_mount" ) )
            return do_mount;
         if( !str_cmp( name, "do_motd" ) )
            return do_motd;
         if( !str_cmp( name, "do_mp_close_passage" ) )
            return do_mp_close_passage;
         if( !str_cmp( name, "do_mp_damage" ) )
            return do_mp_damage;
         if( !str_cmp( name, "do_mp_deposit" ) )
            return do_mp_deposit;
         if( !str_cmp( name, "do_mp_fill_in" ) )
            return do_mp_fill_in;
         if( !str_cmp( name, "do_mp_log" ) )
            return do_mp_log;
         if( !str_cmp( name, "do_mp_open_passage" ) )
            return do_mp_open_passage;
         if( !str_cmp( name, "do_mp_practice" ) )
            return do_mp_practice;
         if( !str_cmp( name, "do_mp_restore" ) )
            return do_mp_restore;
         if( !str_cmp( name, "do_mp_slay" ) )
            return do_mp_slay;
         if( !str_cmp( name, "do_mp_withdraw" ) )
            return do_mp_withdraw;
         if( !str_cmp( name, "do_mpadvance" ) )
            return do_mpadvance;
         if( !str_cmp( name, "do_mpapply" ) )
            return do_mpapply;
         if( !str_cmp( name, "do_mpapplyb" ) )
            return do_mpapplyb;
         if( !str_cmp( name, "do_mpaset" ) )
            return do_mpaset;
         if( !str_cmp( name, "do_mpasound" ) )
            return do_mpasound;
         if( !str_cmp( name, "do_mpasupress" ) )
            return do_mpasupress;
         if( !str_cmp( name, "do_mpat" ) )
            return do_mpat;
         if( !str_cmp( name, "do_mpbodybag" ) )
            return do_mpbodybag;
         if( !str_cmp( name, "do_mp_makecitizen" ) )
            return do_mp_makecitizen;
         if( !str_cmp( name, "do_mp_revokecitizen" ) )
            return do_mp_revokecitizen;
         if( !str_cmp( name, "do_mpdelay" ) )
            return do_mpdelay;
         if( !str_cmp( name, "do_mpdream" ) )
            return do_mpdream;
         if( !str_cmp( name, "do_mpecho" ) )
            return do_mpecho;
         if( !str_cmp( name, "do_mpechoaround" ) )
            return do_mpechoaround;
         if( !str_cmp( name, "do_mpechoat" ) )
            return do_mpechoat;
         if( !str_cmp( name, "do_mpechozone" ) )
            return do_mpechozone;
         if( !str_cmp( name, "do_mpedit" ) )
            return do_mpedit;
         if( !str_cmp( name, "do_mpfavor" ) )
            return do_mpfavor;
         if( !str_cmp( name, "do_mpforce" ) )
            return do_mpforce;
         if( !str_cmp( name, "do_mpgoto" ) )
            return do_mpgoto;
         if( !str_cmp( name, "do_mpinvis" ) )
            return do_mpinvis;
         if( !str_cmp( name, "do_mpjunk" ) )
            return do_mpjunk;
         if( !str_cmp( name, "do_mpkill" ) )
            return do_mpkill;
         if( !str_cmp( name, "do_mpmload" ) )
            return do_mpmload;
         if( !str_cmp( name, "do_mpmorph" ) )
            return do_mpmorph;
         if( !str_cmp( name, "do_mpmset" ) )
            return do_mpmset;
         if( !str_cmp( name, "do_mpmusic" ) )
            return do_mpmusic;
         if( !str_cmp( name, "do_mpmusicaround" ) )
            return do_mpmusicaround;
         if( !str_cmp( name, "do_mpmusicat" ) )
            return do_mpmusicat;
         if( !str_cmp( name, "do_mpnothing" ) )
            return do_mpnothing;
         if( !str_cmp( name, "do_mpnuisance" ) )
            return do_nuisance;
         if( !str_cmp( name, "do_mpoload" ) )
            return do_mpoload;
         if( !str_cmp( name, "do_mposet" ) )
            return do_mposet;
         if( !str_cmp( name, "do_mppardon" ) )
            return do_mppardon;

         if( !str_cmp( name, "do_mpqpadd" ) )
            return do_mpqpadd;

         if( !str_cmp( name, "do_mpmarry" ) )
            return do_mpmarry;

         if( !str_cmp( name, "do_mpdivorce" ) )
            return do_mpdivorce;

         if( !str_cmp( name, "do_mppeace" ) )
            return do_mppeace;
         if( !str_cmp( name, "do_mppkset" ) )
            return do_mppkset;
         if( !str_cmp( name, "do_mppurge" ) )
            return do_mppurge;
         if( !str_cmp( name, "do_mpqset" ) )
            return do_mpqset;
         if( !str_cmp( name, "do_mpscatter" ) )
            return do_mpscatter;
         if( !str_cmp( name, "do_mpsound" ) )
            return do_mpsound;
         if( !str_cmp( name, "do_mpsoundaround" ) )
            return do_mpsoundaround;
         if( !str_cmp( name, "do_mpsoundat" ) )
            return do_mpsoundat;
         if( !str_cmp( name, "do_mpstat" ) )
            return do_mpstat;
         if( !str_cmp( name, "do_mptransfer" ) )
            return do_mptransfer;
         if( !str_cmp( name, "do_mpunmorph" ) )
            return do_mpunmorph;
         if( !str_cmp( name, "do_mpunnuisance" ) )
            return do_mpunnuisance;
         if( !str_cmp( name, "do_mrange" ) )
            return do_mrange;
         if( !str_cmp( name, "do_mset" ) )
            return do_mset;
         if( !str_cmp( name, "do_mstat" ) )
            return do_mstat;
         if( !str_cmp( name, "do_murde" ) )
            return do_murde;
         if( !str_cmp( name, "do_murder" ) )
            return do_murder;
         if( !str_cmp( name, "do_mwhere" ) )
            return do_mwhere;
         break;
      case 'n':
         if( !str_cmp( name, "do_name" ) )
            return do_name;
         if( !str_cmp( name, "do_nations" ) )
            return do_nations;
         if( !str_cmp( name, "do_newbieset" ) )
            return do_newbieset;
         /*
          * if ( !str_cmp( name, "do_news" ))               return do_news;
          */
         if( !str_cmp( name, "do_newscore" ) )
            return do_newscore;
         if( !str_cmp( name, "do_newscore2" ) )
            return do_newscore2;
         if( !str_cmp( name, "do_newzones" ) )
            return do_newzones;
         if( !str_cmp( name, "do_noemote" ) )
            return do_noemote;
         if( !str_cmp( name, "do_nohelps" ) )
            return do_nohelps;
         if( !str_cmp( name, "do_noresolve" ) )
            return do_noresolve;
         if( !str_cmp( name, "do_north" ) )
            return do_north;
         if( !str_cmp( name, "do_northeast" ) )
            return do_northeast;
         if( !str_cmp( name, "do_northwest" ) )
            return do_northwest;
         if( !str_cmp( name, "do_notell" ) )
            return do_notell;
         if( !str_cmp( name, "do_notitle" ) )
            return do_notitle;
         if( !str_cmp( name, "do_noteroom" ) )
            return do_noteroom;
         if( !str_cmp( name, "do_nuisance" ) )
            return do_nuisance;
         break;
      case 'o':
         if( !str_cmp( name, "do_oassign" ) )
            return do_oassign;
         if( !str_cmp( name, "do_observe" ) )
            return do_observe;
         if( !str_cmp( name, "do_ocopy" ) )
            return do_ocopy;
         if( !str_cmp( name, "do_ocreate" ) )
            return do_ocreate;
         if( !str_cmp( name, "do_odelete" ) )
            return do_odelete;
         if( !str_cmp( name, "do_ofind" ) )
            return do_ofind;
         if( !str_cmp( name, "do_ogrub" ) )
            return do_ogrub;
         if( !str_cmp( name, "do_oinvoke" ) )
            return do_oinvoke;
         if( !str_cmp( name, "do_oldreboot" ) )
            return do_oldreboot;
         if( !str_cmp( name, "do_oldscore" ) )
            return do_oldscore;
         if( !str_cmp( name, "do_oldslay" ) )
            return do_oldslay;
         if( !str_cmp( name, "do_olist" ) )
            return do_olist;
         if( !str_cmp( name, "do_omedit" ) )
            return do_omedit;
         if( !str_cmp( name, "do_ooedit" ) )
            return do_ooedit;
         if( !str_cmp( name, "do_oredit" ) )
            return do_oredit;
         if( !str_cmp( name, "do_opedit" ) )
            return do_opedit;
         if( !str_cmp( name, "do_open" ) )
            return do_open;
/*	if ( !str_cmp( name, "do_opentourney" ))	return do_opentourney; */
         if( !str_cmp( name, "do_opstat" ) )
            return do_opstat;
         if( !str_cmp( name, "do_orange" ) )
            return do_orange;
         if( !str_cmp( name, "do_order" ) )
            return do_order;
         if( !str_cmp( name, "do_orders" ) )
            return do_orders;

         if( !str_cmp( name, "do_oset" ) )
            return do_oset;
         if( !str_cmp( name, "do_osearch" ) )
            return do_osearch;
         if( !str_cmp( name, "do_ostat" ) )
            return do_ostat;
         if( !str_cmp( name, "do_outcast" ) )
            return do_outcast;

         if( !str_cmp( name, "do_outputprefix" ) )
            return do_outputprefix;

         if( !str_cmp( name, "do_outputsuffix" ) )
            return do_outputsuffix;
         if( !str_cmp( name, "do_owhere" ) )
            return do_owhere;
         break;
      case 'p':
         if( !str_cmp( name, "do_pager" ) )
            return do_pager;
         if( !str_cmp( name, "do_panic" ) )
            return do_panic;
         if( !str_cmp( name, "do_pardon" ) )
            return do_pardon;
         if( !str_cmp( name, "do_password" ) )
            return do_password;
         if( !str_cmp( name, "do_pcrename" ) )
            return do_pcrename;
         if( !str_cmp( name, "do_peace" ) )
            return do_peace;
         if( !str_cmp( name, "do_pfiles" ) )
            return do_pfiles;
         if( !str_cmp( name, "do_pick" ) )
            return do_pick;
         if( !str_cmp( name, "do_pledge" ) )
            return do_pledge;
         if( !str_cmp( name, "do_plist" ) )
            return do_plist;
         if( !str_cmp( name, "do_poison_weapon" ) )
            return do_poison_weapon;
         if( !str_cmp( name, "do_practice" ) )
            return do_practice;
         if( !str_cmp( name, "do_project" ) )
            return do_project;
         if( !str_cmp( name, "do_promote" ) )
            return do_promote;
         if( !str_cmp( name, "do_prompt" ) )
            return do_prompt;
         if( !str_cmp( name, "do_pset" ) )
            return do_pset;
         if( !str_cmp( name, "do_pstat" ) )
            return do_pstat;
         if( !str_cmp( name, "do_pull" ) )
            return do_pull;
         if( !str_cmp( name, "do_punch" ) )
            return do_punch;
         if( !str_cmp( name, "do_purge" ) )
            return do_purge;
         if( !str_cmp( name, "do_push" ) )
            return do_push;
         if( !str_cmp( name, "do_put" ) )
            return do_put;
         break;
      case 'q':
         if( !str_cmp( name, "do_qoutcast" ) )
            return do_qoutcast;
         if( !str_cmp( name, "do_qbit" ) )
            return do_qbit;
         if( !str_cmp( name, "do_qpset" ) )
            return do_qpset;
         if( !str_cmp( name, "do_qpstat" ) )
            return do_qpstat;
         if( !str_cmp( name, "do_quaff" ) )
            return do_quaff;
         if( !str_cmp( name, "do_qui" ) )
            return do_qui;
         if( !str_cmp( name, "do_quit" ) )
            return do_quit;
         break;
      case 'r':
         if( !str_cmp( name, "do_races" ) )
            return do_races;
         if( !str_cmp( name, "do_rank" ) )
            return do_rank;
         if( !str_cmp( name, "do_rap" ) )
            return do_rap;
         if( !str_cmp( name, "do_rassign" ) )
            return do_rassign;
         if( !str_cmp( name, "do_rat" ) )
            return do_rat;
         if( !str_cmp( name, "do_rdelete" ) )
            return do_rdelete;
         if( !str_cmp( name, "do_reboo" ) )
            return do_reboo;
         if( !str_cmp( name, "do_reboot" ) )
            return do_reboot;
         if( !str_cmp( name, "do_recall" ) )
            return do_recall;
         if( !str_cmp( name, "do_recho" ) )
            return do_recho;
         if( !str_cmp( name, "do_recite" ) )
            return do_recite;
         if( !str_cmp( name, "do_redit" ) )
            return do_redit;
         if( !str_cmp( name, "do_regoto" ) )
            return do_regoto;
         if( !str_cmp( name, "do_remove" ) )
            return do_remove;
         if( !str_cmp( name, "do_remort" ) )
            return do_remort;
         if( !str_cmp( name, "do_remains" ) )
            return do_remains;
         if( !str_cmp( name, "do_rent" ) )
            return do_rent;
         if( !str_cmp( name, "do_repair" ) )
            return do_repair;
         if( !str_cmp( name, "do_repairset" ) )
            return do_repairset;
         if( !str_cmp( name, "do_repairshops" ) )
            return do_repairshops;
         if( !str_cmp( name, "do_repairstat" ) )
            return do_repairstat;
         if( !str_cmp( name, "do_reply" ) )
            return do_reply;
         if( !str_cmp( name, "do_report" ) )
            return do_report;
         if( !str_cmp( name, "do_request" ) )
            return do_request;
         if( !str_cmp( name, "do_reroll" ) )
            return do_reroll;
         if( !str_cmp( name, "do_rescue" ) )
            return do_rescue;
         if( !str_cmp( name, "do_reserve" ) )
            return do_reserve;
         if( !str_cmp( name, "do_reset" ) )
            return do_reset;
         if( !str_cmp( name, "do_residence" ) )
            return do_residence;
         if( !str_cmp( name, "do_rest" ) )
            return do_rest;
         if( !str_cmp( name, "do_restore" ) )
            return do_restore;
         if( !str_cmp( name, "do_restoretime" ) )
            return do_restoretime;
         if( !str_cmp( name, "do_restrict" ) )
            return do_restrict;
         if( !str_cmp( name, "do_retire" ) )
            return do_retire;
         if( !str_cmp( name, "do_retran" ) )
            return do_retran;
         if( !str_cmp( name, "do_retrieve" ) )
            return do_retrieve;
         if( !str_cmp( name, "do_return" ) )
            return do_return;
         if( !str_cmp( name, "do_revert" ) )
            return do_revert;
         if( !str_cmp( name, "do_rgrub" ) )
            return do_rgrub;
         if( !str_cmp( name, "do_rip" ) )
            return do_rip;
         if( !str_cmp( name, "do_rings" ) )
            return do_rings;
         if( !str_cmp( name, "do_rlist" ) )
            return do_rlist;
         if( !str_cmp( name, "do_roload" ) )
            return do_roload;
         if( !str_cmp( name, "do_rpedit" ) )
            return do_rpedit;
         if( !str_cmp( name, "do_rpstat" ) )
            return do_rpstat;
         if( !str_cmp( name, "do_rset" ) )
            return do_rset;
         if( !str_cmp( name, "do_rstat" ) )
            return do_rstat;
         if( !str_cmp( name, "do_ruminate" ) ) 
            return do_ruminate;
         break;
      case 's':
         if( !str_cmp( name, "do_sharpen" ) )
            return do_sharpen;   /* sharpen skill --Cronel */
         if( !str_cmp( name, "do_sacrifice" ) )
            return do_sacrifice;
         if( !str_cmp( name, "do_save" ) )
            return do_save;
         if( !str_cmp( name, "do_saveall" ) )
            return do_saveall;
         if( !str_cmp( name, "do_savearea" ) )
            return do_savearea;
         if( !str_cmp( name, "do_say" ) )
            return do_say;
         if( !str_cmp( name, "do_say_to_char" ) )
            return do_say_to_char;
         if( !str_cmp( name, "do_scan" ) )
            return do_scan;
         if( !str_cmp( name, "do_scatter" ) )
            return do_scatter;
         if( !str_cmp( name, "do_score" ) )
            return do_score;
         if( !str_cmp( name, "do_scribe" ) )
            return do_scribe;
         if( !str_cmp( name, "do_search" ) )
            return do_search;
         if( !str_cmp( name, "do_search_flags" ) )
            return do_search_flags;
         if( !str_cmp( name, "do_sedit" ) )
            return do_sedit;
         if( !str_cmp( name, "do_sell" ) )
            return do_sell;
         if( !str_cmp( name, "do_sellhouse" ) )
            return do_sellhouse;
         if( !str_cmp( name, "do_semote" ) )
            return do_semote;
         if( !str_cmp( name, "do_sendmail" ) )
            return do_sendmail;
         if( !str_cmp( name, "do_set" ) )
            return do_set;
         if( !str_cmp( name, "do_setabit" ) )
            return do_setabit;
         if( !str_cmp( name, "do_set_boot_time" ) )
            return do_set_boot_time;
         if( !str_cmp( name, "do_setchannel" ) )
            return do_setchannel;
         if( !str_cmp( name, "do_showchannels" ) )
            return do_showchannels;
         if( !str_cmp( name, "do_setclan" ) )
            return do_setclan;
         if( !str_cmp( name, "do_setclass" ) )
            return do_setclass;
         if( !str_cmp( name, "do_setcouncil" ) )
            return do_setcouncil;
         if( !str_cmp( name, "do_setdeity" ) )
            return do_setdeity;
         if( !str_cmp( name, "do_setqbit" ) )
            return do_setqbit;
         if( !str_cmp( name, "do_setrace" ) )
            return do_setrace;
         if( !str_cmp( name, "do_setslay" ) )
            return do_setslay;
         if( !str_cmp( name, "do_setweather" ) )
            return do_setweather;
         if( !str_cmp( name, "do_shops" ) )
            return do_shops;
         if( !str_cmp( name, "do_shopset" ) )
            return do_shopset;
         if( !str_cmp( name, "do_shopstat" ) )
            return do_shopstat;
         if( !str_cmp( name, "do_shove" ) )
            return do_shove;
         if( !str_cmp( name, "do_showabit" ) )
            return do_showabit;
         if( !str_cmp( name, "do_showclan" ) )
            return do_showclan;
         if( !str_cmp( name, "do_showclass" ) )
            return do_showclass;
         if( !str_cmp( name, "do_showcouncil" ) )
            return do_showcouncil;
         if( !str_cmp( name, "do_showdeity" ) )
            return do_showdeity;
         if( !str_cmp( name, "do_showqbit" ) )
            return do_showqbit;
         if( !str_cmp( name, "do_showrace" ) )
            return do_showrace;
         if( !str_cmp( name, "do_showslay" ) )
            return do_showslay;
         if( !str_cmp( name, "do_showweather" ) )
            return do_showweather;
         if( !str_cmp( name, "do_shutdow" ) )
            return do_shutdow;
         if( !str_cmp( name, "do_shutdown" ) )
            return do_shutdown;
         if( !str_cmp( name, "do_silence" ) )
            return do_silence;
         if( !str_cmp( name, "do_sit" ) )
            return do_sit;
         if( !str_cmp( name, "do_skin" ) )
            return do_skin;
         if( !str_cmp( name, "do_sla" ) )
            return do_sla;
         if( !str_cmp( name, "do_slay" ) )
            return do_slay;
         if( !str_cmp( name, "do_sleep" ) )
            return do_sleep;
         if( !str_cmp( name, "do_slice" ) )
            return do_slice;
         if( !str_cmp( name, "do_slist" ) )
            return do_slist;
         if( !str_cmp( name, "do_slotlist" ) )
            return do_slotlist;
         if( !str_cmp( name, "do_slookup" ) )
            return do_slookup;
         if( !str_cmp( name, "do_smoke" ) )
            return do_smoke;
         if( !str_cmp( name, "do_smite" ) )
            return do_smite;
         if( !str_cmp( name, "do_sneak" ) )
            return do_sneak;
         if( !str_cmp( name, "do_snoop" ) )
            return do_snoop;
         if( !str_cmp( name, "do_sober" ) )
            return do_sober;
         if( !str_cmp( name, "do_socials" ) )
            return do_socials;
         if( !str_cmp( name, "do_south" ) )
            return do_south;
         if( !str_cmp( name, "do_southeast" ) )
            return do_southeast;
         if( !str_cmp( name, "do_southwest" ) )
            return do_southwest;
         if( !str_cmp( name, "do_speak" ) )
            return do_speak;
         if( !str_cmp( name, "do_split" ) )
            return do_split;
         if( !str_cmp( name, "do_sset" ) )
            return do_sset;
         if( !str_cmp( name, "do_sslist" ) )
            return do_sslist;
         if( !str_cmp( name, "do_stand" ) )
            return do_stand;
         if( !str_cmp( name, "do_stat" ) )
            return do_stat;
         if( !str_cmp( name, "do_statreport" ) )
            return do_statreport;
         if( !str_cmp( name, "do_steal" ) )
            return do_steal;
         if( !str_cmp( name, "do_strew" ) )
            return do_strew;
         if( !str_cmp( name, "do_strip" ) )
            return do_strip;
         if( !str_cmp( name, "do_study" ) )
            return do_study;
         if( !str_cmp( name, "do_stun" ) )
            return do_stun;
         if( !str_cmp( name, "do_style" ) )
            return do_style;
         if( !str_cmp( name, "do_superbless" ) )
            return do_superbless;
         if( !str_cmp( name, "do_supplicate" ) )
            return do_supplicate;
         if( !str_cmp( name, "do_switch" ) )
            return do_switch;
         if( !str_cmp( name, "do_showlayers" ) )
            return do_showlayers;
         if( !str_cmp( name, "do_surrender" ) )
            return do_surrender;
         if( !str_cmp( name, "do_suicide" ) )
            return do_suicide;
         break;
      case 't':
         if( !str_cmp( name, "do_tamp" ) )
            return do_tamp;
         if( !str_cmp( name, "do_tell" ) )
            return do_tell;
         if( !str_cmp( name, "do_time" ) )
            return do_time;
         if( !str_cmp( name, "do_timecmd" ) )
            return do_timecmd;
         if( !str_cmp( name, "do_title" ) )
            return do_title;
         if( !str_cmp( name, "do_track" ) )
            return do_track;
         if( !str_cmp( name, "do_trivia" ) )
            return do_trivia;
         if( !str_cmp( name, "do_tlist" ) )
            return do_tlist;
         if( !str_cmp( name, "do_tlist" ) )
            return do_tlist;
         if( !str_cmp( name, "do_trans" ) )
            return do_trans;
         if( !str_cmp( name, "do_trance" ) )
            return do_trance;
         if( !str_cmp( name, "do_train" ) )
            return do_train;
         if( !str_cmp( name, "do_transfer" ) )
            return do_transfer;
         if( !str_cmp( name, "do_trust" ) )
            return do_trust;
         if( !str_cmp( name, "do_typo" ) )
            return do_typo;
         break;
      case 'u':
/*	if ( !str_cmp( name, "do_unalias" ))		return do_unalias; */
         if( !str_cmp( name, "do_unbounty" ) )
            return do_unbounty;
         if( !str_cmp( name, "do_unfoldarea" ) )
            return do_unfoldarea;
         if( !str_cmp( name, "do_unhell" ) )
            return do_unhell;
         if( !str_cmp( name, "do_unlock" ) )
            return do_unlock;
         if( !str_cmp( name, "do_unnuisance" ) )
            return do_unnuisance;
         if( !str_cmp( name, "do_unsilence" ) )
            return do_unsilence;
         if( !str_cmp( name, "do_up" ) )
            return do_up;
         if( !str_cmp( name, "do_users" ) )
            return do_users;
         if( !str_cmp( name, "do_users2" ) )
            return do_users2;
         break;
      case 'v':
         if( !str_cmp( name, "do_value" ) )
            return do_value;
         if( !str_cmp( name, "do_vassign" ) )
            return do_vassign;
         if( !str_cmp( name, "do_victories" ) )
            return do_victories;
         if( !str_cmp( name, "do_visible" ) )
            return do_visible;
         if( !str_cmp( name, "do_visogy" ) )
            return do_visogy;
         if( !str_cmp( name, "do_vnums" ) )
            return do_vnums;
         if( !str_cmp( name, "do_vsearch" ) )
            return do_vsearch;
         break;
      case 'w':
         if( !str_cmp( name, "do_wake" ) )
            return do_wake;
         if( !str_cmp( name, "do_warn" ) )
            return do_warn;
         if( !str_cmp( name, "do_watch" ) )
            return do_watch;
         if( !str_cmp( name, "do_wear" ) )
            return do_wear;
         if( !str_cmp( name, "do_weather" ) )
            return do_weather;
         if( !str_cmp( name, "do_west" ) )
            return do_west;
         if( !str_cmp( name, "do_wfilter" ) )
            return do_wfilter;
         if( !str_cmp( name, "do_where" ) )
            return do_where;
         if( !str_cmp( name, "do_whisper" ) )
            return do_whisper;
         if( !str_cmp( name, "do_who" ) )
            return do_who;
         if( !str_cmp( name, "do_whois" ) )
            return do_whois;
         if( !str_cmp( name, "do_wimpy" ) )
            return do_wimpy;
         if( !str_cmp( name, "do_withdraw" ) )
            return do_withdraw;
         if( !str_cmp( name, "do_wizhelp" ) )
            return do_wizhelp;
         if( !str_cmp( name, "do_wizinduct" ) )
            return do_wizinduct;
         if( !str_cmp( name, "do_wizlist" ) )
            return do_wizlist;
         if( !str_cmp( name, "do_wizlock" ) )
            return do_wizlock;
         if( !str_cmp( name, "do_worth" ) )
            return do_worth;
         break;
      case 'y':
         break;
      case 'z':
         if( !str_cmp( name, "do_zecho" ) )
            return do_zecho;
         if( !str_cmp( name, "do_zap" ) )
            return do_zap;
         if( !str_cmp( name, "do_zones" ) )
            return do_zones;
   }
   return skill_notfound;
}

char *skill_name( DO_FUN * skill )
{
   static char buf[64];

   if( skill == NULL )
      return "reserved";
   if( skill == do_aassign )
      return "do_aassign";
   if( skill == do_accessories )
      return "do_accessories";
   if( skill == do_abit )
      return "do_abit";
   if( skill == do_addchange )
      return "do_addchange";
   if( skill == do_advance )
      return "do_advance";
   if( skill == do_aentrance )
      return "do_aentrance";
   if( skill == do_aexit )
      return "do_aexit";
   if( skill == do_affected )
      return "do_affected";
   if( skill == do_afk )
      return "do_afk";
   if( skill == do_aid )
      return "do_aid";
   if( skill == do_alias )
      return "do_alias";
   if( skill == do_allow )
      return "do_allow";
   if( skill == do_ansi )
      return "do_ansi";
   if( skill == do_apply )
      return "do_apply";
   if( skill == do_appraise )
      return "do_appraise";
   if( skill == do_aquest )
      return "do_aquest";
   if( skill == do_areas )
      return "do_areas";
/* Scion arenacode */
   if( skill == do_arena )
      return "do_arena";
   if( skill == do_aset )
      return "do_aset";
   if( skill == do_assassinate )
      return "do_assassinate";
   if( skill == do_astat )
      return "do_astat";
   if( skill == do_at )
      return "do_at";
   if( skill == do_atmob )
      return "do_atmob";
   if( skill == do_atobj )
      return "do_atobj";
   if( skill == do_auction )
      return "do_auction";
   if( skill == do_authorize )
      return "do_authorize";
   if( skill == do_avator )
      return "do_avator";
   if( skill == do_avhelp )
      return "do_avhelp";
   if( skill == do_backstab )
      return "do_backstab";
   if( skill == do_backup )
      return "do_backup";
   if( skill == do_balance )
      return "do_balance";
   if( skill == do_balzhur )
      return "do_balzhur";
   if( skill == do_bamfin )
      return "do_bamfin";
   if( skill == do_bamfout )
      return "do_bamfout";
   if( skill == do_ban )
      return "do_ban";
   if( skill == do_bandage )
      return "do_bandage";
   if( skill == do_bash )
      return "do_bash";
   if( skill == do_bashdoor )
      return "do_bashdoor";
   if( skill == do_beep )
      return "do_beep";
   if( skill == do_berserk )
      return "do_berserk";
   if( skill == do_bestow )
      return "do_bestow";
   if( skill == do_bestowarea )
      return "do_bestowarea";
   if( skill == do_bio )
      return "do_bio";
   if( skill == do_bite )
      return "do_bite";
   if( skill == do_bloodlet )
      return "do_bloodlet";
   if( skill == do_boards )
      return "do_boards";
   if( skill == do_bodybag )
      return "do_bodybag";
   if( skill == do_bounty )
      return "do_bounty";
   if( skill == do_brandish )
      return "do_brandish";
   if( skill == do_brew )
      return "do_brew";
   if( skill == do_broach )
      return "do_broach";
   if( skill == do_bset )
      return "do_bset";
   if( skill == do_bstat )
      return "do_bstat";
   if( skill == do_bug )
      return "do_bug";
   if( skill == do_bury )
      return "do_bury";
   if( skill == do_buy )
      return "do_buy";
   if( skill == do_buylocker )
      return "do_buylocker";
   if( skill == do_cast )
      return "do_cast";
   if( skill == do_cbackup )
      return "do_cbackup";
   if( skill == do_cedit )
      return "do_cedit";
   if( skill == do_changes )
      return "do_changes";
   if( skill == do_channels )
      return "do_channels";
   if( skill == do_chsave )
      return "do_chsave";

   if( skill == do_check_vnums )
      return "do_check_vnums";
   if( skill == do_check_links )
      return "do_check_links";

   if( skill == do_circle )
      return "do_circle";
   if( skill == do_cleararea )
      return "do_cleararea";
   if( skill == do_clans )
      return "do_clans";
   if( skill == do_classes )
      return "do_classes";
   if( skill == do_cleanse )
      return "do_cleanse";
   if( skill == do_climate )
      return "do_climate";
   if( skill == do_climb )
      return "do_climb";
   if( skill == do_close )
      return "do_close";
   if( skill == do_cmdtable )
      return "do_cmdtable";
   if( skill == do_commands )
      return "do_commands";
   if( skill == do_comment )
      return "do_comment";
#ifdef MCCP
   if( skill == do_compress )
      return "do_compress";
#endif
   if( skill == do_cook )
      return "do_cook";
   if( skill == do_config )
      return "do_config";
   if( skill == do_connect )
      return "do_connect";
   if( skill == do_consider )
      return "do_consider";
   if( skill == do_copyover )
      return "do_copyover";
   if( skill == do_corpse )
      return "do_corpse";
   if( skill == do_council_induct )
      return "do_council_induct";
   if( skill == do_council_outcast )
      return "do_council_outcast";
   if( skill == do_councils )
      return "do_councils";
   if( skill == do_credits )
      return "do_credits";
   if( skill == do_cset )
      return "do_cset";
   if( skill == do_deities )
      return "do_deities";
   if( skill == do_delay )
      return "do_delay";
   if( skill == do_deny )
      return "do_deny";
   if( skill == do_deposit )
      return "do_deposit";
   if( skill == do_description )
      return "do_description";
   if( skill == do_destro )
      return "do_destro";
   if( skill == do_destroy )
      return "do_destroy";
   if( skill == do_destroynation )
      return "destroynation";
   if( skill == do_destroyhometown )
      return "destroyhometown";
   if( skill == do_destroyslay )
      return "do_destroyslay";
   if( skill == do_detrap )
      return "do_detrap";
   if( skill == do_devote )
      return "do_devote";
   if( skill == do_dig )
      return "do_dig";
   if( skill == do_disable )
      return "do_disable";
   if( skill == do_disarm )
      return "do_disarm";
   if( skill == do_disconnect )
      return "do_disconnect";
   if( skill == do_dismiss )
      return "do_dismiss";
   if( skill == do_dismount )
      return "do_dismount";
   if( skill == do_dmesg )
      return "do_dmesg";
   if( skill == do_donate )
      return "do_donate";
   if( skill == do_down )
      return "do_down";
   if( skill == do_drag )
      return "do_drag";
   if( skill == do_drink )
      return "do_drink";
   if( skill == do_drop )
      return "do_drop";
   if( skill == do_diagnose )
      return "do_diagnose";
   if( skill == do_divorce )
      return "do_divorce";
   if( skill == do_east )
      return "do_east";
   if( skill == do_eat )
      return "do_eat";
   if( skill == do_echo )
      return "do_echo";
   if( skill == do_editnation )
      return "do_editnation";
   if( skill == do_edithometown )
      return "do_edithometown";
   if( skill == do_editnews )
      return "do_editnews";
   if( skill == do_elevate )
      return "do_elevate";
   if( skill == do_email )
      return "do_email";
   if( skill == do_emote )
      return "do_emote";
   if( skill == do_empty )
      return "do_empty";
   if( skill == do_enter )
      return "do_enter";
   if( skill == do_equipment )
      return "do_equipment";
   if( skill == do_examine )
      return "do_examine";
   if( skill == do_extraname )
      return "do_extraname";
   if( skill == do_exits )
      return "do_exits";
   if( skill == do_feed )
      return "do_feed";
   if( skill == do_fill )
      return "do_fill";
   if( skill == do_findnote )
      return "do_findnote";
   if( skill == do_fire )
      return "do_fire";
   if( skill == do_fixchar )
      return "do_fixchar";
   if( skill == do_fixed )
      return "do_fixed";
   if( skill == do_flee )
      return "do_flee";
   if( skill == do_foldarea )
      return "do_foldarea";
   if( skill == do_follow )
      return "do_follow";
   if( skill == do_for )
      return "do_for";
   if( skill == do_force )
      return "do_force";
   if( skill == do_forceclose )
      return "do_forceclose";
   if( skill == do_form_password )
      return "do_form_password";
   if( skill == do_fprompt )
      return "do_fprompt";
   if( skill == do_fquit )
      return "do_fquit";
   if( skill == do_free_vnums )
      return "do_free_vnums";
   if( skill == do_freeze )
      return "do_freeze";
   if( skill == do_fclear )
      return "do_fclear";
   if( skill == do_fshow )
      return "do_fshow";
   if( skill == do_gaso )
      return "do_gaso";
   if( skill == do_get )
      return "do_get";
   if( skill == do_gfighting )
      return "do_gfighting";
   if( skill == do_ghost )
      return "do_ghost";
   if( skill == do_give )
      return "do_give";
   if( skill == do_global_boards )
      return "do_global_boards";
   if( skill == do_global_note )
      return "do_global_note";
   if( skill == do_glance )
      return "do_glance";
   if( skill == do_gold )
      return "do_gold";
   if( skill == do_gohome )
      return "do_gohome";
   if( skill == do_goto )
      return "do_goto";
   if( skill == do_gouge )
      return "do_gouge";
   if( skill == do_group )
      return "do_group";
   if( skill == do_grub )
      return "do_grub";
   if( skill == do_gtell )
      return "do_gtell";
   if( skill == do_guestlist )
      return "do_guestlist";
   if( skill == do_guilds )
      return "do_guilds";
   if( skill == do_gwhere )
      return "do_gwhere";
   if( skill == do_heal )
      return "do_heal";
   if( skill == do_hedit )
      return "do_hedit";
   if( skill == do_hell )
      return "do_hell";
   if( skill == do_help )
      return "do_help";
   if( skill == do_hide )
      return "do_hide";
   if( skill == do_hitall )
      return "do_hitall";
   if( skill == do_hiscoset )
      return "do_hiscoset";
   if( skill == do_hiscore )
      return "do_hiscore";
   if( skill == do_hl )
      return "do_hl";
   if( skill == do_hlist )
      return "do_hlist";
   if( skill == do_holylight )
      return "do_holylight";
   if( skill == do_hometowns )
      return "do_hometowns";
   if( skill == do_hunt )
      return "do_hunt";
   if( skill == do_homebuy )
      return "do_homebuy";
   if( skill == do_house )
      return "do_house";
   if( skill == do_homepage )
      return "do_homepage";
   if( skill == do_hometowns )
      return "do_hometowns";
   if( skill == do_hset )
      return "do_hset";
   if( skill == do_hskelter )
      return "do_hskelter";
   if( skill == do_ide )
      return "do_ide";
   if( skill == do_idea )
      return "do_idea";
   if( skill == do_ignore )
      return "do_ignore";
   if( skill == do_immlist )
      return "do_immlist";
   if( skill == do_immortalize )
      return "do_immortalize";
   if( skill == do_add_imm_host )
      return "do_add_imm_host";
   if( skill == do_imm_exam )
      return "do_imm_exam";
   if( skill == do_imm_morph )
      return "do_imm_morph";
   if( skill == do_imm_unmorph )
      return "do_imm_unmorph";
   if( skill == do_incognito )
      return "do_incognito";
   if( skill == do_induct )
      return "do_induct";
   if( skill == do_info )
      return "do_info";
   if( skill == do_installarea )
      return "do_installarea";
   if( skill == do_instaroom )
      return "do_instaroom";
   if( skill == do_instazone )
      return "do_instazone";
   if( skill == do_intermud )
      return "do_intermud";
   if( skill == do_invade )
      return "do_invade";
   if( skill == do_inventory )
      return "do_inventory";
   if( skill == do_invis )
      return "do_invis";
   if( skill == do_ipcompare )
      return "do_ipcompare";
   if( skill == do_iscore )
      return "do_iscore";
   if( skill == do_ispell )
      return "do_ispell";
   if( skill == do_junk )
      return "do_junk";
   if( skill == do_khistory )
      return "do_khistory";
   if( skill == do_kick )
      return "do_kick";
   if( skill == do_kill )
      return "do_kill";
   if( skill == do_kills )
      return "do_kills";
   if( skill == do_languages )
      return "do_languages";
   if( skill == do_last )
      return "do_last";

   if( skill == do_lastname )
      return "do_lastname";
   if( skill == do_laws )
      return "do_laws";
   if( skill == do_leave )
      return "do_leave";
   if( skill == do_level )
      return "do_level";
   if( skill == do_light )
      return "do_light";
   if( skill == do_list )
      return "do_list";
   if( skill == do_listen )
      return "do_listen";
   if( skill == do_litterbug )
      return "do_litterbug";
   if( skill == do_loadarea )
      return "do_loadarea";
   if( skill == do_loadup )
      return "do_loadup";
   if( skill == do_lock )
      return "do_lock";
   if( skill == do_locker )
      return "do_locker";
   if( skill == do_log )
      return "do_log";
   if( skill == do_look )
      return "do_look";
   if( skill == do_low_purge )
      return "do_low_purge";
   if( skill == do_mailroom )
      return "do_mailroom";
   if( skill == do_make )
      return "do_make";
   if( skill == do_makeboard )
      return "do_makeboard";
   if( skill == do_makeclan )
      return "do_makeclan";
   if( skill == do_makenation )
      return "do_makenation";
   if( skill == do_makehometown )
      return "makehometown";
   if( skill == do_makecouncil )
      return "do_makecouncil";
   if( skill == do_makedeity )
      return "do_makedeity";
   if( skill == do_makelocker )
      return "do_makelocker";
   if( skill == do_makerepair )
      return "do_makerepair";
   if( skill == do_makeshop )
      return "do_makeshop";
   if( skill == do_makeslay )
      return "do_makeslay";
   if( skill == do_makewizlist )
      return "do_makewizlist";
   if( skill == do_mana )
      return "do_mana";
   if( skill == do_map )
      return "do_map";
   if( skill == do_marry )
      return "do_marry";
   if( skill == do_massign )
      return "do_massign";
   if( skill == do_mcreate )
      return "do_mcreate";
   if( skill == do_mdelete )
      return "do_mdelete";
   if( skill == do_memorize )
      return "do_memorize";
   if( skill == do_memory )
      return "do_memory";
   if( skill == do_mfind )
      return "do_mfind";
   if( skill == do_minvoke )
      return "do_minvoke";
   if( skill == do_meditate )
      return "do_meditate";
   if( skill == do_mistwalk )
      return "do_mistwalk";
   if( skill == do_mlist )
      return "do_mlist";
   if( skill == do_moblog )
      return "do_moblog";
   if( skill == do_morphcreate )
      return "do_morphcreate";
   if( skill == do_morphdestroy )
      return "do_morphdestroy";
   if( skill == do_morphlist )
      return "do_morphlist";
   if( skill == do_morphset )
      return "do_morphset";
   if( skill == do_morphstat )
      return "do_morphstat";
   if( skill == do_mortalize )
      return "do_mortalize";
   if( skill == do_mount )
      return "do_mount";
   if( skill == do_motd )
      return "do_motd";
   if( skill == do_mp_close_passage )
      return "do_mp_close_passage";
   if( skill == do_mp_damage )
      return "do_mp_damage";
   if( skill == do_mp_makecitizen )
      return "mp_makecitizen";
   if( skill == do_mp_revokecitizen )
      return "mp_revokecitizen";
   if( skill == do_mp_deposit )
      return "do_mp_deposit";
   if( skill == do_mp_fill_in )
      return "do_mp_fill_in";
   if( skill == do_mp_log )
      return "do_mp_log";
   if( skill == do_mp_open_passage )
      return "do_mp_open_passage";
   if( skill == do_mp_practice )
      return "do_mp_practice";
   if( skill == do_mp_restore )
      return "do_mp_restore";
   if( skill == do_mp_slay )
      return "do_mp_slay";
   if( skill == do_mp_withdraw )
      return "do_mp_withdraw";
   if( skill == do_mpadvance )
      return "do_mpadvance";
   if( skill == do_mpapply )
      return "do_mpapply";
   if( skill == do_mpapplyb )
      return "do_mpapplyb";
   if( skill == do_mpaset )
      return "do_mpaset";
   if( skill == do_mpasound )
      return "do_mpasound";
   if( skill == do_mpasupress )
      return "do_mpasupress";
   if( skill == do_mpat )
      return "do_mpat";
   if( skill == do_mpbodybag )
      return "do_mpbodybag";
   if( skill == do_mpdelay )
      return "do_mpdelay";
   if( skill == do_mpdream )
      return "do_mpdream";
   if( skill == do_mpecho )
      return "do_mpecho";
   if( skill == do_mpechoaround )
      return "do_mpechoaround";
   if( skill == do_mpechoat )
      return "do_mpechoat";
   if( skill == do_mpechozone )
      return "do_mpechozone";
   if( skill == do_mpedit )
      return "do_mpedit";
   if( skill == do_mpfavor )
      return "do_mpfavor";
   if( skill == do_mpforce )
      return "do_mpforce";
   if( skill == do_mpgoto )
      return "do_mpgoto";
   if( skill == do_mpinvis )
      return "do_mpinvis";
   if( skill == do_mpjunk )
      return "do_mpjunk";
   if( skill == do_mpkill )
      return "do_mpkill";
   if( skill == do_mpmload )
      return "do_mpmload";
   if( skill == do_mpmorph )
      return "do_mpmorph";
   if( skill == do_mpmset )
      return "do_mpmset";
   if( skill == do_mpmusic )
      return "do_mpmusic";
   if( skill == do_mpmusicaround )
      return "do_mpmusicaround";
   if( skill == do_mpmusicat )
      return "do_mpmusicat";
   if( skill == do_mpnothing )
      return "do_mpnothing";
   if( skill == do_mpnuisance )
      return "do_mpnuisance";
   if( skill == do_mpoload )
      return "do_mpoload";
   if( skill == do_mposet )
      return "do_mposet";
   if( skill == do_mppardon )
      return "do_mppardon";
   if( skill == do_mpqset )
      return "do_mpqset";

   if( skill == do_mpqpadd )
      return "do_mpqpadd";

   if( skill == do_mpmarry )
      return "do_mpmarry";

   if( skill == do_mpdivorce )
      return "do_mpdivorce";
   if( skill == do_mppeace )
      return "do_mppeace";
   if( skill == do_mppkset )
      return "do_mppkset";
   if( skill == do_mppurge )
      return "do_mppurge";
   if( skill == do_mpscatter )
      return "do_mpscatter";
   if( skill == do_mpsound )
      return "do_mpsound";
   if( skill == do_mpsoundaround )
      return "do_mpsoundaround";
   if( skill == do_mpsoundat )
      return "do_mpsoundat";
   if( skill == do_mpstat )
      return "do_mpstat";
   if( skill == do_mptransfer )
      return "do_mptransfer";
   if( skill == do_mpunmorph )
      return "do_mpunmorph";
   if( skill == do_mpunnuisance )
      return "do_mpunnuisance";
   if( skill == do_mrange )
      return "do_mrange";
   if( skill == do_mset )
      return "do_mset";
   if( skill == do_mstat )
      return "do_mstat";
   if( skill == do_murde )
      return "do_murde";
   if( skill == do_murder )
      return "do_murder";
   if( skill == do_mwhere )
      return "do_mwhere";
   if( skill == do_name )
      return "do_name";
   if( skill == do_nations )
      return "do_nations";
   if( skill == do_newbieset )
      return "do_newbieset";
   /*
    * if ( skill == do_news )             return "do_news";
    */
   if( skill == do_newscore )
      return "do_newscore";
   if( skill == do_newscore2 )
      return "do_newscore2";
   if( skill == do_newzones )
      return "do_newzones";
   if( skill == do_noemote )
      return "do_noemote";
   if( skill == do_nohelps )
      return "do_nohelps";
   if( skill == do_noresolve )
      return "do_noresolve";
   if( skill == do_north )
      return "do_north";
   if( skill == do_northeast )
      return "do_northeast";
   if( skill == do_northwest )
      return "do_northwest";
   if( skill == do_notell )
      return "do_notell";
   if( skill == do_notitle )
      return "do_notitle";
   if( skill == do_noteroom )
      return "do_noteroom";
   if( skill == do_nuisance )
      return "do_nuisance";
   if( skill == do_oassign )
      return "do_oassign";
   if( skill == do_observe )
      return "do_observe";
   if( skill == do_ocopy )
      return "do_ocopy";
   if( skill == do_ocreate )
      return "do_ocreate";
   if( skill == do_odelete )
      return "do_odelete";
   if( skill == do_ofind )
      return "do_ofind";
   if( skill == do_ogrub )
      return "do_ogrub";
   if( skill == do_oinvoke )
      return "do_oinvoke";
   if( skill == do_oldreboot )
      return "do_oldreboot";
   if( skill == do_oldscore )
      return "do_oldscore";
   if( skill == do_oldslay )
      return "do_oldslay";
   if( skill == do_olist )
      return "do_olist";
   if( skill == do_omedit )
      return "do_omedit";
   if( skill == do_ooedit )
      return "do_ooedit";
   if( skill == do_oredit )
      return "do_oredit";
   if( skill == do_opedit )
      return "do_opedit";
   if( skill == do_open )
      return "do_open";
   if( skill == do_opstat )
      return "do_opstat";
   if( skill == do_orange )
      return "do_orange";
   if( skill == do_order )
      return "do_order";
   if( skill == do_orders )
      return "do_orders";

   if( skill == do_oset )
      return "do_oset";
   if( skill == do_osearch )
      return "do_osearch";
   if( skill == do_ostat )
      return "do_ostat";
   if( skill == do_outcast )
      return "do_outcast";

   if( skill == do_outputprefix )
      return "do_outputprefix";

   if( skill == do_outputsuffix )
      return "do_outputsuffix";
   if( skill == do_owhere )
      return "do_owhere";
   if( skill == do_pager )
      return "do_pager";
   if( skill == do_panic )
      return "do_panic";
   if( skill == do_pardon )
      return "do_pardon";
   if( skill == do_password )
      return "do_password";
   if( skill == do_pcrename )
      return "do_pcrename";
   if( skill == do_peace )
      return "do_peace";
   if( skill == do_pfiles )
      return "do_pfiles";
   if( skill == do_pick )
      return "do_pick";
   if( skill == do_pledge )
      return "do_pledge";
   if( skill == do_plist )
      return "do_plist";
   if( skill == do_poison_weapon )
      return "do_poison_weapon";
   if( skill == do_practice )
      return "do_practice";
   if( skill == do_promote )
      return "do_promote";
   if( skill == do_prompt )
      return "do_prompt";
   if( skill == do_project )
      return "do_project";
   if( skill == do_pset )
      return "do_pset";
   if( skill == do_pstat )
      return "do_pstat";
   if( skill == do_pull )
      return "do_pull";
   if( skill == do_punch )
      return "do_punch";
   if( skill == do_purge )
      return "do_purge";
   if( skill == do_push )
      return "do_push";
   if( skill == do_put )
      return "do_put";
   if( skill == do_qoutcast )
      return "do_qoutcast";
   if( skill == do_qbit )
      return "do_qbit";
   if( skill == do_qpset )
      return "do_qpset";
   if( skill == do_qpstat )
      return "do_qpstat";
   if( skill == do_quaff )
      return "do_quaff";
   if( skill == do_qui )
      return "do_qui";
   if( skill == do_quit )
      return "do_quit";
   if( skill == do_races )
      return "do_races";
   if( skill == do_rank )
      return "do_rank";
   if( skill == do_rap )
      return "do_rap";
   if( skill == do_rassign )
      return "do_rassign";
   if( skill == do_rat )
      return "do_rat";
   if( skill == do_rdelete )
      return "do_rdelete";
   if( skill == do_reboo )
      return "do_reboo";
   if( skill == do_reboot )
      return "do_reboot";
   if( skill == do_recall )
      return "do_recall";
   if( skill == do_recho )
      return "do_recho";
   if( skill == do_recite )
      return "do_recite";
   if( skill == do_redit )
      return "do_redit";
   if( skill == do_regoto )
      return "do_regoto";
   if( skill == do_remort )
      return "do_remort";
   if( skill == do_remove )
      return "do_remove";
   if( skill == do_remains )
      return "do_remains";
   if( skill == do_rent )
      return "do_rent";
   if( skill == do_repair )
      return "do_repair";
   if( skill == do_repairset )
      return "do_repairset";
   if( skill == do_repairshops )
      return "do_repairshops";
   if( skill == do_repairstat )
      return "do_repairstat";
   if( skill == do_reply )
      return "do_reply";
   if( skill == do_report )
      return "do_report";
   if( skill == do_request )
      return "do_request";
   if( skill == do_reroll )
      return "do_reroll";
   if( skill == do_rescue )
      return "do_rescue";
   if( skill == do_reserve )
      return "do_reserve";
   if( skill == do_reset )
      return "do_reset";
   if( skill == do_residence )
      return "do_residence";
   if( skill == do_rest )
      return "do_rest";
   if( skill == do_restore )
      return "do_restore";
   if( skill == do_restoretime )
      return "do_restoretime";
   if( skill == do_restrict )
      return "do_restrict";
   if( skill == do_retire )
      return "do_retire";
   if( skill == do_retran )
      return "do_retran";
   if( skill == do_retrieve )
      return "do_retrieve";
   if( skill == do_return )
      return "do_return";
   if( skill == do_revert )
      return "do_revert";
   if( skill == do_rgrub )
      return "do_rgrub";
   if( skill == do_rip )
      return "do_rip";
   if( skill == do_rings )
      return "do_rings";
   if( skill == do_rlist )
      return "do_rlist";
   if( skill == do_roload )
      return "do_roload";
   if( skill == do_rpedit )
      return "do_rpedit";
   if( skill == do_rpstat )
      return "do_rpstat";
   if( skill == do_rset )
      return "do_rset";
   if( skill == do_rstat )
      return "do_rstat";
   if( skill == do_ruminate )      
      return "do_ruminate"; 
   if( skill == do_sharpen )
      return "do_sharpen"; /* sharpen skill --Cronel */
   if( skill == do_sacrifice )
      return "do_sacrifice";
   if( skill == do_save )
      return "do_save";
   if( skill == do_saveall )
      return "do_saveall";
   if( skill == do_savearea )
      return "do_savearea";
   if( skill == do_say )
      return "do_say";
   if( skill == do_say_to_char )
      return "do_say_to_char";
   if( skill == do_scan )
      return "do_scan";
   if( skill == do_scatter )
      return "do_scatter";
   if( skill == do_score )
      return "do_score";
   if( skill == do_scribe )
      return "do_scribe";
   if( skill == do_search )
      return "do_search";
   if( skill == do_search_flags )
      return "do_search_flags";
   if( skill == do_sedit )
      return "do_sedit";
   if( skill == do_sell )
      return "do_sell";
   if( skill == do_sellhouse )
      return "do_sellhouse";
   if( skill == do_semote )
      return "do_semote";
   if( skill == do_sendmail )
      return "do_sendmail";
   if( skill == do_set )
      return "do_set";
   if( skill == do_setabit )
      return "do_setabit";
   if( skill == do_set_boot_time )
      return "do_set_boot_time";
   if( skill == do_setclan )
      return "do_setclan";
   if( skill == do_setclass )
      return "do_setclass";
   if( skill == do_setcouncil )
      return "do_setcouncil";
   if( skill == do_setdeity )
      return "do_setdeity";
   if( skill == do_setqbit )
      return "do_setqbit";
   if( skill == do_setrace )
      return "do_setrace";
   if( skill == do_setslay )
      return "do_setslay";
   if( skill == do_setweather )
      return "do_setweather";
   if( skill == do_shops )
      return "do_shops";
   if( skill == do_shopset )
      return "do_shopset";
   if( skill == do_shopstat )
      return "do_shopstat";
   if( skill == do_shove )
      return "do_shove";
   if( skill == do_showabit )
      return "do_showabit";
   if( skill == do_showclan )
      return "do_showclan";
   if( skill == do_showclass )
      return "do_showclass";
   if( skill == do_showcouncil )
      return "do_showcouncil";
   if( skill == do_showdeity )
      return "do_showdeity";
   if( skill == do_showqbit )
      return "do_showqbit";
   if( skill == do_showrace )
      return "do_showrace";
   if( skill == do_showslay )
      return "do_showslay";
   if( skill == do_showweather )
      return "do_showweather";
   if( skill == do_shutdow )
      return "do_shutdow";
   if( skill == do_shutdown )
      return "do_shutdown";
   if( skill == do_silence )
      return "do_silence";
   if( skill == do_sit )
      return "do_sit";
   if( skill == do_skin )
      return "do_skin";
   if( skill == do_sla )
      return "do_sla";
   if( skill == do_slay )
      return "do_slay";
   if( skill == do_sleep )
      return "do_sleep";
   if( skill == do_slice )
      return "do_slice";
   if( skill == do_slist )
      return "do_slist";
   if( skill == do_slotlist )
      return "do_slotlist";
   if( skill == do_slookup )
      return "do_slookup";
   if( skill == do_smoke )
      return "do_smoke";
   if( skill == do_smite )
      return "do_smite";
   if( skill == do_sneak )
      return "do_sneak";
   if( skill == do_snoop )
      return "do_snoop";
   if( skill == do_sober )
      return "do_sober";
   if( skill == do_socials )
      return "do_socials";
   if( skill == do_south )
      return "do_south";
   if( skill == do_southeast )
      return "do_southeast";
   if( skill == do_southwest )
      return "do_southwest";
   if( skill == do_speak )
      return "do_speak";
   if( skill == do_split )
      return "do_split";
   if( skill == do_sset )
      return "do_sset";
   if( skill == do_sslist )
      return "do_sslist";
   if( skill == do_stand )
      return "do_stand";
   if( skill == do_stat )
      return "do_stat";
   if( skill == do_statreport )
      return "do_statreport";
   if( skill == do_steal )
      return "do_steal";
   if( skill == do_strew )
      return "do_strew";
   if( skill == do_strip )
      return "do_strip";
   if( skill == do_study )
      return "do_study";
   if( skill == do_stun )
      return "do_stun";
   if( skill == do_style )
      return "do_style";
   if( skill == do_superbless )
      return "do_superbless";
   if( skill == do_supplicate )
      return "do_supplicate";
   if( skill == do_surrender )
      return "do_surrender";
   if( skill == do_switch )
      return "do_switch";
   if( skill == do_showlayers )
      return "do_showlayers";

   if( skill == do_suicide )
      return "do_suicide";
   if( skill == do_tamp )
      return "do_tamp";
   if( skill == do_tell )
      return "do_tell";
   if( skill == do_time )
      return "do_time";
   if( skill == do_timecmd )
      return "do_timecmd";
   if( skill == do_title )
      return "do_title";
   if( skill == do_track )
      return "do_track";
   if( skill == do_trivia )
      return "do_trivia";
   if( skill == do_tlist )
      return "do_tlist";
   if( skill == do_trans )
      return "do_trans";
   if( skill == do_train )
      return "do_train";
   if( skill == do_trance )   
      return "do_trance";
   if( skill == do_transfer )
      return "do_transfer";
   if( skill == do_trust )
      return "do_trust";
   if( skill == do_typo )
      return "do_typo";
   /*
    * if ( skill == do_unalias )      return "do_unalias"; 
    */
   if( skill == do_unbounty )
      return "do_unbounty";
   if( skill == do_unfoldarea )
      return "do_unfoldarea";
   if( skill == do_unhell )
      return "do_unhell";
   if( skill == do_unlock )
      return "do_unlock";
   if( skill == do_unnuisance )
      return "do_unnuisance";
   if( skill == do_unsilence )
      return "do_unsilence";
   if( skill == do_up )
      return "do_up";
   if( skill == do_users )
      return "do_users";
   if( skill == do_users2 )
      return "do_users2";
   if( skill == do_value )
      return "do_value";
   if( skill == do_vassign )
      return "do_vassign";
   if( skill == do_victories )
      return "do_victories";
   if( skill == do_visible )
      return "do_visible";
   if( skill == do_visogy )
      return "do_visogy";
   if( skill == do_vnums )
      return "do_vnums";
   if( skill == do_vsearch )
      return "do_vsearch";
   if( skill == do_wake )
      return "do_wake";
   if( skill == do_warn )
      return "do_warn";
   if( skill == do_watch )
      return "do_watch";
   if( skill == do_wear )
      return "do_wear";
   if( skill == do_weather )
      return "do_weather";
   if( skill == do_west )
      return "do_west";
   if( skill == do_wfilter )
      return "do_wfilter";
   if( skill == do_where )
      return "do_where";
   if( skill == do_whisper )
      return "do_whisper";
   if( skill == do_who )
      return "do_who";
   if( skill == do_whois )
      return "do_whois";
   if( skill == do_wimpy )
      return "do_wimpy";
   if( skill == do_withdraw )
      return "do_withdraw";
   if( skill == do_wizhelp )
      return "do_wizhelp";
   if( skill == do_wizinduct )
      return "do_wizinduct";
   if( skill == do_wizlist )
      return "do_wizlist";
   if( skill == do_wizlock )
      return "do_wizlock";
   if( skill == do_worth )
      return "do_worth";
   if( skill == do_zecho )
      return "do_zecho";
   if( skill == do_zap )
      return "do_zap";
   if( skill == do_zones )
      return "do_zones";
   if( skill == do_illegal_pk )
      return "do_illegal_pk";
   if( skill == do_listen )
      return "do_listen";
   if( skill == do_makechannel )
      return "do_makechannel";
   if( skill == do_setchannel )
      return "do_set_channel";
   if( skill == do_showchannels )
      return "do_showchannels";

   sprintf( buf, "(%p)", skill );
   return buf;
}
