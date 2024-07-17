/**
 * @file /adm/daemons/boot.c
 * @description Daemon to manage incrementing boot number and calling the
 * boot alarm.
 *
 * @created 2024/03/05 - Gesslar
 * @last_modified 2024/03/05 - Gesslar
 *
 * @history
 * 2024/03/05 - Gesslar - Created
 */


#include <daemons.h>
#include <origin.h>

inherit STD_DAEMON ;
inherit M_LOG ;

private nomask int boot_number ;

void setup() {
    set_log_level(0) ;
    set_persistent(1) ;
}

/**
 * @daemon_function event_boot
 * @description Called when the mud boots up. Increments the boot number and
 *              saves the data. Must be included in `adm/etc/preload`.
 * @param {object} prev - The previous object that called this function.
 */
void event_boot(object prev) {
    if(previous_object() != master())
        return ;

    _log(1, "Boot #%d loaded.", ++boot_number) ;

    save_data() ;

    catch(ALARM_D->execute_boot_alarms()) ;
}

int query_boot_number() {
    return boot_number ;
}
