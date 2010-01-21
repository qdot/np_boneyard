integer LM_FASTFLY = 10;
integer LM_FFCONTROL = 20;
integer LM_STREAMMANAGER = 30;
integer LM_THREADMANAGER_SPAWN_EMAIL_THREAD = 40;
integer LM_THREADMANAGER_REPLACE_EMAIL_THREAD=41;
integer LM_THREADMANAGER_SPAWN_XMLRPC_THREAD = 42;
integer LM_THREADMANAGER_REPLACE_XMLRPC_THREAD=43;
integer LM_EMAIL_THREAD = 50;
integer LM_XMLRPC_THREAD = 60;


list g_decisionTypes=["y", "n", "1", "0", "yes", "no", "on", "off", "start", "stop"];

integer g_fastFlyMagnitude = 0;
integer g_isNotInTurn = 0;
vector g_fastFlyMovementVector = <0,0,0>;

default
{
    state_entry()
    {
        g_fastFlyMagnitude = 0;
        g_fastFlyMovementVector = <0,0,0>;
    }
    
    link_message(integer sender_num, integer num, string str, key id)
    {
        list commandParams = llParseString2List(str, ["."], [""]);
        if(num == LM_FASTFLY)
        {
            integer decisionNum = llListFindList(g_decisionTypes, llList2List(commandParams,1,1));
            if(decisionNum < 0) return;
            //Avoid div by zero in our modulus operation
            decisionNum+=2;
            //Initial Start
            if(!(decisionNum%2))
            {
                g_fastFlyMagnitude = llList2Integer(commandParams, 2);
                state fastFly;
            }
        }
    }
}

state fastFly
{
    state_entry()
    {
        if(!(llGetPermissions() & PERMISSION_TAKE_CONTROLS))
        {
            llRequestPermissions(llGetOwner(), PERMISSION_TAKE_CONTROLS);
        }
        //If rejected, return
        if(!(llGetPermissions() & PERMISSION_TAKE_CONTROLS))
        {
            state default;
        }
        llTakeControls(CONTROL_FWD | CONTROL_BACK | CONTROL_UP | CONTROL_DOWN | CONTROL_ROT_LEFT | CONTROL_ROT_RIGHT, TRUE, TRUE);
        llSetTimerEvent(0.2);
    }
    
    state_exit()
    {
        g_fastFlyMagnitude = 0;
        llSetTimerEvent(0);
        llReleaseControls();
    }
    
    run_time_permissions(integer perm) // permissions dialog answered
    { 
        if (!(perm & PERMISSION_TAKE_CONTROLS)) state default;
    }
    
    link_message(integer sender_num, integer num, string str, key id)
    {
        list commandParams = llParseString2List(str, ["."], [""]);
        if(num == LM_FASTFLY)
        {
            integer decisionNum = llListFindList(g_decisionTypes, llList2List(commandParams,1,1));
            if(decisionNum < 0) return;
            //Avoid div by zero in our modulus operation
            decisionNum+=2;
            //Initial Start
            if(!(decisionNum%2)) g_fastFlyMagnitude = llList2Integer(commandParams,2);
            else
            {
                state default;
            }
        }
    }

    
    control(key id, integer held, integer change)
    {
        //Button Hit
        if ((change & held & CONTROL_UP))
        {
            g_fastFlyMovementVector.z = 1;
        }
        //Button Release
        else if ((change & ~held & CONTROL_UP))
        {
            g_fastFlyMovementVector.z = 0;
        }
        //Button Hit
        if ((change & held & CONTROL_DOWN))
        {
            g_fastFlyMovementVector.z = -1;
        }
        //Button Release
        else if (change & ~held & CONTROL_DOWN)
        {
            g_fastFlyMovementVector.z = 0;            
        }

        vector rotVector = llRot2Euler(llGetRot());       
        if ((change & held & CONTROL_FWD))
        {
            g_fastFlyMovementVector.x = llCos(rotVector.z);
            g_fastFlyMovementVector.y = llSin(rotVector.z);
        }
        //Button Release
        else if ((change & ~held & CONTROL_FWD))
        {
            g_fastFlyMovementVector.x = 0;            
            g_fastFlyMovementVector.y = 0;            
        }
                //Button Hit
        if ((change & held & CONTROL_BACK))
        {
            g_fastFlyMovementVector.x = -llCos(rotVector.z);
            g_fastFlyMovementVector.y = -llSin(rotVector.z);          
        }
        //Button Release
        else if ((change & ~held & CONTROL_BACK))
        {
            g_fastFlyMovementVector.x = 0;            
            g_fastFlyMovementVector.y = 0;            
        }
		
		if ((held & CONTROL_ROT_LEFT) || (held & CONTROL_ROT_LEFT))
        {
            g_isNotInTurn = 0;         
        }
		else
        {
            g_isNotInTurn = 1;
        }
    }
    
    timer()
    {
        if(g_fastFlyMovementVector != <0,0,0>)
        {           
            llApplyImpulse(-llGetMass()*(g_fastFlyMovementVector*-g_fastFlyMagnitude*g_isNotInTurn),FALSE);
        }
    }
}