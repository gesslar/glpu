// /std/modules/save.c
// Module to handle persistent saving of data
//
// Created:     2024/02/02: Gesslar
// Last Change: 2024/02/02: Gesslar
//
// 2024/02/02: Gesslar - Created

private nosave int persistent = 0;
private nosave string save_file = 0;

varargs string set_save_file(string file);

// whether this object will save/load
int set_persistent(int x: (: 1 :)) {
    x = !!x;

    if(!save_file) set_save_file();

    return persistent = x;
}

int query_persistent() {
    return persistent;
}

// the save file, if none, provided, the object will determine its own
// save file based on the master_object() to be backwards compatible with
// old save.c
varargs string set_save_file(string file) {
    if(!file) {
        file = object_save_file(this_object());
    }

    if(file[<2..] == __SAVE_EXTENSION__) file = file[0..<3];
    save_file = file;

    return save_file ;
}

string query_save_file() {
    return save_file;
}

private string get_base_path(string path) {
    int pos = strsrch( path, "/", -1 ) ;

    return path[ 0 .. pos - 1 ] ;
}

// saving and loading data
int save_data() {
    string base;

    if(!persistent) return 0;
    if(!save_file) set_save_file();
    if(!save_file) return 0;

    base = get_base_path(save_file);
    if(!directory_exists(base)) {
        if(strsrch(save_file, "/data/") == 0) {
            if(!assure_dir(base))
                return 0;
        }
    }

    return save_object(save_file);
}

int restore_data() {
    if(!persistent) return 0;

    if(!save_file) save_file = object_save_file(this_object());

    if(!file_exists(save_file + __SAVE_EXTENSION__)) return 0;
    return restore_object(save_file);
}
