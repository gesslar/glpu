#include <simul_efun.h>

/**
 * Sends a message upward through the containment hierarchy, such as from an
 * object to its container, and further up to the room or environment.
 *
 * @param {object STD_OBJECT} ob - The object to send the message from.
 * @param {string} str - The message string to send.
 * @param {int} [msg_type] - The message type, which will be combined with UP_MSG.
 * @param {mixed} [exclude] - The objects to exclude from receiving the message.
 */
varargs void tell_up(object ob, string str, int msg_type, mixed exclude) {
  ob->receive_up(str, exclude, msg_type | UP_MSG);
}

/**
 * Sends a message downward through the containment hierarchy, such
 * as from a container to all objects it contains.
 *
 * @param {object STD_OBJECT} ob - The object to send the message from.
 * @param {string} str - The message string to send.
 * @param {int} [msg_type] - The message type, which will be combined with DOWN_MSG.
 * @param {mixed} [exclude] - The objects to exclude from receiving the message.
 */
varargs void tell_down(object ob, string str, int msg_type, mixed exclude) {
  ob->receive_down(str, exclude, msg_type | DOWN_MSG);
}

/**
 * Sends a message to all objects in the same environment, regardless
 * of their position in the containment hierarchy.
 *
 * @param {object STD_OBJECT} ob - The object to send the message from.
 * @param {string} str - The message string to send.
 * @param {int} [msg_type] - The message type, which will be combined with ALL_MSG.
 * @param {mixed} [exclude] - The objects to exclude from receiving the message.
 */
varargs void tell_all(object ob, string str, int msg_type, mixed exclude) {
  ob->receive_all(str, exclude, msg_type | ALL_MSG);
}

/**
 * Sends a direct message to the specified object without considering
 * containment hierarchy.
 *
 * @param {object STD_OBJECT} ob - The object to send the message to.
 * @param {string} str - The message string to send.
 * @param {int} [msg_type] - The message type, which will be combined with DIRECT_MSG.
 */
varargs void tell_direct(object ob, string str, int msg_type) {
  ob->receive_direct(str, msg_type | DIRECT_MSG);
}

/**
 * Sends a direct message to the specified object without considering
 * containment hierarchy.
 *
 * @param {string|object} [args] - If one argument, it's the message string (previous_object() is assumed).
 * @param {object} [args.0] - The object to send the message to (required if two+ arguments).
 * @param {string} [args.1] - The message string to send.
 * @param {int} [args.2] - The message type.
 *
 * @errors If insufficient arguments are provided.
 */
varargs void tell(mixed args...) {
  int sz;
  object ob;
  string str;
  int msg_type;

  if(!(sz = sizeof(args)))
    error("tell() insufficient arguments");

  if(sz == 1) {
    ob = previous_object();
    str = args[0];
  } else if(sz >= 2) {
    ob = args[0];
    str = args[1];
    if(sz == 3)
      msg_type = args[2];
  } else {
    error("tell() too many arguments");
  }

  tell_direct(ob, str, msg_type);
}
