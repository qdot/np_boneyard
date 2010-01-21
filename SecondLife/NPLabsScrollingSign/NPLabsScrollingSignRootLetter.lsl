list g_displayList;
string g_sendString;
integer g_listLength;
integer g_notecardIndex = 0;
integer g_notecardLimit = 0;
integer g_stringLength = 0;
integer g_isScrolling = FALSE;
integer g_timerIndex = 0;
integer g_doHaveString = FALSE;
integer g_isChangingTexture = FALSE;
key g_textureUUID = "";
key g_notecardCountRequest = "";
key g_notecardLineRequest = "";

//Ascii list taken from LSL Forth Compiler
//http://forums.secondlife.com/showthread.php?t=34567
list g_ascii1 = ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""];
list g_ascii2 = [" ","!","quote","#","$","%","&","'","(",")","*","+",",","-",".","/",
                "0","1","2","3","4","5","6","7","8","9",":",";","<","=",">","?","@"];
list g_ascii3 = ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V",
                "W","X","Y","Z","[","\\","]","^","_","`"];
list g_ascii4 = ["a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v",
                "w","x","y","z","{","|","}","~","DEL"];
list g_ascii;

ascii_init()
{
    g_ascii = g_ascii1+g_ascii2+g_ascii3+g_ascii4;
    g_ascii1 = [];
    g_ascii2 = [];
    g_ascii3 = [];
    g_ascii4 = [];
}

string generateSendString(string unformattedString)
{
    integer i;
    integer strLength = llStringLength(unformattedString);
    string sendString = "";
    string finalString = "";
    for(i=0;i<strLength;++i)
    {
         sendString += (string)(llListFindList(g_ascii,[llGetSubString(unformattedString,i,i)]) + 1)+",";
    }
    integer spaceLength = ((20-strLength)/2);
    for(i=0;i<spaceLength;++i)
    {
        finalString += "32,";
        sendString += "32,";
    }
    finalString += sendString;
    return finalString;
}

default
{
    state_entry()
    {
        llSetStatus(STATUS_BLOCK_GRAB, TRUE);
        llSetPrimitiveParams([PRIM_TEMP_ON_REZ, TRUE]);
        llSetTextureAnim(ANIM_ON, ALL_SIDES, 16, 16, 32, 0, .0001);
        ascii_init();
        llSetTimerEvent(2.0);
		g_notecardCountRequest = llGetNumberOfNotecardLines("note");
        g_isScrolling = FALSE;
        g_timerIndex = 0;    
    }

        
    dataserver(key queryid, string data)
    {
        if(queryid == g_notecardCountRequest)
        {
            g_notecardLimit = (integer)data;
        }
        else
        {
			string strData = data;
			if(llSubStringIndex(data,"x,") == 0)
			{
				strData = llGetSubString(data, 2, llStringLength(data));
				list parseList = llParseString2List(strData, "%", "");
				integer i;
				string strTemp;
				for(i=0; i < llGetListLength(parseList); ++i)
				{
					strTemp = llList2String(parseList, i);
					if(strTemp
				}
			}
			if(llSubStringIndex(data, "texture,") == 0)
			{
				strData = "";
				list textureData = llCSV2List(data);
				g_isChangingTexture = TRUE;
				g_textureUUID = llList2Key(textureData, 1);
				llMessageLinked(LINK_SET, -1, "", g_textureUUID);
				integer i;
				integer listLen = llGetListLength(textureData);
				for(i = 2; i < listLen; ++i)
				{
					strData += llList2String(textureData, i);
					if(i+1 != listLen)
					{
						strData += ",";
					}
				}
				while(listLen < llGetListLength(textureData))
				{
					strData+="32,";
				}
			}
			g_sendString = generateSendString(strData);
			g_stringLength = llGetListLength(llCSV2List(g_sendString));
			g_doHaveString = TRUE;
			++g_notecardIndex;
			if(g_notecardIndex == g_notecardLimit)
			{
				g_notecardIndex = 0;
			}
			llMessageLinked(LINK_SET, -1, g_sendString, "");
			llSetTimerEvent(2.0);
        }
    }
    
    link_message(integer sender_num, integer num, string str, key id)
    {
        if(id != "")
        {
            llSetTexture(id, ALL_SIDES);
        }
        else if(str != "")
        {
            g_displayList = llCSV2List(str);
            llSetTextureAnim(ANIM_ON, ALL_SIDES, 16, 16, llList2Integer(g_displayList, llGetLinkNumber() - 1), 0, .0001);
        }
        else
        {
            llSetTextureAnim(ANIM_ON, ALL_SIDES, 16, 16, 32, 0, .0001);
        }
    }
    
    timer()
    {
        if(!g_doHaveString)
        {
            llSetTimerEvent(0.0);
            llGetNotecardLine("note", g_notecardIndex);
        }
        else if(g_timerIndex < 20)
        {
            ++g_timerIndex;
            g_doHaveString = FALSE;
        }
        else
        {
            g_timerIndex = 0;
            g_doHaveString = FALSE;
            llDie();
        }        
    }

    on_rez(integer rez_param)
    {
        llResetScript();
    }

    changed(integer change) 
    {
        if (change & CHANGED_INVENTORY)
        {
            llSetTimerEvent(2.0);
        }
    }
}
