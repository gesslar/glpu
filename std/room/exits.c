/**
 * @file /std/room/exits.c
 * @description Exits are the connections between rooms.
 *
 * @created 2024-09-13 - Gesslar
 * @last_modified 2024-09-13 - Gesslar
 *
 * @history
 * 2024-09-13 - Gesslar - Created
 */

#include <exits.h>

private nosave mapping _exits = ([]);
private nosave mapping _pre_exit_funcs = ([]);
private nosave mapping _post_exit_funcs = ([]);

mapping set_exits(mapping exit) {
  _exits = exit;
  return query_exits();
}

mapping query_exits() {
  return copy(_exits);
}

string *query_exit_ids() {
  return keys(_exits);
}

string query_exit(string id) {
  mixed dest;

  if(nullp(_exits[id]))
    return null;

  dest = _exits[id];
  if(valid_function(dest))
    dest = (*dest)();

  if(stringp(dest))
    dest = resolve_path(query_directory(), dest);

  return dest;
}

varargs object query_exit_dest(string id, int loaded) {
  mixed dest   = _exits[id];

  if(!dest)
    return null;

  if(valid_function(dest))
    dest = (*dest)();

  if(stringp(dest)) {
    dest = resolve_path(query_directory(), dest);
    if(loaded)
      dest = find_object(dest);
    else
      dest = load_object(dest);
  }

  if(!objectp(dest))
    return null;

  return dest;
}

object *query_exit_dests(int loaded) {
  object *dests = ({ });
  string *exits = query_exit_ids();
  foreach(string exit in exits) {
    object dest = query_exit_dest(exit, loaded);
    if(objectp(dest))
      dests += ({ dest });
  }
  return dests;
}

int valid_exit(string exit) {
  return !nullp(_exits[exit]);
}

mapping remove_exit(string id) {
  if(nullp(_exits[id]))
    return query_exits();

  map_delete(_exits, id);

  return query_exits();
}

mapping add_exit(string id, string path) {
  _exits[id] = path;

  return query_exits();
}

int has_pre_exit_func(string dir) {
  return !nullp(_pre_exit_funcs[dir]);
}

int has_post_exit_func(string dir) {
  return !nullp(_post_exit_funcs[dir]);
}

void add_pre_exit_func(string dir, mixed func) {
  map_delete(_pre_exit_funcs, dir);

  if(!valid_function(dir) && !stringp(func))
    return;

  _pre_exit_funcs[dir] = func;
}

void add_post_exit_func(string dir, mixed func) {
  map_delete(_post_exit_funcs, dir);

  if(!valid_function(dir) && !stringp(func))
    return;

  _post_exit_funcs[dir] = func;
}

void remove_pre_exit_func(string dir) {
  map_delete(_pre_exit_funcs, dir);
}

void remove_post_exit_func(string dir) {
  map_delete(_post_exit_funcs, dir);
}

mixed query_pre_exit_func(string dir) {
  return _pre_exit_funcs[dir];
}

mixed query_post_exit_func(string dir) {
  return _post_exit_funcs[dir];
}

void evaluate_pre_exit_func(string dir, object who) {
  mixed func = _pre_exit_funcs[dir];

  if(stringp(func))
    return catch(call_other(this_object(), func, who));

  if(valid_function(func))
    return catch((*func)(who));
}

void evaluate_post_exit_func(string dir, object who) {
  mixed func = _post_exit_funcs[dir];

  if(stringp(func))
    return catch(call_other(this_object(), func, who));

  if(valid_function(func))
    return catch((*func)(who));
}
