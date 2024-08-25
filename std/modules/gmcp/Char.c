#include <daemons.h>
#include <gmcp_defines.h>

inherit __DIR__ "gmcp_module" ;

void setup() {
    set_privs("gmcp") ;

    cooldown_limits = ([
        GMCP_PKG_CHAR_STATUS : 0,
        GMCP_PKG_CHAR_STATUSVARS: 0,
    ]) ;
}

void StatusVars() {
    object prev = previous_object();

    if(!cooldown_check(GMCP_PKG_CHAR_STATUSVARS, prev))
        return;

    apply_cooldown(GMCP_PKG_CHAR_STATUSVARS, prev);

    GMCP_D->send_gmcp(prev, GMCP_PKG_CHAR_STATUSVARS) ;
}

void Status() {
    object prev = previous_object();

    if(!cooldown_check(GMCP_PKG_CHAR_STATUS, prev))
        return;

    apply_cooldown(GMCP_PKG_CHAR_STATUS, prev);

    GMCP_D->send_gmcp(prev, GMCP_PKG_CHAR_STATUS) ;
}

void Login(string submodule, mapping data) {
    object prev = previous_object() ;

    switch(submodule) {
        case "Credentials" : {
            string username, account_name, password, character, curr, test ;
            mapping account ;

            username = lower_case(data["account"]) ;
            sscanf(username, "%s@%s", character, account_name) ;

            account = ACCOUNT_D->load_account(account_name) ;
            password = data["password"] ;

            if(!account || !account["password"]) {
                GMCP_D->send_gmcp(prev,
                    GMCP_PKG_CHAR_LOGIN_RESULT,
                    ([ "success" : "false", "message": "Invalid account name."])
                ) ;
                tell(prev, "Invalid account name.\n") ;
                prev->remove() ;
                return ;
            }

            curr = account["password"] ;
            test = crypt(password, curr) ;
            if(test != curr) {
                GMCP_D->send_gmcp(prev,
                    GMCP_PKG_CHAR_LOGIN_RESULT,
                    ([ "success" : "false", "message": "Invalid password."])
                ) ;
                tell(prev, "Invalid password.\n") ;
                prev->remove() ;
                return ;
            }

            GMCP_D->send_gmcp(prev,
                GMCP_PKG_CHAR_LOGIN_RESULT,
                ([ "success" : "true", "message": "Login successful."])
            ) ;
            prev->gmcp_authenticated(username, character) ;
        }
    }
}

void Items(string submodule, string target) {
    object prev = previous_object() ;

    _debug("Char.Items.%s %s", submodule, target) ;

    switch(submodule) {
        case "Contents" :
            GMCP_D->send_gmcp(prev, GMCP_PKG_CHAR_ITEMS_LIST, target) ;
            break ;
        case "Inv" :
            GMCP_D->send_gmcp(prev, GMCP_PKG_CHAR_ITEMS_LIST, GMCP_LIST_INV) ;
            break ;
        case "Room" :
            GMCP_D->send_gmcp(prev, GMCP_PKG_CHAR_ITEMS_LIST, GMCP_LIST_ROOM) ;
            break ;
    }
}
