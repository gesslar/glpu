#include "/adm/obj/simul_efun.h"

private int data_index(string *lines, string key) {
    string *parts ;
    int i, sz ;

    for(i = 0, sz = sizeof(lines); i < sz; i++) {
        if(strsrch(lines[i], key) == 0)
            return i ;
    }

    return -1 ;
}

varargs mixed data_value(string file, string key, mixed def) {
    string *lines, line ;
    mixed *parts ;
    int i, sz ;

    if(nullp(file) || nullp(key))
        return null ;

    if(!file_exists(file))
        return null ;

    key = append(key, "|") ;
    lines = explode_file(file);
    i = data_index(lines, key) ;
    if(i == -1)
        return def ;

    line = lines[i] ;
    parts = explode(line, "|");
    parts = parts[1..];
    parts = map(parts, (: from_string :)) ;
    if(sizeof(parts) == 1)
        return parts[0] ;

    return parts ;
}

varargs void data_write(string file, string key, mixed data...) {
    string *lines, out ;
    mixed *parts ;
    int i, sz ;

    if(nullp(file) || nullp(key) || nullp(data))
        return ;

    parts = map(data, (: stringify :)) ;
    out = implode(parts, "|") ;
    key = append(key, "|") ;
    out = key + out ;

    lines = explode_file(file);
    i = data_index(lines, key) ;
    if(i > -1) {
        lines[i] = out ;
        write_file(file, implode(lines, "\n") + "\n", 1) ;
        return ;
    }

    write_file(file, out + "\n", 1) ;
}

int data_del(string file, string key) {
    string *lines ;
    int i, sz ;

    if(nullp(file) || nullp(key))
        return 0 ;

    lines = explode_file(file);
    i = data_index(lines, key) ;
    if(i == -1)
        return 0 ;

    lines = lines[0..i-1] + lines[i+1..] ;
    if(sizeof(lines) == 0)
        rm(file) ;
    else
        write_file(file, implode(lines, "\n") + "\n", 1) ;

    return 1 ;
}

varargs int data_inc(string file, string key, int inc) {
    string *lines, line ;
    int i, val ;

    if(nullp(file) || nullp(key))
        return null ;

    lines = explode_file(file);
    i = data_index(lines, key) ;
    if(i == -1) {
        data_write(file, key, inc) ;
        return inc ;
    }

    key = append(key, "|") ;
    line = lines[i] ;
    sscanf(line, key + "%d", val) ;
    val += inc ;
    lines[i] = key + val ;
    write_file(file, implode(lines, "\n") + "\n", 1) ;
}
