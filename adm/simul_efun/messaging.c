#include "/adm/obj/simul_efun.h"

varargs void tell_from_inside(object ob, string str, int msg_type, mixed exclude) {
    ob->receive_inside_message(str, exclude, msg_type | INSIDE_MSG);
}

varargs void tell_from_outside(object ob, string str, int msg_type, mixed exclude) {
    ob->receive_outside_message(str, exclude, msg_type | OUTSIDE_MSG);
}

varargs void tell(object ob, string str, int msg_type) {
    ob->receive_private_message(str, msg_type | PRIVATE_MSG);
}
