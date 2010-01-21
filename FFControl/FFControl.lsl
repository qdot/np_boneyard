// XML-RPC example

integer gSayRange = 0;
key     gChannel; // my llRemoteData channel
integer     gChangeValue;
integer gListenChannel;

integer randBetween(float min, float max)
{
    return llCeil(llFrand(max - min) + min);
}

DEBUG(list out) { // output debug info
    llSay(0, llList2CSV(out));
}

default { // initialize XML-RPC
    state_entry() {
        llOpenRemoteDataChannel(); // create an XML-RPC channel
        // this will raise remote_data event REMOTE_DATA_CHANNEL when created
    }

    remote_data(integer type, key channel, key message_id, string sender, integer ival, string sval) {
        if (type == REMOTE_DATA_CHANNEL) { // channel created
            gChannel = channel;
            llSay(gSayRange, "Ready to receive requests on channel \"" + (string)channel +"\"");
            state go; // start handling requests
        } else DEBUG(["Unexpected event type", type, channel, message_id, sender, ival, sval]);
    }
}
 
state go { // handle requests

    state_entry()
    {
        gChangeValue = 0;
        gListenChannel = randBetween(1, 100);
        llListen(gListenChannel, "", "", "");
        llSay(gSayRange, "To talk to me, chat on channel " + (string)gListenChannel);
        llSay(gSayRange, "Change speeds by typing in ff <speed>");
        llSay(gSayRange, "I.e. if you want 100% speed, use the command");
        llSay(gSayRange, "ff 100");
        llSay(gSayRange, "ff -1 to turn off client query and shut down");
    }    
    
    listen(integer channel, string name, key id, string message)
    {
        list sayList = llParseString2List(message, [" "], []);
        if(llList2String(sayList, 0) == "ff" || llList2String(sayList, 1) == "ff")
        {
            gChangeValue = (integer)llList2String(sayList,1);
            llSay(gSayRange, "Changing Vibe Value to " + (string)gChangeValue);
            if(gChangeValue < 0)
            {
                llSay(gSayRange, "Calling Timer to Exit");
            }
        }
    }
    
    remote_data(integer type, key channel, key message_id, string sender, integer ival, string sval) {
        if (type == REMOTE_DATA_REQUEST) { // handle requests sent to us
            // handle request
                llRemoteDataReply(channel, message_id, "Request Received", gChangeValue);
                llSay(gSayRange, "Update sent to client with value " + (string)gChangeValue);
            }
        else DEBUG(["Unexpected event type:", type, channel, message_id, sender, ival, sval]);
    }

    state_exit() {
        llCloseRemoteDataChannel(gChannel); // clean up when you no longer want to handle requests
        llSay(gSayRange, "Shutting down channel");
    }
    
    on_rez(integer start_param)
    {
        gChangeValue = 0;
        llCloseRemoteDataChannel(gChannel); // clean up when you no longer want to handle requests
        llSay(gSayRange, "Shutting down channel");
        llResetScript();
    }
}
