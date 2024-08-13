/**
 * @file /std/object/item.c
 * @description Real objects, whereas object is the base of everything
 *
 * @created 2024-07-27 - Gesslar
 * @last_modified 2024-07-27 - Gesslar
 *
 * @history
 * 2024-07-27 - Gesslar - Created
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
        if(!dest->ignore_capacity())
            if(query_mass() + dest->query_fill() > dest->query_capacity())
                return MOVE_TOO_HEAVY ;

    if(environment())
        if(!environment()->can_release(this_object()))
            return 0;

    return MOVE_OK ;
}

int move(mixed dest) {
    int result;
    object prev = environment();
    int use_mass = mud_config("USE_MASS");
    int prev_ignore_capacity = prev ? call_if(prev, "ignore_capacity") : 0 ;
    int dest_ignore_capacity = call_if(dest, "ignore_capacity") || 0 ;
    int prev_ignore_mass = prev ? call_if(prev, "ignore_mass") : 0 ;
    int dest_ignore_mass = call_if(dest, "ignore_mass") || 0 ;
    int mass = query_mass();

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

    if(prev && prev == dest)
        return MOVE_ALREADY_THERE;

    move_object(dest);

    if(use_mass) {
        object env, *envs = all_environment(dest);

        // First do the checks
        foreach(env in envs) {
            int fill = env->query_fill();
            int cap = env->query_capacity();
            int env_ignore_capacity = env->ignore_capacity();

            if(env_ignore_capacity)
                break ;

            if(mass + fill > cap) {
                return MOVE_TOO_HEAVY ;
            }
        }

        // Now do the adjustements
        if(prev && !prev_ignore_capacity)
            prev->adjust_fill(-mass);
        if(!dest_ignore_capacity)
            dest->adjust_fill(mass);
        if(prev && !prev_ignore_mass)
            prev->adjust_mass(-mass);
        if(!dest_ignore_mass)
            dest->adjust_mass(mass);
    }

    event(this_object(), "moved", prev);
    if(prev && this_object()) event(prev, "released", environment());
    if(this_object()) event(environment(), "received", prev);

    if(this_object()) return MOVE_OK;
    else return MOVE_DESTRUCTED;
}
