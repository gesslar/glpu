/**
 * @file /adm/daemons/modules/gmcp/Char.c
 * @description Module to handle Char.* GMCP packages
 *
 * @created 2024-02-23 - Gesslar
 * @last_modified 2024-02-23 - Gesslar
 *
 * @history
 * 2024-02-23 - Gesslar - Created
 */

#include <gmcp_defines.h>

inherit STD_DAEMON ;

// This function should present all of the labels that correspond to the
// variables that are sent in the Char.Status package. The purpose of this
// is to inform the client what to display, if desired, for each variable
// on labels in a GUI or messaging resulting from GMCP.
//
// The labels and values are all found in /include/gmcp_defines.h
void StatusVars(object who, mapping payload) {
    mapping data ;

    data = payload || ([
        GMCP_LBL_CHAR_STATUS_NAME        : GMCP_DIS_CHAR_STATUS_NAME,
        GMCP_LBL_CHAR_STATUS_FILL        : GMCP_DIS_CHAR_STATUS_FILL,
        GMCP_LBL_CHAR_STATUS_CAPACITY    : GMCP_DIS_CHAR_STATUS_CAPACITY,
        GMCP_LBL_CHAR_STATUS_XP          : GMCP_DIS_CHAR_STATUS_XP,
        GMCP_LBL_CHAR_STATUS_TNL         : GMCP_DIS_CHAR_STATUS_TNL,
        GMCP_LBL_CHAR_STATUS_WEALTH      : GMCP_DIS_CHAR_STATUS_WEALTH,
    ]);

    who->do_gmcp(GMCP_PKG_CHAR_STATUSVARS, data) ;
}

void Status(object who, mapping payload) {
    mapping data ;
    mapping wealth = who->query_all_wealth() ;

    wealth = map(wealth, (: sprintf("%d", $2) :)) ;

    data = payload || ([
        GMCP_LBL_CHAR_STATUS_NAME        : who->query_name(),
        GMCP_LBL_CHAR_STATUS_FILL        : sprintf("%d", who->query_fill()),
        GMCP_LBL_CHAR_STATUS_CAPACITY    : sprintf("%d", who->query_capacity()),
        GMCP_LBL_CHAR_STATUS_XP          : sprintf("%d", who->query_xp()),
        GMCP_LBL_CHAR_STATUS_TNL         : sprintf("%d", who->query_tnl()),
        GMCP_LBL_CHAR_STATUS_WEALTH      : wealth,
    ]) ;

    who->do_gmcp(GMCP_PKG_CHAR_STATUS, data) ;
}

void Vitals(object who, mapping payload) {
    mapping data ;

    data = payload || ([
        GMCP_LBL_CHAR_VITALS_HP     : sprintf("%.2f", who->query_hp()),
        GMCP_LBL_CHAR_VITALS_MAX_HP : sprintf("%.2f", who->query_max_hp()),
        GMCP_LBL_CHAR_VITALS_SP     : sprintf("%.2f", who->query_sp()),
        GMCP_LBL_CHAR_VITALS_MAX_SP : sprintf("%.2f", who->query_max_sp()),
        GMCP_LBL_CHAR_VITALS_MP     : sprintf("%.2f", who->query_mp()),
        GMCP_LBL_CHAR_VITALS_MAX_MP : sprintf("%.2f", who->query_max_mp()),
    ]) ;

    who->do_gmcp(GMCP_PKG_CHAR_VITALS, data) ;
}

void Login(object who, string submodule, mapping payload) {
    switch(submodule) {
        case "Default" : {
            who->do_gmcp(GMCP_PKG_CHAR_LOGIN_DEFAULT, payload) ;
            break ;
        }
    }
}

private string get_item_attrib(object item) ;
private string get_item_location(object who, object item) ;

void Items(object who, string submodule, mixed arg) {
    object container ;
    object *items, item ;
    mapping data ;
    mixed item_data ;
    string location ;
    string package ;

    // 1. If the submodule is list, then the arg is going to be a string
    //    location. It will be one of the GMCP_LIST_* constants, or the hash
    //    of the container, in which case we have to find the container in
    //    order to get its contents.
    // 2. If the submodule is Add, Remove, or Update, then arg is going to be
    //    an array of objects where the first object is the item and the second
    //    object is the container. We will have to determine the location of
    //    the container in order to set the location in the payload.

    switch(submodule) {
        case "List" :
            switch(arg) {
                case GMCP_LIST_ROOM :
                    container = environment(who) ;
                    location = GMCP_LIST_ROOM ;
                    break ;
                case GMCP_LIST_INV :
                    container = who ;
                    location = GMCP_LIST_INV ;
                    break ;
                default :
                    if(!arg)
                        return ;

                    if(!container = find_target(who, arg, who) &&
                       !container = find_target(who, arg, environment(who)))
                        return ;
                    location = arg ;
            }
            package = GMCP_PKG_CHAR_ITEMS_LIST ;
            break ;
        case "Add" :
        case "Remove" :
        case "Update" :
            item = arg[0] ;
            container = arg[1] ;
            location = get_item_location(who, container) ;
            package = GMCP_PKG_CHAR_ITEMS + "." + submodule ;
            break ;
    }

    data = ([
        "location": location,
    ]) ;

    // Now we do another switch to build the item data
    switch(submodule) {
        case "List" :
            items = find_targets(who, null, container, (: $1 != $2 :), who) ;
            item_data = ({}) ;

            foreach(item in items) {
                item_data += ({ ([
                    "name": COLOUR_D->substitute_colour(get_short(item), "high"),
                    "id"  : item->query_ids(),
                    "attrib": get_item_attrib(item),
                    "hash": hash("md4", file_name(item)),
                ]) }) ;
            }
            data["items"] = item_data ;
            break ;
        case "Add" :
        case "Remove" :
        case "Update" :
            if(!item)
                return ;

            item_data = ([
                "name"    : COLOUR_D->substitute_colour(get_short(item), "high"),
                "id"      : item->query_ids(),
                "attrib"  : get_item_attrib(item),
                "hash"    : hash("md4", file_name(item)),
            ]) ;
            data["item"] = item_data ;
            break ;
    }

    who->do_gmcp(package, data, 1) ;
}

private string get_item_attrib(object item) {
    string attrib = "" ;

    if(item->is_clothing() || item->is_armour())
       if(item->equipped())
            attrib += GMCP_ITEM_ATTRIB_WORN ;
        else
            attrib += GMCP_ITEM_ATTRIB_WEARABLE ;
    if(item->is_weapon() && item->equipped())
        attrib += GMCP_ITEM_ATTRIB_WIELDED ;
    if(!living(item) && item->is_container())
        attrib += GMCP_ITEM_ATTRIB_CONTAINER ;
    if(!item->query_prevent_get())
        attrib += GMCP_ITEM_ATTRIB_TAKEABLE ;
    if(item->is_npc())
        attrib += GMCP_ITEM_ATTRIB_MONSTER ;
    if(item->is_npc_corpse())
        attrib += GMCP_ITEM_ATTRIB_DEAD_MONSTER ;

    return attrib ;
}

private string get_item_location(object who, object container) {
    string location ;

    if(container == who)
        location = "inv" ;
    else if(container == environment(who))
        location = "room" ;
    else if(environment(container) == who)
        location = hash("md4", file_name(container)) ;
    else
        location = "unknown" ;

    return location ;
}
