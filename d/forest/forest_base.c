/**
 * @file /d/village/forest/forest_base.c
 * @description The main forest room.
 *
 * @created 2024-08-21 - Gesslar
 * @last_modified 2024-08-21 - Gesslar
 *
 * @history
 * 2024-08-21 - Gesslar - Created
 */

inherit STD_ROOM;

void repopulate();

private nosave string *mob_files = ({});
private nosave object *mobs = ({});
private nosave float spawn_chance = 10.0;

void setup() {
  set_light(1);
  set_terrain("forest");
}

void virtual_setup(mixed args...) {
  string file = args[0];
  object ob;

  set_zone("shadowy_forest");

  __DIR__ "forest_daemon"->setup_exits(this_object(), file);
  __DIR__ "forest_daemon"->setup_short(this_object(), file);
  __DIR__ "forest_daemon"->setup_long(this_object(), file);

  add_reset((: repopulate :));

  mob_files = ({
    "/mob/deer",
    "/mob/squirrel",
    "/mob/bird",
    "/mob/fox",
  });
}

void repopulate() {
  string file;

  mobs -= ({ 0 });
  foreach(object mob in mobs) {
    if(objectp(mob)) {
      mob->simple_action("$N $vscamper away into the forest.");
      mob->remove();
    }
  }

  if(random_float(100.0) < spawn_chance) {
    file = element_of(mob_files);
    mobs += ({ add_inventory(file) });
    mobs->simple_action("$N $vappear from the undergrowth.");
  }
}
