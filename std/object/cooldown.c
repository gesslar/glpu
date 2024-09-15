/**
 * @file /std/object/cooldown.c
 * @description Cooldown object.
 *
 * @created 2024-09-15 - Gesslar
 * @last_modified 2024-09-15 - Gesslar
 *
 * @history
 * 2024-09-15 - Gesslar - Created
 */

#include <cooldown.h>

private mapping _cooldowns = ([]);

void cooldown() {
  _cooldowns = _cooldowns || ([]);
  foreach(string id, int time in _cooldowns)
    if(time < time())
      map_delete(_cooldowns, id);
}

mapping query_cooldowns() {
  return copy(_cooldowns);
}

void set_cooldown(string id, int cooldown) {
  _cooldowns[id] = cooldown;
}

// We can only add a new cooldown if there is no existing cooldown.
int add_cooldown(string id, int cooldown) {
  int time = query_cooldown(id);

  if(time > 0)
    return null ;

  _cooldowns[id] = time() + cooldown;

  return query_cooldown(id);
}

int remove_cooldown(string id) {
  if(!nullp(query_cooldown(id)))
    return null ;

  map_delete(_cooldowns, id);

  return 1 ;
}

int query_cooldown(string id) {
  _cooldowns = _cooldowns || ([]);

  return _cooldowns[id] ;
}

int query_cooldown_remaining(string id) {
  int time = query_cooldown(id);

  if(time < 0)
    return 0;

  return time - time();
}

int adjust_cooldown(string id, int amount) {
  int time = query_cooldown(id);

  if(time < 0)
    return null ;

  _cooldowns[id] = time + amount;

  return query_cooldown(id);
}

int wipe_cooldowns() {
  _cooldowns = ([]);

  return 1;
}
