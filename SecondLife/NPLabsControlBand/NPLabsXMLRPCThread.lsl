integer LM_FASTFLY = 10;
integer LM_FFCONTROL = 20;
integer LM_STREAMMANAGER = 30;
integer LM_THREADMANAGER_SPAWN_EMAIL_THREAD = 40;
integer LM_THREADMANAGER_REPLACE_EMAIL_THREAD=41;
integer LM_THREADMANAGER_SPAWN_XMLRPC_THREAD = 42;
integer LM_THREADMANAGER_REPLACE_XMLRPC_THREAD=43;
integer LM_EMAIL_THREAD = 50;
integer LM_XMLRPC_THREAD = 60;

integer g_scriptNumber = 100;
integer NP_XML_FFCONTROL = 1;

integer LM_REPLACEXMLRPCTHREAD=10200;

default
{
    state_entry()
    {
        
    }
    
    link_message(integer sender_num, integer num, string str, key id)
    {
		list xmlParams = llParseString2List(str, ["|"], [""]);
        
        if(llList2Integer(xmlParams, 0) == g_scriptNumber)
        {
            if(llList2Integer(xmlParams, 1) == NP_XML_FFCONTROL)
            {
				list rpcParams = llParseString2List(str, ["|"], [""]);
				llRemoteDataReply(llList2String(xmlParams, 2), id, "Request Received", num);
			}
		}
		llMessageLinked(LINK_THIS, LM_REPLACEXMLRPCTHREAD, (string)num, "");
    }
}