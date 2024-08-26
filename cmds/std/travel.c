/**
 * @file /cmds/std/travel.c
 * @description Command to run to common destinations from your current
 *              location. Requires GMCP.
 *
 * @created 2024-08-24 - Gesslar
 * @last_modified 2024-08-24 - Gesslar
 *
 * @history
 * 2024-08-24 - Gesslar - Created
 */

#include <gmcp_defines.h>

inherit STD_CMD ;

private nosave mapping destinations ;
private nosave string *dests ;

void setup() {
    destinations = mud_config("TRAVEL_DESTINATIONS") ;
    destinations = map(destinations, (: chop($2, ".c", -1) :)) ;
    destinations = map(destinations, (: prepend($2, "/") :)) ;

    dests = keys(destinations) ;
    dests = sort_array(dests, 1) ;

    usage_text =
"travel list - List all valid travel destinations.\n"
"travel <destination> - Travel to a common destination.";
    help_text =
"This command allows you to travel to common destinations from your current "
"location.\n\n"
"Valid destinations are: " + implode(dests, ", ") + ".\n\n"
"Please note that you will always pass through the village square when "
"traveling to any destination, regardless of the proximity of your current "
"location to the destination.\n\n"
"You must have GMCP enabled to use this command." ;
}

mixed main(object tp, string str) {
    string *stops ;

    if(!get_config(__RC_ENABLE_GMCP__))
        return 0 ;

    if(!tp->gmcp_enabled())
        return "You must have GMCP enabled to use this command." ;

    if(!str)
        return "Travel to where?" ;

    if(str == "list")
        return "Valid destinations are: " + implode(dests, ", ") + "." ;

    if(!of(str, destinations))
        return "That is not a valid destination." ;

    if(base_name(environment(tp)) == destinations[str])
        return "You are already at that destination." ;

    tell(tp, "Traveling to " + str + "...\n") ;

    stops = ({ destinations[str] }) ;

    if(str != "square")
        stops = ({ destinations["square"], destinations[str] }) ;

    GMCP_D->send_gmcp(tp, GMCP_PKG_ROOM_TRAVEL, stops) ;

    return 1 ;
}
