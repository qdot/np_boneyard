float g_fadeLevel = 0.0;

default
{
    state_entry()
    {
        llTargetOmega(<0,0,1>, 2, 1);
        llSetPrimitiveParams([PRIM_SIZE, <1.0, 1.0, 1.0>]);
    }

    touch(integer total_number) 
    {
        llSetTimerEvent(2.0);
        llSetPrimitiveParams([PRIM_SIZE, <1.0, 1.0, 0.01>]);
    }    
    timer()
    {
        llSetPrimitiveParams([PRIM_SIZE, <1.0, 1.0, 1.0>]);
    }
}
