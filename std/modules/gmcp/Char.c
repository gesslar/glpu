#include <daemons.h>
#include <gmcp_defines.h>

inherit __DIR__ "gmcp_module" ;

void setup() {
    cooldown_limits = ([
        GMCP_PKG_CHAR_STATUS : 60,
        GMCP_PKG_CHAR_STATUSVARS: 60,
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

void Login(string command, mapping data) {
    object prev = previous_object(1) ;

    switch(command) {
        case "Credentials" : {
            string account, password, curr, test ;
            object link ;

            account = lower_case(data["account"]) ;
            password = data["password"] ;
            if(!objectp(link = FINGER_D->get_user(account))) {
                GMCP_D->send_gmcp(prev,
                    GMCP_PKG_CHAR_LOGIN_RESULT,
                    ([ "success" : "false", "message": "Invalid username."])
                ) ;
                tell(prev, "Invalid username.\n") ;
                prev->remove() ;
                return ;
            }

            curr = link->query_password() ;
            test = crypt(password, curr) ;
            if(test != curr) {
                GMCP_D->send_gmcp(prev,
                    GMCP_PKG_CHAR_LOGIN_RESULT,
                    ([ "success" : "false", "message": "Invalid password."])
                ) ;
                tell(prev, "Invalid password.\n") ;
                link->remove() ;
                prev->remove() ;
                return ;
            }

            link->remove() ;
            GMCP_D->send_gmcp(prev,
                GMCP_PKG_CHAR_LOGIN_RESULT,
                ([ "success" : "true", "message": "Login successful."])
            ) ;
            prev->gmcp_authenticated(account) ;
        }
    }
}
