list g_displayList;
integer g_listLength;

default
{
    state_entry()
    {
        llSetStatus(STATUS_BLOCK_GRAB, TRUE);
        llSetPrimitiveParams([PRIM_TEMP_ON_REZ, TRUE]);
        llSetTextureAnim(ANIM_ON, ALL_SIDES, 16, 16, 32, 0, .0001);
    }

    link_message(integer sender_num, integer num, string str, key id)
    {
		if(id != "")
		{
			llSetTexture(id, ALL_SIDES);
		}
        if(str != "")
        {
            g_displayList = llCSV2List(str);
			llSetTextureAnim(ANIM_ON, ALL_SIDES, 16, 16, llList2Integer(g_displayList, llGetLinkNumber() - 1), 0, .0001);
        }
        else
        {
            llSetTextureAnim(ANIM_ON, ALL_SIDES, 16, 16, 32, 0, .0001);
        }
    }

    on_rez(integer rez_param)
    {
        llResetScript();
	}
}
