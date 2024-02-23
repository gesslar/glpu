#include <daemons.h>

void Hello(mapping data) {
    object prev = previous_object();
    string key, value ;

    // Core.Hello should only arrive from the login object at the start of the
    // connection.
    if(base_name(prev) != LOGIN_OB)
        return;

    foreach(key, value in data) {
        map_delete(data, key);
        data[lower_case(key)] = value;
    }

    prev->set_gmcp_client(data) ;
}

void Supports(string command, mixed data) {
    object prev = previous_object();
    string *curr ;

    if(stringp(data)) {
        data = ({ data }) ;
    }

    data = filter(data, (: stringp :)) ;
    if(!sizeof(data))
        return;

    data = map(data, function(string support) {
        string *parts = explode(support, " ") ;

        return parts[0] ;
    }) ;

    switch(command) {
        case "Set" :
            // we're not doing anything cos we're setting later
            break ;
        case "Add" :
            curr = prev->query_gmcp_supports() ;
            if(!curr) {
                curr = ({ }) ;
            }
            data = curr + data ;
            break ;
        case "Remove" :
            curr = prev->query_gmcp_supports() ;
            if(!curr) {
                curr = ({ }) ;
            }
            curr -= data ;
            break ;
        default:
            return ;
    }

    prev->set_gmcp_supports(data) ;
}

void Ping(int time) {
    object prev = previous_object();

    if(!time)
        return;

    GMCP_D->send_gmcp(prev, "Core.Ping") ;
}
