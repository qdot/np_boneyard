key g_objKey = "";

default
{
    state_entry()
    {
        llSetTimerEvent(.01);
    }
    
    object_rez(key child)
    {
        g_objKey = child;
        llGiveInventory(child, "note");
    }
        
    on_rez(integer rez_param)
    {
        llResetScript(); 
    }
    
    timer()
    {
        if(llKey2Name(g_objKey) == "")
        {
            llRezAtRoot("row", llGetPos()+(<-.05, 4.25, 0>*llGetRot()), ZERO_VECTOR, llGetRot(), 0);
            llSetTimerEvent(40.0);
        }
        else
        {
            llSetTimerEvent(3.0);
        }
    }
}