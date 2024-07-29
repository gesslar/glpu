#ifndef __EQUIPMENT_H__
#define __EQUIPMENT_H__

public mapping query_equipped();
public object equipped_on(string slot);
mixed can_equip(string slot, object ob);
int equip(string slot, object ob);
int unequip(string slot);

#endif // __EQUIPMENT_H__
