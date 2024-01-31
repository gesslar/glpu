// /std/object/id.c
// ID functions
//
// Created:     2024/01/31: Gesslar
// Last Change: 2024/01/31: Gesslar
//
// 2024/01/31: Gesslar - Created

// Prototypes
void set_id(mixed str) ;
void add_id(mixed str) ;
void remove_id(mixed str) ;
void set_ids(mixed arg) ;
int id(string arg) ;

// From other objects
string query_name();

private string *ids = ({});

void set_id(mixed str) {
    if(pointerp(str)) set_ids(str);
    else set_ids(({ str }));
}

void add_id(mixed str) {
    if(pointerp(str)) set_ids(ids + str);
    else set_ids(ids + ({ str }));
}

void remove_id(mixed str) {
    if(pointerp(str)) set_ids(ids - str);
    else set_ids(ids - ({ str }));
}

void set_ids(mixed arg) {
    if(stringp(arg)) arg = ({arg});
    else if(pointerp(arg)) arg = filter(arg, (: stringp :));
    else error("Bad argument 1 to set_ids().\n");

    if(!sizeof(arg)) ids = ({});
    else ids = arg;

    if(member_array(query_name(), ids) == -1 && query_name()) {
        ids += ({query_name()});
    }
}

int id(string arg) {
    if (!arg || !stringp(arg))
        return 0;

    if(sizeof(ids) < 1) ids = ({query_name()});
    if(member_array(arg, ids) != -1) return 1;
    return 0;
}
