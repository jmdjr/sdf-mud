/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

#ifndef __DO_H__
#define __DO_H__

/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */
DECLARE_DO_FUN( skill_notfound );
DECLARE_DO_FUN( do_aassign );
DECLARE_DO_FUN( do_add_imm_host );
DECLARE_DO_FUN( do_advance );
DECLARE_DO_FUN( do_aentrance );
DECLARE_DO_FUN( do_aexit );
DECLARE_DO_FUN( do_affected );
DECLARE_DO_FUN( do_afk );
DECLARE_DO_FUN( do_aid );
DECLARE_DO_FUN( do_alias );
DECLARE_DO_FUN( do_allow );
DECLARE_DO_FUN( do_ansi );
DECLARE_DO_FUN( do_answer );
DECLARE_DO_FUN( do_apply );
DECLARE_DO_FUN( do_appraise );
DECLARE_DO_FUN( do_aquest );
DECLARE_DO_FUN( do_areas );
/* Scion arenacode */
DECLARE_DO_FUN( do_arena );
DECLARE_DO_FUN( do_aset );
DECLARE_DO_FUN( do_ask );
DECLARE_DO_FUN( do_assassinate );
DECLARE_DO_FUN( do_astat );
DECLARE_DO_FUN( do_at );
DECLARE_DO_FUN( do_atmob );
DECLARE_DO_FUN( do_atobj );
DECLARE_DO_FUN( do_auction );
DECLARE_DO_FUN( do_authorize );
DECLARE_DO_FUN( do_avator );
DECLARE_DO_FUN( do_avhelp );
DECLARE_DO_FUN( do_avtalk );
DECLARE_DO_FUN( do_backstab );
DECLARE_DO_FUN( do_backup );  /* Garinan */
DECLARE_DO_FUN( do_balzhur );
DECLARE_DO_FUN( do_bamfin );
DECLARE_DO_FUN( do_bamfout );
DECLARE_DO_FUN( do_watch );
DECLARE_DO_FUN( do_ban );
DECLARE_DO_FUN( do_bandage );
DECLARE_DO_FUN( do_bash );
DECLARE_DO_FUN( do_bashdoor );
DECLARE_DO_FUN( do_beep );
DECLARE_DO_FUN( do_berserk );
DECLARE_DO_FUN( do_bestow );
DECLARE_DO_FUN( do_bestowarea );
DECLARE_DO_FUN( do_bio );
DECLARE_DO_FUN( do_bite );
DECLARE_DO_FUN( do_bloodlet );
DECLARE_DO_FUN( do_boards );
DECLARE_DO_FUN( do_bodybag );
DECLARE_DO_FUN( do_bounty );
DECLARE_DO_FUN( do_brandish );
DECLARE_DO_FUN( do_brew );
DECLARE_DO_FUN( do_broach );
DECLARE_DO_FUN( do_bset );
DECLARE_DO_FUN( do_bstat );
DECLARE_DO_FUN( do_bug );
DECLARE_DO_FUN( do_bury );
DECLARE_DO_FUN( do_buy );
DECLARE_DO_FUN( do_buylocker );
DECLARE_DO_FUN( do_cast );
DECLARE_DO_FUN( do_cbackup );
DECLARE_DO_FUN( do_cedit );
DECLARE_DO_FUN( do_channels );
DECLARE_DO_FUN( do_chat );
DECLARE_DO_FUN( do_check_vnums );
DECLARE_DO_FUN( do_check_links );
DECLARE_DO_FUN( do_circle );
DECLARE_DO_FUN( do_clans );
DECLARE_DO_FUN( do_clantalk );
DECLARE_DO_FUN( do_classes );
DECLARE_DO_FUN( do_claw );
DECLARE_DO_FUN( do_cleararea );
DECLARE_DO_FUN( do_cleanse ); /* Digifuzz */
DECLARE_DO_FUN( do_climate ); /* FB */
DECLARE_DO_FUN( do_climb );
DECLARE_DO_FUN( do_close );
DECLARE_DO_FUN( do_cmdtable );
DECLARE_DO_FUN( do_commands );
DECLARE_DO_FUN( do_comment );
#ifdef MCCP
DECLARE_DO_FUN( do_compress );
#endif
DECLARE_DO_FUN( do_config );
DECLARE_DO_FUN( do_connect );
DECLARE_DO_FUN( do_consider );
DECLARE_DO_FUN( do_copyover );
DECLARE_DO_FUN( do_corpse );
DECLARE_DO_FUN( do_cook );
DECLARE_DO_FUN( do_council_induct );
DECLARE_DO_FUN( do_council_outcast );
DECLARE_DO_FUN( do_councils );
DECLARE_DO_FUN( do_counciltalk );
DECLARE_DO_FUN( do_counciltalk2 );
DECLARE_DO_FUN( do_credits );
DECLARE_DO_FUN( do_cset );
DECLARE_DO_FUN( do_deities );
DECLARE_DO_FUN( do_delay );
DECLARE_DO_FUN( do_deny );
DECLARE_DO_FUN( do_demi );
DECLARE_DO_FUN( do_description );
DECLARE_DO_FUN( do_destro );
DECLARE_DO_FUN( do_destroy );
DECLARE_DO_FUN( do_detrap );
DECLARE_DO_FUN( do_devote );
DECLARE_DO_FUN( do_dig );
DECLARE_DO_FUN( do_disable );
DECLARE_DO_FUN( do_disarm );
DECLARE_DO_FUN( do_disconnect );
DECLARE_DO_FUN( do_dismiss );
DECLARE_DO_FUN( do_dismount );
DECLARE_DO_FUN( do_dmesg );
DECLARE_DO_FUN( do_donate );
DECLARE_DO_FUN( do_down );
DECLARE_DO_FUN( do_drag );
DECLARE_DO_FUN( do_drink );
DECLARE_DO_FUN( do_drop );
DECLARE_DO_FUN( do_diagnose );
DECLARE_DO_FUN( do_divorce );
DECLARE_DO_FUN( do_east );
DECLARE_DO_FUN( do_eat );
DECLARE_DO_FUN( do_echo );
DECLARE_DO_FUN( do_elevate );
DECLARE_DO_FUN( do_email );
DECLARE_DO_FUN( do_emote );
DECLARE_DO_FUN( do_empty );
DECLARE_DO_FUN( do_enter );
DECLARE_DO_FUN( do_equipment );
DECLARE_DO_FUN( do_examine );
DECLARE_DO_FUN( do_extraname );
DECLARE_DO_FUN( do_exits );
DECLARE_DO_FUN( do_feed );
DECLARE_DO_FUN( do_fill );
DECLARE_DO_FUN( do_findnote );
DECLARE_DO_FUN( do_fire );
DECLARE_DO_FUN( do_fixchar );
DECLARE_DO_FUN( do_fixed );
DECLARE_DO_FUN( do_flee );
DECLARE_DO_FUN( do_foldarea );
DECLARE_DO_FUN( do_follow );
DECLARE_DO_FUN( do_for );
DECLARE_DO_FUN( do_force );
DECLARE_DO_FUN( do_forceclose );
DECLARE_DO_FUN( do_fprompt );
DECLARE_DO_FUN( do_fquit );   /* Gorog */
DECLARE_DO_FUN( do_form_password );
DECLARE_DO_FUN( do_free_vnums );
DECLARE_DO_FUN( do_freeze );
DECLARE_DO_FUN( do_fclear );
DECLARE_DO_FUN( do_fshow );
DECLARE_DO_FUN( do_gaso );
DECLARE_DO_FUN( do_get );
DECLARE_DO_FUN( do_gfighting );
DECLARE_DO_FUN( do_give );
DECLARE_DO_FUN( do_glance );
DECLARE_DO_FUN( do_ghost );   /* Froboz */
DECLARE_DO_FUN( do_gold );
DECLARE_DO_FUN( do_goto );
DECLARE_DO_FUN( do_gouge );
DECLARE_DO_FUN( do_group );
DECLARE_DO_FUN( do_grub );
DECLARE_DO_FUN( do_gtell );
DECLARE_DO_FUN( do_guestlist );
DECLARE_DO_FUN( do_guilds );
DECLARE_DO_FUN( do_guildtalk );
DECLARE_DO_FUN( do_gwhere );
DECLARE_DO_FUN( do_hskelter );   /* cronel all pkill mode */
DECLARE_DO_FUN( do_heal );
DECLARE_DO_FUN( do_hedit );
DECLARE_DO_FUN( do_hell );
DECLARE_DO_FUN( do_help );
DECLARE_DO_FUN( do_hide );
DECLARE_DO_FUN( do_hiscoset );
DECLARE_DO_FUN( do_hiscore );
DECLARE_DO_FUN( do_hitall );
DECLARE_DO_FUN( do_hl );
DECLARE_DO_FUN( do_hlist );
DECLARE_DO_FUN( do_holylight );
DECLARE_DO_FUN( do_memorize );
DECLARE_DO_FUN( do_homepage );
DECLARE_DO_FUN( do_hunt );
DECLARE_DO_FUN( do_hset );
DECLARE_DO_FUN( do_ide );
DECLARE_DO_FUN( do_idea );
DECLARE_DO_FUN( do_ignore );
DECLARE_DO_FUN( do_illegal_pk );
DECLARE_DO_FUN( do_immortalize );
DECLARE_DO_FUN( do_immlist );
DECLARE_DO_FUN( do_immtalk );
DECLARE_DO_FUN( do_imm_exam );
DECLARE_DO_FUN( do_imm_morph );
DECLARE_DO_FUN( do_imm_unmorph );
DECLARE_DO_FUN( do_incognito );  /* Froboz */
DECLARE_DO_FUN( do_induct );
DECLARE_DO_FUN( do_info );
DECLARE_DO_FUN( do_installarea );
DECLARE_DO_FUN( do_instaroom );
DECLARE_DO_FUN( do_instazone );
DECLARE_DO_FUN( do_intermud );
DECLARE_DO_FUN( do_invade );
DECLARE_DO_FUN( do_inventory );
DECLARE_DO_FUN( do_invis );
DECLARE_DO_FUN( do_ipcompare );
DECLARE_DO_FUN( do_iscore );  /* Froboz */
DECLARE_DO_FUN( do_ispell );  /* Kratas */
DECLARE_DO_FUN( do_junk );
DECLARE_DO_FUN( do_khistory );
DECLARE_DO_FUN( do_kick );
DECLARE_DO_FUN( do_kill );
DECLARE_DO_FUN( do_kills );
DECLARE_DO_FUN( do_languages );
DECLARE_DO_FUN( do_last );
DECLARE_DO_FUN( do_lastname );
DECLARE_DO_FUN( do_laws );
DECLARE_DO_FUN( do_leave );
DECLARE_DO_FUN( do_level );
DECLARE_DO_FUN( do_light );
DECLARE_DO_FUN( do_list );
DECLARE_DO_FUN( do_litterbug );
DECLARE_DO_FUN( do_loadarea );
DECLARE_DO_FUN( do_loadup );
DECLARE_DO_FUN( do_lock );
DECLARE_DO_FUN( do_locker );
DECLARE_DO_FUN( do_log );
DECLARE_DO_FUN( do_look );
DECLARE_DO_FUN( do_low_purge );
DECLARE_DO_FUN( do_mailroom );
DECLARE_DO_FUN( do_make );
DECLARE_DO_FUN( do_makeboard );
DECLARE_DO_FUN( do_makeclan );
DECLARE_DO_FUN( do_makecouncil );
DECLARE_DO_FUN( do_makedeity );
DECLARE_DO_FUN( do_makeguild );
DECLARE_DO_FUN( do_makelocker );
DECLARE_DO_FUN( do_makerepair );
DECLARE_DO_FUN( do_makeshop );
DECLARE_DO_FUN( do_makewizlist );
DECLARE_DO_FUN( do_mana );
DECLARE_DO_FUN( do_map );
DECLARE_DO_FUN( do_marry );
DECLARE_DO_FUN( do_massign );
DECLARE_DO_FUN( do_memory );
DECLARE_DO_FUN( do_mcreate );
DECLARE_DO_FUN( do_mdelete );
DECLARE_DO_FUN( do_mfind );
DECLARE_DO_FUN( do_minvoke );
DECLARE_DO_FUN( do_meditate );
DECLARE_DO_FUN( do_mistwalk );
DECLARE_DO_FUN( do_mlist );
DECLARE_DO_FUN( do_moblog );
DECLARE_DO_FUN( do_morphcreate );
DECLARE_DO_FUN( do_morphdestroy );
DECLARE_DO_FUN( do_morphlist );
DECLARE_DO_FUN( do_morphset );
DECLARE_DO_FUN( do_morphstat );
DECLARE_DO_FUN( do_mortalize );
DECLARE_DO_FUN( do_mount );
DECLARE_DO_FUN( do_motd );
DECLARE_DO_FUN( do_mset );
DECLARE_DO_FUN( do_mstat );
DECLARE_DO_FUN( do_murde );
DECLARE_DO_FUN( do_murder );
DECLARE_DO_FUN( do_muse );
DECLARE_DO_FUN( do_music );
DECLARE_DO_FUN( do_mwhere );
DECLARE_DO_FUN( do_name );
DECLARE_DO_FUN( do_newbiechat );
DECLARE_DO_FUN( do_newbieset );
DECLARE_DO_FUN( do_news );
DECLARE_DO_FUN( do_newscore );
DECLARE_DO_FUN( do_newscore2 );
DECLARE_DO_FUN( do_newzones );
DECLARE_DO_FUN( do_noemote );
DECLARE_DO_FUN( do_nohelps );
DECLARE_DO_FUN( do_noresolve );
DECLARE_DO_FUN( do_north );
DECLARE_DO_FUN( do_northeast );
DECLARE_DO_FUN( do_northwest );
DECLARE_DO_FUN( do_notell );
DECLARE_DO_FUN( do_notitle );
DECLARE_DO_FUN( do_noteroom );
DECLARE_DO_FUN( do_nuisance );
DECLARE_DO_FUN( do_oassign );
DECLARE_DO_FUN( do_observe );
DECLARE_DO_FUN( do_ocopy );   /* tag */
DECLARE_DO_FUN( do_ocreate );
DECLARE_DO_FUN( do_odelete );
DECLARE_DO_FUN( do_ofind );
DECLARE_DO_FUN( do_ogrub );
DECLARE_DO_FUN( do_oinvoke );
DECLARE_DO_FUN( do_oldreboot );
DECLARE_DO_FUN( do_oldscore );
DECLARE_DO_FUN( do_oldslay );
DECLARE_DO_FUN( do_olist );
DECLARE_DO_FUN( do_ooedit );  /* Tag */
DECLARE_DO_FUN( do_omedit );  /* Tag */
DECLARE_DO_FUN( do_oredit );  /* Tag */
DECLARE_DO_FUN( do_open );
DECLARE_DO_FUN( do_opentourney );
DECLARE_DO_FUN( do_order );
DECLARE_DO_FUN( do_orders );
DECLARE_DO_FUN( do_ordertalk );
DECLARE_DO_FUN( do_oset );
DECLARE_DO_FUN( do_osearch );
DECLARE_DO_FUN( do_ostat );
DECLARE_DO_FUN( do_ot );
DECLARE_DO_FUN( do_outcast );
DECLARE_DO_FUN( do_outputprefix );
DECLARE_DO_FUN( do_outputsuffix );
DECLARE_DO_FUN( do_owhere );
DECLARE_DO_FUN( do_pager );
DECLARE_DO_FUN( do_panic );
DECLARE_DO_FUN( do_pardon );
DECLARE_DO_FUN( do_password );
DECLARE_DO_FUN( do_pcrename );
DECLARE_DO_FUN( do_peace );
DECLARE_DO_FUN( do_pick );
DECLARE_DO_FUN( do_pledge );
DECLARE_DO_FUN( do_plist );
DECLARE_DO_FUN( do_poison_weapon );
DECLARE_DO_FUN( do_pose );
DECLARE_DO_FUN( do_practice );
DECLARE_DO_FUN( do_project );
DECLARE_DO_FUN( do_promote );
DECLARE_DO_FUN( do_prompt );
DECLARE_DO_FUN( do_pset );
DECLARE_DO_FUN( do_pstat );
DECLARE_DO_FUN( do_pull );
DECLARE_DO_FUN( do_punch );
DECLARE_DO_FUN( do_purge );
DECLARE_DO_FUN( do_push );
DECLARE_DO_FUN( do_put );
DECLARE_DO_FUN( do_qoutcast );   /* Quick Outcast -Garinan */
DECLARE_DO_FUN( do_qpset );
DECLARE_DO_FUN( do_qpstat );
DECLARE_DO_FUN( do_quaff );
DECLARE_DO_FUN( do_quest );
DECLARE_DO_FUN( do_qui );
DECLARE_DO_FUN( do_quit );
DECLARE_DO_FUN( do_races );
DECLARE_DO_FUN( do_racetalk );
DECLARE_DO_FUN( do_rank );
DECLARE_DO_FUN( do_rap );
DECLARE_DO_FUN( do_rassign );
DECLARE_DO_FUN( do_rat );
DECLARE_DO_FUN( do_rdelete );
DECLARE_DO_FUN( do_reboo );
DECLARE_DO_FUN( do_reboot );
DECLARE_DO_FUN( do_recall );
DECLARE_DO_FUN( do_recompressRoom );
DECLARE_DO_FUN( do_recho );
DECLARE_DO_FUN( do_recite );
DECLARE_DO_FUN( do_recover );
DECLARE_DO_FUN( do_redit );
DECLARE_DO_FUN( do_regoto );
DECLARE_DO_FUN( do_remains );
DECLARE_DO_FUN( do_remort );
DECLARE_DO_FUN( do_remove );
DECLARE_DO_FUN( do_rent );
DECLARE_DO_FUN( do_repair );
DECLARE_DO_FUN( do_repairset );
DECLARE_DO_FUN( do_repairshops );
DECLARE_DO_FUN( do_repairstat );
DECLARE_DO_FUN( do_repeat );
DECLARE_DO_FUN( do_reply );
DECLARE_DO_FUN( do_report );
DECLARE_DO_FUN( do_request );
DECLARE_DO_FUN( do_reroll );
DECLARE_DO_FUN( do_rescue );
DECLARE_DO_FUN( do_reserve );
DECLARE_DO_FUN( do_reset );
DECLARE_DO_FUN( do_rest );
DECLARE_DO_FUN( do_restore );
DECLARE_DO_FUN( do_restoretime );
DECLARE_DO_FUN( do_restrict );
DECLARE_DO_FUN( do_retell );
DECLARE_DO_FUN( do_retire );
DECLARE_DO_FUN( do_retran );
DECLARE_DO_FUN( do_retrieve );   /* Garinan */
DECLARE_DO_FUN( do_return );
DECLARE_DO_FUN( do_revert );
DECLARE_DO_FUN( do_rgrub );
DECLARE_DO_FUN( do_rip );
DECLARE_DO_FUN( do_rings );
DECLARE_DO_FUN( do_rlist );
DECLARE_DO_FUN( do_roload );
DECLARE_DO_FUN( do_rset );
DECLARE_DO_FUN( do_rstat );
DECLARE_DO_FUN( do_ruminate );
DECLARE_DO_FUN( do_sharpen ); /* sharpen skill --Cronel */
DECLARE_DO_FUN( do_sacrifice );
DECLARE_DO_FUN( do_save );
DECLARE_DO_FUN( do_saveall );
DECLARE_DO_FUN( do_savearea );
DECLARE_DO_FUN( do_say );
DECLARE_DO_FUN( do_say_to_char );
DECLARE_DO_FUN( do_scan );
DECLARE_DO_FUN( do_scatter );
DECLARE_DO_FUN( do_score );
DECLARE_DO_FUN( do_scribe );
DECLARE_DO_FUN( do_search );
DECLARE_DO_FUN( do_search_flags );
DECLARE_DO_FUN( do_sedit );
DECLARE_DO_FUN( do_sell );
DECLARE_DO_FUN( do_semote );
DECLARE_DO_FUN( do_sendmail );
DECLARE_DO_FUN( do_set );
DECLARE_DO_FUN( do_set_boot_time );
DECLARE_DO_FUN( do_setclan );
DECLARE_DO_FUN( do_setclass );
DECLARE_DO_FUN( do_setcouncil );
DECLARE_DO_FUN( do_setdeity );
DECLARE_DO_FUN( do_setrace );
DECLARE_DO_FUN( do_setweather );
DECLARE_DO_FUN( do_shops );
DECLARE_DO_FUN( do_shopset );
DECLARE_DO_FUN( do_shopstat );
DECLARE_DO_FUN( do_shout );
DECLARE_DO_FUN( do_shove );
DECLARE_DO_FUN( do_showclass );
DECLARE_DO_FUN( do_showclan );
DECLARE_DO_FUN( do_showcouncil );
DECLARE_DO_FUN( do_showdeity );
DECLARE_DO_FUN( do_showrace );
DECLARE_DO_FUN( do_showweather );   /* FB */
DECLARE_DO_FUN( do_shutdow );
DECLARE_DO_FUN( do_shutdown );
DECLARE_DO_FUN( do_silence );
DECLARE_DO_FUN( do_sit );
DECLARE_DO_FUN( do_skin );
DECLARE_DO_FUN( do_sla );
DECLARE_DO_FUN( do_slay );
DECLARE_DO_FUN( do_sleep );
DECLARE_DO_FUN( do_slice );
DECLARE_DO_FUN( do_slist );
DECLARE_DO_FUN( do_slotlist );
DECLARE_DO_FUN( do_slookup );
DECLARE_DO_FUN( do_smoke );
DECLARE_DO_FUN( do_smite );
DECLARE_DO_FUN( do_sneak );
DECLARE_DO_FUN( do_snoop );
DECLARE_DO_FUN( do_sober );
DECLARE_DO_FUN( do_socials );
DECLARE_DO_FUN( do_south );
DECLARE_DO_FUN( do_southeast );
DECLARE_DO_FUN( do_southwest );
DECLARE_DO_FUN( do_speak );
DECLARE_DO_FUN( do_split );
DECLARE_DO_FUN( do_sset );
DECLARE_DO_FUN( do_sslist );
DECLARE_DO_FUN( do_stand );
DECLARE_DO_FUN( do_stat );
DECLARE_DO_FUN( do_statreport );
DECLARE_DO_FUN( do_status );
DECLARE_DO_FUN( do_starttourney );
DECLARE_DO_FUN( do_steal );
DECLARE_DO_FUN( do_sting );
DECLARE_DO_FUN( do_strew );
DECLARE_DO_FUN( do_strip );
DECLARE_DO_FUN( do_study );
DECLARE_DO_FUN( do_stun );
DECLARE_DO_FUN( do_style );
DECLARE_DO_FUN( do_supplicate );
DECLARE_DO_FUN( do_switch );
DECLARE_DO_FUN( do_showlayers );
DECLARE_DO_FUN( do_surrender );
DECLARE_DO_FUN( do_suicide );
DECLARE_DO_FUN( do_superbless );
DECLARE_DO_FUN( do_tail );
DECLARE_DO_FUN( do_tamp );
DECLARE_DO_FUN( do_tell );
DECLARE_DO_FUN( do_test );
DECLARE_DO_FUN( do_think );
DECLARE_DO_FUN( do_time );
DECLARE_DO_FUN( do_timecmd );
DECLARE_DO_FUN( do_title );
DECLARE_DO_FUN( do_track );
DECLARE_DO_FUN( do_trivia );  /* Garinan */
DECLARE_DO_FUN( do_tlist );
DECLARE_DO_FUN( do_trans );
DECLARE_DO_FUN( do_train );
DECLARE_DO_FUN( do_trance );
DECLARE_DO_FUN( do_trust );
DECLARE_DO_FUN( do_typo );
DECLARE_DO_FUN( do_unbounty );
DECLARE_DO_FUN( do_unfoldarea );
DECLARE_DO_FUN( do_unhell );
DECLARE_DO_FUN( do_unlock );
DECLARE_DO_FUN( do_unnuisance );
DECLARE_DO_FUN( do_unsilence );
DECLARE_DO_FUN( do_up );
DECLARE_DO_FUN( do_users );
DECLARE_DO_FUN( do_users2 );
DECLARE_DO_FUN( do_vassign );
DECLARE_DO_FUN( do_value );
DECLARE_DO_FUN( do_victories );
DECLARE_DO_FUN( do_visible );
DECLARE_DO_FUN( do_visogy );
DECLARE_DO_FUN( do_vnums );
DECLARE_DO_FUN( do_vsearch );
DECLARE_DO_FUN( do_wake );
DECLARE_DO_FUN( do_warn );
DECLARE_DO_FUN( do_wartalk );
DECLARE_DO_FUN( do_wear );
DECLARE_DO_FUN( do_weather );
DECLARE_DO_FUN( do_west );
DECLARE_DO_FUN( do_wfilter ); /* Garinan */
DECLARE_DO_FUN( do_where );
DECLARE_DO_FUN( do_whisper );
DECLARE_DO_FUN( do_who );
DECLARE_DO_FUN( do_whois );
DECLARE_DO_FUN( do_wimpy );
DECLARE_DO_FUN( do_wizhelp );
DECLARE_DO_FUN( do_wizinduct );
DECLARE_DO_FUN( do_wizlist );
DECLARE_DO_FUN( do_wizlock );
DECLARE_DO_FUN( do_worth );
DECLARE_DO_FUN( do_yell );
DECLARE_DO_FUN( do_zecho );
DECLARE_DO_FUN( do_zap );
DECLARE_DO_FUN( do_zones );

/* mob prog stuff */
DECLARE_DO_FUN( do_mp_close_passage );
DECLARE_DO_FUN( do_mp_damage );
DECLARE_DO_FUN( do_mp_log );
DECLARE_DO_FUN( do_mp_restore );
DECLARE_DO_FUN( do_mp_open_passage );
DECLARE_DO_FUN( do_mp_practice );
DECLARE_DO_FUN( do_mp_slay );
DECLARE_DO_FUN( do_mpadvance );
DECLARE_DO_FUN( do_mpasound );
DECLARE_DO_FUN( do_mpasupress );
DECLARE_DO_FUN( do_mpat );
DECLARE_DO_FUN( do_mpdream );
DECLARE_DO_FUN( do_mp_deposit );
DECLARE_DO_FUN( do_mp_fill_in );
DECLARE_DO_FUN( do_mp_withdraw );
DECLARE_DO_FUN( do_mpecho );
DECLARE_DO_FUN( do_mpechoaround );
DECLARE_DO_FUN( do_mpechoat );
DECLARE_DO_FUN( do_mpechozone );
DECLARE_DO_FUN( do_mpedit );
DECLARE_DO_FUN( do_mrange );
DECLARE_DO_FUN( do_opedit );
DECLARE_DO_FUN( do_orange );
DECLARE_DO_FUN( do_rpedit );
DECLARE_DO_FUN( do_mpforce );
DECLARE_DO_FUN( do_mpinvis );
DECLARE_DO_FUN( do_mpgoto );
DECLARE_DO_FUN( do_mpjunk );
DECLARE_DO_FUN( do_mpkill );
DECLARE_DO_FUN( do_mpmload );
DECLARE_DO_FUN( do_mpmset );
DECLARE_DO_FUN( do_mpnothing );
DECLARE_DO_FUN( do_mpoload );
DECLARE_DO_FUN( do_mposet );
DECLARE_DO_FUN( do_mppardon );
DECLARE_DO_FUN( do_mpqpadd );
DECLARE_DO_FUN( do_mpmarry );
DECLARE_DO_FUN( do_mpdivorce );
DECLARE_DO_FUN( do_mppeace );
DECLARE_DO_FUN( do_mppurge );
DECLARE_DO_FUN( do_mpstat );
DECLARE_DO_FUN( do_opstat );
DECLARE_DO_FUN( do_rpstat );
DECLARE_DO_FUN( do_mptransfer );
DECLARE_DO_FUN( do_mpmorph );
DECLARE_DO_FUN( do_mpunmorph );
DECLARE_DO_FUN( do_mpnuisance );
DECLARE_DO_FUN( do_mpunnuisance );
DECLARE_DO_FUN( do_mpbodybag );
DECLARE_DO_FUN( do_mpapply );
DECLARE_DO_FUN( do_mpapplyb );
DECLARE_DO_FUN( do_mppkset );
DECLARE_DO_FUN( do_mpfavor );
DECLARE_DO_FUN( do_mpscatter );
DECLARE_DO_FUN( do_mpdelay );
DECLARE_DO_FUN( do_mpsound );
DECLARE_DO_FUN( do_mpsoundaround );
DECLARE_DO_FUN( do_mpsoundat );
DECLARE_DO_FUN( do_mpmusic );
DECLARE_DO_FUN( do_mpmusicaround );
DECLARE_DO_FUN( do_mpmusicat );

/* Abit/QBit commands */
DECLARE_DO_FUN( do_abit );
DECLARE_DO_FUN( do_qbit );
DECLARE_DO_FUN( do_showabit );
DECLARE_DO_FUN( do_showqbit );
DECLARE_DO_FUN( do_setabit );
DECLARE_DO_FUN( do_setqbit );
DECLARE_DO_FUN( do_mpaset );
DECLARE_DO_FUN( do_mpqset );

/* mud changes commands */
DECLARE_DO_FUN( do_chsave );
DECLARE_DO_FUN( do_changes );
DECLARE_DO_FUN( do_addchange );

#endif
