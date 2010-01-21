integer LM_FASTFLY = 10;
integer LM_FFCONTROL = 20;
integer LM_STREAMMANAGER = 30;
integer LM_THREADMANAGER_SPAWN_EMAIL_THREAD = 40;
integer LM_THREADMANAGER_REPLACE_EMAIL_THREAD=41;
integer LM_THREADMANAGER_SPAWN_XMLRPC_THREAD = 42;
integer LM_THREADMANAGER_REPLACE_XMLRPC_THREAD=43;
integer LM_EMAIL_THREAD = 50;
integer LM_XMLRPC_THREAD = 60;

integer g_scriptNumber = 0;

default
{
    state_entry()
    {
    }
    
    link_message(integer sender_num, integer num, string str, key id)
    {
        if(num == LM_EMAIL_THREAD)
        {
            //Mail Params: [script number]|[subject]|[message]
            list mailParams = llParseString2List(str, ["|"], [""]);
            if(llList2Integer(mailParams, 0) == g_scriptNumber)
            {
                if(llStringLength(id) > 0) llEmail((string)id+"@lsl.secondlife.com", llList2String(mailParams,1), llList2String(mailParams,2));
                llMessageLinked(LINK_THIS, LM_THREADMANAGER_REPLACE_EMAIL_THREAD, (string)g_scriptNumber, "");
            }
        }
    }
}