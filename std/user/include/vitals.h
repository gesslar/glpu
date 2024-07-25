#ifndef __VITALS_H__
#define __VITALS_H__

protected varargs void heal_tick(int force) ;
protected void init_vitals() ;
public float add_hp(float x) ;
public float add_mp(float x) ;
public float add_sp(float x) ;
public float hp_ratio() ;
public float mp_ratio() ;
public float query_hp() ;
public float query_max_hp() ;
public float query_max_mp() ;
public float query_max_sp() ;
public float query_mp() ;
public float query_sp() ;
public float sp_ratio() ;
public int is_dead() ;
public int set_dead(int x) ;
public varargs void update_heal_tick(int tth) ;
public void restore() ;
public void set_hp(float x) ;
public void set_max_hp(float x) ;
public void set_max_mp(float x) ;
public void set_max_sp(float x) ;
public void set_mp(float x) ;
public void set_sp(float x) ;

#endif // __VITALS_H__
