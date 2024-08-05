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
    int result;
    object prev;
    object env = environment();
    int use_bulk = mud_config("USE_BULK");
    int use_mass = mud_config("USE_MASS");
    int env_ignore_mass = env ? call_if(env, "ignore_mass") : 0 ;
    int env_ignore_bulk = env ? call_if(env, "ignore_bulk") : 0 ;
    int dest_ignore_mass = call_if(dest, "ignore_mass") || 0 ;
    int dest_ignore_bulk = call_if(dest, "ignore_bulk") || 0 ;
    int mass = query_mass();
    int bulk = query_bulk();

    result = allow_move(dest);

    if(result)
        return result;

    if(!objectp(dest)) {
        if(stringp(dest)) {
            dest = load_object(dest);
        } else {
            return MOVE_NO_DEST;
        }
    }

    if(!objectp(dest))
        return MOVE_NO_DEST;

    if(env && env == dest)
        return MOVE_ALREADY_THERE;

    if(use_mass || use_bulk) {
        int cap, max_cap ;
        int vol, max_vol ;

        cap = dest->query_capacity();
        max_cap = dest->query_max_capacity();
        vol = dest->query_volume();
        max_vol = dest->query_max_volume();

        // First do the checks
        if(use_mass) {
            if(!dest_ignore_mass) {
                if(cap - mass < 0) {
                    return MOVE_TOO_HEAVY ;
                }
            }
        }
        if(use_bulk) {
            if(!dest_ignore_bulk) {
                if(vol - bulk < 0) {
                    return MOVE_NO_ROOM ;
                }
            }
        }

        // Now do the adjustements
        if(use_mass) {
            if(!dest_ignore_mass)
                dest->adjust_capacity(-mass);
            if(env && !env_ignore_mass)
                env->adjust_capacity(mass);
        }
        if(use_bulk) {
            if(!dest_ignore_bulk)
                dest->adjust_volume(-bulk);
            if(env && !env_ignore_bulk)
                env->adjust_volume(bulk);
        }
    }

    move_object(dest);
    event(this_object(), "moved", prev);
    if(prev && this_object()) event(prev, "released", environment());
    if(this_object()) event(environment(), "received", prev);

    if(this_object()) return MOVE_OK;
    else return MOVE_DESTRUCTED;
}
