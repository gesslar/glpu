#ifndef __CLOTHING_H__
#define __CLOTHING_H__

public void set_slots(mapping sl) ;
private void add_slot(string slot, string type) ;
private void remove_slot(string slot) ;
public mapping query_slots() ;
public string query_slot_type(string slot) ;
private void set_covers(mapping sl) ;
private void remove_cover(string slot) ;
private void remove_covers(mapping sl) ;
public mapping query_covers() ;
private void add_cover(string slot) ;

#endif // __CLOTHING_H__
