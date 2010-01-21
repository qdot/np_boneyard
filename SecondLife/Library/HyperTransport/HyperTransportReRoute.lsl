//Re-Routing System
//by Hank Ramos

//Variables
key     notecardReadKey;
list    Sims;
list    Routes;
integer lineNum;

default
{
    state_entry()
    {
        notecardReadKey = llGetNotecardLine("Reroutes", lineNum);
    }
    
    link_message(integer sender_num, integer num, string str, key id)
    {
        integer listPos;
        integer x;
        list    tempList;
        string  tempString;
        
        if (num == 8449550)
        {
            //Lookup Sim Location, if found send back the re-route
            vector dirVector = (vector)str;
            vector testVector = llGetRegionCorner() + dirVector*256;
            list tempList;
            
            //llOwnerSay("Searching for:" + str);
            
            for (x = 0; x < llGetListLength(Sims) - 1; x++)
            {
                tempList = llCSV2List(llList2String(Sims, x));
                //llOwnerSay((string)testVector.x + ":" + (string)testVector.y);
                if (testVector.x >= llList2Float(tempList, 0))
                {
                    if (testVector.x <= llList2Float(tempList, 1))
                    {
                        if (testVector.y >= llList2Float(tempList, 2))
                        {
                            if (testVector.y <= llList2Float(tempList, 3))
                            {
                                //Match Found
                                //llOwnerSay("Search term found at pos=" + (string)x);
                                tempList = llCSV2List(llList2String(Routes, x));
                                if (dirVector.x == 0)
                                {
                                    //North-South
                                    //llOwnerSay("North-South Movement:" + (string)testVector);
                                    if (dirVector.y > 0)
                                    {
                                        tempString = llList2String(tempList, 0);
                                    }
                                    else
                                    {
                                        tempString = llList2String(tempList, 1);
                                    }
                                    
                                }
                                else
                                {
                                    //East-West
                                    //llOwnerSay("East-West Movement:" + (string)testVector);
                                    if (dirVector.x > 0)
                                    {
                                        tempString = llList2String(tempList, 2);
                                    }
                                    else
                                    {
                                        tempString = llList2String(tempList, 3);
                                    }
                                }
                                //Parse
                                //llOwnerSay("Line:" + tempString + ":");
                                if (llToLower(tempString) != "(unknown)")
                                {
                                    tempList = llParseString2List(tempString, [";"], []);
                                    if (llGetListLength(tempList) > 0)
                                    {
                                        //llOwnerSay("Sending Back:" + llList2CSV(tempList));
                                        llMessageLinked(LINK_SET, 8449551, llList2CSV(tempList), NULL_KEY);
                                    }
                                    else
                                    {
                                        llMessageLinked(LINK_SET, 8449552, str, NULL_KEY);
                                    }
                                }
                                else
                                {
                                    llMessageLinked(LINK_SET, 8449552, str, NULL_KEY);
                                }
                                return;
                            }
                        }
                    }
                    
                }
            }
            llMessageLinked(LINK_SET, 8449552, "", NULL_KEY);
        }
        else if (num == 612)
        {
            llResetScript();
        }
    }
    dataserver(key queryid, string data)


    {
        list tempList;
        if (notecardReadKey == queryid)
        {
            if (data == EOF)
            {
                //llOwnerSay("Loaded " + (string)(lineNum - 1)  + " lines and " + (string)llGetListLength(Routes)  + " reroutes. Rerouter Memory Free = " + (string)llGetFreeMemory());
            }
            else
            {
                tempList = llCSV2List(data);
                if ((llGetSubString(data, 0, 0) != "#") && (data != "") && (llGetListLength(tempList) == 8))
                {
                    Sims += llDumpList2String(llList2List(tempList, 0,3), ",");
                    Routes += llDumpList2String(llList2List(tempList, 4,7), ",");
                }
                lineNum++;
                notecardReadKey = llGetNotecardLine("Reroutes", lineNum);
            }
            
        }
    }
}