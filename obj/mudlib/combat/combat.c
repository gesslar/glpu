// /combat/combat.c
// Combat-related object
//
// Created:     2024/02/19: Gesslar
// Last Change: 2024/02/19: Gesslar
//
// 2024/02/19: Gesslar - Created

#include <daemons.h>

inherit STD_OBJECT ;

void stop_combat() ;
void update_victims() ;
void process_attack(int num) ;

private nosave object attacker, *victims = ({ }) ;
private nosave int tick = 0 ;
private nosave float gametick = get_config(__RC_GAMETICK_MSEC__) ;
private nosave float tick_speed = to_float(gametick) / 1_000.0 ;
private nosave float attack_speed = 2.0 ;

void start_combat(object me, object victim) {
    if(!objectp(me))
        return stop_combat() ;

    if(!objectp(victim))
        return stop_combat() ;

    if(!environment())
        move(me) ;

    if(member_array(victim, victims) > -1)
        return ;

    attacker = me ;
    victims += ({ victim }) ;

    if(sizeof(victims) > 0) {
        call_out("combat_tick", tick_speed) ;
    }
}

void stop_combat() {
    if(objectp(attacker))
        tell(attacker, "You stop fighting.\n") ;

    remove() ;
}

void combat_tick() {
    int required_ticks ;

    if(!objectp(attacker) || !environment(attacker))
        return stop_combat();

    update_victims();
    if(sizeof(victims) == 0)
        return stop_combat();

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

    victim = victims[0] ;
    room = environment(attacker) ;

    tell_down(room, attacker->query_name() + " attacks " + victim->query_name() + ".\n", 0, ({ attacker, victim })) ;
    tell(victim, attacker->query_name() + " attacks you.\n") ;
    tell(attacker, "You attack " + victim->query_name() + ".\n") ;
}

void update_victims() {
    victims = filter(victims, (:
        objectp($1) &&
        environment($1) == environment(attacker)
    :)) ;
}

int prevent_drop() {
    return 1 ;
}
