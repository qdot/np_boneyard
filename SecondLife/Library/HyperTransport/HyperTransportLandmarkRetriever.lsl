//Landmark Retriever
//by Hank Ramos

//Variables
key landmarkReadKey;
string targetName;

default
{
    on_rez(integer startup_param)
    {
        llPlaySound("d070a839-63b7-3e8e-ccb1-73c8c13d5349", 1.0);
    }
    changed(integer change)
    {
        if (change & CHANGED_INVENTORY)
        {
            if (llGetInventoryNumber(INVENTORY_LANDMARK) > 0)
            {             
                targetName = llGetInventoryName(INVENTORY_LANDMARK, 0);
                if (llGetInventoryKey(targetName) != NULL_KEY) landmarkReadKey = llRequestInventoryData(targetName);
            }
        }
    }
    dataserver(key requested, string data)
    {
        vector targetRegionWPos;
        vector targetRegionLPos;
        
        if (requested == landmarkReadKey)
        {
            vector targetPos = (vector)data;
            
            //llRemoveInventory(targetName);
            //llSleep(0.1);
            targetPos += llGetRegionCorner();
            targetRegionWPos = targetPos;
            targetRegionWPos.x = ((integer)((targetRegionWPos.x) / 256)) * 256;
            targetRegionWPos.y = ((integer)((targetRegionWPos.y) / 256)) * 256;
            targetRegionLPos.z = targetRegionWPos.z;
            targetRegionWPos.z = 0;
            targetRegionLPos.x = targetPos.x - targetRegionWPos.x;
            targetRegionLPos.y = targetPos.y - targetRegionWPos.y;
            llWhisper(llGetStartParameter(), "landmark " + (string)targetRegionWPos + "," + (string)targetRegionLPos.x + "," + (string)targetRegionLPos.y + "," + (string)targetRegionLPos.z);
            llSleep(1);
            llPlaySound("c0cb87d9-7463-8656-efae-32b705f25cde", 1.0);
            llDie();
        }
    }
}