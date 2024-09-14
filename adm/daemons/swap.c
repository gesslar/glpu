/**
 * @file /adm/daemons/swap.c
 * @description This daemon exists to temporarily house data, such as when an object
 * is destructing but will be back and needs the information again.
 *
 * @created 2024/02/20 - Gesslar
 * @last_modified 2024/02/20 - Gesslar
 *
 * @history
 * 2024/02/20 - Gesslar - Created
 */

inherit STD_DAEMON ;

// Forward declarations
void swap_in(string label, mixed value) ;
mixed swap_out(string label) ;

private nosave mapping data = ([]) ;

/**
 * @daemon_function swap_in
 * @description Stores a value in the swap daemon under a given label.
 * @param {string} label - The identifier for the stored value.
 * @param {mixed} value - The value to be stored.
 */
void swap_in(string label, mixed value) {
  data[label] = value ;
}

/**
 * @daemon_function swap_out
 * @description Retrieves and removes a value from the swap daemon by its label.
 * @param {string} label - The identifier for the stored value.
 * @returns {mixed} The value associated with the given label, or 0 if not found.
 */
mixed swap_out(string label) {
  mixed value = data[label] ;

  map_delete(data, label) ;

  return value ;
}

int clean_up(int refs) {
  if(sizeof(data) > 0)
    return CLEAN_LATER ;

  return ::clean_up(refs) ;
}
