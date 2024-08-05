#ifndef __EQUIPMENT_H__
#define __EQUIPMENT_H__

public mapping query_equipped() ;
public mapping query_wielded() ;
public object equipped_on(string slot) ;
public object wielded_in(string slot) ;
int equip(object ob, string slot) ;
mixed equipped(object ob) ;
int unequip(mixed ob) ;
mixed can_equip(mixed ob, string slot) ;

#endif // __EQUIPMENT_H__
