/* std_newsclient.c

 Tacitus @ LPUniversity
 27-JAN-06
 Mudlib Object

*/

// Last edited by Tacitus on October 4th, 2006

inherit STD_OBJECT;

/* Global Variables*/

string *groups;
mapping *messages;
mapping viewed;

int current_group;
int edit_post;
int did_write;
int current_post;
int current_index;
string current_file = "";
string current_subject;

/* Function Prototypes */

int select_group(string arg);
int group_input(string args);
int create_msg(string subject);
int callback_write(string fname, int flag);
void callback_exit();
void callback_edit();
int edit_msg(int num);
int display_msg();
void done_page();
int is_all_read(int group);
int is_new();

/* Functions */

void create()
{
    viewed = ([]);
    if(file_exists(user_data_directory(query_privs(this_player())) + "std_newsclient.o"))
        viewed = restore_variable(read_file(user_data_directory(query_privs(this_player()))
            + "std_newsclient.o"));

}

int remove() {
    write_file(user_data_directory(query_privs(this_player())) + "std_newsclient.o",
        save_variable(viewed), 1);
    return 1 ;
}

int is_new()
{
    int i;
    groups = NEWS_D->get_group_listings();

    for(i = 0; i < sizeof(groups); i++)
    {
        if(!viewed[groups[i]]) viewed += ([groups[i] : ([]) ]);
        if(!is_all_read(i)) return 1;
    }

    return 0;
}

int start_client()
{
    int i;

    groups = NEWS_D->get_group_listings();

    for(i = 0; i < sizeof(groups); i++)
    {
        if((i % 3)==0) printf("\n");
        if(!viewed[groups[i]]) viewed += ([groups[i] : ([]) ]);
        printf("%-20s", (i + 1) + ". " + capitalize(groups[i]) + (is_all_read(i) ? "" : "*"));
    }

    write("\n News Prompt> ");

    input_to("select_group");
    return 1;
}

int is_all_read(int group)
{
    int i;

    messages = NEWS_D->get_posts(groups[group]);

    for(i = 0; i < sizeof(messages); i++)
    {
        if(viewed[groups[group]][i]) continue;
        else return 0;
    }

    return 1;
}

int select_group(string arg)
{
    int i, b;
    string cmd, argument;

    current_group = to_int(arg) - 1;

    if(current_group < 0 )
    {
        if(sscanf(arg, "%s %s", cmd, argument) != 2) cmd = arg;

        switch(cmd)
        {
            case "help" :
            case "h" :
                {
                    write("\nPlease select a group by inputting the number listed\n"
                    "beside the group name. After entering a group, type 'help'\n"
                    "or 'h' for more information.\n\n\t [Hit an key to continue]\n");
                    input_to("start_client");
                    return 1;
                }
        }
    }

    if((cmd && to_int(cmd) && (current_group > sizeof(groups) - 1 || current_group < 0)))
    {
        write("There is no group with that number.\n");
        write("\n News Prompt> ");
        input_to("select_group");
        return 1;
    }

    else if(!stringp(cmd) && (current_group > sizeof(groups) - 1 || current_group < 0))
    {
        write("There is no group with that number.\n");
        write("\n News prompt> ");
        input_to("select_group");
        return 1;
    }

    else if(stringp(cmd))
    {
        write("\nExiting News Client...\n");
        remove() ;
        return 1;
    }

    if(!NEWS_D->has_permission(groups[current_group], "r"))
    {
        write("\nYou do not have read permissions for that group.\n");
        write("\n News prompt> ");
        input_to("select_group");
        return 1;
    }

    messages = NEWS_D->get_posts(groups[current_group]);

    if(nullp(current_index) || current_index > sizeof(messages) - 18)
            current_index = sizeof(messages) - 18;
    if(current_index < 0) current_index = 0;

    write("\n");

    for(b = 0, i = current_index ; i < sizeof(messages); i++, b++)
    {
        if(b >= 20) break;
        if(!viewed[groups[current_group]][i]) viewed[groups[current_group]] += ([i : 0]);

        printf("%s %-35s : %s on %s  %s\n", (i + 1) + ".", messages[i]["subject"], capitalize(""
            + messages[i]["author"]), ctime(messages[i]["date"]),
                (viewed[groups[current_group]][i] ? "" : "<") );
    }

    write("\n" + capitalize(groups[current_group]) + " > ");
    input_to("group_input");
    return 1;
}

int group_input(string args)
{
    string cmd, arg;
    int numerical_arg, i, b;
    mapping posts = ([]);

    if(!args || args == "")
    {
        messages = NEWS_D->get_posts(groups[current_group]);

        if(nullp(current_index) || current_index > sizeof(messages) - 18)
            current_index = sizeof(messages) - 18;
        if(current_index < 0) current_index = 0;

        write("\n");

        for(b = 0, i = current_index ; i < sizeof(messages); i++, b++)
        {
            if(b >= 20) continue;

            if(!viewed[groups[current_group]][i]) viewed[groups[current_group]] += ([i : 0]);

            printf("%s %-35s : %s on %s  %s\n", (i + 1) + ".", messages[i]["subject"],
                capitalize("" + messages[i]["author"]), ctime(messages[i]["date"]),
                    (viewed[groups[current_group]][i] ? "" : "<") );
        }

        write("\n" + capitalize(groups[current_group]) + " > ");
        input_to("group_input");
        return 1;
    }

    if(sscanf(args, "%s %s", cmd, arg) != 2) cmd = args;

    numerical_arg = to_int(arg);

    switch(cmd)
    {
        case "h" :
        case "help" :
            {
                write(" --= Standard LPUniversity News Client =--\n\n"
                    " Command:                  Description:\n"
                    " h/help                 : Will display this help.\n"
                    " quit                   : Will exit the client.\n"
                    " l/list                 : Relist posts in the current group.\n"
                    " return/main/home/exit  : Will return to main menu.\n"
                    " i/index  #             : Allows you to set the index.\n"
                    " post/p [<subject>]     : Allows you to make a post.\n"
                    " r/reply [<post#>]      : Reply to a post.\n"
                    " e/edit [<post#>]       : Edit a post.\n"
                    " ra/read_all            : Mark all posts read in group.\n"
                    " #                      : View/Read post.\n\n");
                write("\n" + capitalize(groups[current_group]) + " > ");
                input_to("group_input");
                return 1;
            }

        case "e" :
        case "edit" :
            {
            posts = NEWS_D->get_posts(groups[current_group]);

            if(lower_case(posts[numerical_arg - 1]["author"]) != this_player()->query_proper_name()
                && !adminp(this_player()))
            {
                write("\nYou cannot edit a post that you did not write.\n");
                write("\n" + capitalize(groups[current_group]) + " > ");
                input_to("group_input");
                return 1;
            }

            if(!NEWS_D->has_permission(groups[current_group], "e"))
            {
                write("\nYou do not have edit permissions for this group.\n");
                write("\n" + capitalize(groups[current_group]) + " > ");
                input_to("group_input");
                return 1;
            }

            if(!numerical_arg)
            {
                if(current_post >= 0)
                edit_msg(current_post);
            }

            else    edit_msg(numerical_arg-1);

            return 1;
            break;
            }

        case "l" :
        case "list" :
            {
                messages = NEWS_D->get_posts(groups[current_group]);

                if(nullp(current_index) || current_index > sizeof(messages) - 18)
                    current_index = sizeof(messages) - 18;
                if(current_index < 0) current_index = 0;

                write("\n");

                for(b = 0, i = current_index ; i < sizeof(messages); i++, b++)
                {
                    if(b >= 20) continue;
                    if(!viewed[groups[current_group]][i])
                        viewed[groups[current_group]] += ([i : 0]);
                    printf("%s %-35s : %s on %s  %s\n", (i + 1) + ".", messages[i]["subject"],
                        capitalize("" + messages[i]["author"]), ctime(messages[i]["date"]),
                            (viewed[groups[current_group]][i] ? "" : "<") );
                }

                write("\n" + capitalize(groups[current_group]) + " > ");
                input_to("group_input");
                return 1;
                break;
            }
        case "ra" :
        case "read_all" :
            {
                for(i = 0; i < sizeof(messages); i++)
                {
                    viewed[groups[current_group]] += ([i : 1]);
                }

                write("\n\nSuccess [std_newsclient]: All posts marked read in group " + groups[current_group] + ".\n");
                write("\n" + capitalize(groups[current_group]) + " > ");
                input_to("group_input");
                return 1;
            }

        case "q" :
        case "quit" : write("\nExiting News Client...\n"); remove() ; return 1;

        case "return" :
        case "main" :
        case "home" :
        case "exit" : start_client(); return 1;
        case "i" :
        case "index" :
            {
                if(numerical_arg) current_index = numerical_arg - 1;
                break;
            }

        case "post" :
        case "p" :
            {
                if(!NEWS_D->has_permission(groups[current_group], "p"))
                {
                    write("\nYou do not have posting permissions for this group.\n");
                    write("\n" + capitalize(groups[current_group]) + " > ");
                    input_to("group_input");
                    return 1;
                }

                if(!arg)
                {
                    write("Subject: ");
                    input_to("create_msg");
                }

                else create_msg(arg);

                return 1;
            }

        case "r" :
        case "reply" :
            {
                if(!NEWS_D->has_permission(groups[current_group], "p"))
                {
                    write("\nYou do not have posting permissions in this group.\n");
                    write("\n" + capitalize(groups[current_group]) + " > ");
                    input_to("group_input");
                    return 1;
                }

                if(!numerical_arg)
                {
                    if(current_post >= 0)
                    {
                        create_msg("RE: " + messages[current_post]["subject"]);
                        return 1;
                    }

                    select_group("" + (current_group + 1));
                    return 1;
                }

                if((numerical_arg-1) < sizeof(messages))
                {
                    create_msg("RE: " + messages[numerical_arg - 1]["subject"]);
                    return 1;
                }
            }
    }

    if(args) current_post = to_int(args) - 1;

    if(current_post < sizeof(messages) && current_post >= 0)
    {
        display_msg();
        current_index = current_post;
        return 1;
    }

    else
    {
        write("\nThere is no post with that number\n");
        write("\n" + capitalize(groups[current_group]) + " > ");
        input_to("group_input");
        return 1;
    }

    return 1;
}

int display_msg()
{
    string output = "";
    mapping msg = messages[current_post];
    object pager = new("/obj/mudlib/pager/pager.c");

    if(!mapp(msg))
    {
        write("Error [news]: Invalid msg id '" + current_post + "'.\n");
        select_group("" + (current_group + 1));
        return 1;
    }

    viewed[groups[current_group]][msg["id"] - 1] = 1;

    output += ("\nGroup: " + msg["group"] + " (id: " + msg["id"] + ")\n");
    output += ("Author: " + msg["author"] + "\n");
    output += ("Date: " +  ctime(msg["date"]) + "\n");
    output += ("Subject: " + msg["subject"] + "\n");
    output += ("=--------------------------------------=\n\n");
    output += msg["content"] + "\n";
    pager->page(output, 0, (: done_page :));
}

void done_page()
{
    write("\n" + capitalize(groups[current_group]) + " > ");
    input_to("group_input");
}

int edit_msg(int num)
{
    mapping posts = ([]);

    edit_post = num;
    posts = NEWS_D->get_posts(groups[current_group]);

    write("Editing post: " + posts[num]["subject"] + "\n");
    write("=--------------------------------------=\n\n");

    current_file = "/tmp/" + random(9999999) + "." + this_player()->query_proper_name();
    while(file_exists(current_file)) current_file = "/tmp/" + random(9999999) + "."
        + this_player()->query_proper_name();

    write_file(current_file, implode(explode(posts[num]["content"], "\n"), "\n"), 1);

    if(!devp(this_player())) ed(current_file, "callback_write", "callback_edit", 1);
    else ed(current_file, "callback_write", "callback_edit", 0);

    return 1;
}

int create_msg(string subject)
{
    if(!subject) subject = "(No Subject)";
    current_subject = subject;
    write("\nGroup: " + groups[current_group] + "\n");
    write("Author: " + capitalize(this_player()->query_proper_name()) + "\n");
    write("Date: " + ctime(time()) + "\n");
    write("Subject: " + subject + "\n");
    write("=--------------------------------------=\n\n");

    current_file = "/tmp/" + random(9999999) + "." + this_player()->query_proper_name();
    while(file_exists(current_file)) current_file = "/tmp/" + random(9999999) + "."
        + this_player()->query_proper_name();

    write_file(current_file, "");
    if(!devp(this_player())) ed(current_file, "callback_write", "callback_exit", 1);
    else ed(current_file, "callback_write", "callback_exit", 0);

    return 1;
}

int callback_write(string fname, int flag)
{
    if(!master()->valid_read(fname, this_object(), "callback_exit")) return 0;
    current_file = fname;
    did_write = 1;
    return 1;
}

void callback_edit()
{
    string contents = read_file(current_file);

    if(!contents || !did_write)
    {
        rm(current_file);
        select_group("" + (current_group + 1));
        return;
    }

    NEWS_D->client_action_edit(groups[current_group], contents, edit_post);

    rm(current_file);
    current_file = "";
    edit_post = 0;

    write("\n" + capitalize(groups[current_group]) + " > ");
    input_to("group_input");
}

void callback_exit()
{
    string contents = read_file(current_file);

    if(!contents || !did_write)
    {
        rm(current_file);
        select_group("" + (current_group + 1));
        return;
    }

    NEWS_D->client_action_post(groups[current_group], capitalize(this_player()->query_proper_name()),
        current_subject, contents);

    rm(current_file);
    current_file = "";
    current_subject = 0;

    write("\n" + capitalize(groups[current_group]) + " > ");
    input_to("group_input");
}
