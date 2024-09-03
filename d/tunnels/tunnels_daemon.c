/**
 * @file /d/village/tunnels/tunnels_daemon.c
 * @description Daemon for managing the underground tunnels area
 *
 * @created 2024-08-25 - Gesslar
 * @last_modified 2024-08-25 - Gesslar
 *
 * @history
 * 2024-08-25 - Gesslar - Created
 */

inherit STD_VIRTUAL_MAP;

private void setup_tunnel_shorts();
private void setup_tunnel_longs();

private nosave string *tunnel_shorts;
private nosave string *tunnel_longs;
private nosave string cavern_long;
private nosave int rot = 0;

void setup() {
    set_map_file(__DIR__ "tunnels_map.txt");
    setup_tunnel_shorts();
    setup_tunnel_longs();
}

private void setup_tunnel_shorts() {
    tunnel_shorts = ({
        "Narrow Tunnel",
        "Winding Passage",
        "Damp Corridor",
        "Dark Tunnel",
    });
}

private void setup_tunnel_longs() {
    tunnel_longs = ({
        "The tunnel here is narrow and claustrophobic, with rough dirt walls "
        "pressing in from all sides. The air is stale and heavy with the scent "
        "of earth. Your footsteps echo softly in the confined space, and you "
        "can hear the occasional drip of water from unseen crevices.",

        "This section of the tunnel winds unpredictably, twisting and turning "
        "through the earth. The uneven floor is littered with small rocks and "
        "debris, making each step treacherous. The darkness seems to deepen "
        "around each bend, hiding whatever may lie ahead.",

        "Moisture seeps through the walls here, creating a damp and chilly "
        "atmosphere. The tunnel widens slightly, but the ceiling remains low. "
        "Patches of pale fungi cling to the walls, providing a faint, eerie glow "
        "that barely illuminates the passage.",

        "Darkness envelops this part of the tunnel, making it difficult to see "
        "more than a few feet ahead. The walls are smooth here, as if worn down "
        "by countless years of flowing water or passing creatures. An unsettling "
        "silence hangs in the air, broken only by your own careful movements.",
    });

    cavern_long =
        "The tunnel opens up into a small, natural cavern. The ceiling arches "
        "high above, disappearing into darkness. Stalactites hang precariously "
        "overhead, while stalagmites rise from the uneven floor. A faint echo "
        "of dripping water can be heard, and the air feels slightly less "
        "oppressive here. This cavern seems to be a crossroads of sorts, with "
        "several tunnel openings visible along its perimeter.";
}

public void setup_short(object room, string file) {
    int x, y, z;
    string room_type;

    sscanf(file, "%d,%d,%d", x, y, z);
    room_type = get_room_type(x, y);

    if(room_type == "O")
        room->set_short(element_of(tunnel_shorts));
    else if(room_type == "X")
        room->set_short("Underground Cavern");
}

public void setup_long(object room, string file) {
    int x, y, z;
    string room_type;

    sscanf(file, "%d,%d,%d", x, y, z);
    room_type = get_room_type(x, y);

    if(room_type == "O") {
        room->set_long(tunnel_longs[rot]);
        if(++rot >= sizeof(tunnel_longs))
            rot = 0;
    } else if(room_type == "X")
        room->set_long(cavern_long);
}

public void setup_exits(object room, string file) {
    int x, y, z;
    string room_type;
    mapping exits;

    if(sscanf(file, "%d,%d,%d", x, y, z) != 3)
        return;

    room_type = get_room_type(x, y);
    exits = get_exits(x, y, z);

    if(file == "0,0,-1")
        exits["up"] = "../village/square";

    room->set_exits(exits);
}
