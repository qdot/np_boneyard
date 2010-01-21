integer LM_FASTFLY = 10;
integer LM_FFCONTROL = 20;
integer LM_STREAMMANAGER = 30;
integer LM_THREADMANAGER_SPAWN_EMAIL_THREAD = 40;
integer LM_THREADMANAGER_REPLACE_EMAIL_THREAD=41;
integer LM_THREADMANAGER_SPAWN_XMLRPC_THREAD = 42;
integer LM_THREADMANAGER_REPLACE_XMLRPC_THREAD=43;
integer LM_EMAIL_THREAD = 50;
integer LM_XMLRPC_THREAD = 60;


default
{
    state_entry()
    {
        llListen(76367, "", llGetOwner(), "");
    }
    
    listen(integer channel, string name, key id, string message)
    {
        if(channel == 76367)
        {
            list commandParams = llParseString2List(message, ["."], [""]);
            if(llToLower(llList2String(commandParams,0)) == "stream")
            {
                llMessageLinked(LINK_THIS, LM_STREAMMANAGER, message, "");
            }        
            else if(llToLower(llList2String(commandParams,0)) == "fastfly")
            {
				llMessageLinked(LINK_THIS, LM_FASTFLY, message, "");
            }
        }
    } 
}