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
        if(adminp(user) && user->query_proper_name() != "login")
            OAdminArr += ({ user });
        else if(devp(user))
            ODevArr += ({ user });
        else
            OUserArr += ({ user });
    }

    OAdminArr = sort_array(OAdminArr, (: strcmp($1->query_proper_name(), $2->query_proper_name()) :) );
    ODevArr = sort_array(ODevArr, (: strcmp($1->query_proper_name(), $2->query_proper_name()) :) );
    OUserArr = sort_array(OUserArr, (: strcmp($1->query_proper_name(), $2->query_proper_name()) :) );

    OUser = OAdminArr + ODevArr + OUserArr;

    foreach(object user in OUser) {
        string this_rank, this_ip, this_env;
        int this_idle ;
        string name ;

        this_env = file_name(environment(user));

        if(adminp(user)) this_rank = "Admin";
        else if(devp(user)) this_rank = "Dev";
        else this_rank = "User";

        this_idle = query_idle(user)/60;
        this_ip = query_ip_number(user);

        name = capitalize(user->query_proper_name());
        printf("%-15s%-10s%-20s%5d %-30s\n", name, this_rank, this_ip, this_idle, this_env);
    }

    return 1;
}

string help(object caller) {
    return(" SYNTAX: people\n\n"
      "This command allows you to see all users logged in. They are\n"
      "displayed with their name, rank, ip, location, and minutes of idle\n"
      "time.\n");
}
