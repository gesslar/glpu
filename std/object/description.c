// /std/object/description.c
// Description-related functions and variables for objects.
//
// Created:     2024/01/31: Gesslar
// Last Change: 2024/01/31: Gesslar
//
// 2024/01/31: Gesslar - Created

// Functions
int set_long(mixed str) ;
int set_short(mixed str) ;
string *query_extra_longs() ;
string *query_extra_shorts() ;
string query_extra_long(string id) ;
string query_extra_short(string id) ;
string query_long() ;
string query_short() ;
void add_extra_long(string id, mixed str) ;
void add_extra_short(string id, mixed str) ;
void remove_extra_long(string id) ;
void remove_extra_short(string id) ;

// Variables
private mixed short, long ;
private nosave mapping extra_short = ([ ]), extra_long = ([ ]) ;

int set_short(mixed str) {
    short = str;
}

string query_short(object viewer: (: this_player() :)) {
    mixed result ;

    result = short ;

    if(valid_function(result))
        result = (*result)(viewer) ;

    if(!stringp(result))
        result = 0;

    return result ;
}

int set_long(mixed str) {
    long = str;
}

string query_long() {
    string result ;

    if(valid_function(long))
        result = (*long)() ;
    if(!stringp(result))
        result = 0;

    return result ;
}

void add_extra_short(string id, mixed str) {
    if(!mapp(extra_short))
        extra_short = ([ ]) ;

    extra_short[id] = str ;
}

string *query_extra_shorts() {
    string *cles ;
    string *result ;

    if(!mapp(extra_short))
        extra_short = ([ ]) ;

    cles = keys(extra_short) ;
    cles = sort_array(cles, 1) ;

    result = map(cles, (: query_extra_short :)) ;

    return result ;
}

void remove_extra_short(string id) {
    if(!mapp(extra_short))
        extra_short = ([ ]) ;

    map_delete(extra_short, id) ;
}

string query_extra_short(string id) {
    string result ;

    if(!mapp(extra_short))
        extra_short = ([ ]) ;

    if(valid_function(extra_short[id]))
        result = (*extra_short[id])() ;
    if(!stringp(result))
        result = 0;

    return result ;
}

void add_extra_long(string id, mixed str) {
    if(!mapp(extra_long))
        extra_long = ([ ]) ;

    extra_long[id] = str ;
}

void remove_extra_long(string id) {
    if(!mapp(extra_long))
        extra_long = ([ ]) ;

    map_delete(extra_long, id) ;
}

string query_extra_long(string id) {
    string result ;

    if(!mapp(extra_long))
        extra_long = ([ ]) ;

    if(valid_function(extra_long[id]))
        result = (*extra_long[id])() ;
    if(!stringp(result))
        result = 0;

    return result ;
}

string *query_extra_longs() {
    string *cles ;
    string *result ;

    if(!mapp(extra_long))
        extra_long = ([ ]) ;

    cles = keys(extra_long) ;
    cles = sort_array(cles, 1) ;

    result = map(cles, (: query_extra_long :)) ;

    return result ;
}
