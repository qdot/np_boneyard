string currentRegion;
integer ghostEnabled;

makeGhosted()
{
    vector scale = llGetScale();
    llSetScale(scale+<0.001,0.001,0.001>);
    llSetScale(scale);
}
default
{
    state_entry()
    {
        if (llGetAttached() == 0) llVolumeDetect(TRUE);
        currentRegion = llGetRegionName();
    }
    
    on_rez(integer start_param)
    {
        llResetScript();
    }
    
    link_message(integer link_number, integer num, string str, key id)
    {
        if (num == 611)
        {
            ghostEnabled = TRUE;
            makeGhosted();
        }
        else if (num == 612)
        {
            llResetScript();
        }
    }
    moving_start()
    {
        if ((currentRegion != llGetRegionName())&&(ghostEnabled)) makeGhosted();
    }
}