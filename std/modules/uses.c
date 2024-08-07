/**
 * @file /std/modules/uses.c
 * @description This module is inherited in order to provide consumabling.
 *
 * @created 2024/08/06 - Gesslar
 * @last_modified 2024/08/06 - Gesslar
 *
 * @history
 * 2024/08/06 - Gesslar - Created
 */

private nomask int _max_uses = null ;
private nomask int _uses = null ;

int set_uses(int uses) {
    _uses = uses;

    if(nullp(_max_uses)) {
        _max_uses = uses;
    }

    return _uses;
}

int query_uses() {
    return _uses;
}

int query_max_uses() {
    return _max_uses;
}

mixed adjust_uses(int uses) {
    if(nullp(_uses) || nullp(_max_uses))
        return null ;

    if(_uses + uses < 0)
        return null ;

    if(_uses + uses > _max_uses)
        return null ;

    _uses += uses;

    return _uses;
}

void reset_uses() {
    _uses = _max_uses;
}
