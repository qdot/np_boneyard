string g_emailKey = "56966d97-b0f1-5eeb-851b-5f279cda1f34";
integer g_payAmount = 10;


integer randBetween(integer min, integer max)
{
    return (integer)(llFrand(max - min) + min);
}
default
{
    state_entry()
    {
	}
	
	money(key id, integer amount)
	{
		if(amount == g_payAmount)
		{
			llSay(0, "Thank you! Handing over the controls now...");
			state inMovement;
		}
		else if (amount < g_payAmount)
        {
            // if so, tell them they're getting a refund, then refund their money.
            llSay(0,"You didn't pay enough, " + llKey2Name(id) + ". Refunding your payment of L$" + (string)amount + ".");
            llGiveMoney(id, amount); // refund amount paid.
        }
        // if it's not exactly the amount required, and it's not less than the amount required,
        // the payer has paid too much.
        else
        {
            // tell them they've overpaid.
            llSay(0,"You paid too much, " + llKey2Name(id) + ". Your change is L$" + (string)amount + ".");
            integer refund = amount - g_payAmount; // determine how much extra they've paid.
            llGiveMoney(id, refund); // refund their change.
			llSay(0, "Thank you! Handing over the controls now...");
			state inMovement;
        }
	}
	
    touch(integer total_number)
    {
        llSay(0, "Control the NP Labs XML-RPCtellite, and kill as many invaders as possible! Only $10L!");
		llSay(0, "(Please note, game not fully working yet, but you can still control the dish!)");		
    }
}

state inMovement
{
	state_entry()
	{
		integer chatChannel = randBetween(100000, 1000000000);
		llSetTimerEvent(62.0);
		vector pos = llGetPos();
		rotation rot = llAxisAngle2Rot(<1,0,0>, PI);
		llRezObject("Controller", <pos.x, pos.y, pos.z+.912>, <0,0,0>, rot, chatChannel);
		llEmail(g_emailKey+"@lsl.secondlife.com", "Start Satellite Comms,"+(string)chatChannel, "");
	}
	
	touch(integer total_number)
	{
	}
	
	timer()
	{
		state default;
	}
}