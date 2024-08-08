/* barckr.c

 Tacitus @ LPUniversity
 10-NOV-05
 LPUniversity Cafe

*/

inherit STD_ROOM;

void create() {
    set_short("a hallway");
    set_long(
"The room is curved into a hallway that leads to the bathroom. The railing "
"forks, to a buffer area and to the bathroom. A few chairs are positioned into "
"a circle, each chair cushioned with deep red material."
"\n\n"
"At the foot of the buffer area, there is a staircase leading up but it is "
"blocked off.");
    set_items(([
        "chairs" : "The chairs are cushioned with red material.",
        "railing" : "The railing is stained a deep brown.",
        "staircase" : "It appears that the staircases lead to an area that is under construction.",
    ]));
    set_exits(([
      "west" : __DIR__ + "bathroom",
      "south": __DIR__ + "cafe",
    ]));

    set_light(1);
}
