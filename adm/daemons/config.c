// /adm/daemons/config.c
// This daemon is used to read and hold information regarding game
// configuration. It reads from the file "/adm/etc/default.json" and
// and then reads from the file "/adm/etc/config.json" and overwrites
// any values found in the default.json file.
//
// This enables the game to maintain a set of default values for storage
// in the git repo, but allows for the game to be configured differently
// depending up on your own desires. "/adm/etc/config.json" is not stored
// in the git repo and is not overwritten by updates to the game.
//
// Created:     2024/02/03: Gesslar
// Last Change: 2024/02/03: Gesslar
//
// 2024/02/03: Gesslar - Created

inherit STD_DAEMON ;

public void rehash_config() ;
public mixed get_mud_config(string key) ;

private nosave string DEFAULT_CONFIG = "/adm/etc/default.json" ;
private nosave string CONFIG_FILE = "/adm/etc/config.json" ;
private nosave mapping config = ([ ]) ;

void setup() {
    set_no_clean() ;
    rehash_config() ;
}

public mixed get_mud_config(string key) {
    if(nullp(config))
        error("get_mud_config: No configuration found.") ;

    if(!key)
        error("get_mud_config: Missing key.") ;

    if(nullp(config[key]))
        error("get_mud_config: Invalid key: " + key + ".") ;

    return config[key] ;
}

public void rehash_config() {
     mapping temp ;

    if(file_exists(DEFAULT_CONFIG)) {
        temp = json_decode(read_file(DEFAULT_CONFIG)) ;
        if(mapp(temp)) {
            config += temp ;
        }
    }

    if(file_exists(CONFIG_FILE)) {
        temp = json_decode(read_file(CONFIG_FILE)) ;
        if(mapp(temp)) {
            config += temp ;
        }
    }
}
