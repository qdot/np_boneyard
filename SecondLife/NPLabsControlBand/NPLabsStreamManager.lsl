integer LM_FASTFLY = 10;
integer LM_FFCONTROL = 20;
integer LM_STREAMMANAGER = 30;
integer LM_THREADMANAGER_SPAWN_EMAIL_THREAD = 40;
integer LM_THREADMANAGER_REPLACE_EMAIL_THREAD=41;
integer LM_THREADMANAGER_SPAWN_XMLRPC_THREAD = 42;
integer LM_THREADMANAGER_REPLACE_XMLRPC_THREAD=43;
integer LM_EMAIL_THREAD = 50;
integer LM_XMLRPC_THREAD = 60;

integer NP_BUILDINGMANAGER_STREAMCHANGE = 1;

list g_locationManagerIDs = ["home","1c423267-b795-a8f7-8dfc-fa269ef8b122", "lab", "34f07e55-fd4d-543e-4f78-ce01f2757596"];

list g_streamURLs = ["glowdot","http://radio.glowdot.com:8000/", 
                    "systrum sistem","http://205.138.198.226:9000", 
                    "xstream","http://xstreamradio.dyndns.org:80/", 
                    "addictive beat","http://69.31.78.14:8000/", 
                    "cliqhop","http://somafm-sc.streamguys.com:8062"];
                    
                    
changeStreamInt(integer locationIndex, integer streamIndex)
{    
    string mailParams = (string)NP_BUILDINGMANAGER_STREAMCHANGE + "*" + llList2String(g_streamURLs, (streamIndex*2)+1) + "|";
    llMessageLinked(LINK_THIS, LM_THREADMANAGER_SPAWN_EMAIL_THREAD, mailParams, llList2String(g_locationManagerIDs, (locationIndex*2)+1));
    llSay(0, "NPLabs Control Band: Stream change to "+llList2String(g_streamURLs, streamIndex*2)+" at "+llList2String(g_locationManagerIDs, (locationIndex*2)));
}

changeStreamString(integer locationIndex, string streamName)
{
    integer streamIndex = llListFindList(g_locationManagerIDs, [llToLower(streamName)]);
    if(streamIndex > 0)
    {
        changeStreamInt(locationIndex, streamIndex/2);
    }
    else if(streamIndex == 0)
    {
        changeStreamInt(locationIndex, 0);
    }
}

integer changeLocationString(string locationName)
{
    integer locationPos = llListFindList(g_locationManagerIDs, [llToLower(locationName)]);
    if(locationPos < 0)
    {
        llOwnerSay("Stream Change: Invalid Location");
        return -1;
    }
    return locationPos/2;
}

default
{
    state_entry()
    {
    }
    
    link_message(integer sender_num, integer num, string str, key id)
    {
        list commandParams = llParseString2List(str, ["."], [""]);
        if(num == LM_STREAMMANAGER)
        {
            integer locationIndex;
            if(llStringLength(llList2String(commandParams,1)) < 3)
            {
                locationIndex = llList2Integer(commandParams,1);
            }
            else
            {
                locationIndex = changeLocationString(llList2String(commandParams,1));
 
            }
            if(locationIndex < 0 || locationIndex > (llGetListLength(g_locationManagerIDs) * 2) - 1)
            {
                return;
            }
            
            if(llStringLength(llList2String(commandParams,2)) < 3)
            {
                changeStreamInt(locationIndex, llList2Integer(commandParams,2));
            }
            else
            {
                changeStreamString(locationIndex, llList2String(commandParams,2));
            }
        }
    }
}