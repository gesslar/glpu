#ifndef __ARMOR_H__
#define __ARMOR_H__

#include <clothing.h>

public void set_defense(mapping def) ;
public void add_defense(string type, float amount) ;
public mapping query_defense() ;
public float query_defense_amount(string type) ;
int is_armor() ;


#endif // __ARMOR_H__
