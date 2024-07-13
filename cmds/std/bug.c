/**
 * @file /cmds/std/bug.c
 * @description Bug command for reporting bugs.
 *
 * @created 2024/07/07 - Gesslar
 * @last_modified 2024/07/07 - Gesslar
 *
 * @history
 * 2024/07/07 - Gesslar - Created
 */

#include <daemons.h>

inherit STD_CMD ;

mixed main(object tp, string arg) {
    object editor ;

    editor = new(OBJ_EDITOR) ;

    return 1 ;
}
