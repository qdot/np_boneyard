integer MOVE_UP = 2;
integer MOVE_DOWN = 3;
integer MOVE_LEFT = 4;
integer MOVE_RIGHT = 5;
integer MOVE_STOP = 6;
integer MOVE_OFF = 7;
string g_emailKey = "d8bc8638-7566-399a-003c-e61a0e1fd27c";
integer g_listenerHandle = 0;
integer g_chatChannel = 0;
integer g_timerCounter = 0;
integer g_moveState = 0;
vector g_satelliteRotation = <180, 0, 0>;

default
{
    state_entry()
    {
        vector eul = <165, 42.95, 201.50>; //45 degrees around the z-axis, in Euler form
        eul *= DEG_TO_RAD; //convert to radians
        rotation quat = llEuler2Rot(eul); //convert to quaternion
        llSetRot(quat); //rotate the object
        g_chatChannel = 0;
        llTargetOmega(<0, 0, 1>, .2, 1);
        llSetTimerEvent(3.0);
    }


    timer()
    {
        llGetNextEmail("", ""); 
    }
    
    email(string time, string address, string subj, string message, integer num_left)
    {
        if(address == g_emailKey+"@lsl.secondlife.com")
        {
            list subjectInfo = llParseString2List(subj, [","], []);
            g_chatChannel = llList2Integer(subjectInfo,1);
            llSetTimerEvent(0.0);
            state inMovement;
        }
        else
        {
            llSetTimerEvent(3.0);
        }
        
        if(num_left > 0)
        {
            llGetNextEmail("","");
        }
    }
}

state inMovement
{
    state_entry()
    {
        llTargetOmega(<0,0,0>,0,0);
        vector eul = <180, 0, 0>; //45 degrees around the z-axis, in Euler form
        eul *= DEG_TO_RAD; //convert to radians
        rotation quat = llEuler2Rot(eul); //convert to quaternion
        llSetRot(quat); //rotate the object
        llSetTimerEvent(0.25);
        g_listenerHandle = llListen(g_chatChannel, "", "", "");
		g_timerCounter = 0;
		g_moveState = 0;
		g_satelliteRotation = <180,0,0>;
    }
    
    listen(integer channel, string name, key id, string message)
    {
		g_moveState = (integer)message;
    }
    
    timer()
    {
		++g_timerCounter;
		if(g_timerCounter >= 240)
		{
			llSetTimerEvent(0.0);
			llListenRemove(g_listenerHandle);
			state default;
			return;
		}
		if(g_moveState == MOVE_STOP) return;
		else if(g_moveState == MOVE_UP)
		{
			g_satelliteRotation.x += 5.0;
			//if(g_satelliteRotation.x < -0.0001) g_satelliteRotation.x += 5.0;
			//else if (g_satelliteRotation.x > 0.0001) g_satelliteRotation.x -= 5.0;
		}
		else if(g_moveState == MOVE_DOWN)
		{
			g_satelliteRotation.x -= 5.0;
			//if(g_satelliteRotation.x < -0.0001) g_satelliteRotation.x -= 5.0;
			//else if (g_satelliteRotation.x > 0.0001) g_satelliteRotation.x += 5.0;
		}
		else if(g_moveState == MOVE_RIGHT)
		{
			g_satelliteRotation.y += 5.0;
			//if(g_satelliteRotation.y < -0.0001)
			//else if (g_satelliteRotation.y > 0.0001) g_satelliteRotation.z += 5.0;
		}
		else if(g_moveState == MOVE_LEFT)
		{
			g_satelliteRotation.y -= 5.0;
			//if(g_satelliteRotation.y < -0.0001) g_satelliteRotation.y -= 5.0;
			//else if (g_satelliteRotation.y > 0.0001) g_satelliteRotation.y += 5.0;
		}
		else if(g_moveState == MOVE_OFF)
		{
			g_timerCounter = 240;
			return;
		}
		vector eul = g_satelliteRotation; //45 degrees around the z-axis, in Euler form
        eul *= DEG_TO_RAD; //convert to radians
        rotation quat = llEuler2Rot(eul); //convert to quaternion
        llSetRot(quat); //rotate the object
		
    }
    
    on_rez(integer start_param)
    {
		llSetTimerEvent(0.0);
        llListenRemove(g_listenerHandle);
        state default;
    }
}