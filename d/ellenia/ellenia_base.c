/**
 * @file /d/wastes/wastes_base.c
 * @description The wastes room.
 *
 * @created 2024-08-30 - Gesslar
 * @last_modified 2024-08-30 - Gesslar
 *
 * @history
 * 2024-08-30 - Gesslar - Created
 */

inherit STD_ROOM;

void repopulate();

private nosave string *mob_files = ({});
private nosave object *mobs = ({});
private nosave float spawn_chance = 15.0;

void setup() {
  set_light(0);
  set_terrain("wastes");
}

void virtual_setup(mixed args...) {
  string file = args[0];
  object ob;

  set_zone("ellenia");
  // debug("Setting up room type for %O", this_object());
  __DIR__ "ellenia_daemon"->setup_room_type(this_object());
  __DIR__ "ellenia_daemon"->setup_room_subtype(this_object());
  __DIR__ "ellenia_daemon"->setup_exits(this_object(), file);
  __DIR__ "ellenia_daemon"->setup_short(this_object(), file);
  __DIR__ "ellenia_daemon"->setup_long(this_object(), file);
  // debug("Room type for %O is %O", this_object(), query_room_environment());

  add_reset((: repopulate :));

  mob_files = ({
    "/mob/waste_rat",
    "/mob/wasterel",
  });
}

void repopulate() {
  mobs -= ({ 0 });
  foreach(object mob in mobs) {
    if(objectp(mob)) {
      mob->simple_action("$N $vscurry away across the barren wasteland.");
      mob->remove();
    }
  }

  if(random_float(100.0) < spawn_chance) {
    string file = element_of(mob_files);

    mobs += ({ add_inventory(file) });
    mobs->simple_action("$N $varrive.");
  }
}
