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

void set_adj(mixed str) ;
void add_adj(mixed str) ;
void remove_adj(mixed str) ;
void set_adjs(mixed arg) ;

void rehash_ids() ;

// From other objects
string query_name();

private string *ids = ({});
private string *_ids = ({});
private string *_plural_ids = ({});
private string *_adj = ({});

void set_id(mixed str) {
    if(pointerp(str)) set_ids(str);
    else set_ids(({ str }));
}

void set_adj(mixed str) {
    if(pointerp(str)) set_adjs(str);
    else set_adjs(({ str }));
}

void add_id(mixed str) {
    if(pointerp(str)) set_ids(_ids + str);
    else set_ids(_ids + ({ str }));
}

void add_adj(mixed str) {
    if(pointerp(str)) set_adjs(_adj + str);
    else set_adjs(_adj + ({ str }));
}

void remove_id(mixed str) {
    if(pointerp(str)) set_ids(_ids - str);
    else set_ids(_ids - ({ str }));
}

void remove_adj(mixed str) {
    if(pointerp(str)) set_adjs(_adj - str);
    else set_adjs(_adj - ({ str }));
}

void set_ids(mixed arg) {
    if(stringp(arg)) arg = ({arg});
    else if(pointerp(arg)) arg = filter(arg, (: stringp :));
    else error("Bad argument 1 to set_ids().\n");

    if(!sizeof(arg)) _ids = ({});
    else _ids = arg;

    if(query_name() && member_array(query_name(), _ids) == -1) {
        _ids += ({ query_name() });
    }
    rehash_ids();
}

void set_adjs(mixed arg) {
    if(stringp(arg)) arg = ({arg});
    else if(pointerp(arg)) arg = filter(arg, (: stringp :));
    else error("Bad argument 1 to set_adjs().\n");

    if(!sizeof(arg)) _adj = ({});
    else _adj = arg;

    rehash_ids();
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

    if(sizeof(ids) < 1) ids = ({query_name()});
    if(member_array(arg, ids) != -1) return 1;
    return 0;
}

void rehash_ids() {
    string *tmp = ({});
    string name ;

    name = query_name() ;

    tmp = ({ name });
    _plural_ids = map(_ids, (: pluralize :));
    tmp += _ids + _plural_ids;
    foreach(string id in _ids) {
        tmp += map(_adj, (: $1 + " " + $2 :), id);
    }

    tmp += ({ name }) ;

    ids = distinct_array(tmp);
}
