#include "/adm/obj/simul_efun.h"

varargs void tell_up(object ob, string str, int msg_type, mixed exclude) {
    ob->receive_up(str, exclude, msg_type | UP_MSG);
}

varargs void tell_down(object ob, string str, int msg_type, mixed exclude) {
    ob->receive_down(str, exclude, msg_type | DOWN_MSG);
}

varargs void tell_all(object ob, string str, int msg_type, mixed exclude) {
    ob->receive_all(str, exclude, msg_type | ALL_MSG);
}

varargs void tell_direct(object ob, string str, int msg_type, mixed exclude) {
    ob->receive_direct(str, exclude, msg_type | DIRECT_MSG);
}

varargs void tell(object ob, string str, int msg_type, mixed exclude) {
    tell_direct(ob, str, msg_type, exclude);
}
