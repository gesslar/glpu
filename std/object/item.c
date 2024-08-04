/**
 * @file /std/object/item.c
 * @description Real objects, whereas object is the base of everything
 *
 * @created 2024/07/27 - Gesslar
 * @last_modified 2024/07/27 - Gesslar
 *
 * @history
 * 2024/07/27 - Gesslar - Created
 */

inherit STD_OBJECT ;
inherit STD_VALUE ;

private nosave mapping spawn_info = ([]) ;

void set_spawn_info(mapping info) {
    spawn_info = info ;
}

void add_spawn_info(string key, mixed value) {
    spawn_info[key] = value ;
}

mixed query_spawn_info(string key) {
    return spawn_info[key] ;
}

mapping query_all_spawn_info() {
    return copy(spawn_info) ;
}

int allow_move(mixed dest) {
    object ob;

    if(stringp(dest)) catch(ob = load_object(dest)) ;
    else if(objectp(dest)) ob = dest;

    if(!objectp(ob))
        return MOVE_NO_DEST;

    if(environment() == ob)
        return MOVE_ALREADY_THERE ;

    if(!ob->can_receive(this_object()))
        return MOVE_NOT_ALLOWED;

    if(mud_config("USE_MASS"))
        if(!dest->ignore_mass())
            if(dest->query_capacity() < query_mass())
                return MOVE_TOO_HEAVY ;

    if(mud_config("USE_BULK"))
        if(!dest->ignore_bulk())
            if(dest->query_volume() < query_bulk())
                return MOVE_NO_ROOM ;

    if(environment())
        if(!environment()->can_release(this_object()))
            return 0;

    return MOVE_OK ;
}

int move(mixed dest) {
    int result ;
    object prev ;

    result = allow_move(dest) ;
    if(!(result & MOVE_OK))
        return result ;

    if(result & MOVE_ALREADY_THERE)
        return MOVE_OK | MOVE_ALREADY_THERE ;

    prev = environment() ;
    if(prev) {
        if(!prev->ignore_capacity())
            prev->add_capacity(query_mass());
        if(!prev->ignore_bulk())
            prev->add_volume(query_bulk());
    }

    if(!dest->ignore_capacity())
        dest->add_capacity(-query_mass());
    if(!dest->ignore_bulk())
        dest->add_volume(-query_bulk());

    move_object(dest);
    event(this_object(), "moved", prev) ;
    if(prev && this_object()) event(prev, "released", environment()) ;
    if(this_object()) event(environment(), "received", prev) ;

    if(this_object()) return MOVE_OK ;
    else return MOVE_OK | MOVE_DESTRUCTED ;
}
