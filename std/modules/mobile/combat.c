// /combat/combat.c
// Combat-related object
//
// Created:     2024/02/19: Gesslar
// Last Change: 2024/02/19: Gesslar
//
// 2024/02/19: Gesslar - Created

#include <daemons.h>

inherit __DIR__ "module" ;

void update_victims() ;
void process_attack(int num) ;

private nosave object *victims = ({ }) ;
private nosave int tick = 0 ;
private nosave float gametick = get_config(__RC_GAMETICK_MSEC__) ;
private nosave float tick_speed = to_float(gametick) / 1_000.0 ;
private nosave float attack_speed = 2.0 ;

void setup() {
    module_name = "combat" ;
}

int start_module(object victim) {
    if(!objectp(victim)) {
        detach() ;
        return 0 ;
    }

    if(member_array(victim, victims) > -1) {
        return 1 ;
    }

    victims += ({ victim }) ;

    if(sizeof(victims) > 0) {
        call_out("combat_tick", tick_speed) ;
    }

    victim->add_module(module_name, owner) ;
}

void stop_module() {
    if(objectp(owner))
        tell(owner, "You stop fighting.\n") ;

    remove() ;
}

void combat_tick() {
    int required_ticks ;

    if(!objectp(owner) || !environment(owner))
        return detach();

    update_victims();
    if(sizeof(victims) == 0)
        return detach();

    // Increment tick
    tick++;

    // Calculate the number of ticks required for one attack cycle
    // Assuming gametick is in milliseconds and attack_speed is in seconds
    required_ticks = to_int(attack_speed / tick_speed);

    // If the number of ticks matches the required ticks for an attack
    if(tick >= required_ticks) {
        tick = 0; // Reset tick for the next attack cycle
        catch(process_attack(1));
    }

    // Schedule the next call based on tick_speed
    // Assuming tick_speed is the delay in seconds for each combat tick
    call_out("combat_tick", tick_speed);
}

void process_attack(int num) {
    object victim, room ;
    string name, vname ;

    victim = victims[0] ;
    room = environment(owner) ;

    name = owner->query_cap_name() ;
    vname = victim->query_cap_name() ;

    tell_down(room, name + " attacks " + vname + ".\n", 0, ({ owner, victim })) ;
    tell(victim, name + " attacks you.\n") ;
    tell(owner, "You attack " + vname + ".\n") ;
}

void update_victims() {
    victims = filter(victims, (:
        objectp($1) &&
        environment($1) == environment(owner)
    :)) ;
}

object query_target() {
    return victims[0] ;
}

object *query_victims() {
    return victims ;
}

int prevent_drop() {
    return 1 ;
}
