integer LM_OPTIONDATA = 10002;
integer LM_MENULOADED = 10003;
integer LM_READSTRINGDATA = 10006;

resetMenu() 
{
    llMessageLinked(llGetLinkNumber(),10004,"",NULL_KEY); // 10004: LM_RESETMENUSYSTEM
}

doMenu( key user, string menuname ) 
{
    llMessageLinked(llGetLinkNumber(),10001,menuname,user); // 10001: LM_DOMENU
}

readString( key user, string var, string prompt ) 
{
    llMessageLinked(llGetLinkNumber(),10005,var+" "+prompt,user); // 10005: READSTRING
}

customMenu( key user, string menuname, string message, list buttons ) 
{
    llMessageLinked(llGetLinkNumber(),10007,llDumpList2String([ menuname, message ] + buttons,"~|~"),user);
}

default
{
	state_entry()
	{
        resetMenu();
	}
	
    link_message(integer sender_num, integer num, string str, key id) 
	{
        if (num==LM_OPTIONDATA) {
            llSay(0,"MenuOutput: "+str);
        }
        else if (num==LM_MENULOADED) {
            llSay(0,"Menu has just been (re)loaded");   
        }
        else if (num==LM_READSTRINGDATA) {
            integer wpos = llSubStringIndex(str,"|");
            string varname = llGetSubString(str,0,wpos - 1);
            string response = llGetSubString(str,wpos+1,-1);
            llSay(0,"Variable: "+varname+" String read: "+response);
        }
	}

    touch_start(integer total_number)
    {
        //doMenu(llDetectedKey(0),"DEFAULT");

        // other things you can try (uncomment it)
        //customMenu(llDetectedKey(0),"CUST1","Test custom menu",["Test A","Test B","Test C"]);
        //readString(llDetectedKey(0),"FAVCOLOR","Please enter your favorite color in chat:");
    }

}