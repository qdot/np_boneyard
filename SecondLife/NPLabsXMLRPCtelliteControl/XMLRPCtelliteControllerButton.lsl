integer MOVE_UP = 2;
integer MOVE_DOWN = 3;
integer MOVE_LEFT = 4;
integer MOVE_RIGHT = 5;
integer MOVE_STOP = 6;

integer g_linkType = MOVE_UP;

default
{
    state_entry()
    {
		
    }    
	
	touch_start(integer num_detected)
	{
		llMessageLinked(LINK_ROOT, g_linkType, "", "");
	}
	
	touch_end(integer num_detected)
	{
		llMessageLinked(LINK_ROOT, MOVE_STOP, "", "");
	}
}
