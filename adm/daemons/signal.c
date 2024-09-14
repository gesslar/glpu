/**
 * @file /adm/daemons/signal.c
 * @description Signal daemon for handling system-wide event notifications
 *
 * @created 2024-07-21 - Gesslar
 * @last_modified 2024-07-21 - Gesslar
 *
 * @history
 * 2024-07-21 - Gesslar - Created
 */

inherit STD_DAEMON ;

private nomask void invalidate_slots() ;
public nomask int register_slot(int sig, object ob, string func) ;
public nomask int unregister_slot(int sig, object ob) ;
public nomask void dispatch_signal(int sig, mixed arg...) ;

private nosave mapping slots = ([]) ;

void setup() {
  set_no_clean(1) ;

  slots = SWAP_D->swap_out("signal") ;

  set_heart_beat(60) ;

  invalidate_slots() ;
}

public nomask int register_slot(int sig, object ob, string func) {
  if(previous_object() != simul_efun())
    return SIG_SLOT_INVALID_CALLER ;

  if(nullp(sig))
   return SIG_MISSING_SIGNAL ;

  if(nullp(ob))
    return SIG_MISSING_OBJECT ;

  if(nullp(func))
    return SIG_MISSING_FUNCTION ;

  if(!objectp(ob))
    return SIG_INVALID_OBJECT ;

  if(!function_exists(func, ob))
    return SIG_INVALID_FUNCTION ;

  if(!mapp(slots[sig]))
    slots[sig] = ([]) ;

  slots[sig][ob] = func ;

  return SIG_SLOT_OK ;
}

public nomask int unregister_slot(int sig, object ob) {
  mapping sig_slot ;

  if(previous_object() != simul_efun())
    return SIG_SLOT_INVALID_CALLER ;

  if(nullp(sig))
    return SIG_MISSING_SIGNAL ;

  if(nullp(ob))
    return SIG_MISSING_OBJECT ;

  if(!objectp(ob))
    return SIG_INVALID_OBJECT ;

  sig_slot = slots[sig] ;
  if(of(ob, sig_slot))
    map_delete(sig_slot, ob) ;

  if(!sizeof(sig_slot))
    map_delete(slots, sig) ;
  else
    slots[sig] = sig_slot ;

  return SIG_SLOT_OK ;
}

private nomask void invalidate_slots() {
  mapping new_slots = ([ ]) ;

  if(!mapp(slots))
    slots = ([ ]) ;

  foreach(int sig, mapping slot in slots) {
    foreach(object ob, string func in slot) {
      if(objectp(ob) && function_exists(func, ob)) {
        if(!mapp(new_slots[sig]))
          new_slots[sig] = ([ ]) ;

        new_slots[sig][ob] = func ;
      }
    }
  }

  slots = new_slots ;
}

public nomask void dispatch_signal(int sig, mixed arg...) {
  mapping sig_slot = slots[sig] ;

  if(previous_object() != simul_efun())
    return ;

  if(!mapp(sig_slot))
    return ;

  foreach(object ob, string func in sig_slot) {
    if(objectp(ob) && function_exists(func, ob)) {
      string e ;

      catch(call_other(ob, func, arg...)) ;

      if(e)
        log_file("SIGNAL_ERROR", "Error in signal dispatch: " + e) ;
    }
  }
}

void heart_beat() {
  invalidate_slots() ;
}

void unsetup() {
  invalidate_slots() ;
  SWAP_D->swap_in("signal", slots) ;
}
