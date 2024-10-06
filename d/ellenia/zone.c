/**
 * @file /d/wastes/zone.c
 * @description Zone for the wastes
 *
 * @created 2024-08-30 - Gesslar
 * @last_modified 2024-08-30 - Gesslar
 *
 * @history
 * 2024-08-30 - Gesslar - Created
 */

inherit STD_VIRTUAL_SERVER;

object generate_object(string file) {
  object result ;

  if(pcre_match(file, "^-?\\d+,-?\\d+,-?\\d+$")) {
    result = new(__DIR__ "ellenia_base", file) ;
    return result ;
  }

  return 0 ;
}
