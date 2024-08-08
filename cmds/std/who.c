//who.c

//Tacitus @ LPUniversity
//08-APR-05
//Standard Command

//I don't like it, please recode this for me :) [Tacitus]

//Last edited October 4th, 2006 by Tacitus

inherit STD_CMD ;

object *add_array(object *oldlist, object *newlist);

mixed main(object caller, string arg) {
    string ret, mudname;
    mixed *packet;
    object *list, c;
    object *admin_arr,*dev_arr, *user_arr;
    int i;


    admin_arr = ({});
    dev_arr   = ({});
    user_arr  = ({});
    ret = "";

    /* Fixed your error. Tricky */

    ret += read_file("/adm/etc/logo") + "\n\n";

    ret += sprintf("%10s  %10s\n\n", "Username [* editing, + in input]", "Idle");

    list = users();

    foreach(object name in list) {
        if(adminp(name) && name->query_proper_name() != "login")
            admin_arr += ({ name });
        else if(devp(name))
            dev_arr += ({ name });
        else
            user_arr += ({ name });
    }

    admin_arr = sort_array(admin_arr, "sort_name");
    dev_arr   = sort_array(dev_arr,   "sort_name");
    user_arr = sort_array(user_arr, "sort_name");

    list = ({});
    list = add_array(list, admin_arr);
    list = add_array(list, dev_arr);
    list = add_array(list, user_arr);

    for(i = 0; i < sizeof(list); i ++) {
        string tag;

        if(!(string)list[i]->query_name())
            continue;

        if(list[i]->query_proper_name() == "login")
            tag = "[ LOGIN ]";
        else if(adminp(list[i]))
            tag = "[ Admin ]";
        else if(devp(list[i]))
            tag = "[ Dev   ]";
        else
            tag = "[ User  ]";

        ret += sprintf(" %-s   %-15s %15s\n", tag,
          list[i]->query_name() +
          (list[i]->query_env("away") ? " (afk)" : "") +
          (in_edit(list[i]) ? "*" : "") +
          (in_input(list[i]) && !list[i]->query_env("away") ? "+" : ""),
          query_idle(list[i]) / 60 + "m");
    }

    ret +=("\n");
    write(ret);
    return 1;
}

int sort_name(object ob1, object ob2) {
    if(ob1->query_name() > ob2->query_name())    return 1;
    else if(ob1->query_name() < ob2 ->query_name())    return -1;
    else return 0;
}

object *add_array(object *oldarr, object *newarr) {
    foreach(object name in newarr) {
        oldarr += ({ name });
    }

    return oldarr;
}

string help(object caller) {
    return(" SYNTAX: who\n\n" +
    "This command will display all the users who are currently logged\n" +
    "into " + mud_name() + ". It also lets you know if they are currently\n" +
    "editing, in input, and/or idle.\n") ;
}
