//void.c

//Tacitus @ LPUniversity
//02-APR-05
//The Void

//Last updated June 9th, 2006 by Tacitus

inherit STD_ROOM;

void setup() {
     set_no_clean() ;
     set_light(1);
     set_short("The void");
     set_long("This is the first room to ever exists on LPUniversity!\n");
     set_exits(([
          "cafe" : "/areas/lpuni/entrance.c"
     ]));
     set_items((["test" : "Test test test!\n"]));
}
