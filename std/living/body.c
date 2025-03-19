/**
 * @file /std/living/body.c
 * @description Body object that is shared by players and NPCs.
 *
 * @created 2024-07-29 - Gesslar
 * @last_modified 2024-07-29 - Gesslar
 *
 * @history
 * 2024-07-29 - Gesslar - Created
 */

#include <body.h>
#include <commands.h>
#include <env.h>
#include <player.h>
#include <gmcp_defines.h>
#include <driver/origin.h>

inherit STD_CONTAINER;
inherit STD_ITEM;

inherit __DIR__ "act";
inherit __DIR__ "advancement";
inherit __DIR__ "alias";
inherit __DIR__ "appearance";
inherit __DIR__ "attributes";
inherit __DIR__ "boon";
inherit __DIR__ "combat";
inherit __DIR__ "communication";
inherit __DIR__ "ed";
inherit __DIR__ "env";
inherit __DIR__ "equipment";
inherit __DIR__ "module";
inherit __DIR__ "pager";
inherit __DIR__ "race";
inherit __DIR__ "skills";
inherit __DIR__ "visibility";
inherit __DIR__ "vitals";
inherit __DIR__ "wealth";

inherit M_ACTION;
inherit M_LOG;

/** @type {STD_BODY} */ object su_body;

/* Prototypes */

void mudlib_setup() {
  if(!clonep() &&
     origin() != ORIGIN_LOCAL &&
     previous_object() != body_d())
    return;

  enable_commands();
  add_standard_paths();
  if(wizardp())
    add_wizard_paths();

  if(!query_pref("prompt"))
    set_pref("prompt", ">");
  set_log_level(0);
  set_prevent_get(1);
  add_action("command_hook", "", 1);
  set_ignore_mass(1);
}

private nosave string *body_slots = ({
  "head", "neck", "torso", "back", "arms", "hands", "legs", "feet"
});

private nosave string *weapon_slots = ({
  "right hand", "left hand"
});

string *query_body_slots() {
  return copy(body_slots);
}

string *query_weapon_slots() {
  return copy(weapon_slots);
}

string *query_all_commands() {
  return commands();
}

void rehash_capacity() {
  if(mud_config("USE_MASS") && !query_capacity())
    set_capacity(1000);

  ::rehash_capacity();
}

void die() {
  object
  /** @type {OBJ_CORPSE} */ corpse,
  /** @type {STD_ITEM} */   ob,
  /** @type {STD_ITEM} */   next;

  if(!environment())
    return;

  if(!is_dead())
    return;

  stop_all_attacks();

  if(objectp(su_body)) {
    exec(su_body, this_object());
    su_body->move(environment());
    su_body->simple_action("$N $vis violently ejected from the body of $o.", this_object());
    clear_su_body();
  }

  simple_action("$N $vhave perished.");
  save_body();
  emit(SIG_PLAYER_DIED, this_object(), killed_by());
  corpse = new(OBJ_CORPSE);
  corpse->setup_corpse(this_object(), killed_by());

  if(function_exists("query_loot_table"))
    LOOT_D->loot_drop(killed_by(), this_object());
  if(function_exists("query_coin_table"))
    LOOT_D->coin_drop(killed_by(), this_object());

  ob = first_inventory(this_object());
  while(ob) {
    next = next_inventory(ob);
    if(ob->move(corpse))
      ob->remove();
    ob = next;
  }

  // Now move coin objects to the corpse
  if(query_total_wealth()) {
    mapping wealth = query_all_wealth();
    foreach(string currency, int amount in wealth) {
      object coin = new(OBJ_COIN);
      coin->set_up(currency, amount);
      if(coin->move(corpse))
        if(coin)
          coin->remove();
    }
  }

  if(corpse->move(environment()))
    corpse->remove();

  if(userp())  {
    object ghost = BODY_D->create_ghost(query_privs());
    exec(ghost, this_object());
    ghost->setup_body();
    ghost->set_hp(-1.0);
    ghost->set_sp(-1.0);
    ghost->set_mp(-1.0);
    ghost->move(environment());
  } else {
    ADVANCE_D->kill_xp(killed_by(), this_object());
  }

  remove();
}

varargs int move(mixed ob) {
  int result;
  object env;

  env = environment();
  result = ::move(ob);

  if(result)
    return result;

  if(env)
    set_last_location(env);

  return result;
}

void event_remove(object prev) {
  object
  /** @type {STD_ITEM} */ ob,
  /** @type {STD_ITEM} */ next;

  ob = first_inventory(this_object());
  while(ob) {
    next = next_inventory(ob);
    if(call_if(ob, "prevent_drop")) {
      ob->remove();
    } else {
      if(environment()) {
        int result = ob->move(environment());
        if(result)
          ob->remove();
      }
    }
    ob = next;
  }
}

void receive_message(string type, string msg) {
  do_receive(msg, DIRECT_MSG);
}

varargs int move_living(mixed dest, string dir, string depart_message, string arrive_message) {
  int result;
  object curr = environment();
  string tmp;

  result = move(dest);
  if(result)
    return result;

  if(is_acting()) {
    tell(this_object(), "You stop what you are doing.\n");
    cancel_acts();
  }

  if(curr) {
    if(depart_message != "SILENT") {
      depart_message = depart_message || query_env("move_out") || "$N leaves $D.";
      dir = dir || "somewhere";

      tmp = replace_string(depart_message, "$N", query_name());
      tmp = replace_string(tmp, "$D", dir);

      tmp = append(tmp, "\n");

      tell_down(curr, tmp);
    }
  }

  if(arrive_message != "SILENT") {
    curr = environment();

    arrive_message = arrive_message || query_env("move_in") || "$N arrives.\n";
    tmp = replace_string(arrive_message, "$N", query_name());

    tmp = append(tmp, "\n");

    tell_down(curr, tmp, null, ({ this_object() }));
  }

  force_me("look");

  GMCP_D->send_gmcp(this_object(), GMCP_PKG_ROOM_INFO, environment());

  return result;
}

mixed* query_commands() {
  return commands();
}

int force_me(string cmd) {
  if(
        this_body() != this_object()
    && !adminp(previous_object())
    && !adminp(this_caller()))
    return 0;
  else
    return command(cmd);
}

//Misc functions
void write_prompt() {
  string prompt = query_pref("prompt");

  receive(prompt + " ");
}

int query_log_level() {
  if(!query_pref("log_level")) return 0;

  return to_int(query_pref("log_level"));
}

void set_su_body(object source) {
  su_body = source;
}

object query_su_body() {
  return su_body;
}

void clear_su_body() {
  su_body = null;
}
