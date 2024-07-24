inherit STD_OBJECT;

void setup() {
    set_id(({"toycar", "car", "toy car"}));
    set_short("toy car");
    set_name("toy car");
    set_long("A small toy car for testing purposes.\n");
    set_mass(20) ;
}

void event_moved(object prev, object from) {
    _debug("I was moved from " + identify(from));
    _debug("I'm now in " + identify(environment()));
}
