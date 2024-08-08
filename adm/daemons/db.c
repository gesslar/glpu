// /adm/daemons/db.c
// DB Daemon
//
// Created:     2024/02/27: Gesslar
// Last Change: 2024/02/27: Gesslar
//
// 2024/02/27: Gesslar - Created

#include <type.h>

inherit STD_DAEMON ;

#ifndef __PACKAGE_DB__
#else
#ifndef __USE_SQLITE3__
#else

int valid_db(string db) ;
int valid_table(string db, string table) ;

private nosave mapping databases = ([ ]);
private nosave mapping table_definitions = ([ ]);
mixed query(string db, string q) ;
int allow_upsert(string db) ;
mixed sqlite_version(string db) ;
string statement_from_mapping(mapping data) ;

void setup() {
    string db_path = mud_config("DB_PATH");
    string db_suffix = mud_config("DB_SUFFIX");
    string table_suffix = mud_config("DB_TABLE_SUFFIX");

    databases = ([ ]);
    table_definitions = ([ ]);

    if(strlen(db_path) > 0) {
        // Find all table definition files first
        string *table_files = get_dir(db_path + "*" + table_suffix);

        foreach(string table_file in table_files) {
            string db_name = chop(table_file, table_suffix, -1) ;
            string table_file_name = db_path + table_file;

            if(file_size(table_file_name) > 0) {
                string line, *lines = explode_file(table_file_name);
                table_definitions[db_name] = ([]);

                foreach(line in lines) {
                    int pos;
                    string table_name, table_definition;

                    if(sscanf(line, "%s=%s", table_name, table_definition) == 2) {
                        table_definitions[db_name][table_name] = table_definition;
                    }
                }
            }
        }

        // Now, create databases and tables based on the table definitions
        foreach(string db_name, mapping tables in table_definitions) {
            string database_file = db_path + db_name + db_suffix;
            int con = -1 ;

            databases[db_name] = database_file;

            if(sizeof(tables)) {
                mixed err = catch {
                    int fd ;
                    mixed result ;
                    int close_result ;
                    string current ;

                    fd = db_connect("", database_file, "", __USE_SQLITE3__);
                    if(fd == 0) {
                        log_file("system/db", "Error connecting to " + db_name + " at " + database_file + "\n");
                            return;
                    }

                    foreach(string table_name, string table_definition in tables) {
                        current = table_name ;
                        result = db_exec(fd, "CREATE TABLE IF NOT EXISTS " + table_name + " (" + table_definition + ")");

                        if(stringp(result)) {
                            log_file("system/db", "Error creating table " + table_name + " in " + db_name + ": " + result + "\n");
                            return;
                        }
                    }

                    close_result = db_close(fd);
                    if(close_result == 0) {
                        log_file("system/db", "Error closing connection to " + db_name + " at " + database_file + "\n");
                        return;
                    }
                } ;
                if(err) {
                    log_file("system/db", "Error creating tables in " + db_name + ": " + err + "\n");
                    continue ;
                }
            }
        }
    }
}

mixed query(string db, string q) {
    string database_file = databases[db];
    int fd ;
    int close_result, i ;
    mixed rows, *result = ({ }) ;
    int sz ;
    mapping *data = ({ }) ;

    if(!db || !q)
        return "Invalid db or query." ;

    q = append(q, ";") ;

    fd = db_connect("", database_file, "", __USE_SQLITE3__);
    if(fd == 0) {
        log_file("system/db", "Error connecting to " + db + " at " + database_file + "\n");
        return 0;
    }

    rows = db_exec(fd, q);

    if(stringp(rows)) {
        log_file("system/db", "Error querying " + db + ": " + rows + "\n");
        return "Error querying " + db + ": " + rows + "\n";
    }

    if(rows == 0)
        return 0 ;

    result = allocate(rows+1) ;
    catch {
        for(i = 0; i <= rows; i++) {
            mixed info = db_fetch(fd, i) ;

            if(stringp(info))
                log_file("system/db", "Error fetching row " + i + " in " + db + ": '" + q + "' " + info + "\n");
            else
                result[i] = info ;
        }
    } ;

    close_result = db_close(fd);
    if(close_result == 0) {
        log_file("system/db", "Error closing connection to " + db + " at " + database_file + "\n");
        return "Error closing connection to " + db + " at " + database_file + "\n";
    }

    data = ({}) ;
    sz = sizeof(result) ;
    for(i = 1; i < sz; i++) {
        mapping row = ([ ]) ;
        int j, sz2 ;

        sz2 = sizeof(result[i]) ;
        for(j = 0; j < sz2; j++) {
            row[result[0][j]] = result[i][j] ;
        }

        data += ({ row }) ;
    }

    return data;
}

mapping query_databases() {
    return copy(databases) ;
}

mapping query_tables(string db_name) {
    return copy(table_definitions[db_name]) ;
}

int valid_db(string db) {
    return !nullp(databases[db]) ;
}

int valid_table(string db, string table) {
    string statement = sprintf("SELECT name FROM sqlite_master WHERE type='table' AND name='%s';", table) ;
    mixed result ;

    if(!valid_db(db))
        return 0 ;

    result = query(db, statement) ;

    if(!result)
        return 0 ;

    if(stringp(result))
        return 0 ;

    return sizeof(result) > 0 ;
}

mixed sqlite_version(string db) {
    string statement = "SELECT sqlite_version() ;" ;
    mixed result ;

    if(!valid_db(db))
        return 0 ;

    result = query(db, statement) ;

    if(!result)
        return -1 ;

    if(stringp(result))
        return 0 ;

    return explode(result[0][0], ".") ;
}

string statement_from_mapping(mapping data) {
    string statement = "" ;

    if(!sizeof(data))
        return null ;

    statement = "(" + implode(keys(data), ",") + ") VALUES (" ;
    foreach(mixed k, mixed v in data) {
        if(!stringp(k))
            continue ;

        if(typeof(v) == T_STRING)
            statement += "'" + v + "'," ;
        else
            statement += v + "," ;
    }

    statement += ")" ;

    return statement ;
}

int allow_upsert(string db) {
    mixed version = sqlite_version(db) ;

    if(!version)
        return 0 ;

    if(stringp(version))
        return 0 ;

    if(sizeof(version) < 3)
        return 0 ;

    if(version[0] < 3)
        return 0 ;

    if(version[0] == 3 && version[1] < 24)
        return 0 ;

    return 1 ;

}

#endif // __USE_SQLITE3__
#endif // __PACKAGE_DB__
