/*
setplan.c

Parthenon@LPUniversity
July 22nd, 2006
command to allow editing of plan

*/

//Last edited on July 23rd, 2006 by Parthenon

inherit STD_CMD ;

mapping in_edits = ([]);

void write_plan();
int delete_plan();

mixed main(object caller, string arg) {
    string tmp = "";
    string user = this_player()->query_proper_name();

    if(arg && (arg == "-d" || sscanf(arg, "-d %*s"))) return delete_plan();

    if(arg && (arg == "-o" || sscanf(arg, "-o %*s")))
    in_edits[user] = ({ tmp, 1 });
    else
    in_edits[user] = ({ tmp, 0 });

    if(in_edits[user][1] == 1)
    write("\nEnter the text you wish to have for your plan.\n" +
      "Enter '.' on a line alone to finish, and '~q' to exit without saving\n\n:");
    else
    write("\nEnter the text you wish to append to your plan.\n" +
      "Enter '.' on a line alone to finish, and '~q' to exit without saving\n\n:");

    input_to("get_text");

    return 1;
}

int get_text(string arg) {
    if(arg == "~q") {
        write("\nSetplan not modified.\n\n");
        return 1;
    }

    if(arg == ".") {
        write_plan();
        return 1;
    }

    in_edits[this_player()->query_proper_name()][0] += arg + "\n";
    write(":");

    input_to("get_text");
    return 1;
}

int delete_plan() {
    string file = "/home/" + this_player()->query_proper_name()[0..0] + "/" + this_player()->query_proper_name() + "/.plan";

    map_delete(in_edits, this_player()->query_proper_name());

    if(!rm(file))
        return notify_fail("\nError [setplan]: could not delete plan file\n");
    else {
        write("\nSuccess [setplan]: plan successfully deleted\n");
        return 1;
    }
}

void write_plan() {
    string file = "/home/" + this_player()->query_proper_name()[0..0] + "/" + this_player()->query_proper_name() + "/.plan";

    if(!in_edits[this_player()->query_proper_name()][0] || in_edits[this_player()->query_proper_name()][0] == "") get_text("~q");

    if(in_edits[this_player()->query_proper_name()][1] == 1) {
        if(!write_file(file, in_edits[this_player()->query_proper_name()][0], 1)) {
            write("\nError [setplan]: New plan could not be created\n\n");
            map_delete(in_edits, this_player()->query_proper_name());
            return;
        } else {
            write("\nSuccess [setplan]: New plan created\n\n");
            map_delete(in_edits, this_player()->query_proper_name());
            return;
        }
    } else {
        if(!write_file(file, in_edits[this_player()->query_proper_name()][0])) {
            write("\nError [setplan]: Plan could not be appended to\n\n");
            map_delete(in_edits, this_player()->query_proper_name());
            return;
        } else {
            write("\nSuccess [setplan]: Plan successfully edited\n\n");
            map_delete(in_edits, this_player()->query_proper_name());
            return;
        }
    }
}

string help(object caller) {
    return
    " SYNTAX: setplan [-o|-d]\n\n" +
    "This command allows you to edit your plan that is shown in your finger\n"+
    "information. If you use the '-o' option then your plan will be overwritten\n"+
    "otherwise what you type in will be appended to your current plan. You \n"+
    "may also delete your plan by using the '-d' option.\n";
}
