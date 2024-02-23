// /std/modules/gmcp.c
// Module inherited by living objects to handle GMCP
//
// Created:     2024/02/22: Gesslar
// Last Change: 2024/02/22: Gesslar
//
// 2024/02/22: Gesslar - Created

// Functions from other objects
mixed query_env(string var_name);
int set_env(string var_name, string var_value) ;

// Functions
int gmcp_enabled() ;

int gmcp_enabled() {
    return has_gmcp(this_object()) && query_env("gmcp") == "on";
}

void gmcp(string message) {
    debugf("GMCP: %O", message);
}

void gmcp_enable() {
    debugf("Enabling GMCP");
    if (has_gmcp(this_object())) {
        set_env("gmcp", "on");
        debugf("GMCP enabled");
    }
}
