key g_ffXMLRPCChannel = "";
integer g_isInFFControl = 0;
integer g_ffValue = 0;
integer g_ffChatChannel = 0;
integer g_ffChatHandle = 0;

default
{
	state_entry()
	{
	}
	
	remote_data(integer type, key channel, key message_id, string sender, integer ival, string sval) 
    {
        if (type == REMOTE_DATA_REQUEST) 
        { 
            string rpcParams = llList2String(g_rpcThreads, 0) + "|" + (string)NP_XML_FFCONTROL + "|" + (string)channel;
			llOwnerSay("Got Message " + (string)message_id + ", routing to thread "+llList2String(g_rpcThreads, 0));
			llRemoteDataReply(channel, id, "Request Received", g_ffValue);
            //llDeleteSubList(g_rpcThreads, 0, 0);
            //llMessageLinked(LINK_THIS, g_ffValue, rpcParams, message_id);
        }
        else if (type == REMOTE_DATA_CHANNEL) 
        {
            g_ffXMLRPCChannel = channel;
            llOwnerSay("Ready to receive requests on channel "+ (string)channel);
        }
    }
}

state ForceFeedbackControl
{
	state_exit()
	{
		llListenRemove(g_ffChatHandle);
        llCloseRemoteDataChannel(g_ffXMLRPCChannel);
	}

}