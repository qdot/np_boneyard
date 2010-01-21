integer MOVE_UP = 2;
integer MOVE_DOWN = 3;
integer MOVE_LEFT = 4;
integer MOVE_RIGHT = 5;
integer MOVE_STOP = 6;

integer g_chatChannel = 0;
integer debug = FALSE;

default
{
    state_entry()
    {
		llSetStatus(STATUS_BLOCK_GRAB, TRUE);
		//llSetPrimitiveParams([PRIM_TEMP_ON_REZ, TRUE]);
		//llSetTimerEvent(60.0);
	}
    
	timer()
	{
		llDie();
	}
	
	link_message(integer sender_num, integer num, string str, key id)
	{
		if(debug) llOwnerSay("Move "+(string)num);
		llSay(g_chatChannel, num);
	}
	
	on_rez(integer start_param)
	{
		g_chatChannel = start_param;
	}
}
