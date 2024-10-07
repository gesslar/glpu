/**
 * @file /std/object/description.c
 * @description Functions and variables for object descriptions
 *
 * @created 2024-01-31 - Gesslar
 * @last_modified 2024-01-31 - Gesslar
 *
 * @history
 * 2024-01-31 - Gesslar - Created
 */

#include <description.h>

// Variables
private mixed short, long ;
private nosave mapping extra_short = ([ ]), extra_long = ([ ]) ;

int set_short(mixed str) {
    short = str ;

    if(environment())
        event(environment(), "gmcp_item_update", environment()) ;
}

string query_short(object viewer: (: this_body() :)) {
    mixed result ;

    result = short ;

    if(valid_function(result))
        result = (*result)(viewer) ;
    if(!stringp(result))
        result = 0 ;

    return result ;
}

int set_long(mixed str) {
    long = str ;
}

string query_long(object viewer: (: this_body() :)) {
    mixed result ;

    result = long ;

    if(valid_function(result))
        result = (*result)(viewer) ;

    if(!stringp(result))
        result = 0 ;

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
        result = 0 ;

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
        result = 0 ;

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
