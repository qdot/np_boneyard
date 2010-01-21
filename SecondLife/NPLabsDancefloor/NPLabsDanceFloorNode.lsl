default
{
    state_entry()
    {
        llSetTextureAnim(ANIM_ON, ALL_SIDES, 8, 64, 0, 1, 0);
    }

    link_message(integer sender_num, integer num, string str, key id)
    {
        integer nodeIndex = llGetLinkNumber()*2;
        string nodeString = "0x"+llGetSubString(str, nodeIndex, nodeIndex+1);
        integer nodeValue = (integer)nodeString;
         llSetTextureAnim(ANIM_ON, ALL_SIDES, 8, 64, nodeValue, 1, .00001);
    }
}
