// /std/cmd/ability.c
// Standard ability inheritance for commands
//
// Created:     2024/02/20: Gesslar
// Last Change: 2024/02/20: Gesslar
//
// 2024/02/20: Gesslar - Created

inherit STD_ACT ;

// Functions
int condition_check(object tp, string arg) ;
void apply_conditions(object tp, string arg) ;
mixed use(object tp, string arg) {}
int cost_check(object tp, string arg) ;
void apply_cost(object tp, string arg) ;
object local_target(object tp, string arg, function f) ;

// Variables
protected nomask nosave string ability_type ;
protected nomask nosave int aggressive ;

// Targetting
protected nomask nosave int target_current ;

// Conditions
protected nomask nosave float hp_cost, sp_cost, mp_cost ;
protected nomask nosave float cooldown ;

void mudlib_setup() {
    ability_type = "ability";
}

mixed main(object tp, string arg) {
    int check_result ;

    // Do pre checks
    check_result = condition_check(tp, arg) ;
    if(check_result == 0) return 0 ; // Failure/unavailable/etc
    if(check_result == 2) return 1 ; // Failure, but message already sent
    apply_conditions(tp, arg) ;

    // Now use
    return use(tp, arg);
}

int condition_check(object tp, string arg) {
    // Cost checks
    if(!cost_check(tp, arg)) return 2 ;
    if(tp->is_acting()) {
        tell(tp, "You are already doing something.\n") ;
        return 2 ;
    }

    return 1;
}

void apply_conditions(object tp, string arg) {
    apply_cost(tp, arg) ;
}

string query_ability_type() {
    return ability_type;
}

varargs object local_target(object tp, string arg, function f) {
    object t ;
    object source ;
    mixed result ;

    if(!objectp(tp))
        error("Bad argument 1 to local_target().\n");

    if(nullp(arg)) {
        if(target_current) {
            if(t = tp->highest_threat()) {
                return t ;
            } else {
                tell(tp, "You need to specify a target.\n") ;
                return 0 ;
            }
        } else {
            tell(tp, "You need to specify a target.\n") ;
            return 0 ;
        }
    }

    source = environment(tp) ;

    if(!t = find_target(tp, arg, source, f)) {
        tell(tp, "You don't see that here.\n") ;
        return 0 ;
    }

    if(aggressive == 1) {
        if(stringp(result = tp->prevent_combat(t))) {
            tp->tell(append(result, "\n")) ;
            return 0 ;
        }
    }

    return t;
}

int cost_check(object tp, string arg) {
    if(hp_cost && tp->query_hp() < hp_cost) {
        tp->tell("You are too weak to do that.\n") ;
        return 0 ;
    }

    if(sp_cost && tp->query_sp() < sp_cost) {
        tp->tell("You are too drained to do that.\n") ;
        return 0 ;
    }

    if(mp_cost && tp->query_mp() < mp_cost) {
        tp->tell("You are too tired to do that.\n") ;
        return 0 ;
    }

    return 1 ;
}

void apply_cost(object tp, string arg) {
    if(hp_cost) tp->adjust_hp(-hp_cost) ;
    if(sp_cost) tp->adjust_sp(-sp_cost) ;
    if(mp_cost) tp->adjust_mp(-mp_cost) ;
}
