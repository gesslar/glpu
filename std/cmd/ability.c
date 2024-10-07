/**
 * @file /std/cmd/ability.c
 * @description Standard ability inheritance for commands
 *
 * @created 2024/02/20: Gesslar
 * @last_modified 2024/09/24: Gesslar
 *
 * @history
 * 2024/02/20 - Gesslar - Created
 * 2024/09/24 - Gesslar - Added cooldown support
 */

inherit STD_ACT ;

// Functions
int condition_check(object tp, string arg) ;
mixed use(object tp, string arg) {}
int cost_check(object tp, string arg) ;
void apply_cost(object tp, string arg) ;
object local_target(object tp, string arg, function f) ;
int cooldown_check(object tp, string arg) ;
void apply_cooldown(object tp, string arg) ;

// Variables
protected nomask nosave string ability_type ;
protected nomask nosave int aggressive ;

// Targetting
protected nomask nosave int target_current ;

// Conditions
protected nomask nosave float hp_cost, sp_cost, mp_cost ;
protected nomask nosave mapping cooldowns ;

void mudlib_setup() {
  ability_type = "ability" ;
}

mixed main(object tp, string arg) {
  int check_result ;

  // Do pre checks
  check_result = condition_check(tp, arg) ;
  if(check_result == 0) return 0 ; // Failure/unavailable/etc
  if(check_result == 2) return 1 ; // Failure, but message already sent

  // Now use
  return use(tp, arg) ;
}

/**
 * @function condition_check
 * @description Checks the conditions for using the ability.
 * @param {object} tp - The player to check the conditions for.
 * @param {string} arg - The argument to check the conditions for.
 * @returns {int} 1 if the conditions are met, 2 if the conditions are not met, 0 if the conditions are unknown.
 */
int condition_check(object tp, string arg) {
  if(tp->is_acting()) {
    tell(tp, "You are already doing something.\n") ;
    return 2 ;
  }

  // Cooldown checks
  if(!cooldown_check(tp, arg))
    return 2 ;

  // Cost checks
  if(!cost_check(tp, arg))
    return 2 ;

  return 1 ;
}

/**
 * @function query_ability_type
 * @description Returns the type of ability.
 * @returns {string} The type of ability.
 */
string query_ability_type() {
  return ability_type ;
}

/**
 * @function local_target
 * @description Finds a local target for the ability.
 * @param {object} tp - The player to find the target for.
 * @param {string} arg - The argument to find the target for.
 * @param {function} f - The function to check the target for.
 * @returns {object} The target if found, 0 if not found.
 */
varargs object local_target(object tp, string arg, function f) {
  object t ;
  object source ;
  mixed result ;

  if(!objectp(tp))
    error("Bad argument 1 to local_target().\n") ;

  if(nullp(arg)) {
    if(target_current) {
      if(t = tp->highest_threat())
        return t ;
      else {
        tell(tp, "You need to specify a target.\n") ;
        return 0 ;
      }
    } else {
      tell(tp, "You need to specify a target.\n") ;
      return 0 ;
    }
  }

  source = environment(tp) ;

  if(!t = find_target(tp, arg, source, f)) {
      tell(tp, "You don't see that here.\n") ;
      return 0 ;
  }

  if(aggressive == 1) {
    if(stringp(result = tp->prevent_combat(t))) {
      tell(tp, append(result, "\n")) ;
      return 0 ;
    }
  }

  return t ;
}

int cost_check(object tp, string arg) {
  if(hp_cost && tp->query_hp() < hp_cost) {
    tell(tp, "You are too weak to do that.\n") ;
    return 0 ;
  }

  if(sp_cost && tp->query_sp() < sp_cost) {
    tell(tp, "You are too drained to do that.\n") ;
    return 0 ;
  }

  if(mp_cost && tp->query_mp() < mp_cost) {
    tell(tp, "You are too tired to do that.\n") ;
    return 0 ;
  }

  return 1 ;
}

/**
 * @function apply_cost
 * @description Applies the cost of the ability to the player.
 * @param {object} tp - The player to apply the cost to.
 * @param {string} arg - The argument to apply the cost to.
 */
void apply_cost(object tp, string arg) {
  if(hp_cost) tp->adjust_hp(-hp_cost) ;
  if(sp_cost) tp->adjust_sp(-sp_cost) ;
  if(mp_cost) tp->adjust_mp(-mp_cost) ;
}

/**
 * @function find_cooldown_key
 * @description Finds the cooldown key for the ability.
 * @param {string} arg - The argument to find the cooldown key for.
 * @returns {string} The cooldown key if found, 0 if not found.
 */
private string find_cooldown_key(string arg) {
  string key, test ;
  mixed *keys ;

  keys = keys(cooldowns) ;
  if(sizeof(keys) == 1) {
    key = keys[0] ;
    test = cooldowns[key][0] ;

    if(test == "")
      return key ;

    return test ;
  }

  foreach(key in keys) {
    test = cooldowns[key][0] ;

    if(test == "")
      continue ;

    if(arg == test || pcre_match(arg, test))
      return key ;
  }

  return 0 ;
}

/**
 * @function cooldown_check
 * @description Checks the cooldown for the ability.
 * @param {object} tp - The player to check the cooldown for.
 * @param {string} arg - The argument to check the cooldown for.
 * @returns {int} 1 if the cooldown is ready, 0 if the cooldown is not ready, 2 if the cooldown is unknown.
 */
int cooldown_check(object tp, string arg) {
  string key ;
  int time ;
  string test ;
  mixed *value ;

  if(!(key = find_cooldown_key(arg)))
    return 1 ;

  if(tp->query_cooldown_remaining(key) > 0) {
    tell(tp, "You must wait before you can use that again.\n") ;
    return 0 ;
  }

  return 1 ;
}

/**
 * @function apply_cooldown
 * @description Applies the cooldown for the ability.
 * @param {object} tp - The player to apply the cooldown to.
 * @param {string} arg - The argument to apply the cooldown to.
 */
void apply_cooldown(object tp, string arg) {
  string key ;
  mixed result ;

  key = find_cooldown_key(arg) ;
  if(!key)
    return ;

  tp->add_cooldown(key, cooldowns[key][1]) ;
}
