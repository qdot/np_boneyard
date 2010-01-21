//Inter-Simulator Transport
//by HankRamos
//Version 1.4.2

//Options
float   travelHeight  = 1000;
float   moveTau       = 0.05;
integer HudOn         = TRUE;
integer maxFreeAge    = 60;
integer teleportPrice = 1;

//Variables
list    targetRegionList;
vector  targetLandmarkFileWPos; // World Coordinates for Landmark File
vector  evalPosition;
vector  subTravelWPos;
vector  subTravelLPos;
vector  targetRegionWPos; //World Coordinates
vector  targetRegionLPos; //Local Coordinates
string  targetRegionName;
string  targetName;
string  travelMode;
key     regionReadKey;
key     agentInfoKey;
key     ownerKey;
integer menuChatID;
integer targetHandle;
integer timeoutCounter;
integer evalDirection;
integer secondTrigger;
float   subArrivalDistance;
float   distanceToTarget;

//Constants
 
integer timestamp_to_unixtime(string timestamp) 
{
    integer year  = (integer) llGetSubString(timestamp,  0,  3);
    integer month = (integer) llGetSubString(timestamp,  5,  6);
    
    return
    ( 
        // days = (year - 1970) * 365;
        (year - 1970) * 365
        // days += (integer) (((year - 1) - 1968) / 4); 
        + (year - 1969) / 4
        // days += llList2Integer( daysPerMonth, month );
        + llList2Integer( [0, -1,  30, 58, 89, 119, 150, 180,  211, 242, 272, 303, 333], month )
        // days += day;
        + (integer) llGetSubString(timestamp,  8,  9)
        // if ( month > 2 ) days+= !(year % 4);
        + (month>2) * !(year % 4)
    ) 
    // time += days   * 86400;
    * 86400;
    // time += hour   * 3600;  // SECONDS_PER_HOUR;
    //+ (integer) llGetSubString(timestamp, 11, 12) * 3600
    // time += hour   * 3600;  // SECONDS_PER_HOUR;
    //+ (integer) llGetSubString(timestamp, 14, 15) * 60
    // time += second;
    //+ (integer) llGetSubString(timestamp, 17, 18);
}

vector totarget(vector global, vector local)
{ 
    vector target;
    vector dir;
    vector corner = llGetRegionCorner();
    vector pos = llGetPos();
    
    pos.x = pos.x + corner.x;
    pos.y = pos.y + corner.y; 
    
    target.x = global.x + local.x; 
    target.y = global.y + local.y;
    target.z = pos.z; 
    
    dir = target - pos;
    
    distanceToTarget = llVecMag(dir); //Update global distance calculation variable
     
    return llVecNorm(global - llGetRegionCorner());   
}


dispUpdate()
{
    if (HudOn == TRUE)
    {
        vector pos = llGetPos();
        float currentSpeed = llVecMag(llGetVel());
        
        llSetText("Location: " + llGetRegionName() + " <" + (string)((integer)pos.x) + ", " + (string)((integer)pos.y) + ", " + (string)((integer)pos.z) + "> \nSpeed: " + (string)((integer)currentSpeed) +"m/s \nTarget: " + targetName +" \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n", <1,1,1>, 1);
    }
    else if (HudOn == FALSE)
    {
        llSetText("", <1,1,1>, 0);
        HudOn = -1;
    }
    //Otherwise send no client update! :)
}

showDialog(string message, list options)
{
    integer randomChannel = -(integer)(llFrand(2147483647) + 1);
    llListenRemove(menuChatID);
    menuChatID = llListen(randomChannel, "", NULL_KEY, "");
    llDialog(llGetOwner(), message, options, randomChannel);
}
showOptionsMenu()
{
    list dialogList = ["Landmark", "Height", "Help", "Reset"];
    if (HudOn == TRUE)
    {
        dialogList += "HUD Off";
    }
    else
    {
        dialogList += "HUD On";
    }
    showDialog("Main Options Menu", dialogList);
}
processGoCommand(string message)
{
    list tempList = llCSV2List(message);
    integer listLength = llGetListLength(tempList);
    //teleport clara,100,125,65
    //landmark <325520, 275502, 0>,100,125,65

    //Process Local Coordinates
    targetRegionLPos = <128,128,0>;

    if (listLength == 4)
    {
        targetRegionLPos.z = llList2Float(tempList, 3);
    }
    if ((listLength == 4) || (listLength == 3))
    {
        targetRegionLPos.x = llList2Float(tempList, 1);
        targetRegionLPos.y = llList2Float(tempList, 2);
    }

    //Process World Coordinates
    if (targetName == "Landmark")
    {
        targetRegionWPos = (vector)llList2String(tempList, 0);
        targetRegionName = "LandmarkFile";
        targetLandmarkFileWPos = targetRegionWPos;
        if (TRUE) state prep;
    }
    else
    {
        targetRegionList = llParseString2List(llList2String(tempList, 0), [";"], []);
        targetRegionName = llList2String(targetRegionList, 0);
        targetRegionList = llDeleteSubList(targetRegionList, 0, 0);
        regionReadKey = llRequestSimulatorData(targetRegionName, DATA_SIM_POS);                   
    }
}
processChat(string message)
{
    float checkN;
    vector localPos = llGetPos();
    message = llToLower(message);

    if (llSubStringIndex(message, "teleport ") == 0) 
    {
        travelMode = "teleport";
        targetName = "Manual Location";
        processGoCommand(llGetSubString(message, 8, llStringLength(message)));
    }
    else if (llSubStringIndex(message, "hop ") == 0) 
    {
        travelMode = "hop";
        targetName = "Manual Location";
        processGoCommand(llGetSubString(message, 3, llStringLength(message)));
    }
    else if (llSubStringIndex(message, "landmark ") == 0)
    {
        //Reading Landmark key
        travelMode = "teleport";
        targetName = "Landmark";
        processGoCommand(llGetSubString(message, 8, llStringLength(message)));
    }
    else if (message == "help")
    {
        llGiveInventory(llGetOwner(), llGetInventoryName(INVENTORY_NOTECARD, 0));
    }
    else if (message == "reset")
    {
        llResetScript();
    }
    else if (message == "hud on")
    {
        HudOn = TRUE;
    }
    else if (message == "hud off")
    {
        HudOn = FALSE;
    }
    else if (message == "height")
    {
        showDialog("Travel Height Options. Select a travel height that you wish to use while moving...", ["1000 m", "500 m", "100 m", "Ground Level"]);
    }
    else if (message == "ghost")
    {
        //llOwnerSay("Ghost Enabled!");
        llMessageLinked(LINK_SET, 611, "", NULL_KEY);
    }
    else if (message == "noghost")
    {
        //llOwnerSay("Ghost Disabled!");
        llMessageLinked(LINK_SET, 612, "", NULL_KEY);
    }
    else if (llGetSubString(message, llStringLength(message) - 2, llStringLength(message) - 1) == " m")
    {
        travelHeight = (float)llGetSubString(message, 0, llStringLength(message) - 2);
        llOwnerSay("Travel Height Set to " + (string)((integer)travelHeight) + " m");
    }
    else if (message == "ground level")
    {
        travelHeight = 0;
        llOwnerSay("Travel Height Set to Just Above Ground Level.");
    }
}
travelEvaluation()
{
    llSetTimerEvent(0);
    //llApplyImpulse(-llGetMass()*llGetVel(),FALSE);
    vector currentSim = llGetRegionCorner();
    vector pos = llGetPos();
    evalPosition = totarget(targetRegionWPos, targetRegionLPos);         
    integer moveHorizontal;
    
    //Figure out what direction to go...
    if (currentSim != targetRegionWPos)
    {
        secondTrigger = FALSE;
        vector directionPos;               
        if (evalPosition.x == 0) 
        {
            //llOwnerSay("evalPosition.x == 0");
            //move vertical
            directionPos.x = 0;
            directionPos.y = evalPosition.y;
            //Check Edge of World
            if (llEdgeOfWorld(pos, llVecNorm(directionPos)))
            {
                //Request Reroute
                llSetTimerEvent(0);
                llMessageLinked(LINK_SET, 8449550, (string)llVecNorm(directionPos), NULL_KEY);
                return;
            }
        }
        else if (evalPosition.y == 0)
        {
            //llOwnerSay("evalPosition.y == 0");
            //move horizontal
            directionPos.x = evalPosition.x;
            directionPos.y = 0;
            //Check Edge of World
            if (llEdgeOfWorld(pos, llVecNorm(directionPos)))
            {
                //Request Reroute
                llSetTimerEvent(0);
                llMessageLinked(LINK_SET, 8449550, (string)llVecNorm(directionPos), NULL_KEY);
                return;
            }
        }
        else if (llFabs(evalPosition.x) >= llFabs(evalPosition.y))
        {
            //move horizontal
            directionPos.x = evalPosition.x;
            directionPos.y = 0;
            //Check Edge of World
            if (llEdgeOfWorld(pos, llVecNorm(directionPos)))
            {
                //move vertical
                directionPos.x = 0;
                directionPos.y = evalPosition.y;
                if (llEdgeOfWorld(pos, llVecNorm(directionPos)))
                {
                    //Request Reroute
                    directionPos.x = evalPosition.x;
                    directionPos.y = 0;
                    llSetTimerEvent(0);
                    llMessageLinked(LINK_SET, 8449550, (string)llVecNorm(directionPos), NULL_KEY);
                    return;
                }
            }
        }
        else if (llFabs(evalPosition.x) < llFabs(evalPosition.y))
        {
            //move vertical
            directionPos.x = 0;
            directionPos.y = evalPosition.y;
            //Check Edge of World
            if (llEdgeOfWorld(pos, llVecNorm(directionPos)))
            {
                //move horizontal
                directionPos.x = evalPosition.x;
                directionPos.y = 0;
                if (llEdgeOfWorld(pos, llVecNorm(directionPos)))
                {
                    //Request Reroute
                    directionPos.x = 0;
                    directionPos.y = evalPosition.y;
                    llSetTimerEvent(0);
                    llMessageLinked(LINK_SET, 8449550, (string)llVecNorm(directionPos), NULL_KEY);
                    return;
                }
            }
        }
        directionPos.z = evalPosition.z;
                    
        //llOwnerSay("Need to Move to Another Sim. Direction: " + (string)llVecNorm(directionPos));
        subTravelWPos = currentSim;
        subTravelWPos += llVecNorm(directionPos) * 256;
        subTravelLPos.x = 128;
        subTravelLPos.y = 128;
        subArrivalDistance = 45;
    }
    else if (llGetListLength(targetRegionList) > 0)
    {
        targetRegionName = llList2String(targetRegionList, 0);
        targetRegionList = llDeleteSubList(targetRegionList, 0, 0);
        if (targetRegionName == "LandmarkFile")
        {
            targetRegionWPos = targetLandmarkFileWPos;
            travelEvaluation();
        }
        else
        {
            regionReadKey = llRequestSimulatorData(targetRegionName, DATA_SIM_POS);
        }
        return;                
    }
    else if (distanceToTarget < 1)
    {
        if (TRUE) state arrive;
    }
    else
    {
        //llOwnerSay("Need to move within this sim.");
        subTravelWPos = targetRegionWPos;
        subTravelLPos = targetRegionLPos;
        subTravelLPos.z = travelHeight;
        subArrivalDistance = 1;
    }
    llSetTimerEvent(moveTau);
}
default
{ 
    state_entry() 
    {
        llSetText("Initializing...", <1,0, 0>, 1);
        ownerKey = llGetOwner();
        agentInfoKey = llRequestAgentData(ownerKey, DATA_BORN);
        llSetStatus(STATUS_PHYSICS, TRUE);
    }
    run_time_permissions(integer permissions)
    { 
        if (llGetPermissions() & PERMISSION_TRIGGER_ANIMATION)
        {
            llOwnerSay("Transport Initialized!");
            llOwnerSay("To Hypertransport to your destination, just say teleport and the sim name. Example: teleport Stillman. You can also give local coordinates: teleport Stillman,148, 83. Click the Hypertransport pack for options and help. Enjoy!");
            state idle;            
        }
    }
    attach(key id)
    {
        llResetScript();
    }
    on_rez(integer star_param)
    {
        llResetScript();
    }
    listen(integer channel, string name, key id, string message)
    {
        llRequestPermissions(llGetOwner(), PERMISSION_DEBIT | PERMISSION_TRIGGER_ANIMATION);
    }
    dataserver(key requested, string data)
    {
        integer currentCustomerAge;
        
        if (requested == agentInfoKey)
        {
            currentCustomerAge = (timestamp_to_unixtime(llGetDate()) - timestamp_to_unixtime(data))/86000;
            if (currentCustomerAge > maxFreeAge) 
            {
                showDialog("Welcome to HyperTeleportation! L$1 cost per destination. Please accept L$ debit and animation permissions to use.", ["OK"]);
            }
            else
            {
                teleportPrice = 0;
                llOwnerSay("Welcome to Second Life! You may use this Hyperteleporter pack for FREE until you are " + (string)maxFreeAge + " days old!");
                llRequestPermissions(llGetOwner(), PERMISSION_TRIGGER_ANIMATION);
            }
        }
    }
    touch_start(integer num_detected)
    {
        llResetScript();
    }
}

state idle
{
    state_entry()
    {
        llMessageLinked(LINK_SET, 612, "", NULL_KEY);
        llListen(0, "", llGetOwner(), "");
        llSetForce(<0,0,0>, FALSE);
        llOwnerSay("Memory Free=" + (string)llGetFreeMemory());
        targetName = "Idle";
        distanceToTarget = 0;
        llSetTimerEvent(1);
    }


    timer()
    {
        dispUpdate();
    }
    attach(key id)
    {
        if (ownerKey != llGetOwner()) llResetScript();
        if (teleportPrice == 0) agentInfoKey = llRequestAgentData(llGetOwner(), DATA_BORN);
    }
    on_rez(integer star_param)
    {
        if (ownerKey != llGetOwner()) llResetScript();
    }
    
    dataserver(key requested, string data)
    {
        integer currentCustomerAge;
        
        if (requested == regionReadKey)
        {
            targetRegionWPos = (vector)data;
            state prep;
        }

        if (requested == agentInfoKey)
        {
            currentCustomerAge = (timestamp_to_unixtime(llGetDate()) - timestamp_to_unixtime(data))/86000;
            if (currentCustomerAge > maxFreeAge) 
            {
                llResetScript();
            }
        }
    }
    
    listen(integer channel, string name, key id, string message)
    {
        vector localPos = llGetPos();
        
        if (llToLower(message) == "landmark")
        {
            //if (llScriptDanger(localPos) == FALSE)
            //{
                integer randomChannel = (integer)llFrand(2000000);
                llRezObject("HyperTeleporter Landmark Retriever", localPos, ZERO_VECTOR, ZERO_ROTATION, randomChannel);
                menuChatID = llListen(randomChannel, "", NULL_KEY, "");
                llOwnerSay("Drag-And-Drop a Landmark File Onto This Sphere...");
            //}
            //else
            //{
            //    llOwnerSay("Can't Rez Landmark Accepter. Please move to a build and script-enabled area.");
            //}
        }   
        processChat(message);
    }

    touch_start(integer total_number)
    {
        showOptionsMenu();
    }
    state_exit()
    {
    }
}

state prep
{
    state_entry()
    {
        llMessageLinked(LINK_SET, 611, "", NULL_KEY);
        //float  testHeight;
        if (teleportPrice > 0) 
        {
            if (!(llGetPermissions() & PERMISSION_DEBIT)) llResetScript();
            llGiveMoney("273e1e0e-3f95-429c-b093-ab4fb8e4cf6b", teleportPrice);
        }
        
        list tempList = llParseString2List(llKey2Name(llGetOwner()), [" "], []);
        if (llToLower(llList2String(tempList, llGetListLength(tempList) - 1)) != "linden") llSetForce(<0,0,9.8> * llGetMass(), FALSE);
        
        llListen(0, "", llGetOwner(), "");
        
        //Targets
        subTravelLPos = llGetPos();
        if (travelMode == "hop") subTravelLPos.z = llGround(ZERO_VECTOR) + 50;
        if (travelMode == "teleport") subTravelLPos.z = travelHeight;
        if (travelHeight == 0) subTravelLPos.z = llGround(ZERO_VECTOR) + 50;
        if (llGetRegionCorner() == targetRegionWPos)
        {
            travelEvaluation();
            state movement;
        }
        targetHandle = llTarget(subTravelLPos, 0.4);        
    }
    
    not_at_target()
    {
        vector tempPos = subTravelLPos;
        vector pos = llGetPos();
        
        if (llFabs(pos.z - tempPos.z) > 50 )
        {
            if (pos.z > tempPos.z)
            {
                tempPos.z = pos.z - 50;
            }
            else
            {
                tempPos.z = pos.z + 50;
            }
        }
        llMoveToTarget(tempPos, 0.1);        
        dispUpdate();
    }
    
    at_target(integer tnum, vector targetpos, vector ourpos)
    {
        subTravelWPos   = llGetRegionCorner();
        subTravelLPos.x = 128;
        subTravelLPos.y = 128;
        subArrivalDistance = 75;
        state movement;
    }
    
    attach(key id)
    {
        state idle;
    }
    on_rez(integer star_param)
    {
        state idle;
    }
    touch_start(integer total_number)
    {
        showOptionsMenu();
    }
    
    listen(integer channel, string name, key id, string message)
    {
        processChat(message);
    }

    state_exit()
    {
        llStopMoveToTarget();
        llApplyImpulse(-llGetMass()*llGetVel(),FALSE);
        llTargetRemove(targetHandle);
    }
}

state movement
{
    state_entry()
    {
        llOwnerSay("Moving...");
        //llOwnerSay("Region Route List Currently: " + llDumpList2String(targetRegionList, "*"));
        llListen(0, "", llGetOwner(), "");
        llSetTimerEvent(0.1);       
    }
    
    listen(integer channel, string name, key id, string message)
    {    
        message = llToLower(message);
        
        if ((message == "stop") || (message == "abort"))
        {
            llApplyImpulse(-llGetMass()*llGetVel(),FALSE);
            state idle;
        }
        processChat(message);
    }
    attach(key id)
    {
        state idle;
    }

    on_rez(integer star_param)
    {
        state idle;
    }
    
    dataserver(key requested, string data)
    {
        if (requested == regionReadKey)
        {
            targetRegionWPos = (vector)data;
            travelEvaluation();
        }
    }

    link_message(integer sender_num, integer num, string str, key id)
    {
        if (num == 8449551)
        {
            secondTrigger = FALSE;               
            targetRegionList = llCSV2List(str) + [targetRegionName] + targetRegionList;
            //llOwnerSay("Region Route List Updated to: " + llDumpList2String(targetRegionList, "*"));
            targetRegionName = llList2String(targetRegionList, 0);
            targetRegionList = llDeleteSubList(targetRegionList, 0, 0);
            if (targetRegionName == "LandmarkFile")
            {
                targetRegionWPos = targetLandmarkFileWPos;
                travelEvaluation();
            }
            else
            {
                regionReadKey = llRequestSimulatorData(targetRegionName, DATA_SIM_POS);
            }
        }
        else if (num == 8449552)
        {
            if (secondTrigger)
            {
                llOwnerSay("Can't find a route. Please manually route to your destination. Route aborted!");
                state idle;
            }
            //Try Other Direction
            vector directionPos;        
            //llOwnerSay("Trying Other Direction: " + (string)evalPosition);       
            if (llFabs(evalPosition.x) >= llFabs(evalPosition.y))
            {
                //move vertical
                //llOwnerSay("Second Change Try Vertical...");
                directionPos.x = 0;
                directionPos.y = evalPosition.y;
            }
            else
            {
                //move horizontal
                //llOwnerSay("Second Change Try Horizontal...");
                directionPos.x = evalPosition.x;
                directionPos.y = 0;
            }
            directionPos.z = evalPosition.z;
            if (llEdgeOfWorld(llGetPos(), directionPos))
            {
                //Request Reroute
                secondTrigger = TRUE;
                llMessageLinked(LINK_SET, 8449550, (string)llVecNorm(directionPos), NULL_KEY);
                return;
            }
            subTravelWPos = llGetRegionCorner();
            subTravelWPos += llVecNorm(directionPos) * 256;
            subTravelLPos = <128,128, travelHeight>;
            subArrivalDistance = 45;
            llSetTimerEvent(moveTau);
        }
    }

    timer()
    {
        integer whileCounter;
        vector pos = llGetPos();
        float tau;
        integer x;
        
        while (((pos.x < 0) || (pos.x > 255) || (pos.y < 0) || (pos.y > 255)) && (whileCounter < 100))
        {
            llSleep(0.1);
            pos = llGetPos();
            whileCounter++;
        }

        if (travelHeight == 0) subTravelLPos.z = llGround(ZERO_VECTOR) + 50;
        if (travelMode == "hop") subTravelLPos.z = llGround(ZERO_VECTOR) + 50;
        if (travelMode == "teleport") subTravelLPos.z = travelHeight;
        vector targetPos = subTravelWPos - llGetRegionCorner() - pos + subTravelLPos;
        float distance = llVecMag(targetPos);
        if (distance > 60) targetPos = llVecNorm(targetPos) * 60;
        targetPos += pos;
        //llApplyImpulse(evalPosition * llGetMass(), TRUE);
        llMoveToTarget(targetPos, moveTau);
        dispUpdate();
        if (distance < subArrivalDistance)
        {
            travelEvaluation();
        }
    }
    state_exit()
    {
        llStopMoveToTarget();
        llTargetRemove(targetHandle);
    }
}

state arrive
{
    state_entry()
    {
        llApplyImpulse(-llGetMass()*llGetVel(),FALSE);
        llMessageLinked(LINK_SET, 612, "", NULL_KEY);
        llOwnerSay("Arrived at " + targetName + ", " + targetRegionName + "(" + (string)((integer)targetRegionLPos.x) + "," + (string)((integer)targetRegionLPos.y)+ "," + (string)((integer)targetRegionLPos.z) + ")");
        float groundHeight = llGround(ZERO_VECTOR);
        //llOwnerSay("Ground height = " + (string)groundHeight);
        llSetForce(<0,0,0> * llGetMass(), FALSE);
        //llOwnerSay("Moving to " + (string)targetRegionLPos);
        llListen(0, "", llGetOwner(), "");
        if (targetRegionLPos.z < groundHeight) targetRegionLPos.z = groundHeight;
                
        //Targets
        targetRegionLPos.z += 60;
        targetHandle = llTarget(targetRegionLPos, 0.1);                
        //llOwnerSay("Landing at " + (string)(targetRegionLPos.z ) + " meters...");
    }
    not_at_target()
    {
        vector tempPos = targetRegionLPos;
        vector pos = llGetPos();
        //llOwnerSay("Not At Arrival Target!" + (string)tempPos);

        if (llFabs(pos.z - tempPos.z) > 60 )
        {
            if (pos.z > tempPos.z)
            {
                tempPos.z = pos.z - 60;
            }
            else
            {
                tempPos.z = pos.z + 60;
            }
        }
        llMoveToTarget(tempPos, 0.1);        
        dispUpdate();
    }
    
    at_target(integer tnum, vector targetpos, vector ourpos)
    {
        llApplyImpulse(-llGetMass()*llGetVel(),FALSE);
        llSleep(0.1);
        llStopMoveToTarget();
        llTargetRemove(targetHandle);
        targetRegionLPos.z -= 60;
        targetRegionLPos.z -= llGround(ZERO_VECTOR) - 0.5;
        //llOwnerSay("SoftLanding at " + (string)targetRegionLPos.z + " meters...");
        llSetHoverHeight(targetRegionLPos.z, FALSE, 0.16);
        llSleep(0.1);
        llSetHoverHeight(0, FALSE, 0.1);
        state idle;
    }
    
    touch_start(integer total_number)
    {
        showOptionsMenu();
    }
    
    listen(integer channel, string name, key id, string message)
    {
        processChat(message);
    }
    
    on_rez(integer star_param)
    {
        state idle;
    }
    
    collision_start(integer num_detected)
    {    
        llSay(0, "Collided with " + llDetectedName(0));
        state idle;
    }
    state_exit()
    {
        llStopMoveToTarget();
        llTargetRemove(targetHandle);
        llOwnerSay("Transport Complete!");
        llSleep(0.25);
        llApplyImpulse(-llGetMass()*llGetVel(),FALSE);
        llSleep(1.0);
        llStopAnimation("falldown");
    }
}