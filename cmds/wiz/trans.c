/*

 trans.c

 Parthenon@LPuniversity
 August 7th, 2006
 command to transport a user to your current location

 Last edited on August 7th, 2006 by Parthenon

*/

inherit STD_CMD ;

mixed main(object caller, string arg) {
    object player;

    if(!arg || arg == "")
        return notify_fail("Syntax: trans <user>\n");

    if(!player = find_player(arg))
        return notify_fail("Error [trans]: Cannot find that player\n");

    if(environment(player) == environment(this_body()))
        return notify_fail("Error [trans]: You are already in the same room.\n");

    if(adminp(player) || devp(player) && !adminp(this_body())) {
        catch(tell_object(player, this_body()->query_name()
            + " requests your presence in " +
            environment(this_body())->query_short() + ".\n"));

        return notify_fail("Error [trans]: You cannot transport them but your wish has been made known.\n");
    }

    if(!environment(this_body()))
        return notify_fail("Error [trans]: You have no environment to transport a player to!\n");

    if(!player->move(environment(this_body())))
        return notify_fail("Error [trans]: Could not move " + player->query_name() + " to you.\n");

    tell_object(player, "You have been transported to " + this_body()->query_name() + ".\n");
    tell_object(this_body(), "You have transport " + player->query_name() + " to you.\n");

    return 1;
}

string help(object caller) {
    return
    " SYNTAX: trans <user>\n\n" +
    "This command allows you to move a user to your environment. You cannot\n" +
    "trans admins, and if you are a developer you cannot trans other developers\n";
}
