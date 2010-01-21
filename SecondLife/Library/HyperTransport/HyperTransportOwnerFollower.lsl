default
{
    state_entry()
    {
        llSetStatus(STATUS_ROTATE_X | STATUS_ROTATE_Y, FALSE);
        llSetStatus(STATUS_PHYSICS, TRUE);
        llSensorRepeat("", llGetOwner(), AGENT, 96, TWO_PI, 0.1);
    }

    sensor(integer total_number)
    {
        llMoveToTarget(llDetectedPos(0), 0.1);        
    }
}