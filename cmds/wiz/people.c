/* people.c

 Abbot @ LPUniversity
 14-JULY-2006
 Developer People Command

*/

/*  Last editted by Tacitus on October 4 */

inherit STD_CMD ;

mixed main(object tp, object room, string arg) {
    object *OAdminArr, *ODevArr,*OUserArr;
    object *OUser;

    OUser = ({});
    OAdminArr = ({});
    ODevArr = ({});
    OUserArr = ({});

    printf("%-15s%-10s%-20s%-5s %-30s\n", "Name:", "Rank:", "IP:", "Idle:", "Location:");
    printf("%-15s%-10s%-20s%-5s %-30s\n", "-----", "-----", "---", "-----", "---------");

    OUser = filter(users(), (: environment($1) && interactive($1) :)) ;

    foreach(object user in OUser) {
        if(adminp(user) && user->name() != "login")
            OAdminArr += ({ user });
        else if(devp(user))
            ODevArr += ({ user });
        else
            OUserArr += ({ user });
    }

    OAdminArr = sort_array(OAdminArr, (: strcmp($1->name(), $2->name()) :) );
    ODevArr = sort_array(ODevArr, (: strcmp($1->name(), $2->name()) :) );
    OUserArr = sort_array(OUserArr, (: strcmp($1->name(), $2->name()) :) );

    OUser = OAdminArr + ODevArr + OUserArr;

    foreach(object user in OUser) {
        string thisRank, thisIP, thisEnv;
        int thisIdle ;
        string name ;

        thisEnv = file_name(environment(user));

        if(adminp(user)) thisRank = "Admin";
        else if(devp(user)) thisRank = "Dev";
        else thisRank = "User";

        thisIdle = query_idle(user)/60;
        thisIP = query_ip_number(user);

        name = capitalize(user->name());
        printf("%-15s%-10s%-20s%5d %-30s\n", name, thisRank, thisIP, thisIdle, thisEnv);
    }

    return 1;
}

string help(object caller) {
    return(" SYNTAX: people\n\n"
      "This command allows you to see all users logged in. They are\n"
      "displayed with their name, rank, ip, location, and minutes of idle\n"
      "time.\n");
}
