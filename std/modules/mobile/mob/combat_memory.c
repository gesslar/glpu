/**
 * @file /std/modules/mobile/mob/combat_memory.c
 * @description NPC combat memory module
 *
 * @created 2024-07-29 - Gesslar
 * @last_modified 2024-07-29 - Gesslar
 *
 * @history
 * 2024-07-29 - Gesslar - Created
 */

#include <origin.h>

inherit DIR_STD_MODULES_MOBILE "module";

void attack_on_sight(object target);

private nomask nosave string *combat_memory = ({});
public function attack_on_sight = (: attack_on_sight :);

void setup() {
  module_name = query_file_name();
}

int start_module(object ob, mixed args...) {
  ob->add_init(attack_on_sight);

  return 1;
}

void attack_on_sight(object target) {
  string name;

  if(target->is_ghost())
      return;

  name = target->query_name();
  if(of(name, combat_memory)) {
    query_owner()->targetted_action(
      "{{FF0033}}Raging, $N $vattack $t with a vengeance!{{res}}\n\n",
      target
    );

    query_owner()->start_attack(target);
    query_owner()->strike_enemy(target);
    query_owner()->strike_enemy(target);
  }
}

void add_to_memory(object target) {
  string name = target->query_name();

  if(!of(name, combat_memory))
    combat_memory += ({ name });
}

void unsetup(object prev) {
  if(query_owner() && prev != query_owner()) {
    query_owner()->remove_init(attack_on_sight);
    query_owner()->remove_module(module_name);
  }
}
