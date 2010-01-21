integer MOVE_UP = 2;
integer MOVE_DOWN = 3;
integer MOVE_LEFT = 4;
integer MOVE_RIGHT = 5;
integer MOVE_STOP = 6;

default
{
    state_entry()
    {
    }
    
    link_message(integer sender_num, integer num, string str, key id)
	{
		if(sender_num == LINK_ROOT)
		{
			llSetLocalRot((rotation)str);
		}
	}
}
