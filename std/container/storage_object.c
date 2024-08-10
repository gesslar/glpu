/**
 * @file /std/container/storage_object.c
 * @description Object for permanent or temporary storage, like shops or
 *              armouries.
 *
 * @created 2024/08/01 - Gesslar
 * @last_modified 2024/08/01 - Gesslar
 *
 * @history
 * 2024/08/01 - Gesslar - Created
 */

#include <classes.h>

inherit STD_ITEM ;
inherit STD_CONTAINER ;

inherit CLASS_STORAGE ;

private nomask void save_contents() ;
private nomask void restore_contents() ;

private nomask class StorageOptions storage_options ;

void mudlib_setup(class StorageOptions storage) {
    set_capacity(MAX_INT) ;

    storage_options = storage ;
    set_ignore_capacity(1) ;

    restore_contents() ;
    set_debug_clean(1) ;
}

void mudlib_unsetup() {
    save_contents() ;
}

private nomask void save_contents() {
    if(classp(storage_options)) {
        if(storage_options.restore_on_load &&
           storage_options.storage_id &&
           storage_options.storage_directory) {
            string storage_dir = mud_config("STORAGE_DATA_DIR") ;
            string dest ;
            string data ;

            storage_dir = append(storage_dir, "/") ;
            storage_options.storage_directory = append(
                storage_options.storage_directory,
                "/"
            ) ;

            dest = sprintf("%s%s%s",
                storage_dir,
                storage_options.storage_directory,
                storage_options.storage_id
            ) ;

            assure_file(dest) ;

            data = save_to_string(1) ;
        }
    }
}

private nomask void restore_contents() {
    if(classp(storage_options)) {
        if(storage_options->restore_on_load &&
           storage_options->storage_id &&
           storage_options->storage_directory) {
            string storage_dir = mud_config("STORAGE_DATA_DIR") ;
            string dest ;
            string data ;

            storage_dir = append(storage_dir, "/") ;
            storage_options->storage_directory = append(
                storage_options->storage_directory,
                "/"
            ) ;

            dest = sprintf("%s%s%s",
                storage_dir,
                storage_options->storage_directory,
                storage_options->storage_id
            ) ;

            if(file_exists(dest)) {
                data = read_file(dest) ;
                load_from_string(data, 1) ;
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
    storage_options.storage_directory ;
}

int request_clean_up() {
    if(sizeof(all_inventory()) > 0) {
        if(storage_options.clean_on_empty)
            return 1 ;
    }

    return 0 ;
}

int query_storage_object() { return 1 ; }
