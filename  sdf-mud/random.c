int find_rand_room(int vnum)
{
 OBJ_DATA *obj;
 ROOM_INDEX_DATA	*room = NULL;
 int count = 0;
 int i;

for (i = 300; i <=  sysdata.maxvnum; i++)
 {
    if (get_room_index(i) != NULL) count++;
 }

 for (i = 300; i <=  sysdata.maxvnum; i++)
 {
    if ((room = get_room_index(i)) != NULL 
    && number_range(1, count * 2) == 1)
	break;
    room = NULL;
 }

 if (room) 
 {
 	obj = create_object(get_obj_index(vnum),0);
	if (!obj) 
	  return 0;
	obj_to_room(obj, room, supermob);
 }
 else 
    find_rand_room(vnum);
    
 return room->vnum;
}

void do_roload(CHAR_DATA *ch, char *argument)
{
 char buf[MSL];
 int i, wh;
 char arg1[MAX_STRING_LENGTH];
 
 argument = one_argument( argument, arg1 );
 
 if ( arg1[0] == '\0' )
  {
    send_to_char( "Syntax: roload <vnum>.\r\n", ch );
    return;
  }
 
 i = atoi(arg1);
 
 if (!get_obj_index(i))
 {
    send_to_char("That object doesn't exist!\r\n", ch);
    return;
 }

 wh = find_rand_room(i);
 sprintf( buf, "%s was loaded into the mud at vnum %d\r\n",get_obj_index(i)->short_descr, wh);
 send_to_char(buf, ch);
 return;
}

