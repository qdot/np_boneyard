integer LM_FASTFLY = 10;
integer LM_FFCONTROL = 20;
integer LM_STREAMMANAGER = 30;
integer LM_THREADMANAGER_SPAWN_EMAIL_THREAD = 40;
integer LM_THREADMANAGER_REPLACE_EMAIL_THREAD=41;
integer LM_THREADMANAGER_SPAWN_XMLRPC_THREAD = 42;
integer LM_THREADMANAGER_REPLACE_XMLRPC_THREAD=43;
integer LM_EMAIL_THREAD = 50;
integer LM_XMLRPC_THREAD = 60;

list g_emailThreads=[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19];
list g_rpcThreads=[100,101,102,103,104,105,106,107,108,109];

default
{
    state_entry()
    {
    }
    
    link_message(integer sender_num, integer num, string str, key id)
    {
        if(num - 40 < 0 || num - 40 > 10) return;
        if(num == LM_THREADMANAGER_SPAWN_EMAIL_THREAD)
        {
            str = llList2String(g_emailThreads, 0) + "|" + str;
            llDeleteSubList(g_emailThreads, 0, 0);
            llMessageLinked(LINK_THIS, LM_EMAIL_THREAD, str, id);
        }
        else if(num == LM_THREADMANAGER_SPAWN_XMLRPC_THREAD)
        {
        }
        else if(num == LM_THREADMANAGER_REPLACE_EMAIL_THREAD)
        {
            llListInsertList(g_emailThreads, [(integer)str], llGetListLength(g_emailThreads) - 1);
        }
        else if(num == LM_THREADMANAGER_REPLACE_XMLRPC_THREAD)
        {
            llListInsertList(g_rpcThreads, [(integer)str], llGetListLength(g_rpcThreads) - 1);
        }
        
    }
}