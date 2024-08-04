#include <gmcp_defines.h>

inherit STD_DAEMON ;

void GUI(object who, string submodule, mapping payload) {
    switch(submodule) {
        case "Install" :
            who->do_gmcp(
                GMCP_PKG_CLIENT_GUI,
                ([
                    GMCP_LBL_GMCP_CLIENT_GUI_URL : payload["url"],
                    GMCP_LBL_GMCP_CLIENT_GUI_VERSION : payload["version"],
                ])
            ) ;
            break ;
    }
}
