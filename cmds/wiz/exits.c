// /cmds/wiz/exits.c
// Command to show all exits in the current room
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit STD_CMD ;

mixed main(object tp, object room, string args) {
    string *exits;
    string str;
    int i;
    mapping x ;

    exits = keys(room->query_exits());
    if (!sizeof(exits))
        return "There are no exits from this room.";

    x = allocate_mapping(exits, (:
        $(room)->query_exit($1)
    :));

    str = "Exits:\n";
    for (i = 0; i < sizeof(exits); i++) {
        str += exits[i] + " = " + x[exits[i]] + "\n";
    }

    return str;
}

string help(object caller) {
    return "Syntax: exits\n\n"
"This command will show all the exits in the current room and where they "
"lead.";
}
