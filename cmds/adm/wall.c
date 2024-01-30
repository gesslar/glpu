//wall.c

//Tacitus @ LPUniversity
//01-JULY-05
//Admin wall cmd

int main(object caller, object room, string str)
{
    int i;
    object *users;

    if(!adminp(previous_object())) return notify_fail("Error [wall]: Access denied.\n");

    if(!stringp(str))
    {
        write("Error [wall]: You must supply an argument. Syntax: wall <msg>\n");
        return 1;
    }

    users = users();

    for(i = 0; i < sizeof(users); i++)
    {
        tell_object(users[i], "**** System Wide Message From: "
        + capitalize(caller->query_name()) + " at " + ctime(time())
        + " ****\n\n\t" + str + "\n");
    }

    return 1;
}

string help(object caller) {
    return (HIW + " SYNTAX:" + NOR + " wall <msg>\n\n"
      + "This command pages a message to everybody on the MUD.\n");
}

