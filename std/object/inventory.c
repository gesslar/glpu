/**
 * @file /std/object/inventory.c
 * @description Inventory inheritable for objects that can hold other objects.
 *
 * @created 2024-07-23 - Gesslar
 * @last_modified 2024-07-23 - Gesslar
 *
 * @history
 * 2024-07-23 - Gesslar - Created
 */

// Functions from other objects
void set_spawn_info(mapping info);

// Elements have the format of: ([ "object" : string, "number" : int|function, "args" : array ])
private nosave mapping inventory;


/**
 * @description Add or clone an object to the inventory of this_object()
 * @param {mixed} ob - Object to add to the inventory
 * @returns {object} - The object that was added to the inventory or 0 if the
 *                     object could not be added
 */
object add_inventory(mixed file, mixed args...) {
    object ob ;
    string e ;
    int result ;
    int new_ob ;

    if(nullp(file))
        return null ;

    e = catch {
        if(objectp(file)) {
            if(environment(file) == this_object())
                return file ;

            result = file->move(this_object()) ;

            if(result)
                return 0 ;

            return file ;
        }

        if(!stringp(file))
            return 0 ;

        new_ob = 1 ;

        if(sizeof(args))
            ob = new(file, args...) ;
        else
            ob = new(file) ;
    } ;

    if(e) {
        if(ob) {
            if(new_ob) {
                ob->remove() ;
            }
        }
        log_file("OBJECT", e) ;
    }

    if(!ob)
        return 0 ;

    ob->set_spawn_info(([
        "created" : time(),
        "spawn_point" : file_name(),
        "spawn_trace" : call_trace(),
    ])) ;

    e = catch(ob->move(this_object())) ;
    if(e) {
        log_file("OBJECT", e) ;
        ob->remove() ;
        return 0 ;
    }

    if(environment(ob) != this_object()) {
        ob->remove() ;
        return 0 ;
    }

    return ob ;
}

/**
 * @description Add an object to the object list of this_object() to be
 *              cloned when the object is created.
 * @param {mixed} ob - Object to add to the list
 * @returns {string} - The UUID of the object in the inventory mapping
 */
string add_object(mixed arg...) {
    mapping element ;
    string uuid ;
    int sz ;

    if(!inventory)
        inventory = ([]) ;

    // Count the number of arguments
    sz = sizeof(arg) ;

    if(sz == 0)
        return ;

    // Double-check we don't already have this object in our inventory mapping
    foreach(element in values(inventory))
        if(element["object"] == arg[0])
            return ;

    element = ([]) ;
    element["object"] = arg[0] ;

    if(sz >= 2)
        element["number"] = arg[1] ;
    else
        element["number"] = 1 ;

    if(sz >= 3)
        element["args"] = arg[2..] ;
    else
        element["args"] = ({}) ;

    uuid = generate_uuid() ;
    inventory[uuid] = element ;

    return uuid ;
}

void remove_object(string uuid) {
    if(!inventory) return ;
    if(!uuid) return ;
    if(!inventory[uuid]) return ;
    map_delete(inventory, uuid) ;
}

// This is mainly used for rooms, but other containers can use it as well.
// The intent is that when reset is called, it will iterate over the inventory
// mapping and clone everything the container has said that it wants.
// If the object is not present, it will clone it. If it is present, it will
// check if the object is the correct number. If it is not, it will clone the
// object until it is the correct number.
void reset_objects() {
    string uuid ;
    int sz, i ;
    int result ;
    mapping element ;
    object ob ;
    string e;

    if(!inventory)
        return ;

    foreach(uuid, element in inventory) {
        object *clones = present_clones(element["object"], this_object()) ;
        int num_clones = sizeof(clones) ;
        int diff ;

        if(num_clones < element["number"]) {
            diff = element["number"] - num_clones ;

            while(diff--) {
                ob = add_inventory(element["object"], element["args"]) ;
                if(ob) {
                    ob->add_spawn_info("belongs_to", file_name()) ;
                    ob->add_spawn_info("object_uuid", uuid) ;
                    event(this_object(), "object_spawned", ob) ;
                }
            }
        }
    }
}

mapping query_all_objects() {
    return copy(inventory) ;
}
