// /std/daemon/daemon.c
// Base daemon inheritible
//
// Created:     2022/08/23: Gesslar
// Last Change: 2022/08/23: Gesslar
//
// 2022/08/23: Gesslar - Created

inherit M_CLEAN ;
inherit M_SETUP ;
inherit M_SAVE ;

// Functions
void remove() ;

protected void create() {
    setup_chain() ;
    if(query_persistent())
        restore_data() ;
}

void remove() {
    if(query_persistent())
        save_data() ;

    event(({ this_object() }), "remove") ;
}

string query_directory() {
    string file = base_name(this_object()), *parts;
    string dir ;

    parts = explode(file, "/");
    parts = parts[0..<2];
    dir = implode(parts, "/");

    return prepend(dir, "/");
}

string query_file_name() {
    string file = base_name(this_object()), *parts;
    string dir ;

    parts = explode(file, "/");
    file = parts[<1];

    return file;
}
