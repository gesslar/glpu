// /std/object/id.c
// ID functions
//
// Created:     2024/01/31: Gesslar
// Last Change: 2024/01/31: Gesslar
//
// 2024/01/31: Gesslar - Created

#include <id.h>
#include <object.h>

private nosave string *_ids = ({});
private nosave string *_plural_ids = ({});
private nosave string *_adj = ({});

void set_id(mixed str) {
    if(stringp(str))
        str = ({ str });
    if(!pointerp(str))
        error("Bad argument 1 to set_id().");
    if(!uniform_array(str, T_STRING))
        error("Bad argument 1 to set_id().");

    _ids = distinct_array(str);
}

void set_adj(mixed str) {
    if(stringp(str))
        str = ({ str });
    if(!pointerp(str))
        error("Bad argument 1 to set_adj().");
    if(!uniform_array(str, T_STRING))
        error("Bad argument 1 to set_adj().");

    _adj = distinct_array(str);
}

void add_id(mixed str) {
    if(stringp(str))
        str = ({ str });
    if(!pointerp(str))
        error("Bad argument 1 to add_id().");
    if(!uniform_array(str, T_STRING))
        error("Bad argument 1 to add_id().");

    _ids = distinct_array(_ids + str);
}

void add_adj(mixed str) {
    if(stringp(str))
        str = ({ str });
    if(!pointerp(str))
        error("Bad argument 1 to add_adj().");
    if(!uniform_array(str, T_STRING))
        error("Bad argument 1 to add_adj().");

    _adj = distinct_array(_adj + str);
}

void remove_id(mixed str) {
    if(stringp(str))
        str = ({ str });
    if(!pointerp(str))
        error("Bad argument 1 to remove_id().");
    if(!uniform_array(str, T_STRING))
        error("Bad argument 1 to remove_id().");

    _ids = distinct_array(_ids - str);
}

void remove_adj(mixed str) {
    if(stringp(str))
        str = ({ str });
    if(!pointerp(str))
        error("Bad argument 1 to remove_adj().");
    if(!uniform_array(str, T_STRING))
        error("Bad argument 1 to remove_adj().");

    _adj = distinct_array(_adj - str);
}

string *query_ids() {
    return _ids;
}

string *query_plural_ids() {
    return _plural_ids;
}

string *query_adjs() {
    return _adj;
}

int id(string arg) {
    if(!arg || !stringp(arg))
        return 0;

    if(sizeof(_ids) < 1) _ids = ({query_name()});
    if(member_array(arg, _ids) != -1) return 1;
    return 0;
}

void rehash_ids() {
    string *tmp = ({});
    string name;

    name = query_name();

    tmp = ({ name });
    _plural_ids = map(_ids, (: pluralize :));
    tmp += _ids + _plural_ids;
    foreach(string id in _ids) {
        tmp += map(_adj, (: $1 + " " + $2 :), id);
    }

    tmp += ({ name });

    _ids = distinct_array(tmp);
}
