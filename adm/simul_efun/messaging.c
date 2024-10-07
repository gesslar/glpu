#include <simul_efun.h>

/**
 * @simul_efun tell_up
 * @description Sends a message upward through the containment hierarchy, such as
 *              from an object to its container, and further up to the room or
 *              environment.
 * @param {object} ob - The object to send the message from.
 * @param {string} str - The message string to send.
 * @param {int} [msg_type] - The message type, combined with UP_MSG.
 * @param {mixed} [exclude] - The objects to exclude from receiving the message.
 */
varargs void tell_up(object ob, string str, int msg_type, mixed exclude) {
    ob->receive_up(str, exclude, msg_type | UP_MSG) ;
}

/**
 * @simul_efun tell_down
 * @description Sends a message downward through the containment hierarchy, such
 *              as from a container to all objects it contains.
 * @param {object} ob - The object to send the message from.
 * @param {string} str - The message string to send.
 * @param {int} [msg_type] - The message type, combined with DOWN_MSG.
 * @param {mixed} [exclude] - The objects to exclude from receiving the message.
 */
varargs void tell_down(object ob, string str, int msg_type, mixed exclude) {
    ob->receive_down(str, exclude, msg_type | DOWN_MSG) ;
}

/**
 * @simul_efun tell_all
 * @description Sends a message to all objects in the same environment, regardless
 *              of their position in the containment hierarchy.
 * @param {object} ob - The object to send the message from.
 * @param {string} str - The message string to send.
 * @param {int} [msg_type] - The message type, combined with ALL_MSG.
 * @param {mixed} [exclude] - The objects to exclude from receiving the message.
 */
varargs void tell_all(object ob, string str, int msg_type, mixed exclude) {
    ob->receive_all(str, exclude, msg_type | ALL_MSG) ;
}

/**
 * @simul_efun tell_direct
 * @description Sends a direct message to the specified object without considering
 *              containment hierarchy.
 * @param {object} ob - The object to send the message to.
 * @param {string} str - The message string to send.
 * @param {int} [msg_type] - The message type, combined with DIRECT_MSG.
 */
varargs void tell_direct(object ob, string str, int msg_type) {
    ob->receive_direct(str, msg_type | DIRECT_MSG) ;
}

/**
 * @simul_efun tell
 * @description Sends a direct message to the specified object without considering
 *              containment hierarchy.
 * @param {object} ob - The object to send the message to.
 * @param {string} str - The message string to send.
 * @param {int} [msg_type] - The message type.
 */
varargs void tell(mixed args...) {
    int sz ;
    object ob ;
    string str ;
    int msg_type ;

    if(!(sz = sizeof(args)))
        error("tell() insufficient arguments") ;

    if(sz == 1) {
        ob = previous_object() ;
        str = args[0] ;
    } else if(sz >= 2) {
        ob = args[0] ;
        str = args[1] ;
        if(sz == 3)
            msg_type = args[2] ;
    } else {
        error("tell() too many arguments") ;
    }

    tell_direct(ob, str, msg_type) ;
}
