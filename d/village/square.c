// /d/village/square.c
// Short description of this file and its purpose.
//
// Created:     2024/02/04: Gesslar
// Last Change: 2024/02/04: Gesslar
//
// 2024/02/04: Gesslar - Created

inherit __DIR__ "village_inherit" ;

void setup() {
    set_short("Village Square of Olum");
    set_long(
"At the village center lies a bustling square, surrounded by ancient timber-"
"framed buildings. Cobblestone paths lead to a moss-covered stone well, the "
"communal heart where market stalls flaunt local produce and crafts. Laughter "
"and conversation ripple through the air as elders on benches share stories, "
"overseeing the vibrant tableau of daily life. The well stands as a testament "
"to the village's enduring spirit, anchoring the square's lively exchange "
"and community bonds.") ;
    set_exits(([
        "north": "village_path1",
        "west" : "village_path2",
        "east" : "village_path3",
        "south" : "village_path4",
    ])) ;
}
