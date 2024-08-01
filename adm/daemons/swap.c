// /adm/daemons/swap.c
// This daemon exists to temporarily house data, such as when an object
// is destructing but will be back and needs the information again.
//
// Created:     2024/02/20: Gesslar
// Last Change: 2024/02/20: Gesslar
//
// 2024/02/20: Gesslar - Created

inherit STD_DAEMON ;

private nosave mapping data = ([]);

void swap_in(string label, mixed value) {
    data[label] = value ;
}

mixed swap_out(string label) {
    mixed value = data[label] ;
    map_delete(data, label) ;
    return value ;
}

int clean_up(int refs) {
    if(sizeof(data) > 0)
        return CLEAN_LATER ;

    return ::clean_up(refs) ;
}
