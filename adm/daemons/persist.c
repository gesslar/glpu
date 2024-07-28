// /adm/daemons/persist.c
// Persist daemon ensures objects are all saved.
//
// Created:     2024/03/05: Gesslar
// Last Change: 2024/03/05: Gesslar
//
// 2024/03/05: Gesslar - Created

void find_persistent_objects() ;
void register_peristent(object ob) ;
void unregister_persistent(object ob) ;
void persist_objects() ;

private nosave object  *_persistents = ({});

void setup() {
    set_heart_beat(30) ;
    find_persistent_objects();
    slot(SIG_SYS_CRASH, "persist_objects");
}

void find_persistent_objects() {
    _persistents = objects((: $1->query_persistent() :));
}

void register_peristent(object ob) {
    if (member_array(ob, _persistents) == -1) {
        _persistents += ({ ob });
    }
}

void unregister_persistent(object ob) {
    if (member_array(ob, _persistents) != -1) {
        _persistents -= ({ ob });
    }
}

void persist_objects() {
    _persistents -= ({ 0 });
    catch( filter(_persistents, (: $1->save_data() :)) ) ;
}

void heart_beat() {
    persist_objects();
}
