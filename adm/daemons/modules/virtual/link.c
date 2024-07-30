/**
 * @file /adm/daemons/modules/virtual/link.c
 * @description The virtual daemon that is responsible for creating user
 *              objects.
 *
 * @created 2024/07/30 - Gesslar
 * @last_modified 2024/07/30 - Gesslar
 *
 * @history
 * 2024/07/30 - Gesslar - Created
 */

inherit STD_DAEMON;

public nomask object compile_object(string file) {
    string name ;
    object ob ;
    string e ;
    string type = query_file_name() ;
    string test = sprintf("^/%s/\\w+$", type) ;

    if(!pcre_match(file, test))
        return 0;

    e = catch(ob = new(STD_USER));
    if(e) {
        log_file("VIRTUAL", e);
        return 0;
    }

    return ob;
}
