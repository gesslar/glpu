/**
 * @file /std/container/storage_object.c
 * @description Object for permanent or temporary storage, like shops or
 *              armouries.
 *
 * @created 2024-08-01 - Gesslar
 * @last_modified 2024-08-01 - Gesslar
 *
 * @history
 * 2024-08-01 - Gesslar - Created
 */

#include <classes.h>

inherit STD_ITEM ;
inherit STD_CONTAINER ;

inherit CLASS_STORAGE ;

public nomask void save_contents() ;
private nomask void restore_contents() ;

private nomask class StorageOptions storage_options ;
private nosave string link ;

void mudlib_setup() {
    set_ignore_capacity(1) ;
    set_ignore_mass(1) ;
    set_debug_clean(1) ;
}

void set_storage_options(class StorageOptions storage) {
    mixed id;

    storage_options = storage;

    id = storage_options.storage_id;
    if(id) {
        if(valid_function(id))
            id = (*id)();
    } else if(storage_options.storage_org) {
        id = storage_options.storage_org;
    } else {
        error("Either storage_id or storage_org must be specified in storage options.");
    }

    if(!stringp(id))
        error("storage_id must resolve to a string.");

    storage_options.storage_id = id;

    if(storage_options.restore_on_load) {
        if(storage_options.storage_type == "private") {
            // we must have an storage_directory. if we don't, then we can use
            // the storage_org

            if(!storage_options.storage_directory)
                if(!storage_options.storage_org)
                    error("For private storage, storage_directory is only optional if storage_org is specified.");
                else
                    storage_options.storage_directory = storage_options.storage_org;
        }

        restore_contents();
    }
}

class StorageOptions query_storage_options() {
    return storage_options ;
}

public nomask void save_contents() {
    if(classp(storage_options)) {
        if(storage_options.restore_on_load &&
           storage_options.storage_id) {
            string storage_dir = mud_config("STORAGE_DATA_DIR") ;
            string dest ;
            string data ;

            if(storage_options.storage_directory) {
                storage_dir = resolve_path(storage_dir, storage_options.storage_directory) ;
                storage_dir = append(storage_dir, "/") ;
            }

            dest = sprintf("%s%s",
                storage_dir,
                storage_options.storage_id
            ) ;

            assure_file(dest) ;

            if(sizeof(all_inventory())) {
                data = save_to_string(1) ;
                write_file(dest, data, 1) ;
            } else {
                if(file_exists(dest))
                    rm(dest) ;
            }
        }
    }
}

public nomask void restore_contents() {
    if(classp(storage_options)) {
        if(storage_options.restore_on_load &&
           storage_options.storage_id) {
            string storage_dir = mud_config("STORAGE_DATA_DIR") ;
            string dest ;
            string data ;

            if(storage_options.storage_directory) {
                storage_dir = resolve_path(storage_dir, storage_options.storage_directory) ;
                storage_dir = append(storage_dir, "/") ;
            }
            storage_options.storage_directory = storage_dir ;

            dest = sprintf("%s%s",
                storage_dir,
                storage_options.storage_id
            ) ;

            if(file_exists(dest)) {
                data = read_file(dest) ;
                if(sizeof(data))
                    load_from_string(data, 1) ;

                save_contents() ;
            }
        }
    }
}

void set_clean_on_empty(int clean) {
    storage_options.clean_on_empty = clean ;
}

int query_clean_on_empty() {
    return storage_options.clean_on_empty ;
}

void set_restore_on_load(int restore) {
    storage_options.restore_on_load = restore ;
}

int query_restore_on_load() {
    return storage_options.restore_on_load ;
}

void set_storage_id(string id) {
    storage_options.storage_id = id ;
}

string query_storage_id() {
    return storage_options.storage_id ;
}

void set_storage_directory(string dir) {
    storage_options.storage_directory = dir ;
}

string query_storage_directory() {
    return storage_options.storage_directory;
}

int request_clean_up() {
    if(sizeof(all_inventory()) == 0 && classp(storage_options) &&
       storage_options.clean_on_empty)
        return 1;

    return 0;
}

int id(string id) {
    if(classp(storage_options)) {
        if(storage_options.storage_id)
            if(storage_options.storage_id == id)
                return 1 ;
    }

    return ::id(id) ;
}

string set_link(string link) {
    return link ;
}

string query_link() {
    return link ;
}

int is_storage_object() { return 1 ; }
