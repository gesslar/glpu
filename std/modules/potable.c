/**
 * @file /std/modules/potable.c
 * @description This module is inherited in order to make something potable.
 *
 * @created 2024-08-06 - Gesslar
 * @last_modified 2024-08-06 - Gesslar
 *
 * @history
 * 2024-08-06 - Gesslar - Created
 */

inherit M_USES ;

private nomask int _potable = null ;
private nomask mapping _default_actions = ([
    "drink": "$N $vdrink a $o.",
    "sip"  : "$N $vsip from a $o.",
]) ;

private nomask mapping _actions = ([
    "drink" : ([
        "action": null,
        "self"  : null,
        "room"  : null
    ]),
    "sip"   : ([
        "action": null,
        "self"  : null,
        "room"  : null
    ]),
]) ;

void set_drink_action(string action) {
    _actions["drink"]["action"] = action;
}

void set_self_drink_action(string action) {
    _actions["drink"]["self"] = action;
}

void set_room_drink_action(string action) {
    _actions["drink"]["room"] = action;
}

void set_sip_action(string action) {
    _actions["sip"]["action"] = action;
}

void set_self_sip_action(string action) {
    _actions["sip"]["self"] = action;
}

void set_room_sip_action(string action) {
    _actions["sip"]["room"] = action;
}

int set_potable(int potable) {
    _potable = potable;

    return _potable;
}

int is_potable() {
    return _potable;
}

int drink(object tp) {
    if(!_potable)
        return 0 ;

    if(nullp(adjust_uses(-query_uses())))
        return 0 ;

    if(_actions["drink"]["action"])
        tp->simple_action(_actions["drink"]["action"], this_object());
    else {
        if(!_actions["drink"]["self"] && !_actions["drink"]["room"]) {
            tp->simple_action(_default_actions["drink"], this_object());
        } else {
            if(_actions["drink"]["self"])
                tp->simple_action(_actions["drink"]["self"], this_object());
            else
                tp->simple_action(_default_actions["drink"], this_object());
            if(_actions["drink"]["room"])
                tp->simple_action(_actions["drink"]["room"], this_object());
            else
                tp->simple_action(_default_actions["drink"], this_object());
        }
    }


    return 1 ;
}

int sip(object tp, int amount) {
    if(!_potable)
        return 0 ;

    if(nullp(adjust_uses(-amount)))
        return 0 ;

    if(_actions["sip"]["action"])
        tp->simple_action(_actions["sip"]["action"], this_object());
    else {
        if(!_actions["sip"]["self"] && !_actions["sip"]["room"]) {
            tp->simple_action(_default_actions["sip"], this_object());
        } else {
            if(_actions["sip"]["self"])
                tp->simple_action(_actions["sip"]["self"], this_object());
            else
                tp->simple_action(_default_actions["sip"], this_object());
            if(_actions["sip"]["room"])
                tp->simple_action(_actions["sip"]["room"], this_object());
            else
                tp->simple_action(_default_actions["sip"], this_object());
        }
    }

    return 1 ;
}

void reset_potable() {
    reset_uses();
}
