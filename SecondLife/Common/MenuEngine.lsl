// Menu Engine - v 1.0.2
// by Zonax Delorean
// License: BSD, but please provide patches/bugfixes if you can!

integer LM_DOMENU   = 10001;
integer LM_OPTIONDATA = 10002;
integer LM_MENULOADED = 10003;
integer LM_RESETMENUSYSTEM = 10004;
integer LM_READSTRING = 10005;
integer LM_READSTRINGDATA = 10006;
integer LM_CUSTOMMENU   = 10007;

string confNoteName = "menudefs";
integer cLine;
key cqid;

integer MC_TEXT = 1;
integer MC_TOMENU = 2;
integer MC_OPTION = 3;
integer MC_OPTIONASK = 4;
integer MC_END = 999;
// ...

integer numMenus;
list menudata;
list menuOffsets;
integer MENUREC_SIZE = 4;

integer debug = FALSE;

emitMsgSender( integer num, string msg ) {
    if (debug) debugSay("Emit MSG: chn#"+(string)num+" : "+msg);
    llMessageLinked(llGetLinkNumber(),num,msg,cUser);
}
emitMsgGlobal( integer num ) {
    if (debug) debugSay("Emit MSG Global: chn#"+(string)num+" : ");
    llMessageLinked(llGetLinkNumber(),num,"",NULL_KEY);
}

debugSay( string st ) {
    llSay(0,st);
}

string clMenuID;
list clButtons;
list clActions;
string clMenuText;

// read data
startReadConfig() {
    numMenus = 0;
    menudata = [];
    menuOffsets = [];
    clMenuID = "";
    clButtons = [];
    clActions = [];
    clMenuText = "";

    cLine = 1;
    cqid = llGetNotecardLine( confNoteName, cLine - 1 ); // request first line
}

commitEntry() {
    // [ menuID, startOffset, startOffsetActions, endOffset ]
    integer sofs = llGetListLength(menudata);
    menuOffsets += [ clMenuID, sofs, sofs + 1 + llGetListLength(clButtons), sofs + 1 + llGetListLength(clButtons) + llGetListLength(clActions) ];
    menudata += [ clMenuText ];
    menudata += clButtons;
    menudata += clActions;
    
    clMenuID = "";
    clMenuText = "";
    clButtons = [];
    clActions = [];
}

readConfigLine( string data ) {
    if (data != EOF) {    
        integer spos = llSubStringIndex(data," ");
        if (spos==-1) jump LreadNextLine; // not a line with space

        string cmd = llGetSubString( data, 0, spos - 1);
        data = llGetSubString( data, spos+1, -1);
        
        integer cmdid = llListFindList(["MENU","TEXT","TOMENU","OPTION","OPTIONASK"],[cmd]);
        if (cmdid==-1) jump LreadNextLine; // not a command
        
        if (cmdid==0) { // MENU
            if (clMenuID!="") commitEntry();
            clMenuID = data;                        
        }
        else if (cmdid==1) { // TEXT
            if (clMenuText!="") clMenuText += "\n";
            list tmpl = llParseString2List(data,["\\n"],[]);
            data = llDumpList2String(tmpl,"\n");
            clMenuText += data;
        }
        else if (cmdid==2) { // TOMENU <MenuID> <ButtonText> or TOMENU <MenuID>
            integer wpos = llSubStringIndex(data," ");
            string tomenuid = data;
            string btext = data;
            if (wpos>=0) {
                tomenuid = llGetSubString(data,0,wpos - 1);
                btext = llGetSubString(data,wpos+1,-1);
            }
        
            clButtons  += [ btext ];
            clActions  += [ MC_TOMENU, tomenuid ];
        }
        else if (cmdid==3) { // OPTION <ButtonText>
            clButtons  += [ data ];
            clActions  += [ MC_OPTION, data ];
        }
        else if (cmdid==4) { // OPTIONASK <ButtonText>
            clButtons  += [ data ];
            clActions  += [ MC_OPTIONASK, data ];
        }    

@LreadNextLine; // jump point
        cLine++;
        cqid = llGetNotecardLine( confNoteName, cLine - 1 );
    } else {
        if (clMenuID!="") commitEntry();
        numMenus = llGetListLength(menuOffsets) / MENUREC_SIZE;
        configLoaded();   
    }
}

//
// gets called when the config is done loading
//
configLoaded() {
    if (debug) {
        debugSay("Menu data loaded, mem: "+(string)llGetFreeMemory());
        llInstantMessage(llGetOwner(),"OFS: "+llDumpList2String(menuOffsets,","));
        llInstantMessage(llGetOwner(),"DATA: "+llDumpList2String(menudata,","));
    }

    // signal a message to the main prog
    emitMsgGlobal(LM_MENULOADED);
}

string cMenuName;
integer cMenuNum;
integer CHATBASECHANNEL = 100050;
integer cOfs; // current menu start offset
key cUser; // current user
integer nList;
integer isListening = FALSE;
float RESPONSE_TIMEOUT = 60.0;
list cButtons;
list cActions;
integer cMenuType;
integer MT_ASK = 2;
integer MT_NORMAL = 1;
integer MT_READSTRING = 3;
integer MT_CUSTOM = 4;
integer cSenderID;

clearListens() {
    if (isListening) {
        llListenRemove(nList);
        isListening = FALSE;
    }
}

integer getMenuNum( string menuid ) {
    integer c;
    for (c=0;c<numMenus;c++)
        if (llList2String(menuOffsets,c*MENUREC_SIZE) == menuid)
            return(c);
    return(-1);
}

// load a data for a menu and start a dialog
doMenu( key user,  string menuname ) {
    if (debug) debugSay("Do menu: "+menuname);
    integer mnum = getMenuNum(menuname);
    if ( mnum == -1 ) { // menu not found
        llSay(0,"Error: no such menu: "+menuname);
        return;
    }    

    cOfs = llList2Integer( menuOffsets, mnum * MENUREC_SIZE + 1 );
    integer actOfs = llList2Integer( menuOffsets, mnum * MENUREC_SIZE + 2 );
    integer actEnd = llList2Integer( menuOffsets, mnum * MENUREC_SIZE + 3 );

    cMenuNum = mnum;
    cMenuName = menuname;
    cUser = user;
    cButtons = llList2List( menudata, cOfs+1, actOfs - 1 );
    cActions = llList2List( menudata, actOfs, actEnd );
    cMenuType = MT_NORMAL; // normal menu

    clearListens();    
    nList = llListen( CHATBASECHANNEL + cMenuNum, "", user, "");
    isListening = TRUE;
    llDialog( user, llList2String( menudata, cOfs ), cButtons, CHATBASECHANNEL + cMenuNum );
    llSetTimerEvent(RESPONSE_TIMEOUT);
}


// param: <TEXT>
doAskMenu( key user, string param ) {    
    cMenuType = MT_ASK;
    cButtons = [ "Yes", "No" ];
    cActions = [ param ];
    clearListens();    
    nList = llListen( CHATBASECHANNEL + cMenuNum, "", user, "");
    isListening = TRUE;
    llDialog( user, param, cButtons, CHATBASECHANNEL + cMenuNum );
    llSetTimerEvent(RESPONSE_TIMEOUT);   
}

////////////////////////////////////////////
default
{
    state_entry()
    {
        if (debug) llSay(0, "Loading menu... mem: "+(string)llGetFreeMemory());
        startReadConfig();
    }

    dataserver(key query_id, string data) {
        if (query_id==cqid) readConfigLine(data);
    }
    
    listen( integer channel, string name, key id, string message ) {
        llSetTimerEvent(0.0);
        clearListens();

        if (cMenuType==MT_ASK) {
            if (message=="Yes") {
                string param = llList2String( cActions, 0 ); // special case
                emitMsgSender(LM_OPTIONDATA,llDumpList2String([ cMenuName, param ],"|"));                
            }
            return;
        }
        if (cMenuType==MT_CUSTOM) {
            emitMsgSender(LM_OPTIONDATA,llDumpList2String([ cMenuName, message ],"|"));
            return;
        }
        if (cMenuType==MT_READSTRING) {
            string param = llList2String( cActions, 0 ); // special case
            emitMsgSender(LM_READSTRINGDATA,llDumpList2String([ param, message ],"|"));
            return;
        }

        integer c;
        integer nb = llGetListLength(cButtons);
        integer fnd = -1;
        for (c=0;c<nb;c++) {
            if (llList2String(cButtons,c)==message) {
                fnd = c;
                jump doneSR;
            }
        }
        @doneSR;
        if (fnd>=0) {
            integer act = llList2Integer( cActions, fnd * 2 );
            string param = llList2String( cActions, fnd * 2 + 1 );
            if (debug) {
                debugSay("r: "+(string)act+" "+param);
            }
            if (act==MC_TOMENU) {
                string tomenu = param;
                integer spos = llSubStringIndex(tomenu," ");
                if (spos>=0) tomenu = llGetSubString(tomenu,0,spos - 1);

                doMenu(id,tomenu);
                return;
            }
            if (act==MC_OPTION) {
                emitMsgSender(LM_OPTIONDATA,llDumpList2String([ cMenuName, param ],"|"));
                return;
            }
            if (act==MC_OPTIONASK) {
                doAskMenu(id,param);
                return;
            }

        }
    }
    
    link_message(integer sender_num, integer num, string str, key id) {
        if (num==LM_DOMENU) {
            cSenderID = sender_num;
            doMenu(id,str);
            return;
        }
        if (num==LM_READSTRING) {
            cSenderID = sender_num;
            cMenuType = MT_READSTRING;

            string message = str;
            string retval  = str;
            integer spos = llSubStringIndex(str," ");
            if (spos>=0) {
                message = llGetSubString( str, spos+1, -1 );
                retval = llGetSubString( str, 0, spos - 1 );
            }
            cActions = [ retval ];
            nList = llListen( 0, "", id, ""); // listen from the user only
            isListening = TRUE;
            llSay(0,message);
            llSetTimerEvent(RESPONSE_TIMEOUT);   
            return;
        }
        if (num==LM_CUSTOMMENU) {
            list par = llParseString2List(str,["~|~"],[]);

            if (llGetListLength(par)<2) {
                llSay(0,"Error: not enough parameters for custom menu");
                return;                
            }

            cMenuName = llList2String(par,0);
            string message = llList2String(par,1);
            cUser = id;
            cButtons = llList2List( par, 2, llGetListLength(par) - 1 );
            cMenuType = MT_CUSTOM;

            clearListens();    
            nList = llListen( CHATBASECHANNEL, "", cUser, "");
            isListening = TRUE;
            llDialog( cUser, message, cButtons, CHATBASECHANNEL );
            llSetTimerEvent(RESPONSE_TIMEOUT);
            return;
        }
        if (num==LM_RESETMENUSYSTEM) {
            llResetScript();
        }
    }
    
    timer() {
        llSetTimerEvent(0.0);
        clearListens();
    }
}