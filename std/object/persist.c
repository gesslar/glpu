/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Inheritable for persistance.
** Beek for nightmare, ported by Rust.
** could use helper funs that save directly to
** a file, and some security (when programatic security arrives)
** save() and restore() or save_to_file(), etc....
**
** Beek - protected a bunch of things which shouldn't be called externally
*/

//:MODULE
//This module implements persistence

#include <item.h>
#include <object.h>
#include <persist.h>

private nosave int save_recurse;
private nosave mixed *saved_vars = ({ }) ;

//:FUNCTION save_var
//Mark a variable as one that gets saved.
protected void save_var(mixed *vars...) {
  saved_vars = distinct_array(({ saved_vars..., vars... }));
}

//###Security problem here - Beek. What is it used for anyway?
//:FUNCTION get_saved_vars
//returns the *of variables that get saved.
public string *get_saved_vars() { return copy(saved_vars); }

//:FUNCTION set_save_recurse
//sets whether or not a save is recursive.
protected void set_save_recurse(int flag) { save_recurse = flag; }

//:FUNCTION save_to_string
//saves an object into a string representation.
varargs string save_to_string(int recursep) {
  string *elements, element ;
  mapping map = ([]);
  mapping tmp ;

  event(({ this_object() }), "saving") ;

//### setting a property based on a function arg?  Gross.
  set_save_recurse(recursep);

  if(sizeof(elements = distinct_array(copy(saved_vars)) - ({ 0 }))) {
    tmp = ([ ]) ;

    foreach(element in elements)
      tmp[element] = fetch_variable(element);

    if(sizeof(tmp))
      map["#vars#"] = tmp ;
  }

  map["#base_name#"] = base_name();

  if(save_recurse)
    map["#inventory#"] = all_inventory()->save_to_string(1) - ({ 0 });

  return save_variable(map);
}

//:FUNCTION load_from_string
//loads an object from a string representation.
void load_from_string(mixed value, int recurse) {
  mixed data;
  mixed *tmpsaved_vars ;
  mixed val ;
  string key ;
  string obj;
  object ob;
  mapping current ;

  if(!mapp(value))
    data = restore_variable(value);
  else
    data = value;

  foreach(key, val in data) {
    if(key == "#vars#") {
      string var ;
      mixed vval ;

      foreach(var, vval in val) {
        if(member_array(var, saved_vars) != -1)
          store_variable(var, vval) ;
        else
          continue ;
      }
    }
  }

  if(data["#inventory#"]) {
    foreach(obj in data["#inventory#"]) {
      mixed result ;

      val = restore_variable(obj);
      result = catch(ob = new(val["#base_name#"])) ;
      if(intp(result) && result == 0)
        ob->load_from_string(val, recurse + 1) ;
      else
        throw(result) ;
    }
  }

  // We don't want to move the object we're restoring into the
  // previous_object() (happens at the end)...  When you restored the body,
  // the body moved into the user object, but couldn't move out! -- Rust
  if(recurse > 1) {
    if(move(previous_object())) {
      object env = environment(previous_object()) ;
      if(env) {
        if(move(env))
          remove() ;
        else
          remove() ;
      }
    }
  }

  event(({ this_object() }), "restored") ;
}
