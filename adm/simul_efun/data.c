#include "/adm/obj/simul_efun.h"

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
    for(i = 0, sz = sizeof(lines); i < sz; i++) {
        if(strsrch(lines[i], key) == 0) {
            line = lines[i];
            break ;
        }
    }

    if(!line)
        return def;

    parts = explode(line, "|");
    parts = parts[1..];
    parts = map(parts, (: from_string :)) ;
    if(sizeof(parts) == 1)
        return parts[0] ;

    return parts ;
}

varargs void data_write(string file, string key, mixed *data) {
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
    for(i = 0, sz = sizeof(lines); i < sz; i++) {
        if(strsrch(lines[i], key) == 0) {
            lines[i] = out ;
            write_file(file, implode(lines, "\n") + "\n", 1) ;
            return ;
        }
    }

    write_file(file, out + "\n", 1) ;
}

varargs int data_inc(string file, string key, int inc) {
    mixed data ;

    if(nullp(file) || nullp(key))
        return null ;

    if(nullp(inc))
        inc = 1 ;

    data = data_value(file, key, 0) ;
    data += inc ;
    data_write(file, key, data) ;

    return data ;
}
