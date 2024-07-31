/**
 * @file /std/living/boon.c
 * @description Buffs/debuffs and other boon for living objects.
 *
 * @created 2024/07/30 - Gesslar
 * @last_modified 2024/07/30 - Gesslar
 *
 * @history
 * 2024/07/30 - Gesslar - Created
 */

#include <boon.h>
#include <attributes.h>
#include <living.h>
#include <skills.h>

private nomask mapping boon = ([]) ;
private nomask mapping curse = ([]) ;

private nomask nosave int BOON = 1 ;
private nomask nosave int CURSE = 2 ;

public nomask void init_boon() {
    boon = boon || ([ ]) ;
    curse = curse || ([ ]) ;
}

public nomask int boon(string name, string cl, string type, int amt, int dur) {
    int tag = time_ns() ;

    if(nullp(name) || nullp(cl) || nullp(type) || nullp(amt) || nullp(dur))
        return 0 ;

    if(!of(cl, boon))
        boon[cl] = ([]) ;

    if(!of(type, boon[cl]))
        boon[cl][type] = ([]) ;

    boon[cl][type][tag] = ([ "name" : name, "amt" : amt, "expires" : time() + dur ]) ;

    return tag ;
}

public nomask int curse(string name, string cl, string type, int amt, int dur) {
    int tag = time_ns() ;

    if(!of(cl, curse))
        curse[cl] = ([]) ;

    if(!of(type, curse[cl]))
        curse[cl][type] = ([]) ;

    curse[cl][type][tag] = ([ "name" : name, "amt" : amt, "expires" : time() + dur ]) ;

    return tag ;
}

public nomask int query_boon(string cl, string type) {
    int total = 0 ;

    if(!of(cl, boon))
        return 0 ;

    if(!of(type, boon[cl]))
        return 0 ;

    foreach(int tag, mapping data in boon[cl][type]) {
        total += data["amt"] ;
    }

    return total ;
}

public mapping query_boon_data() {
    return copy(boon) ;
}

public nomask int query_curse(string cl, string type) {
    int total = 0 ;

    if(!of(cl, curse))
        return 0 ;

    if(!of(type, curse[cl]))
        return 0 ;

    foreach(int tag, mapping data in curse[cl][type]) {
        total += data["amt"] ;
    }

    return total ;
}

public mapping query_curse_data() {
    return copy(curse) ;
}

public nomask int query_effective_boon(string cl, string type) {
    return query_boon(cl, type) - query_curse(cl, type) ;
}

protected nomask void process_boon() {
    int now = time(), tag;
    string cl, type ;
    mapping data, b, boon_data, c, curse_data ;

    foreach(cl, data in boon) {
        foreach(type, boon_data in data) {
            foreach(tag, b in boon_data) {
                if(b["expires"] < now) {
                    map_delete(boon_data, tag) ;
                    tell(this_object(), "Your " + b["name"] + " has worn off.\n") ;
                }
            }
        }
    }

    foreach(cl, data in curse) {
        foreach(type, curse_data in data) {
            foreach(tag, c in curse_data) {
                if(c["expires"] < now) {
                    map_delete(curse_data, tag) ;
                    tell(this_object(), "Your " + c["name"] + " has worn off.\n") ;
                }
            }
        }
    }
}
