integer gLineCount = 0;
integer gCardLimit = 10;
integer gCardIndex = 1;

integer randBetween(float min, float max)
{
    return (integer)(llFrand(max - min) + min);
}

default
{
    state_entry()
    {
        llSetTextureAnim(ANIM_ON, ALL_SIDES, 8, 64, 0, 0, .00001);
        llSetTimerEvent(2.0);
		gCardIndex = randBetween(1, gCardLimit);
    }
    
    dataserver(key query_id, string data) 
    {
        llMessageLinked(LINK_SET, 0, data, "");
        llSetTimerEvent(2.0);
    }
    
    timer()
    {
        llGetNotecardLine((string)gCardIndex, gLineCount);
        ++gLineCount;
        if(gLineCount == 60)
        {
			llDie();
        }
        llSetTimerEvent(0.0);
    }
    
    on_rez(integer rez_param)
    {
        llResetScript();   
    }

    link_message(integer sender_num, integer num, string str, key id)
    {
        integer nodeIndex = llGetLinkNumber()*2;
        string nodeString = "0x"+llGetSubString(str, nodeIndex, nodeIndex+1);
        integer nodeValue = (integer)nodeString;
         llSetTextureAnim(ANIM_ON, ALL_SIDES, 8, 64, nodeValue, 1, .00001);  
    }
}
