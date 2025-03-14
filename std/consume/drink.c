/**
 * @file /std/consume/drink.c
 * @description Drink inheritable for objects that can be consumed.
 *
 * @created 2024-08-06 - Gesslar
 * @last_modified 2024-08-06 - Gesslar
 *
 * @history
 * 2024-08-06 - Gesslar - Created
 */

inherit STD_ITEM;

inherit M_POTABLE;

string consume_message();

void mudlib_setup() {
  set_potable(1);
  save_var("_uses", "_max_uses", "_use_status_message");
  add_extra_long("consume", (: consume_message :));
}

void set_id(mixed str) {
  ::set_id(str);

  add_id("drink");
}

int drink(object user) {
  if(!::drink(user))
    return 0;

  if(query_uses() < 1)
    remove();

  return 1;
}

int sip(object user) {
  int consumed;
  string mess;

  if(!::sip(user, 1))
    return 0;

  if(query_uses() < 1)
    remove();

  return 1;
}

string consume_message() {
  int left;
  string mess;

  left = percent(query_uses(), query_max_uses());
  switch(left) {
    case 100:
      mess = sprintf("This %s is full.", query_name());
      break;
    case 80..99:
      mess = sprintf("This %s has barely been touched.", query_name());
      break;
    case 50..79:
      mess = sprintf("A lot of this %s has been drunk.", query_name());
      break;
    case 25..49:
      mess = sprintf("Most of this %s has been drunk.", query_name());
      break;
    case 0..24:
      mess = sprintf("There is very little left of this %s.", query_name());
      break;
  }

  return mess;
}

int is_drink() { return 1 ; }
