#include "/adm/obj/simul_efun.h"

int validate_db(string str) {
    return DB_D->valid_db(str) ;
}

int validate_table(string db, string str) {
    return DB_D->valid_table(db, str) ;
}

private string *db_table(string str) {
    string db, table ;

    if(!str) return 0 ;
    if(sscanf(str, "%s.%s", db, table) != 2) return 0 ;

    if(!validate_db(db)) return 0 ;
    if(!validate_table(str)) return 0 ;

    return ({ db, table }) ;
}

mixed int db_write(string str, string key, mixed data) {
    string *db_table ;

    if(!str || !key || !data) return "Missing or invalid arguments to db_write" ;

    db_table = db_table(str) ;
    if(!db_table) return "Invalid database or table" ;

    return DB_D->insert_or_update(db_table[0], db_table[1], key, data) ;
}
