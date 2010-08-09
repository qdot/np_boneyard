key requestid;
string listenbuffer;


default
{
    state_entry()
    {
        llSetTimerEvent(1.5);
        llListen(0, "", "", "");
    }

    listen( integer channel, string name, key id, string message )
    {
        listenbuffer += name + ": " + message + "\n";
    }

    timer()
    {
        requestid = llHTTPRequest("http://secondlife.nonpolynomial.com/tfbridge.php", 
            [HTTP_METHOD, "POST",
             HTTP_MIMETYPE, "application/x-www-form-urlencoded"],
            "chatlog="+listenbuffer);
        listenbuffer="";
    }

    http_response(key request_id, integer status, list metadata, string body)
    {
        if (request_id == requestid)
        {
            //Check to make sure this is a properly assembled log
            if(llGetSubString(body, 0, 6) != "12345\n")
            {
                llOwnerSay("Wrong header type for MUCK Comm Log, ignoring");
            }
            body = llDeleteSubString(body, 0, 6);
            body = llDeleteSubString(body, llStringLength(body) - 1,llStringLength(body));
            if(llStringLength(body) > 0) llWhisper(0, "\n"+body);
        }
    }
}
