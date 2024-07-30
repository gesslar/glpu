
// File    :  /cmds/xtra/_trace.c
// Creator    :  Watcher@TMI  (03/04/93)
//
// This command allows you to locate any object, as well as
// any clones of it active in the mud.  Descriptors and
// systematic destruction are options available.

inherit STD_CMD ;

#define SYNTAX "Syntax: trace -[d/v] [object/filename]\n"
#define PROTECT ({ })

mixed main(object tp, string str) {
    object target;
    mixed *clones;
    string para, hold, tmp1, tmp2;
    int loop, dest, view, original;

    if(!str || str == "")
        return SYNTAX ;
    //    Check for requested command parameters

    if(sscanf(str, "-%s %s", para, str) == 2) {
        if(sscanf(" " + para + " ", "%sd%s", tmp1, tmp2) == 2) dest = 1;
        if(sscanf(" " + para + " ", "%sv%s", tmp1, tmp2) == 2) view = 1;
    }

    // Try to locate the requested object
    target = get_object( str );

    // Oh well, couldn't find the object
    if(!target)
        return "Trace: Could not locate requested object.\n" ;

   hold = "Trace: " + target ;

    if(environment(target))
        hold += " in " + environment(target) + "\n";
    else
        hold += "\n";

    // Try to locate all copies of the object
    clones = children( file_name(target) );

    // Its either the Master object or has no other copies around
    if(sizeof(clones) == 1) {
        if(base_name(clones[0]) == file_name(clones[0]))
            hold += "There are no active copies of this object.\n";

        if(dest) {
            if((interactive(target) || userp(target) || member_array(base_name(target), PROTECT) != -1) && adminp(tp))
                return "You do not have authorization to destruct that object.\n" ;

            target->remove();
            if(target)  destruct(target);

            if(target)  hold += "Could not remove or destroy object.\n" ;
            else        hold += "Object has been removed and destructed.\n";
        }

        return explode(hold, "\n");
    }

    // Remove the target object from the clone array
    else clones -= ({ target });

    // Get original number of clones of designated object
    original = sizeof( clones );

    if(!(dest && !view)) {
        hold += "\n   There are " + sizeof(clones) + " copies active.\n\n";

        // Loop through the clone array to display contents
        for(loop=0; loop<sizeof(clones); loop++) {
            if(interactive(clones[loop]))
                hold += " I ";
            else
                hold += "   ";

            hold += file_name(clones[loop]) ;

            //    If the object has an environment, display it...
            if(environment( clones[loop] ))
                hold += "\tin " + sprintf("%O\n", environment(clones[loop]));
            else
                hold += "\n";
        }
    }

   //    If the destruct flag is set, attempt to destruct
   //    all active copies of the requested object.
   if(dest) {
        // Security check...don't want anyone just wiping out
        // specific file objects like /std/user.c

        if(!is_member(query_privs(tp), "admin") && member_array(base_name(target), PROTECT) != -1)
            return "You do not have authorization to destruct that object group.\n" ;

        //    Try to remove all clone copies
        clones->remove();
        clones = children( file_name(target) );
        clones -= ({ target });

        // If there are any copies left...this should get rid of them

        for(loop=0; loop<sizeof(clones); loop++)
            destruct( clones[loop] );

        clones = children( file_name(target) );
        clones -= ({ target });

        hold += "All " + original + " copies of " + file_name(target) + " have been removed and destroyed";

        if(!sizeof(clones)) {
            hold += ".\n";
            return explode(hold, "\n");
        }

        hold += " except:\n";

        for(loop=0; loop<sizeof(clones); loop++)
            hold += "  " + clones[loop] + "\n";
    }

    return explode(hold, "\n");
}

string help(object tp) {
   return SYNTAX + "\n" +
"This command allows the user to locate the requested object and any active "
"clones with their respective locations. The parameter 'd' can be used to "
"remove and destruct every copy. When the 'd' parameter is envoked, the clones "
"and locations will not be displayed. This can be overridden with the 'v' "
"parameter. The 'm' parameter can be used to have the output given in a more "
"format.\n\n" +
"The parameters can be given together and in any combination.\n\n"
"For example: trace -dm /obj/dagger  will destruct every copy of /obj/dagger "
"and display their respective ids and locations in a more format.\n" ;
}
