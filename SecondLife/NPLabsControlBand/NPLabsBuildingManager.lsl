integer NP_BUILDINGMANAGER_STREAMCHANGE = 1;

default
{
    state_entry()
    {
        llOwnerSay(llGetKey());
        llSetTimerEvent(10.0);
    }
    
    timer()
    {
        llGetNextEmail("","");
    }
    
    email(string time, string address, string subj, string message, integer num_left)
    {
        list mailParams = llParseString2List(subj, ["*"], [""]);
        if(llList2Integer(mailParams,0) == NP_BUILDINGMANAGER_STREAMCHANGE)
        {
            llSetParcelMusicURL(llList2String(mailParams,1));
        }
        if(num_left > 0)
        {
            llGetNextEmail("","");
        }
    }
}