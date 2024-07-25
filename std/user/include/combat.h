#ifndef __COMBAT_H__
#define __COMBAT_H__

float adjust_attack_speed(float amount) ;
float adjust_seen_threat(object enemy, float amount) ;
float adjust_threat(object enemy, float amount) ;
int attacking(object victim) ;
int current_enemy(object victim) ;
int in_combat() ;
int next_round() ;
int seen_enemy(object victim) ;
int start_attack(object victim) ;
int stop_attack(object victim) ;
mapping current_enemies() ;
varargs int stop_attack(object victim, int seen) ;
object highest_seen_threat() ;
object highest_threat() ;
object lowest_seen_threat() ;
object lowest_threat() ;
varargs int valid_enemy(object enemy, int threat) ;
varargs int valid_seen_enemy(object enemy, int threat) ;
void clean_up_enemies() ;
void clean_up_seen_enemies() ;
void combat_round() ;
void strike_enemy(object enemy) ;
void swing() ;

#endif // __COMBAT_H__
